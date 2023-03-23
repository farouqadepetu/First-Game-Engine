#include "FASwapChain.h"
#include "FADirectXException.h"

namespace FARender
{
	SwapChain::SwapChain(const Microsoft::WRL::ComPtr<ID3D12Device>& device, const Microsoft::WRL::ComPtr<IDXGIFactory4>& dxgiFactory,
		const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& commandQueue, HWND windowHandle, unsigned int numBuffers) : 
		mBackBufferFormat{ DXGI_FORMAT_R8G8B8A8_UNORM }, mNumOfSwapChainBuffers{ numBuffers }, mCurrentBackBufferIndex{ 0 }
	{
		mRTVSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

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

		ThrowIfFailed(dxgiFactory->CreateSwapChainForHwnd(commandQueue.Get(), windowHandle,
			&swapChainDescription, nullptr, nullptr, &mSwapChain));

		mSwapChainBuffers.resize(mNumOfSwapChainBuffers);

		//Create render target view heap to store descriptions of the render target buffers.
		D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDescription{};
		rtvDescriptorHeapDescription.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvDescriptorHeapDescription.NumDescriptors = mNumOfSwapChainBuffers;
		rtvDescriptorHeapDescription.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		rtvDescriptorHeapDescription.NodeMask = 0;
		ThrowIfFailed(device->CreateDescriptorHeap(&rtvDescriptorHeapDescription, IID_PPV_ARGS(&mRTVHeap)));
	}

	const Microsoft::WRL::ComPtr<ID3D12Resource>* SwapChain::GetSwapChainBuffers() const
	{
		return mSwapChainBuffers.data();
	}

	const Microsoft::WRL::ComPtr<ID3D12Resource>& SwapChain::GetCurrentBackBuffer() const
	{
		return mSwapChainBuffers[mCurrentBackBufferIndex];
	}

	CD3DX12_CPU_DESCRIPTOR_HANDLE SwapChain::GetCurrentBackBufferView() const
	{
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(mRTVHeap->GetCPUDescriptorHandleForHeapStart(),
			mCurrentBackBufferIndex, mRTVSize);
	}

	unsigned int SwapChain::GetNumOfSwapChainBuffers() const
	{
		return mNumOfSwapChainBuffers;
	}

	unsigned int SwapChain::GetCurrentBackBufferIndex() const
	{
		return mCurrentBackBufferIndex;
	}

	DXGI_FORMAT SwapChain::GetBackBufferFormat() const
	{
		return mBackBufferFormat;
	}

	void SwapChain::ResetBuffers()
	{
		for (auto& i : mSwapChainBuffers)
		{
			i.Reset();
		}
	}

	void SwapChain::ResizeSwapChain(unsigned width, unsigned height)
	{
		mSwapChain->ResizeBuffers(mNumOfSwapChainBuffers, width, height, mBackBufferFormat,
			DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
		mCurrentBackBufferIndex = 0;
	}

	void SwapChain::CreateRenderTargetBuffersAndViews(const Microsoft::WRL::ComPtr<ID3D12Device>& device)
	{
		//This is a class that stores the address of the first element in the rtv descriptor heap.
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(mRTVHeap->GetCPUDescriptorHandleForHeapStart());

		for (unsigned int i = 0; i < mNumOfSwapChainBuffers; ++i)
		{
			//store the swap chain buffer in a Resource object.
			ThrowIfFailed(mSwapChain->GetBuffer(i, IID_PPV_ARGS(mSwapChainBuffers[i].GetAddressOf())));

			//Create and store a description to the swap chain buffer in the RTV heap.
			device->CreateRenderTargetView(mSwapChainBuffers[i].Get(), nullptr, rtvHeapHandle.Offset(i, mRTVSize));
		}
	}

	void SwapChain::ClearCurrentBackBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
		const float* backBufferClearValue)
	{
		//Get the address of the view to the current back buffer
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(mRTVHeap->GetCPUDescriptorHandleForHeapStart(),
			mCurrentBackBufferIndex, mRTVSize);

		//Clear the current back buffer.
		commandList->ClearRenderTargetView(rtvHeapHandle, backBufferClearValue, 0, nullptr);
	}

	void SwapChain::Transition(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
		D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
	{
		CD3DX12_RESOURCE_BARRIER currentBackBufferTransition =
			CD3DX12_RESOURCE_BARRIER::Transition(mSwapChainBuffers[mCurrentBackBufferIndex].Get(),
				before, after);

		commandList->ResourceBarrier(1, &currentBackBufferTransition);
	}

	void SwapChain::Present()
	{
		//swap the front and back buffers
		ThrowIfFailed(mSwapChain->Present(0, 0));
		mCurrentBackBufferIndex = (mCurrentBackBufferIndex + 1) % mNumOfSwapChainBuffers;
	}
}