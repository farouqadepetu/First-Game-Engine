#include "SwapChain.h"
#include "DirectXException.h"

namespace RenderingEngine
{
	SwapChain::SwapChain() : mNumRenderTargetBuffers{ 0 }, mCurrentBackBufferIndex{ 0 }
	{}

	SwapChain::SwapChain(const Microsoft::WRL::ComPtr<IDXGIFactory4>& dxgiFactory,
		const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& commandQueue, HWND windowHandle,
		DXGI_FORMAT rtFormat, DXGI_FORMAT dsFormat, unsigned int numRenderTargetBuffers)
	{
		CreateSwapChain(dxgiFactory, commandQueue, windowHandle, rtFormat, dsFormat, numRenderTargetBuffers);
	}

	void SwapChain::CreateSwapChain(const Microsoft::WRL::ComPtr<IDXGIFactory4>& dxgiFactory,
		const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& commandQueue, HWND windowHandle,
		DXGI_FORMAT rtFormat, DXGI_FORMAT dsFormat, unsigned int numRenderTargetBuffers)
	{
		mSwapChain.Reset();

		//This describes the multi-sampling parameters we want
		DXGI_SAMPLE_DESC multiSamplingDescription{};
		multiSamplingDescription.Count = 1;
		multiSamplingDescription.Quality = 0;

		DXGI_SWAP_CHAIN_DESC1 swapChainDescription{};
		swapChainDescription.Width = 0;
		swapChainDescription.Height = 0;
		swapChainDescription.Format = rtFormat;
		swapChainDescription.Stereo = FALSE;
		swapChainDescription.SampleDesc = multiSamplingDescription;
		swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDescription.BufferCount = numRenderTargetBuffers;
		swapChainDescription.Scaling = DXGI_SCALING_NONE;
		swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDescription.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		swapChainDescription.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		ExitIfFailed(dxgiFactory->CreateSwapChainForHwnd(commandQueue.Get(), windowHandle,
			&swapChainDescription, nullptr, nullptr, &mSwapChain));

		for (unsigned int i = 0; i < numRenderTargetBuffers; ++i)
		{
			mRenderTargetBuffers.push_back(std::make_unique<RenderTargetBuffer>());
		}

		mNumRenderTargetBuffers = numRenderTargetBuffers;
		mCurrentBackBufferIndex = 0;
	}

	const Microsoft::WRL::ComPtr<ID3D12Resource>& SwapChain::GetCurrentBackBuffer() const
	{
		return mRenderTargetBuffers[mCurrentBackBufferIndex]->GetRenderTargetBuffer();
	}

	unsigned int SwapChain::GetNumRenderTargetBuffers() const
	{
		return mNumRenderTargetBuffers;
	}

	unsigned int SwapChain::GetCurrentBackBufferIndex() const
	{
		return mCurrentBackBufferIndex;
	}

	DXGI_FORMAT SwapChain::GetBackBufferFormat() const
	{
		DXGI_SWAP_CHAIN_DESC1 swapChainDescription{};
		mSwapChain->GetDesc1(&swapChainDescription);

		return swapChainDescription.Format;
	}

	DXGI_FORMAT SwapChain::GetDepthStencilFormat() const
	{
		return mDepthStencilBuffer.GetDepthStencilFormat();
	}

	void SwapChain::ReleaseBuffers()
	{
		for (auto& i : mRenderTargetBuffers)
		{
			i->ReleaseBuffer();
		}

		mDepthStencilBuffer.ReleaseBuffer();
	}

	const std::vector<std::unique_ptr<RenderTargetBuffer>>& SwapChain::GetRenderTargetBuffers()
	{
		return mRenderTargetBuffers;
	}

	void SwapChain::CreateRenderTargetBuffersAndViews(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
		const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& rtvHeap, unsigned int index,
		unsigned int rtvSize, unsigned width, unsigned height)
	{
		//Resize the swap chain to the specified width and height
		mSwapChain->ResizeBuffers(mNumRenderTargetBuffers, width, height, GetBackBufferFormat(),
			DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

		mCurrentBackBufferIndex = 0;

		//Get the address of where you want to store the first view in the RTV heap.
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(rtvHeap->GetCPUDescriptorHandleForHeapStart(), index,
			rtvSize);

		for (unsigned int i = 0; i < mNumRenderTargetBuffers; ++i)
		{
			//store the swap chain buffer in a render target buffer.
			ExitIfFailed(mSwapChain->GetBuffer(i, IID_PPV_ARGS(mRenderTargetBuffers[i]->GetRenderTargetBuffer().GetAddressOf())));

			//Create and store a description of the render target buffer in the RTV heap.
			device->CreateRenderTargetView(mRenderTargetBuffers[i]->GetRenderTargetBuffer().Get(), nullptr,
				rtvHeapHandle.Offset(i, rtvSize));
		}
	}

	void SwapChain::CreateDepthStencilBufferAndView(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
		const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& dsvHeap, unsigned int indexOfWhereToStoreView, unsigned int dsvSize,
		unsigned int width, unsigned int height)
	{
		mDepthStencilBuffer.CreateDepthStencilBufferAndView(device, dsvHeap, indexOfWhereToStoreView, dsvSize, width, height);
	}

	void SwapChain::ClearCurrentBackBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
		const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& rtvHeap, unsigned int indexOfFirstView, unsigned int rtvSize,
		const float* backBufferClearValue)
	{
		//Get the address of the first swap chain buffer view in the heap.
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(rtvHeap->GetCPUDescriptorHandleForHeapStart(), indexOfFirstView, rtvSize);

		//offset to the current back buffer.
		rtvHeapHandle.Offset(mCurrentBackBufferIndex, rtvSize);

		//Clear the current back buffer.
		commandList->ClearRenderTargetView(rtvHeapHandle, backBufferClearValue, 0, nullptr);
	}

	void SwapChain::ClearDepthStencilBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
		const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& dsvHeap, unsigned int indexOfView, unsigned int dsvSize,
		float clearValue)
	{
		mDepthStencilBuffer.ClearDepthStencilBuffer(commandList, dsvHeap, indexOfView, dsvSize, clearValue);
	}

	void SwapChain::Transition(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
		D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
	{
		CD3DX12_RESOURCE_BARRIER currentBackBufferTransition =
			CD3DX12_RESOURCE_BARRIER::Transition(mRenderTargetBuffers[mCurrentBackBufferIndex]->GetRenderTargetBuffer().Get(),
				before, after);

		commandList->ResourceBarrier(1, &currentBackBufferTransition);
	}

	void SwapChain::Present()
	{
		//swap the front and back buffers
		ExitIfFailed(mSwapChain->Present(0, 0));
		mCurrentBackBufferIndex = (mCurrentBackBufferIndex + 1) % mNumRenderTargetBuffers;
	}
}