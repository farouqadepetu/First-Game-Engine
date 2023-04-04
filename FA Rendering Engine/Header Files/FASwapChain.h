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

		/**@brief Creates a swap chain.
		* 
		* @param[in] dxgiFactory A DXGIFactory4 object.
		* @param[in] A Direct3D 12 command queue.
		* @param[in] windowHandle A handle to a window.
		* @param[in, optional] rtFormat The format of the render target buffer.
		* @param[in, optional] dsFormat The format of the depth stencil buffer.
		* @param[in, optional] numRenderTargetBuffers The number of render target buffers the swap chain has.
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
		* 
		* @param[in] width The width to resize the render target buffers to.
		* @param[in] height The height to resize the render target buffers to.
		*/
		void ResizeSwapChain(unsigned width, unsigned height);

		/**@brief Creates the render target buffers and views to them.
		* 
		* @param[in] device A Direct3D 12 device.
		* @param[in] rtvHeap A descriptor heap for storing render target descriptors.
		* @param[in] indexOfWhereToStoreView The index of where to store the created descriptor in the descriptor heap.
		* @param[in] rtvSize The size of a render target descriptor.
		*/
		void CreateRenderTargetBuffersAndViews(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
			const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& rtvHeap, unsigned int indexOfWhereToStoreFirstView, 
			unsigned int rtvSize);

		/**@brief Creates the swap chains depth stencil buffer and view to it.
		* 
		* @param[in] device A Direct3D 12 device.
		* @param[in] dsvHeap A descriptor heap for storing depth stencil descriptors.
		* @param[in] index The index of where to store the created descriptor in the descriptor heap.
		* @param[in] dsvSize The size of a depth stenicl descriptor.
		* @param[in] width The width of the depth stenicl buffer.
		* @param[in] height The height of the depth stenicl buffer.
		*/
		void CreateDepthStencilBufferAndView(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
			const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& dsvHeap, unsigned int index, unsigned int dsvSize,
			unsigned int width, unsigned int height);

		/**@brief Clears the current render target buffer.
		* 
		* @param[in] commadList A Direct3D 12 graphics command list.
		* @param[in] rtvHeap A render target descriptor heap.
		* @param[in] indexOfFirstView The index of where the render target descriptor 
		* of the first render target buffer is stored in the descriptor heap.
		* 
		* @param[in] rtvSize The size of a render target descriptor.
		* @param[in] backBufferClearValue The RGBA values of what to set every element in the current render target buffer to.
		*/
		void ClearCurrentBackBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
			const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& rtvHeap, unsigned int indexOfFirstView, unsigned int rtvSize,
			const float* backBufferClearValue);

		/**@brief Clears the swap chains depth stencil buffer with the specified clear value.
		* 
		* @param[in] commadList A Direct3D 12 graphics command list.
		* @param[in] dsvHeap A depth stencil descriptor heap.
		* @param[in] indexOfView The index of where the depth stencil descriptor of the  
		* depth stencil buffer is stored in the descriptor heap.
		* 
		* @param[in] dsvSize The size of a depth stencil descriptor.
		* @param[in] clearValue The value of what to set every element in the depth stencil buffer to.
		*/
		void ClearDepthStencilBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
			const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& dsvHeap, unsigned int indexOfView, unsigned int dsvSize,
			float clearValue);

		/**@brief Transitions the current render target buffer from the specified \a before state to the specified \a after state.
		* 
		* @param[in] commandList A Direct3D 12 graphics command list.
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