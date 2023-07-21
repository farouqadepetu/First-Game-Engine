#include "FADeviceResources.h"
#include "FADirectXException.h"
#include <d3dx12.h>

namespace FARender
{
	//-----------------------------------------------------------------------------------------------------------------------
	//DEVICE RESOURCES FUNCTION DEFINITIONS

	DeviceResources& DeviceResources::GetInstance(unsigned int width, unsigned int height, HWND windowHandle,
		bool isMSAAEnabled, bool isTextEnabled)
	{
		static DeviceResources instance(width, height, windowHandle, isMSAAEnabled, isTextEnabled);

		return instance;
	}

	DeviceResources::DeviceResources(unsigned int width, unsigned int height, HWND windowHandle, 
		bool isMSAAEnabled, bool isTextEnabled) : mCurrentFrameIndex{ 0 }, mFenceValue{ 0 }
	{
		mEnableDebugLayer();
		mCreateDirect3DDevice();
		mCreateDXGIFactory();

		mCreateFence();

		mQueryDescriptorSizes();
		mCreateRTVHeap();
		mCreateDSVHeap();
		mCreateCommandObjects();

		mSwapChain.CreateSwapChain(mDXGIFactory, mCommandQueue, windowHandle);
		mMultiSampling.CheckMultiSamplingSupport(mDirect3DDevice, mSwapChain.GetBackBufferFormat(), 4);
		mTextResources = TextResources(mDirect3DDevice, mCommandQueue, mSwapChain.GetNumRenderTargetBuffers());

		Resize(width, height, windowHandle, isMSAAEnabled, isTextEnabled);
		mCommandList->Reset(mDirectCommandAllocator.Get(), nullptr);
	}

	DeviceResources::~DeviceResources()
	{
		if (mDirect3DDevice != nullptr)
			FlushCommandQueue();
	}

	const Microsoft::WRL::ComPtr<ID3D12Device>& DeviceResources::GetDevice() const
	{
		return mDirect3DDevice;
	}

	const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& DeviceResources::GetCommandList() const
	{
		return mCommandList;
	}

	DXGI_FORMAT DeviceResources::GetBackBufferFormat() const
	{
		return mSwapChain.GetBackBufferFormat();
	}

	DXGI_FORMAT DeviceResources::GetDepthStencilFormat() const
	{
		return mSwapChain.GetDepthStencilFormat();
	}

	unsigned int DeviceResources::GetCBVSRVUAVSize() const
	{
		return mCBVSize;
	}

	unsigned int DeviceResources::GetCurrentFrame() const
	{
		return mCurrentFrameIndex;
	}

	const TextResources& DeviceResources::GetTextResources() const
	{
		return mTextResources;
	}

	void DeviceResources::UpdateCurrentFrameFenceValue()
	{
		mCurrentFrameFenceValue[mCurrentFrameIndex] = ++mFenceValue;
	}

	void DeviceResources::mEnableDebugLayer()
	{
		//enables the debug layer for additional information
#if defined(_DEBUG) || defined(DEBUG)
		Microsoft::WRL::ComPtr<ID3D12Debug1> debug;
		ExitIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debug)));
		debug->EnableDebugLayer();
		debug->SetEnableGPUBasedValidation(true);
#endif
	}

	void DeviceResources::mCreateDirect3DDevice()
	{
		ExitIfFailed(D3D12CreateDevice(0, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&mDirect3DDevice)));
	}

	void DeviceResources::mCreateDXGIFactory()
	{
		UINT flag = 0;
#if defined(_DEBUG) || defined(DEBUG)
		flag = DXGI_CREATE_FACTORY_DEBUG;
#endif
		ExitIfFailed(CreateDXGIFactory2(flag, IID_PPV_ARGS(&mDXGIFactory)));
	}

	void DeviceResources::mCreateFence()
	{
		ExitIfFailed(mDirect3DDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence)));
	}

	void DeviceResources::mQueryDescriptorSizes()
	{
		mRTVSize = mDirect3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		mDSVSize = mDirect3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		mCBVSize = mDirect3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}

	void DeviceResources::mCreateRTVHeap()
	{
		//Create render target view heap to store descriptions of the render target buffers.
		//3 descritptors: 2 for the swap chain render target buffers and 1 for the multi-sampling render target buffer.
		D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDescription{};
		rtvDescriptorHeapDescription.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvDescriptorHeapDescription.NumDescriptors = 3;
		rtvDescriptorHeapDescription.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		rtvDescriptorHeapDescription.NodeMask = 0;
		ExitIfFailed(mDirect3DDevice->CreateDescriptorHeap(&rtvDescriptorHeapDescription, IID_PPV_ARGS(&mRTVHeap)));
	}

	void DeviceResources::mCreateDSVHeap()
	{
		//Create depth stencil view heap to store descriptions of the depth stencil buffers.
		//2 descritptors: 1 for the swap chain depth stencil buffer and 1 for the multi-sampling depth stencil buffer.
		D3D12_DESCRIPTOR_HEAP_DESC dsvDescriptorHeapDescription{};
		dsvDescriptorHeapDescription.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		dsvDescriptorHeapDescription.NumDescriptors = 2;
		dsvDescriptorHeapDescription.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		dsvDescriptorHeapDescription.NodeMask = 0;
		ExitIfFailed(mDirect3DDevice->CreateDescriptorHeap(&dsvDescriptorHeapDescription, IID_PPV_ARGS(&mDSVHeap)));
	}

	void DeviceResources::mCreateCommandObjects()
	{
		//describes the command queue.
		D3D12_COMMAND_QUEUE_DESC commandQueueDescription{};
		commandQueueDescription.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		commandQueueDescription.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		commandQueueDescription.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		commandQueueDescription.NodeMask = 0;

		//This is where the initialization and resizing commands are stored.
		ExitIfFailed(mDirect3DDevice->CreateCommandQueue(&commandQueueDescription,
			IID_PPV_ARGS(mCommandQueue.GetAddressOf())));

		ExitIfFailed(mDirect3DDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(mDirectCommandAllocator.GetAddressOf())));

		//This is where the rendering commands are stored.
		for (UINT i = 0; i < NUM_OF_FRAMES; ++i)
		{
			ExitIfFailed(mDirect3DDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
				IID_PPV_ARGS(mCommandAllocators[i].GetAddressOf())));
		}

		ExitIfFailed(mDirect3DDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
			mDirectCommandAllocator.Get(), nullptr, IID_PPV_ARGS(mCommandList.GetAddressOf())));

		// Start off in a closed state.  This is because the first time we refer 
		// to the command list we will Reset it, and it needs to be closed before
		// calling Reset.
		mCommandList->Close();
	}

	void DeviceResources::FlushCommandQueue()
	{
		++mFenceValue;

		//send a command to the command queue that says to set the given fence to the specified value
		ExitIfFailed(mCommandQueue->Signal(mFence.Get(), mFenceValue));

		//if the signal command has not been executed, wait until it is
		if (mFence->GetCompletedValue() < mFenceValue)
		{
			//create an event object and store an handle to it
			HANDLE eventHandle{ CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS) };

			if (eventHandle != nullptr)
			{
				//This function will fire(raise) an event when the signal command is executed by the GPU
				ExitIfFailed(mFence->SetEventOnCompletion(mFenceValue, eventHandle));

				//Wait until the GPU has executed the signal command
				WaitForSingleObject(eventHandle, INFINITE);

				//Closes the event handle
				CloseHandle(eventHandle);
			}
		}
	}

	void DeviceResources::WaitForGPU() const
	{
		//if the signal command has not been executed, wait until it is
		if (mCurrentFrameFenceValue[mCurrentFrameIndex] != 0 &&
			mFence->GetCompletedValue() < mCurrentFrameFenceValue[mCurrentFrameIndex])
		{
			//create an event object and store an handle to it
			HANDLE eventHandle{ CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS) };

			if (eventHandle != nullptr)
			{
				//This function will fire(raise) an event when the signal command is executed by the GPU
				ExitIfFailed(mFence->SetEventOnCompletion(mCurrentFrameFenceValue[mCurrentFrameIndex], eventHandle));

				//Wait until the GPU has executed the signal command
				WaitForSingleObject(eventHandle, INFINITE);

				//Closes the event handle
				CloseHandle(eventHandle);
			}
		}
	}

	void DeviceResources::Signal()
	{
		mCommandQueue->Signal(mFence.Get(), mFenceValue);
	}

	void DeviceResources::Resize(int width, int height, const HWND& windowHandle, bool isMSAAEnabled, bool isTextEnabled)
	{
		//make sure all of the commands in the command list have been executed before adding new commands.
		FlushCommandQueue();

		//reset the command list to add new commands
		ExitIfFailed(mCommandList->Reset(mDirectCommandAllocator.Get(), nullptr));

		//Reset text buffers.
		mTextResources.ResetBuffers();

		//Free the memory of the swap chain buffers.
		mSwapChain.ReleaseBuffers();

		//free the memory of the  MSAA buffers
		mMultiSampling.ReleaseBuffers();

		//Recreate the swap chains render target buffers and views to them.
		mSwapChain.CreateRenderTargetBuffersAndViews(mDirect3DDevice, mRTVHeap, 0, mRTVSize, width, height);

		//Rereate the swap chains depth stencil buffer and a view to it.
		mSwapChain.CreateDepthStencilBufferAndView(mDirect3DDevice, mDSVHeap, 0, mDSVSize, width, height);

		if (isMSAAEnabled)
		{
			//recreate the MSAA buffers
			mMultiSampling.CreateRenderTargetBufferAndView(mDirect3DDevice, mRTVHeap, 2, mRTVSize, width, height);
			mMultiSampling.CreateDepthStencilBufferAndView(mDirect3DDevice, mDSVHeap, 1, mDSVSize, width, height);
		}

		if (isTextEnabled)
			mTextResources.ResizeBuffers(mSwapChain.GetRenderTargetBuffers(), windowHandle);

		//Close the command list.
		//Store all your command lists in a ID3D12CommandList array.
		//Execute the resize commands.
		ExitIfFailed(mCommandList->Close());
		ID3D12CommandList* commandLists[] = { mCommandList.Get() };
		UINT s = sizeof(commandLists) / sizeof(commandLists[0]);
		mCommandQueue->ExecuteCommandLists(s, commandLists);

		//make sure all of the commands in the command list have been executed before adding new commands.
		FlushCommandQueue();

		//Viewport and scissor rectangles
		mViewport.TopLeftX = 0.0f;
		mViewport.TopLeftY = 0.0f;
		mViewport.Width = (float)width;
		mViewport.Height = (float)height;
		mViewport.MinDepth = 0.0f;
		mViewport.MaxDepth = 1.0f;

		//describes where to raster to in the back buffer
		mScissor = { 0, 0, width, height };
	}

	void DeviceResources::Execute() const
	{
		ExitIfFailed(mCommandList->Close());
		ID3D12CommandList* commandLists[] = { mCommandList.Get() };
		UINT s = sizeof(commandLists) / sizeof(commandLists[0]);
		mCommandQueue->ExecuteCommandLists(s, commandLists);
	}

	void DeviceResources::Present()
	{
		//swap the front and back buffers
		mSwapChain.Present();
	}

	void DeviceResources::Draw(bool isMSAAEnabled)
	{
		//Reseting command allocator allows us to reuse the memory.
		//Make sure all the commands in the command list is executed before calling this.
		ExitIfFailed(mCommandAllocators[mCurrentFrameIndex]->Reset());

		//Reset command list
		ExitIfFailed(mCommandList->Reset(mCommandAllocators[mCurrentFrameIndex].Get(), nullptr));

		//Link viewport to the rasterization stage
		mCommandList->RSSetViewports(1, &mViewport);

		//Link scissor rectangle to the rasterization stage
		mCommandList->RSSetScissorRects(1, &mScissor);

		if (isMSAAEnabled)
		{
			//Transistion the current back buffer to resolve dest state from present state
			mSwapChain.Transition(mCommandList, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RESOLVE_DEST);

			//Transistion the msaa RT buffer to render target state from resolve source
			mMultiSampling.Transition(mCommandList, D3D12_RESOURCE_STATE_RESOLVE_SOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);

			//clear the msaa RT buffer
			const float msaaRTClearValue[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
			mMultiSampling.ClearRenderTargetBuffer(mCommandList, mRTVHeap, 2, mRTVSize, msaaRTClearValue);

			//clear the msaa DS buffer
			mMultiSampling.ClearDepthStencilBuffer(mCommandList, mDSVHeap, 1, mDSVSize, 1.0f);
			
			//Get the address of the view to the MSAA render target buffer.
			CD3DX12_CPU_DESCRIPTOR_HANDLE msaaRenderTargetBufferViewAddress(mRTVHeap->GetCPUDescriptorHandleForHeapStart(),
				2, mRTVSize);

			//Get the address of the view to the MSAA depth stencil buffer.
			CD3DX12_CPU_DESCRIPTOR_HANDLE msaaDepthStencilBufferViewAddress(mDSVHeap->GetCPUDescriptorHandleForHeapStart(),
				1, mDSVSize);

			//Link the MSAA render target buffer and depth stencil buffer to the pipeline.
			mCommandList->OMSetRenderTargets(1, &msaaRenderTargetBufferViewAddress, true, &msaaDepthStencilBufferViewAddress);
		}
		else
		{
			//Transistion the current back buffer to render state from present state
			mSwapChain.Transition(mCommandList, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

			//Clear the swap chains current back buffer
			const float backBufferClearValue[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
			mSwapChain.ClearCurrentBackBuffer(mCommandList, mRTVHeap, 0, mRTVSize, backBufferClearValue);

			//Clear the swap chains DS buffer
			mSwapChain.ClearDepthStencilBuffer(mCommandList, mDSVHeap, 0, mDSVSize, 1.0f);

			//Get address of the view to the swap chains current back buffer.
			CD3DX12_CPU_DESCRIPTOR_HANDLE currentBackBufferViewAddress(mRTVHeap->GetCPUDescriptorHandleForHeapStart(),
				mSwapChain.GetCurrentBackBufferIndex(), mRTVSize);

			//Get address of the view to the swap chains depth stencil buffer.
			CD3DX12_CPU_DESCRIPTOR_HANDLE depthStencilViewAddress(mDSVHeap->GetCPUDescriptorHandleForHeapStart(),
				0, mDSVSize);

			//Link the current swap chains back buffer and depth stencil buffer to the pipeline.
			mCommandList->OMSetRenderTargets(1, &currentBackBufferViewAddress, true, &depthStencilViewAddress);
		}
	}

	void DeviceResources::NextFrame()
	{
		mCurrentFrameIndex = (mCurrentFrameIndex + 1) % NUM_OF_FRAMES;
	}

	void DeviceResources::RTBufferTransition(bool isMSAAEnabled, bool isTextEnabled)
	{
		if (isMSAAEnabled)
		{
			//Transistion the msaa RT buffer to resolve source state from render target state
			mMultiSampling.Transition(mCommandList, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_RESOLVE_SOURCE);

			//Copy a multi-sampled resource into a non multi-sampled resource
			mCommandList->ResolveSubresource(mSwapChain.GetCurrentBackBuffer().Get(), 0, 
				mMultiSampling.GetRenderTargetBuffer().Get(), 0, mSwapChain.GetBackBufferFormat());

			if (isTextEnabled)
			{
				//Transistion the current back buffer to render target state from resolve dest state
				mSwapChain.Transition(mCommandList, D3D12_RESOURCE_STATE_RESOLVE_DEST, D3D12_RESOURCE_STATE_RENDER_TARGET);
			}
			else
			{
				//Transistion the current back buffer to present state from resolve dest state
				mSwapChain.Transition(mCommandList, D3D12_RESOURCE_STATE_RESOLVE_DEST, D3D12_RESOURCE_STATE_PRESENT);
			}
		}
		else
		{
			if (!isTextEnabled)
			{
				//Transistion the current back buffer to present state from render target state
				mSwapChain.Transition(mCommandList, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
			}
		}
	}

	void DeviceResources::BeforeTextDraw()
	{
		mTextResources.BeforeRenderText(mSwapChain.GetCurrentBackBufferIndex());
	}

	void DeviceResources::AfterTextDraw()
	{
		mTextResources.AfterRenderText(mSwapChain.GetCurrentBackBufferIndex());
	}
	//-----------------------------------------------------------------------------------------------------------------------
}