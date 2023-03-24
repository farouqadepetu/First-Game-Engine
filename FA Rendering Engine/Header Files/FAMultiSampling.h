#pragma once

#include <wrl.h>
#include "d3dx12.h"

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
			DXGI_FORMAT format, unsigned int sampleCount);

		/**@brief Returns the MSAA render target buffer.
		*/
		const Microsoft::WRL::ComPtr<ID3D12Resource>& GetRenderTargetBuffer();

		/**@brief Returns the address of the MSAA render target buffer view.
		*/
		CD3DX12_CPU_DESCRIPTOR_HANDLE GetRenderTargetBufferView() const;

		/**@brief Returns the address of the MSAA depth stencil buffer view.
		*/
		CD3DX12_CPU_DESCRIPTOR_HANDLE GetDepthStencilBufferView() const;

		/**@brief Resets the MSAA render target buffer and MSAA depth stencil buffer.
		*/
		void ResetBuffers();

		/**@brief Creates the MSAA render target buffer and a view to it.
		*/
		void CreateRenderTargetBufferAndView(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
			DXGI_FORMAT format, unsigned int width, unsigned int height);

		/**@brief Creates the MSAA depth stencil buffer and a view to it.
		*/
		void CreateDepthStencilBufferAndView(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
			DXGI_FORMAT format, unsigned int width, unsigned int height);

		/**@brief Transitions the MSAA render target buffer from the specified before state to the specified after state.
		*/
		void Transition(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
			D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);

		/**@brief Clears the MSAA render target buffer with the specified clear value.
		*/
		void ClearRenderTargetBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList, const float* clearValue);

		/**@brief Clears the MSAA depth stencil buffer with the specified clear value.
		*/
		void ClearDepthStencilBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
			float clearValue);

	private:
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mMSAARTVDescriptorHeap;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mMSAADSVDescriptorHeap;
		Microsoft::WRL::ComPtr<ID3D12Resource> mMSAARenderTargetBuffer;
		Microsoft::WRL::ComPtr<ID3D12Resource> mMSAADepthStencilBuffer;

	};
}