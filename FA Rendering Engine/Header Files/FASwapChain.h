#pragma once

#include <wrl.h>
#include "d3dx12.h"
#include <dxgi1_4.h>
#include <vector>

namespace FARender
{
	/** @class SwapChain ""
	*	@brief A wrapper for swap chain resources. Uses DirectD 12 and DXGI.
	*/
	class SwapChain
	{
	public:

		SwapChain() = default;

		/**@brief Constructor.
		* Creates swap chain and render target view heap.
		*/
		SwapChain(const Microsoft::WRL::ComPtr<ID3D12Device>& device, const Microsoft::WRL::ComPtr<IDXGIFactory4>& dxgiFactory,
			const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& commandQueue, HWND windowHandle, unsigned int numBuffers);

		/**@brief Returns a constant pointer to the render target buffers.
		*/
		const Microsoft::WRL::ComPtr<ID3D12Resource>* GetSwapChainBuffers() const;

		/**@brief Returns a constant reference to the current render target buffer.
		*/
		const Microsoft::WRL::ComPtr<ID3D12Resource>& GetCurrentBackBuffer() const;

		/**@brief Returns the address of the view to the current render target buffer.
		*/
		CD3DX12_CPU_DESCRIPTOR_HANDLE GetCurrentBackBufferView(unsigned int rtvSize) const;

		/**@brief Returns the number of swap chain buffers.
		*/
		unsigned int GetNumOfSwapChainBuffers() const;

		/**@brief Returns the current back buffer index.
		*/
		unsigned int GetCurrentBackBufferIndex() const;

		/**@brief Returns the format of the swap chain.
		*/
		DXGI_FORMAT GetBackBufferFormat() const;

		/**@brief The render target buffers no longer reference the swap chain buffers after this function is executed.
		*/
		void ResetBuffers();

		/**@brief Resizes the swap chain.
		*/
		void ResizeSwapChain(unsigned width, unsigned height);

		/**@brief Creates the render target buffer and views to them.
		*/
		void CreateRenderTargetBuffersAndViews(const Microsoft::WRL::ComPtr<ID3D12Device>& device, unsigned int rtvSize);

		/**@brief Clears the current render target buffer.
		*/
		void ClearCurrentBackBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
			unsigned int rtvSize, const float* backBufferClearValue);

		/**@brief Transitions the current render target buffer from the specified before state to the specified after state.
		*/
		void Transition(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
			D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);

		/**@brief Swaps the front and back buffers.
		*/
		void Present();

	private:

		DXGI_FORMAT mBackBufferFormat;

		unsigned int mNumOfSwapChainBuffers;
		unsigned int mCurrentBackBufferIndex;

		Microsoft::WRL::ComPtr<IDXGISwapChain1> mSwapChain;
		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> mSwapChainBuffers;

		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mRTVHeap;
	};
}