#pragma once

#include <wrl.h>
#include "d3dx12.h"
#include "FABuffer.h"

namespace FARender
{
	/** @class MultiSampling""
	*	@brief A wrapper for multisampling resources. Uses DirectD 12 API.
	*	The copy constructor and assignment operators are explicitly deleted. This makes this class non-copyable.
	*/
	class MultiSampling
	{
	public:

		MultiSampling(const MultiSampling&) = delete;
		MultiSampling& operator=(const MultiSampling&) = delete;

		/**@brief Creates a multisampling object.
		* Call the function CheckMultiSamplingSupport() to check if the desired formats and sample count is supported by a GPU.
		* If they are supported, a mulitsampling render target buffer and a mulitsampling depth stencil buffer can be created.
		*/
		MultiSampling();

		/**@brief Checks if the specifed format and sample count are supported by the specified device for multi-sampling.
		* 
		* Throws a runtime_error if they are not supproted.
		* 
		* @param[in] device A Direct3D 12 device.
		* @param[in] rtFormat The format of the render target buffer.
		* @param[in] dsFormat The format of the depth stencil buffer.
		* @param[in] sampleCount The number of samples for the multi-sampling render tagret and depth stencil buffers.
		*/
		MultiSampling(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
			DXGI_FORMAT rtFormat, unsigned int sampleCount);

		/**@brief Checks if the specifed format and sample count are supported by the specified device for multi-sampling.
		*
		* Throws a runtime_error if they are not supproted.
		*
		* @param[in] device A Direct3D 12 device.
		* @param[in] rtFormat The format of the render target buffer.
		* @param[in] dsFormat The format of the depth stencil buffer.
		* @param[in] sampleCount The number of samples for the multi-sampling render tagret and depth stencil buffers.
		*/
		void CheckMultiSamplingSupport(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
			DXGI_FORMAT rtFormat, unsigned int sampleCount);

		/**@brief Returns a constant refererence to the MSAA render target buffer.
		*/
		const Microsoft::WRL::ComPtr<ID3D12Resource>& GetRenderTargetBuffer();

		/**@brief Returns the format of the MSAA render target buffer.
		*/
		DXGI_FORMAT GetRenderTargetFormat();

		/**@brief Returns the format of the MSAA depth stencil buffer.
		*/
		DXGI_FORMAT GetDepthStencilFormat();

		/**@brief Resets the MSAA render target buffer and MSAA depth stencil buffer.
		*/
		void ReleaseBuffers();

		/**@brief Creates the MSAA render target buffer and a view to it.
		* 
		* @param[in] device A Direct3D 12 device.
		* @param[in] rtvHeap A descriptor heap for storing render target descriptors.
		* @param[in] indexOfWhereToStoreView The index of where to store the created descriptor in the descriptor heap.
		* @param[in] rtvSize The size of a render target descriptor.
		* @param[in] width The width of the render target buffer.
		* @param[in] height The height of the render target buffer.
		*/
		void CreateRenderTargetBufferAndView(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
			const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& rtvHeap, unsigned int indexOfWhereToStoreView, unsigned int rtvSize,
			unsigned int width, unsigned int height);

		/**@brief Creates the MSAA depth stencil buffer and a view to it.
		* 
		* @param[in] device A Direct3D 12 device.
		* @param[in] dsvHeap A descriptor heap for storing depth stencil descriptors.
		* @param[in] indexOfWhereToStoreView The index of where to store the created descriptor in the descriptor heap.
		* @param[in] dsvSize The size of a depth stenicl descriptor.
		* @param[in] width The width of the depth stenicl buffer.
		* @param[in] height The height of the depth stenicl buffer.
		*/
		void CreateDepthStencilBufferAndView(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
			const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& dsvHeap, unsigned int indexOfWhereToStoreView, unsigned int dsvSize, 
			unsigned int width, unsigned int height);

		/**@brief Transitions the MSAA render target buffer from the specified \a before state to the specified \a after state.
		* 
		* @param[in] commandList A Direct3D 12 graphics command list.
		*/
		void Transition(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
			D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);

		/**@brief Clears the MSAA render target buffer with the specified clear value.
		* 
		* @param[in] commadList A Direct3D 12 graphics command list.
		* @param[in] rtvHeap A render target descriptor heap.
		* @param[in] indexOfView The index of where the render target descriptor of the render target buffer is stored in the descriptor heap.
		* @param[in] rtvSize The size of a render target descriptor.
		* @param[in] clearValue The RGBA values of what to set every element in the render target buffer to.
		*/
		void ClearRenderTargetBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
			const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& rtvHeap, unsigned int indexOfView, unsigned int rtvSize,
			const float* clearValue);

		/**@brief Clears the MSAA depth stencil buffer with the specified clear value.
		* 
		* @param[in] commadList A Direct3D 12 graphics command list.
		* @param[in] dsvHeap A depth stencil descriptor heap.
		* @param[in] indexOfView The index of where the depth stencil descriptor of the  depth stencil buffer is stored in the descriptor heap.
		* @param[in] dsvSize The size of a depth stencil descriptor.
		* @param[in] clearValue The value of what to set every element in the depth stencil buffer to.
		*/
		void ClearDepthStencilBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
			const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& dsvHeap, unsigned int indexOfView, unsigned int dsvSize,
			float clearValue);

	private:
		RenderTargetBuffer mMSAARenderTargetBuffer;
		DepthStencilBuffer mMSAADepthStencilBuffer;
		unsigned int mSampleCount;
	};
}