#include "FADeviceResources.h"
#include "FADirectXException.h"
#include <d3dx12.h>

namespace FARender
{
	//-----------------------------------------------------------------------------------------------------------------------
	//DEVICE RESOURCES FUNCTION DEFINITIONS

	DeviceResources& DeviceResources::GetInstance(unsigned int width, unsigned int height, HWND windowHandle)
	{
		static DeviceResources instance(width, height, windowHandle);

		return instance;
	}

	DeviceResources::DeviceResources(unsigned int width, unsigned int height, HWND windowHandle)
	{
		mEnableDebugLayer();
		mCreateDirect3DDevice();
		mCreateDXGIFactory();
		mCreateFence();
		mQueryDescriptorSizes();
		mCreateCommandObjects();

		mSwapChain = SwapChain(mDirect3DDevice, mDXGIFactory, mCommandQueue, windowHandle, 2);
		mDepthStencil = DepthStencil(mDirect3DDevice);
		mMultiSampling = MultiSampling(mDirect3DDevice, mSwapChain.GetBackBufferFormat(), 4);
		mTextResources = TextResources(mDirect3DDevice, mCommandQueue, mSwapChain.GetNumOfSwapChainBuffers());

		/*mCheckMSAASupport();
		mCreateMSAARTVHeap();
		mCreateMSAADSVHeap();*/

		Resize(width, height, windowHandle);
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

	const DXGI_FORMAT& DeviceResources::GetDepthStencilFormat() const
	{
		return mDepthStencil.GetDepthStencilFormat();
	}

	UINT DeviceResources::GetCBVSize() const
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

	bool DeviceResources::IsMSAAEnabled() const
	{
		return mIsMSAAEnabled;
	}

	void DeviceResources::DisableMSAA(unsigned int width, unsigned int height, HWND windowHandle)
	{
		mIsMSAAEnabled = false;
		Resize(width, height, windowHandle);
	}

	void DeviceResources::EnableMSAA(unsigned int width, unsigned int height, HWND windowHandle)
	{
		mIsMSAAEnabled = true;
		Resize(width, height, windowHandle);
	}

	void DeviceResources::UpdateCurrentFrameFenceValue()
	{
		mCurrentFrameFenceValue[mCurrentFrameIndex] = ++mFenceValue;
	}

	void DeviceResources::mEnableDebugLayer()
	{
		//enables the debug layer for additional information
#if defined(_DEBUG) || defined(DEBUG)
		Microsoft::WRL::ComPtr<ID3D12Debug> debug;
		ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debug)));
		debug->EnableDebugLayer();
#endif
	}

	void DeviceResources::mCreateDirect3DDevice()
	{
		ThrowIfFailed(D3D12CreateDevice(0, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&mDirect3DDevice)));
	}

	void DeviceResources::mCreateDXGIFactory()
	{
		UINT flag = 0;
#if defined(_DEBUG) || defined(DEBUG)
		flag = DXGI_CREATE_FACTORY_DEBUG;
#endif
		ThrowIfFailed(CreateDXGIFactory2(flag, IID_PPV_ARGS(&mDXGIFactory)));
	}

	void DeviceResources::mCreateFence()
	{
		ThrowIfFailed(mDirect3DDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence)));
	}

	void DeviceResources::mQueryDescriptorSizes()
	{
		mRTVSize = mDirect3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		mDSVSize = mDirect3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		mCBVSize = mDirect3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}

	void DeviceResources::mCreateCommandObjects()
	{
		//describes the command queue.
		D3D12_COMMAND_QUEUE_DESC commandQueueDescription{};
		commandQueueDescription.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		commandQueueDescription.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		commandQueueDescription.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		commandQueueDescription.NodeMask = 0;

		ThrowIfFailed(mDirect3DDevice->CreateCommandQueue(&commandQueueDescription,
			IID_PPV_ARGS(mCommandQueue.GetAddressOf())));

		ThrowIfFailed(mDirect3DDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(mDirectCommandAllocator.GetAddressOf())));

		for (UINT i = 0; i < NUM_OF_FRAMES; ++i)
		{
			ThrowIfFailed(mDirect3DDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
				IID_PPV_ARGS(mCommandAllocator[i].GetAddressOf())));
		}

		ThrowIfFailed(mDirect3DDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
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
		ThrowIfFailed(mCommandQueue->Signal(mFence.Get(), mFenceValue));

		//if the signal command has not been executed, wait until it is
		if (mFence->GetCompletedValue() < mFenceValue)
		{
			//create an event object and store an handle to it
			HANDLE eventHandle{ CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS) };

			if (eventHandle != nullptr)
			{
				//This function will fire(raise) an event when the signal command is executed by the GPU
				ThrowIfFailed(mFence->SetEventOnCompletion(mFenceValue, eventHandle));

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
				ThrowIfFailed(mFence->SetEventOnCompletion(mCurrentFrameFenceValue[mCurrentFrameIndex], eventHandle));

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

	void DeviceResources::Resize(int width, int height, const HWND& windowHandle)
	{
		//make sure all of the commands in the command list have been executed before adding new commands.
		FlushCommandQueue();

		//reset the command list to add new commands
		ThrowIfFailed(mCommandList->Reset(mDirectCommandAllocator.Get(), nullptr));

		mTextResources.ResetBuffers();

		//Reset/Release all buffers that have a reference to the swap chain.
		mSwapChain.ResetBuffers();

		mDepthStencil.ResetBuffer();

		//reset MSAA buffers
		if (mIsMSAAEnabled)
		{
			mMultiSampling.ResetBuffers();
			//mMSAARenderTargetBuffer.Reset();
			//mMSAADepthStencilBuffer.Reset();
		}

		//Resize the swap chain buffers
		mSwapChain.ResizeSwapChain(width, height);

		//Create render target buffers and views to them.
		mSwapChain.CreateRenderTargetBuffersAndViews(mDirect3DDevice, mRTVSize);

		//Create depth stencil buffer and view to it.
		mDepthStencil.CreateDepthStencilBufferAndView(mDirect3DDevice, width, height);

		if (mIsMSAAEnabled)
		{
			mMultiSampling.CreateRenderTargetBufferAndView(mDirect3DDevice, mSwapChain.GetBackBufferFormat(), width, height);
			mMultiSampling.CreateDepthStencilBufferAndView(mDirect3DDevice, mDepthStencil.GetDepthStencilFormat(), width, height);
			//Make MSAA RT buffer, MSAA DS buffer, MSAA RT view and MSAA DS view
			//mCreateMSAARenderTargetBufferAndView(width, height);
			//mCreateMSAADepthStencilBufferAndView(width, height);
		}

		mTextResources.ResizeBuffers(mSwapChain.GetSwapChainBuffers(), windowHandle);

		//Close the command list.
		//Store all your command lists in a ID3D12CommandList array.
		//Execute the resize commands.
		ThrowIfFailed(mCommandList->Close());
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
		ThrowIfFailed(mCommandList->Close());
		ID3D12CommandList* commandLists[] = { mCommandList.Get() };
		UINT s = sizeof(commandLists) / sizeof(commandLists[0]);
		mCommandQueue->ExecuteCommandLists(s, commandLists);
	}

	void DeviceResources::Present()
	{
		//swap the front and back buffers
		mSwapChain.Present();
	}

	void DeviceResources::Draw()
	{
		//Reseting command allocator allows us to reuse the memory.
		//Make sure all the commands in the command list is executed before calling this.
		ThrowIfFailed(mCommandAllocator[mCurrentFrameIndex]->Reset());

		//Reset command list
		ThrowIfFailed(mCommandList->Reset(mCommandAllocator[mCurrentFrameIndex].Get(), nullptr));

		//Link viewport to the rasterization stage
		mCommandList->RSSetViewports(1, &mViewport);

		//Link scissor rectangle to the rasterization stage
		mCommandList->RSSetScissorRects(1, &mScissor);

		if (mIsMSAAEnabled)
		{
			//Transistion the current back buffer to resolve dest state from present state
			mSwapChain.Transition(mCommandList, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RESOLVE_DEST);

			//Transistion the msaa RT buffer to render target state from resolve source
			mMultiSampling.Transition(mCommandList, D3D12_RESOURCE_STATE_RESOLVE_SOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
			/*CD3DX12_RESOURCE_BARRIER msaaRTVTransitionToRenderTarget =
				CD3DX12_RESOURCE_BARRIER::Transition(mMSAARenderTargetBuffer.Get(),
					D3D12_RESOURCE_STATE_RESOLVE_SOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);

			mCommandList->ResourceBarrier(1, &msaaRTVTransitionToRenderTarget);*/

			//clear the msaa RT buffer
			const float msaaRTClearValue[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
			mMultiSampling.ClearRenderTargetBuffer(mCommandList, msaaRTClearValue);
			/*CD3DX12_CPU_DESCRIPTOR_HANDLE msaaRTVHeapHandle(mMSAARTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
				0, mRTVSize);
			mCommandList->ClearRenderTargetView(msaaRTVHeapHandle, msaaRTClearValue, 0, nullptr);*/

			//clear the msaa DS buffer
			mMultiSampling.ClearDepthStencilBuffer(mCommandList, 1.0f);
			/*mCommandList->ClearDepthStencilView(mMSAADSVDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
				D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);*/

			//Specify which back buffer to render to
			//CD3DX12_CPU_DESCRIPTOR_HANDLE msaaDSVHeapHandle(mMSAADSVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
			CD3DX12_CPU_DESCRIPTOR_HANDLE msaaRenderTargetBufferViewAddress{ mMultiSampling.GetRenderTargetBufferView() };
			CD3DX12_CPU_DESCRIPTOR_HANDLE msaaDepthStencilBufferViewAddress{ mMultiSampling.GetDepthStencilBufferView() };
			mCommandList->OMSetRenderTargets(1, &msaaRenderTargetBufferViewAddress, true, &msaaDepthStencilBufferViewAddress);
		}
		else
		{
			//Transistion the current back buffer to  render state from present state
			mSwapChain.Transition(mCommandList, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

			//Clear the current back buffer
			const float backBufferClearValue[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
			mSwapChain.ClearCurrentBackBuffer(mCommandList, mRTVSize, backBufferClearValue);

			//Clear the DS buffer
			mDepthStencil.ClearDepthStencilBuffer(mCommandList, 1.0f);

			CD3DX12_CPU_DESCRIPTOR_HANDLE currentBackBufferViewAddress{ mSwapChain.GetCurrentBackBufferView(mRTVSize) };
			CD3DX12_CPU_DESCRIPTOR_HANDLE depthStencilViewAddress{ mDepthStencil.GetDepthStencilView() };

			//Link the current back buffer and depth stencil buffer to the pipeline.
			mCommandList->OMSetRenderTargets(1, &currentBackBufferViewAddress, true, &depthStencilViewAddress);
		}
	}

	void DeviceResources::NextFrame()
	{
		mCurrentFrameIndex = (mCurrentFrameIndex + 1) % NUM_OF_FRAMES;
	}

	void DeviceResources::RTBufferTransition(bool renderText)
	{
		if (mIsMSAAEnabled)
		{
			//Transistion the msaa RT buffer to resolve source state from render target state
			mMultiSampling.Transition(mCommandList, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_RESOLVE_SOURCE);
			/*CD3DX12_RESOURCE_BARRIER msaaRTVTransitionToResolveSourceState =
				CD3DX12_RESOURCE_BARRIER::Transition(mMSAARenderTargetBuffer.Get(),
					D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_RESOLVE_SOURCE);

			mCommandList->ResourceBarrier(1, &msaaRTVTransitionToResolveSourceState);*/

			//Copy a multi-sampled resource into a non-multi-sampled resource
			mCommandList->ResolveSubresource(mSwapChain.GetCurrentBackBuffer().Get(), 0, 
				mMultiSampling.GetRenderTargetBuffer().Get(), 0, mSwapChain.GetBackBufferFormat());

			if (renderText)
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
			if (!renderText)
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

	/*void DeviceResources::mCheckMSAASupport()
	{
		//Describes the format and sample count we want to check to see if it is supported.
		D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS levels{};
		levels.Format = mSwapChain.GetBackBufferFormat();
		levels.SampleCount = 4;

		ThrowIfFailed(mDirect3DDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
			&levels, sizeof(levels)));

		if (levels.NumQualityLevels > 0)
			mMSAA4xSupported = true;
	}

	void DeviceResources::mCreateMSAARTVHeap()
	{
		if (mMSAA4xSupported)
		{
			D3D12_DESCRIPTOR_HEAP_DESC msaaRTVHeapDescription{};
			msaaRTVHeapDescription.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			msaaRTVHeapDescription.NumDescriptors = 1;
			msaaRTVHeapDescription.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			msaaRTVHeapDescription.NodeMask = 0;

			ThrowIfFailed(mDirect3DDevice->CreateDescriptorHeap(&msaaRTVHeapDescription,
				IID_PPV_ARGS(&mMSAARTVDescriptorHeap)));
		}
	}

	void DeviceResources::mCreateMSAADSVHeap()
	{
		if (mMSAA4xSupported)
		{
			D3D12_DESCRIPTOR_HEAP_DESC msaaDSVHeapDescription{};
			msaaDSVHeapDescription.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
			msaaDSVHeapDescription.NumDescriptors = 1;
			msaaDSVHeapDescription.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			msaaDSVHeapDescription.NodeMask = 0;

			ThrowIfFailed(mDirect3DDevice->CreateDescriptorHeap(&msaaDSVHeapDescription,
				IID_PPV_ARGS(&mMSAADSVDescriptorHeap)));
		}
	}

	void DeviceResources::mCreateMSAARenderTargetBufferAndView(int width, int height)
	{
		D3D12_RESOURCE_DESC mMSAARenderTargetBufferDesc{};
		mMSAARenderTargetBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		mMSAARenderTargetBufferDesc.Alignment = 0;
		mMSAARenderTargetBufferDesc.Width = width;
		mMSAARenderTargetBufferDesc.Height = height;
		mMSAARenderTargetBufferDesc.DepthOrArraySize = 1;
		mMSAARenderTargetBufferDesc.MipLevels = 1;
		mMSAARenderTargetBufferDesc.Format = mSwapChain.GetBackBufferFormat();
		mMSAARenderTargetBufferDesc.SampleDesc.Count = 4;
		mMSAARenderTargetBufferDesc.SampleDesc.Quality = 0;
		mMSAARenderTargetBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		mMSAARenderTargetBufferDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

		D3D12_CLEAR_VALUE msaaRTBufferClearValue{};
		msaaRTBufferClearValue.Format = mSwapChain.GetBackBufferFormat();
		msaaRTBufferClearValue.Color[0] = 0.0f;
		msaaRTBufferClearValue.Color[1] = 0.0f;
		msaaRTBufferClearValue.Color[2] = 0.0f;
		msaaRTBufferClearValue.Color[3] = 1.0f;

		//Use this class to say which type of heap our buffer will be stored in.
		CD3DX12_HEAP_PROPERTIES msaaRTHeapProp(D3D12_HEAP_TYPE_DEFAULT);

		//Create the RT buffer resouce
		ThrowIfFailed(mDirect3DDevice->CreateCommittedResource(&msaaRTHeapProp, D3D12_HEAP_FLAG_NONE, &mMSAARenderTargetBufferDesc,
			D3D12_RESOURCE_STATE_RESOLVE_SOURCE, &msaaRTBufferClearValue, IID_PPV_ARGS(&mMSAARenderTargetBuffer)));

		//Create the RTV
		mDirect3DDevice->CreateRenderTargetView(mMSAARenderTargetBuffer.Get(), nullptr,
			mMSAARTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	}

	void DeviceResources::mCreateMSAADepthStencilBufferAndView(int width, int height)
	{
		D3D12_RESOURCE_DESC mMSAADepthStencilBufferDesc{};
		mMSAADepthStencilBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		mMSAADepthStencilBufferDesc.Alignment = 0;
		mMSAADepthStencilBufferDesc.Width = width;
		mMSAADepthStencilBufferDesc.Height = height;
		mMSAADepthStencilBufferDesc.DepthOrArraySize = 1;
		mMSAADepthStencilBufferDesc.MipLevels = 1;
		mMSAADepthStencilBufferDesc.Format = mDepthStencil.GetDepthStencilFormat();
		mMSAADepthStencilBufferDesc.SampleDesc.Count = 4;
		mMSAADepthStencilBufferDesc.SampleDesc.Quality = 0;
		mMSAADepthStencilBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		mMSAADepthStencilBufferDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		D3D12_CLEAR_VALUE msaaDSBufferClearValue{};
		msaaDSBufferClearValue.Format = mDepthStencil.GetDepthStencilFormat();
		msaaDSBufferClearValue.DepthStencil.Depth = 1.0f;
		msaaDSBufferClearValue.DepthStencil.Stencil = 0;

		//Use this class to say which type of heap our buffer will be stored in.
		CD3DX12_HEAP_PROPERTIES msaaDSHeapProp(D3D12_HEAP_TYPE_DEFAULT);

		//Create the msaa DS buffer resouce
		ThrowIfFailed(mDirect3DDevice->CreateCommittedResource(&msaaDSHeapProp, D3D12_HEAP_FLAG_NONE, &mMSAADepthStencilBufferDesc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE, &msaaDSBufferClearValue, IID_PPV_ARGS(&mMSAADepthStencilBuffer)));

		//Describe the msaa DSV
		D3D12_DEPTH_STENCIL_VIEW_DESC dsViewDescription{};
		dsViewDescription.Format = mDepthStencil.GetDepthStencilFormat();
		dsViewDescription.Flags = D3D12_DSV_FLAG_NONE;
		dsViewDescription.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DMS;

		//Create the msaa DSV
		mDirect3DDevice->CreateDepthStencilView(mMSAADepthStencilBuffer.Get(), &dsViewDescription,
			mMSAADSVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	}*/
	//-----------------------------------------------------------------------------------------------------------------------
}