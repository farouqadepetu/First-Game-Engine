#include "FADeviceResources.h"
#include "FADirectXException.h"
#include <d3dx12.h>

namespace FARender
{
	//-----------------------------------------------------------------------------------------------------------------------
	//DEVICE RESOURCES FUNCTION DEFINITIONS

	DeviceResources::DeviceResources()
	{}

	DeviceResources::DeviceResources(unsigned int width, unsigned int height, HWND windowHandle)
	{
		initializeDirect3D(width, height, windowHandle);
	}

	DeviceResources::~DeviceResources()
	{
		if (mDirect3DDevice != nullptr)
			flushCommandQueue();
	}

	const Microsoft::WRL::ComPtr<ID3D12Device>& DeviceResources::device() const
	{
		return mDirect3DDevice;
	}

	const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& DeviceResources::commandQueue() const
	{
		return mCommandQueue;
	}

	const Microsoft::WRL::ComPtr<ID3D12CommandAllocator>& DeviceResources::commandAllocator() const
	{
		return mCommandAllocator[currentFrame];
	}

	const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& DeviceResources::commandList() const
	{
		return mCommandList;
	}

	const DXGI_FORMAT& DeviceResources::backBufferFormat() const
	{
		return mBackBufferFormat;
	}

	const UINT DeviceResources::numOfSwapChainBuffers() const
	{
		return mNumOfSwapChainBuffers;
	}

	const Microsoft::WRL::ComPtr<IDXGISwapChain1>& DeviceResources::swapChain() const
	{
		return mSwapChain;
	}

	const UINT& DeviceResources::rtvDescriptorSize() const
	{
		return mRTVSize;
	}

	const UINT& DeviceResources::dsvDescriptorSize() const
	{
		return mDSVSize;
	}

	const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& DeviceResources::rtvDescriptorHeap() const
	{
		return mRTVHeap;
	}

	const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& DeviceResources::dsvDescriptorHeap() const
	{
		return mDSVHeap;
	}

	const D3D12_VIEWPORT& DeviceResources::viewport() const
	{
		return mViewport;
	}

	const D3D12_RECT& DeviceResources::scissor() const
	{
		return mScissor;
	}

	const Microsoft::WRL::ComPtr<ID3D12Resource>* DeviceResources::swapChainBuffers() const
	{
		return mSwapChainBuffers;
	}

	const UINT& DeviceResources::currentBackBuffer() const
	{
		return mCurrentBackBuffer;
	}

	const Microsoft::WRL::ComPtr<ID3D12Resource>& DeviceResources::depthStencilBuffer() const
	{
		return mDepthStencilBuffer;
	}

	const DXGI_FORMAT& DeviceResources::depthStencilFormat() const
	{
		return mDepthStencilFormat;
	}

	bool& DeviceResources::isMSAAEnabled()
	{
		return mIsMSAAEnabled;
	}

	const bool& DeviceResources::isMSAAEnabled() const
	{
		return mIsMSAAEnabled;
	}

	UINT& DeviceResources::sampleCount()
	{
		return mSampleCount;
	}

	const UINT& DeviceResources::sampleCount() const
	{
		return mSampleCount;
	}

	UINT64& DeviceResources::currentFenceValue()
	{
		return mFenceValue;
	}

	const UINT64& DeviceResources::currentFenceValue() const
	{
		return mFenceValue;
	}


	const Microsoft::WRL::ComPtr<ID2D1DeviceContext>& DeviceResources::device2DContext() const
	{
		return mDirect2DDeviceContext;
	}

	const Microsoft::WRL::ComPtr<IDWriteFactory>& DeviceResources::directWriteFactory() const
	{
		return mDirectWriteFactory;
	}

	void DeviceResources::updateCurrentFrameFenceValue()
	{
		mCurrentFrameFenceValue[currentFrame] = ++mFenceValue;
	}

	void  DeviceResources::initializeDirect3D(unsigned int width, unsigned int height, HWND handle)
	{
		enableDebugLayer();
		createDirect3DDevice();
		createDXGIFactory();
		createFence();
		queryDescriptorSizes();
		createCommandObjects();
		createSwapChain(handle);
		createRTVHeap();
		createDSVHeap();

		initializeText();

		checkMSAASupport();
		createMSAARTVHeap();
		createMSAADSVHeap();

		resize(width, height, handle);
		mCommandList->Reset(mDirectCommandAllocator.Get(), nullptr);
	}

	void DeviceResources::enableDebugLayer()
	{
		//enables the debug layer for additional information
#if defined(_DEBUG) || defined(DEBUG)
		Microsoft::WRL::ComPtr<ID3D12Debug> debug;
		ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debug)));
		debug->EnableDebugLayer();
#endif
	}

	void DeviceResources::createDirect3DDevice()
	{
		ThrowIfFailed(D3D12CreateDevice(0, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&mDirect3DDevice)));
	}

	void DeviceResources::createDXGIFactory()
	{
		UINT flag = 0;
#if defined(_DEBUG) || defined(DEBUG)
		flag = DXGI_CREATE_FACTORY_DEBUG;
#endif
		ThrowIfFailed(CreateDXGIFactory2(flag, IID_PPV_ARGS(&mDXGIFactory)));
	}

	void DeviceResources::createFence()
	{
		ThrowIfFailed(mDirect3DDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence)));
	}

	void DeviceResources::queryDescriptorSizes()
	{
		mRTVSize = mDirect3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		mDSVSize = mDirect3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	}

	void DeviceResources::createCommandObjects()
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

		for (UINT i = 0; i < numFrames; ++i)
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

	void DeviceResources::createSwapChain(HWND handle)
	{
		mSwapChain.Reset();

		//This describes the multi-sampling parameters we want
		DXGI_SAMPLE_DESC multiSamplingDescription{};
		multiSamplingDescription.Count = 1;
		multiSamplingDescription.Quality = 0;

		DXGI_SWAP_CHAIN_DESC1 swapChainDescription{};
		swapChainDescription.Width = 0;
		swapChainDescription.Height = 0;
		swapChainDescription.Format = mBackBufferFormat;
		swapChainDescription.Stereo = FALSE;
		swapChainDescription.SampleDesc = multiSamplingDescription;
		swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDescription.BufferCount = mNumOfSwapChainBuffers;
		swapChainDescription.Scaling = DXGI_SCALING_NONE;
		swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDescription.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		swapChainDescription.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		ThrowIfFailed(mDXGIFactory->CreateSwapChainForHwnd(mCommandQueue.Get(), handle,
			&swapChainDescription, nullptr, nullptr, &mSwapChain));
	}

	void DeviceResources::createRTVHeap()
	{
		D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDescription{};
		rtvDescriptorHeapDescription.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvDescriptorHeapDescription.NumDescriptors = mNumOfSwapChainBuffers;
		rtvDescriptorHeapDescription.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		rtvDescriptorHeapDescription.NodeMask = 0;
		ThrowIfFailed(mDirect3DDevice->CreateDescriptorHeap(&rtvDescriptorHeapDescription, IID_PPV_ARGS(&mRTVHeap)));
	}

	void DeviceResources::createDSVHeap()
	{
		D3D12_DESCRIPTOR_HEAP_DESC dsvDescriptorHeapDescription{};
		dsvDescriptorHeapDescription.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		dsvDescriptorHeapDescription.NumDescriptors = 1;
		dsvDescriptorHeapDescription.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		dsvDescriptorHeapDescription.NodeMask = 0;
		ThrowIfFailed(mDirect3DDevice->CreateDescriptorHeap(&dsvDescriptorHeapDescription, IID_PPV_ARGS(&mDSVHeap)));
	}

	void DeviceResources::createRenderTargetBufferAndView()
	{
		//Create Render Target View (Descriptor)

		//store the first swap chain buffer in a Resource object.
		ThrowIfFailed(mSwapChain->GetBuffer(0, IID_PPV_ARGS(mSwapChainBuffers[0].GetAddressOf())));
		//store the second swap chain buffer in a Resource object.
		ThrowIfFailed(mSwapChain->GetBuffer(1, IID_PPV_ARGS(mSwapChainBuffers[1].GetAddressOf())));

		//This is a class that stores the address of the first element in the rtv descriptor heap.
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(mRTVHeap->GetCPUDescriptorHandleForHeapStart());

		mDirect3DDevice->CreateRenderTargetView(mSwapChainBuffers[0].Get(), nullptr, rtvHeapHandle);

		//rtvHeapHandle.Offset(1, rtvDescriptorSize) this function will give me the address of any descriptor in the descriptor heap
		mDirect3DDevice->CreateRenderTargetView(mSwapChainBuffers[1].Get(), nullptr, rtvHeapHandle.Offset(1, mRTVSize));
	}

	void DeviceResources::createDepthStencilBufferAndView(int width, int height)
	{
		//Create Depth/Stenicl Buffer and View (Descriptor)

		D3D12_RESOURCE_DESC depthBufferDescription{};
		depthBufferDescription.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		depthBufferDescription.Alignment = 0;
		depthBufferDescription.Width = width;
		depthBufferDescription.Height = height;
		depthBufferDescription.DepthOrArraySize = 1;
		depthBufferDescription.MipLevels = 1;
		depthBufferDescription.Format = mDepthStencilFormat;
		depthBufferDescription.SampleDesc.Count = 1;
		depthBufferDescription.SampleDesc.Quality = 0;
		depthBufferDescription.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		depthBufferDescription.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		D3D12_CLEAR_VALUE depthBufferClearValue{};
		depthBufferClearValue.Format = mDepthStencilFormat;
		depthBufferClearValue.DepthStencil.Depth = 1.0f;
		depthBufferClearValue.DepthStencil.Stencil = 0;

		//Use this class to say which type of heap our buffer will be stored in.
		CD3DX12_HEAP_PROPERTIES dHeapProp(D3D12_HEAP_TYPE_DEFAULT);

		ThrowIfFailed(mDirect3DDevice->CreateCommittedResource(&dHeapProp, D3D12_HEAP_FLAG_NONE, &depthBufferDescription,
			D3D12_RESOURCE_STATE_DEPTH_WRITE, &depthBufferClearValue, IID_PPV_ARGS(&mDepthStencilBuffer)));

		D3D12_DEPTH_STENCIL_VIEW_DESC depthBufferViewDescription{};
		depthBufferViewDescription.Format = mDepthStencilFormat;
		depthBufferViewDescription.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		depthBufferViewDescription.Flags = D3D12_DSV_FLAG_NONE;
		depthBufferViewDescription.Texture2D.MipSlice = 0;

		mDirect3DDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), &depthBufferViewDescription,
			mDSVHeap->GetCPUDescriptorHandleForHeapStart());
	}

	void DeviceResources::flushCommandQueue()
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

	void DeviceResources::waitForGPU() const
	{
		//if the signal command has not been executed, wait until it is
		if (mCurrentFrameFenceValue[currentFrame] != 0 &&
			mFence->GetCompletedValue() < mCurrentFrameFenceValue[currentFrame])
		{
			//create an event object and store an handle to it
			HANDLE eventHandle{ CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS) };

			if (eventHandle != nullptr)
			{
				//This function will fire(raise) an event when the signal command is executed by the GPU
				ThrowIfFailed(mFence->SetEventOnCompletion(mCurrentFrameFenceValue[currentFrame], eventHandle));

				//Wait until the GPU has executed the signal command
				WaitForSingleObject(eventHandle, INFINITE);

				//Closes the event handle
				CloseHandle(eventHandle);
			}
		}
	}

	void DeviceResources::signal()
	{
		mCommandQueue->Signal(mFence.Get(), mFenceValue);
	}

	void DeviceResources::resize(int width, int height, const HWND& handle)
	{
		//make sure all of the commands in the command list have been executed before adding new commands.
		flushCommandQueue();

		//reset the command list to add new commands
		ThrowIfFailed(mCommandList->Reset(mDirectCommandAllocator.Get(), nullptr));

		resetTextBuffers();

		//Reset/Release all buffers that have a reference to the swap chain.
		mSwapChainBuffers[0].Reset();
		mSwapChainBuffers[1].Reset();
		mDepthStencilBuffer.Reset();

		//reset MSAA buffers
		if (mMSAA4xSupported && mIsMSAAEnabled)
		{
			mMSAARenderTargetBuffer.Reset();
			mMSAADepthStencilBuffer.Reset();
		}

		//Resize the swap chain buffers
		mSwapChain->ResizeBuffers(mNumOfSwapChainBuffers, width, height, mBackBufferFormat,
			DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
		mCurrentBackBuffer = 0;

		//Make RT buffers, DS buffer, RT view and DS view
		createRenderTargetBufferAndView();
		createDepthStencilBufferAndView(width, height);

		if (mMSAA4xSupported && mIsMSAAEnabled)
		{
			//Make MSAA RT buffer, MSAA DS buffer, MSAA RT view and MSAA DS view
			createMSAARenderTargetBufferAndView(width, height);
			createMSAADepthStencilBufferAndView(width, height);
		}

		textResize(handle);

		//Close the command list.
		//Store all your command lists in a ID3D12CommandList array.
		//Execute the resize commands.
		ThrowIfFailed(mCommandList->Close());
		ID3D12CommandList* commandLists[] = { mCommandList.Get() };
		UINT s = sizeof(commandLists) / sizeof(commandLists[0]);
		mCommandQueue->ExecuteCommandLists(s, commandLists);

		//make sure all of the commands in the command list have been executed before adding new commands.
		flushCommandQueue();

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

	void DeviceResources::resetCommandList()
	{
		mCommandList->Reset(mCommandAllocator[currentFrame].Get(), nullptr);
	}

	void DeviceResources::resetCommandAllocator()
	{
		mCommandAllocator[currentFrame].Reset();
	}

	void DeviceResources::resetDirectCommandList()
	{
		mCommandList->Reset(mDirectCommandAllocator.Get(), nullptr);
	}

	void DeviceResources::execute() const
	{
		ThrowIfFailed(mCommandList->Close());
		ID3D12CommandList* commandLists[] = { mCommandList.Get() };
		UINT s = sizeof(commandLists) / sizeof(commandLists[0]);
		mCommandQueue->ExecuteCommandLists(s, commandLists);
	}

	void DeviceResources::present()
	{
		//swap the front and back buffers
		ThrowIfFailed(swapChain()->Present(0, 0));
		mCurrentBackBuffer = (mCurrentBackBuffer + 1) % mNumOfSwapChainBuffers;
	}

	void DeviceResources::draw()
	{
		//Reseting command allocator allows us to reuse the memory.
		//Make sure all the commands in the command list is executed before calling this.
		ThrowIfFailed(mCommandAllocator[currentFrame]->Reset());

		//Reset command list
		ThrowIfFailed(mCommandList->Reset(mCommandAllocator[currentFrame].Get(), nullptr));

		//Link viewport to the rasterization stage
		mCommandList->RSSetViewports(1, &mViewport);

		//Link scissor rectangle to the rasterization stage
		mCommandList->RSSetScissorRects(1, &mScissor);

		if (mMSAA4xSupported && mIsMSAAEnabled)
		{
			//Transistion the current back buffer to resolve dest state from present state
			CD3DX12_RESOURCE_BARRIER currentBackBufferTransitionToResolveDestState =
				CD3DX12_RESOURCE_BARRIER::Transition(mSwapChainBuffers[mCurrentBackBuffer].Get(),
					D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RESOLVE_DEST);

			mCommandList->ResourceBarrier(1, &currentBackBufferTransitionToResolveDestState);

			//Transistion the msaa RT buffer to render target state from resolve source
			CD3DX12_RESOURCE_BARRIER msaaRTVTransitionToRenderTarget =
				CD3DX12_RESOURCE_BARRIER::Transition(mMSAARenderTargetBuffer.Get(),
					D3D12_RESOURCE_STATE_RESOLVE_SOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);

			mCommandList->ResourceBarrier(1, &msaaRTVTransitionToRenderTarget);

			//clear the msaa RT buffer
			const float msaaRTClearValue[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
			CD3DX12_CPU_DESCRIPTOR_HANDLE msaaRTVHeapHandle(mMSAARTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
				0, mRTVSize);
			mCommandList->ClearRenderTargetView(msaaRTVHeapHandle, msaaRTClearValue, 0, nullptr);

			//clear the msaa DS buffer
			mCommandList->ClearDepthStencilView(mMSAADSVDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
				D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

			//Specify which back buffer to render to
			CD3DX12_CPU_DESCRIPTOR_HANDLE msaaDSVHeapHandle(mMSAADSVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
			mCommandList->OMSetRenderTargets(1, &msaaRTVHeapHandle, true, &msaaDSVHeapHandle);
		}
		else
		{
			//Transistion the current back buffer to  render state from present state
			CD3DX12_RESOURCE_BARRIER currentBackBufferTransitionToRenderState =
				CD3DX12_RESOURCE_BARRIER::Transition(mSwapChainBuffers[mCurrentBackBuffer].Get(),
					D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

			mCommandList->ResourceBarrier(1, &currentBackBufferTransitionToRenderState);

			//clear the back buffer
			const float backBufferClearValue[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
			CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(mRTVHeap->GetCPUDescriptorHandleForHeapStart(),
				mCurrentBackBuffer, mRTVSize);
			mCommandList->ClearRenderTargetView(rtvHeapHandle, backBufferClearValue, 0, nullptr);

			//clear the DS buffer
			mCommandList->ClearDepthStencilView(mDSVHeap->GetCPUDescriptorHandleForHeapStart(),
				D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

			//Specify which back buffer to render to
			CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHeapHandle(mDSVHeap->GetCPUDescriptorHandleForHeapStart());
			mCommandList->OMSetRenderTargets(1, &rtvHeapHandle, true, &dsvHeapHandle);
		}
	}

	void DeviceResources::rtBufferTransition(bool renderText)
	{
		if (mMSAA4xSupported && mIsMSAAEnabled)
		{
			//Transistion the msaa RT buffer to resolve source state from render target state
			CD3DX12_RESOURCE_BARRIER msaaRTVTransitionToResolveSourceState =
				CD3DX12_RESOURCE_BARRIER::Transition(mMSAARenderTargetBuffer.Get(),
					D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_RESOLVE_SOURCE);

			mCommandList->ResourceBarrier(1, &msaaRTVTransitionToResolveSourceState);

			//Copy a multi-sampled resource into a non-multi-sampled resource
			mCommandList->ResolveSubresource(mSwapChainBuffers[mCurrentBackBuffer].Get(), 0, mMSAARenderTargetBuffer.Get(),
				0, mBackBufferFormat);

			if (renderText)
			{
				//Transistion the current back buffer to render target state from resolve dest state
				CD3DX12_RESOURCE_BARRIER currentBackBufferTransitionToRenderTarget =
					CD3DX12_RESOURCE_BARRIER::Transition(mSwapChainBuffers[mCurrentBackBuffer].Get(),
						D3D12_RESOURCE_STATE_RESOLVE_DEST, D3D12_RESOURCE_STATE_RENDER_TARGET);

				mCommandList->ResourceBarrier(1, &currentBackBufferTransitionToRenderTarget);
			}
			else
			{
				//Transistion the current back buffer to present state from resolve dest state
				CD3DX12_RESOURCE_BARRIER currentBackBufferTransitionToRenderTarget =
					CD3DX12_RESOURCE_BARRIER::Transition(mSwapChainBuffers[mCurrentBackBuffer].Get(),
						D3D12_RESOURCE_STATE_RESOLVE_DEST, D3D12_RESOURCE_STATE_PRESENT);

				mCommandList->ResourceBarrier(1, &currentBackBufferTransitionToRenderTarget);
			}
		}
		else
		{
			if (!renderText)
			{
				//Transistion the current back buffer to present state from render target state
				CD3DX12_RESOURCE_BARRIER currentBackBufferTransitionToPresent =
					CD3DX12_RESOURCE_BARRIER::Transition(mSwapChainBuffers[mCurrentBackBuffer].Get(),
						D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

				mCommandList->ResourceBarrier(1, &currentBackBufferTransitionToPresent);
			}
		}
	}


	void DeviceResources::initializeText()
	{
		//-----------------------------------------------------------------------------------------------------------------------------
		//Create a D3D11On12 Device

		ThrowIfFailed(D3D11On12CreateDevice(mDirect3DDevice.Get(),
			D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT,
			nullptr, 0, (IUnknown**)mCommandQueue.GetAddressOf(), 1, 0, &mDevice11, &mDevice11Context, nullptr));

		//Query the 11On12 device from the 11 device.
		ThrowIfFailed(mDevice11.As(&mDevice11on12));
		//-----------------------------------------------------------------------------------------------------------------------------


		//-----------------------------------------------------------------------------------------------------------------------------
		//Create Direct2D factory, Direct2D device, Direct2D device context and DirectWrite factory.

		D2D1_FACTORY_OPTIONS factoryOptions{ D2D1_DEBUG_LEVEL_INFORMATION };
		ThrowIfFailed(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, factoryOptions, mDirect2DFactory.GetAddressOf()));

		Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
		ThrowIfFailed(mDevice11on12.As(&dxgiDevice));

		ThrowIfFailed(mDirect2DFactory->CreateDevice(dxgiDevice.Get(), &mDirect2DDevice));
		ThrowIfFailed(mDirect2DDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &mDirect2DDeviceContext));

		ThrowIfFailed(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &mDirectWriteFactory));
		//-----------------------------------------------------------------------------------------------------------------------------

		//-----------------------------------------------------------------------------------------------------------------------------
		//Resize the buffers to match the number of swap chain buffers

		mWrappedBuffers.resize(mNumOfSwapChainBuffers);
		mDirect2DBuffers.resize(mNumOfSwapChainBuffers);
		mSurfaces.resize(mNumOfSwapChainBuffers);

		//-----------------------------------------------------------------------------------------------------------------------------

	}

	void DeviceResources::resetTextBuffers()
	{
		mDirect2DDeviceContext->SetTarget(nullptr);

		for (int i = 0; i < mNumOfSwapChainBuffers; ++i)
		{
			mWrappedBuffers[i].Reset();
			mDirect2DBuffers[i].Reset();
			mSurfaces[i].Reset();
		}

		mDevice11Context->Flush();
	}

	void DeviceResources::textResize(const HWND& handle)
	{
		//-----------------------------------------------------------------------------------------------------------------------------
		//Create a wrapped 11on12 resource to the back buffers

		D3D11_RESOURCE_FLAGS direct11ResourceFlags{ D3D11_BIND_RENDER_TARGET };

		//creates a wrapped resource to each of our swap chain buffers
		for (int i = 0; i < mNumOfSwapChainBuffers; ++i)
		{
			ThrowIfFailed(mDevice11on12->CreateWrappedResource(mSwapChainBuffers[i].Get(), &direct11ResourceFlags,
				D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT, IID_PPV_ARGS(mWrappedBuffers[i].GetAddressOf())));
		}
		//-----------------------------------------------------------------------------------------------------------------------------


		//-----------------------------------------------------------------------------------------------------------------------------
		//Create a render target for D2D to draw directly to the back buffers.

		// Query the window's dpi settings, which will be used to create
		// D2D's render targets.
		float dpi = (float)GetDpiForWindow(handle);

		D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
			D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
			dpi, dpi);

		for (int i = 0; i < mNumOfSwapChainBuffers; ++i)
		{
			ThrowIfFailed(mWrappedBuffers[i].As(&mSurfaces[i]));

			ThrowIfFailed(mDirect2DDeviceContext->CreateBitmapFromDxgiSurface(mSurfaces[i].Get(), &bitmapProperties,
				mDirect2DBuffers[i].GetAddressOf()));
		}
		//-----------------------------------------------------------------------------------------------------------------------------
	}


	void DeviceResources::beforeTextDraw()
	{
		//gives direct2D access to our back buffer
		mDevice11on12->AcquireWrappedResources(mWrappedBuffers[mCurrentBackBuffer].GetAddressOf(), 1);

		//Render text to back buffer
		mDirect2DDeviceContext->SetTarget(mDirect2DBuffers[mCurrentBackBuffer].Get());
		mDirect2DDeviceContext->BeginDraw();
		mDirect2DDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
	}

	void DeviceResources::afterTextDraw()
	{
		ThrowIfFailed(mDirect2DDeviceContext->EndDraw());

		//Releasing the wrapped render target resource transitions the back buffer to present state.
		mDevice11on12->ReleaseWrappedResources(mWrappedBuffers[mCurrentBackBuffer].GetAddressOf(), 1);

		//Submit the commands to the shared D3D12 command queue.
		mDevice11Context->Flush();
	}

	void DeviceResources::checkMSAASupport()
	{
		//Describes the format and sample count we want to check to see if it is supported.
		D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS levels{};
		levels.Format = mBackBufferFormat;
		levels.SampleCount = mSampleCount;

		ThrowIfFailed(mDirect3DDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
			&levels, sizeof(levels)));

		if (levels.NumQualityLevels > 0)
			mMSAA4xSupported = true;
	}

	void DeviceResources::createMSAARTVHeap()
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

	void DeviceResources::createMSAADSVHeap()
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

	void DeviceResources::createMSAARenderTargetBufferAndView(int width, int height)
	{
		//mMSAARenderTargetBuffer.Reset();

		D3D12_RESOURCE_DESC mMSAARenderTargetBufferDesc{};
		mMSAARenderTargetBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		mMSAARenderTargetBufferDesc.Alignment = 0;
		mMSAARenderTargetBufferDesc.Width = width;
		mMSAARenderTargetBufferDesc.Height = height;
		mMSAARenderTargetBufferDesc.DepthOrArraySize = 1;
		mMSAARenderTargetBufferDesc.MipLevels = 1;
		mMSAARenderTargetBufferDesc.Format = mBackBufferFormat;
		mMSAARenderTargetBufferDesc.SampleDesc.Count = mSampleCount;
		mMSAARenderTargetBufferDesc.SampleDesc.Quality = 0;
		mMSAARenderTargetBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		mMSAARenderTargetBufferDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

		D3D12_CLEAR_VALUE msaaRTBufferClearValue{};
		msaaRTBufferClearValue.Format = mBackBufferFormat;
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

	void DeviceResources::createMSAADepthStencilBufferAndView(int width, int height)
	{
		//mMSAADepthStencilBuffer.Reset();

		D3D12_RESOURCE_DESC mMSAADepthStencilBufferDesc{};
		mMSAADepthStencilBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		mMSAADepthStencilBufferDesc.Alignment = 0;
		mMSAADepthStencilBufferDesc.Width = width;
		mMSAADepthStencilBufferDesc.Height = height;
		mMSAADepthStencilBufferDesc.DepthOrArraySize = 1;
		mMSAADepthStencilBufferDesc.MipLevels = 1;
		mMSAADepthStencilBufferDesc.Format = mDepthStencilFormat;
		mMSAADepthStencilBufferDesc.SampleDesc.Count = mSampleCount;
		mMSAADepthStencilBufferDesc.SampleDesc.Quality = 0;
		mMSAADepthStencilBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		mMSAADepthStencilBufferDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		D3D12_CLEAR_VALUE msaaDSBufferClearValue{};
		msaaDSBufferClearValue.Format = mDepthStencilFormat;
		msaaDSBufferClearValue.DepthStencil.Depth = 1.0f;
		msaaDSBufferClearValue.DepthStencil.Stencil = 0;

		//Use this class to say which type of heap our buffer will be stored in.
		CD3DX12_HEAP_PROPERTIES msaaDSHeapProp(D3D12_HEAP_TYPE_DEFAULT);

		//Create the msaa DS buffer resouce
		ThrowIfFailed(mDirect3DDevice->CreateCommittedResource(&msaaDSHeapProp, D3D12_HEAP_FLAG_NONE, &mMSAADepthStencilBufferDesc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE, &msaaDSBufferClearValue, IID_PPV_ARGS(&mMSAADepthStencilBuffer)));

		//Describe the msaa DSV
		D3D12_DEPTH_STENCIL_VIEW_DESC dsViewDescription{};
		dsViewDescription.Format = mDepthStencilFormat;
		dsViewDescription.Flags = D3D12_DSV_FLAG_NONE;
		dsViewDescription.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DMS;

		//Create the msaa DSV
		mDirect3DDevice->CreateDepthStencilView(mMSAADepthStencilBuffer.Get(), &dsViewDescription,
			mMSAADSVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	}
	//-----------------------------------------------------------------------------------------------------------------------
}