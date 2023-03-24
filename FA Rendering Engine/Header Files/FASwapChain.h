#pragma once

#include <wrl.h>
#include "d3dx12.h"
#include <dxgi1_4.h>
#include <vector>
#include "FABuffer.h"

namespace FARender
{
	/** @class SwapChain ""
	*	@brief A wrapper for swap chain resources. Uses DirectD 12 API and DXGI API.
	*/
	class SwapChain
	{
	public:

		SwapChain() = default;

		/**@brief Constructor.
		* Creates a swap chain.
		*/
		SwapChain(const Microsoft::WRL::ComPtr<IDXGIFactory4>& dxgiFactory,
			const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& commandQueue, HWND windowHandle, 
			DXGI_FORMAT rtFormat = DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT dsFormat = DXGI_FORMAT_D24_UNORM_S8_UINT,
			unsigned int numRenderTargetBuffers = 2);

		/**@brief Returns a constant pointer to the render target buffers.
		*/
		const RenderTargetBuffer* GetRenderTargetBuffers() const;

		/**@brief Returns a constant reference to the current render target buffer.
		*/
		const Microsoft::WRL::ComPtr<ID3D12Resource>& GetCurrentBackBuffer() const;

		/**@brief Returns the number of swap chain buffers.
		*/
		unsigned int GetNumRenderTargetBuffers() const;

		/**@brief Returns the current back buffer index.
		*/
		unsigned int GetCurrentBackBufferIndex() const;

		/**@brief Returns the format of the swap chain.
		*/
		DXGI_FORMAT GetBackBufferFormat() const;

		/**@brief Returns the format of the depth stencil buffer.
		*/
		DXGI_FORMAT GetDepthStencilFormat() const;

		/**@brief The render target buffers no longer reference the swap chain buffers after this function is executed.
		*/
		void ResetBuffers();

		/**@brief Resizes the swap chain.
		*/
		void ResizeSwapChain(unsigned width, unsigned height);

		/**@brief Creates the render target buffers and views to them.
		*/
		void CreateRenderTargetBuffersAndViews(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
			const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& rtvHeap, unsigned int indexOfWhereToStoreFirstView, 
			unsigned int rtvSize);

		/**@brief Creates the swap chains depth stencil buffer and view to it.
		*/
		void CreateDepthStencilBufferAndView(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
			const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& dsvHeap, unsigned int index, unsigned int dsvSize,
			unsigned int width, unsigned int height);

		/**@brief Clears the current render target buffer.
		*/
		void ClearCurrentBackBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
			const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& rtvHeap, unsigned int indexOfFirstView, unsigned int rtvSize,
			const float* backBufferClearValue);

		/**@brief Clears the swap chains depth stencil buffer with the specified clear value.
		*/
		void ClearDepthStencilBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
			const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& dsvHeap, unsigned int indexOfView, unsigned int dsvSize,
			float clearValue);

		/**@brief Transitions the current render target buffer from the specified before state to the specified after state.
		*/
		void Transition(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
			D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);

		/**@brief Swaps the front and back buffers.
		*/
		void Present();

	private:
		unsigned int mNumRenderTargetBuffers = 0;
		unsigned int mCurrentBackBufferIndex = 0;

		Microsoft::WRL::ComPtr<IDXGISwapChain1> mSwapChain;
		std::vector<RenderTargetBuffer> mRenderTargetBuffers;

		DepthStencilBuffer mDepthStencilBuffer;
	};
}