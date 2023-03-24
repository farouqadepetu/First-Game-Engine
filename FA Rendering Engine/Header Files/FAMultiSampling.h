#pragma once

#include <wrl.h>
#include "d3dx12.h"
#include "FABuffer.h"

namespace FARender
{
	/** @class MultiSampling""
	*	@brief A wrapper for multisampling resources. Uses DirectD 12 API.
	*/
	class MultiSampling
	{
	public:

		MultiSampling() = default;

		/**@brief Constructor.
		* Checks if the specifed format and sample count are supported by the specified device for multi-sampling.
		* Throws a runtime_error if they are not supproted.
		*/
		MultiSampling(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
			DXGI_FORMAT rtFormat, DXGI_FORMAT dsFormat, unsigned int sampleCount);

		/**@brief Returns the MSAA render target buffer.
		*/
		const Microsoft::WRL::ComPtr<ID3D12Resource>& GetRenderTargetBuffer();

		DXGI_FORMAT GetRenderTargetFormat();

		DXGI_FORMAT GetDepthStencilFormat();

		/**@brief Resets the MSAA render target buffer and MSAA depth stencil buffer.
		*/
		void ResetBuffers();

		/**@brief Creates the MSAA render target buffer and a view to it.
		*/
		void CreateRenderTargetBufferAndView(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
			const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& rtvHeap, unsigned int indexOfWhereToStoreView, unsigned int rtvSize,
			unsigned int width, unsigned int height);

		/**@brief Creates the MSAA depth stencil buffer and a view to it.
		*/
		void CreateDepthStencilBufferAndView(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
			const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& dsvHeap, unsigned int indexOfWhereToStoreView, unsigned int dsvSize, 
			unsigned int width, unsigned int height);

		/**@brief Transitions the MSAA render target buffer from the specified before state to the specified after state.
		*/
		void Transition(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
			D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);

		/**@brief Clears the MSAA render target buffer with the specified clear value.
		*/
		void ClearRenderTargetBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
			const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& rtvHeap, unsigned int indexOfView, unsigned int rtvSize,
			const float* clearValue);

		/**@brief Clears the MSAA depth stencil buffer with the specified clear value.
		*/
		void ClearDepthStencilBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
			const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& dsvHeap, unsigned int indexOfView, unsigned int dsvSize,
			float clearValue);

	private:
		RenderTargetBuffer mMSAARenderTargetBuffer;
		DepthStencilBuffer mMSAADepthStencilBuffer;
		unsigned int mSampleCount{ 0 };

		/*Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mMSAARTVDescriptorHeap;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mMSAADSVDescriptorHeap;
		Microsoft::WRL::ComPtr<ID3D12Resource> mMSAARenderTargetBuffer;
		Microsoft::WRL::ComPtr<ID3D12Resource> mMSAADepthStencilBuffer;*/

	};
}