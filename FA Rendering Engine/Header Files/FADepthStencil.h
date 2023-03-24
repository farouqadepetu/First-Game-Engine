#pragma once

#include <wrl.h>
#include "d3dx12.h"

namespace FARender
{
	/** @class DepthStencil ""
	*	@brief A wrapper for depth stencil resources. Uses DirectD 12 API.
	*/
	class DepthStencil
	{
	public:

		DepthStencil() = default;

		/**@brief Constructor.
		* Creates a depth stencil view heap.
		*/
		DepthStencil(const Microsoft::WRL::ComPtr<ID3D12Device>& device);

		/**@brief Returns the format of the depth stencil buffer.
		*/
		DXGI_FORMAT GetDepthStencilFormat() const;

		/**@brief Returns the address to the view of the depth stencil buffer.
		*/
		CD3DX12_CPU_DESCRIPTOR_HANDLE GetDepthStencilView() const;

		/**@brief Creates the depth stencil buffer and view.
		*/
		void CreateDepthStencilBufferAndView(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
			unsigned int width, unsigned int height);

		/**@brief Resest the depth stencil buffer.
		*/
		void ResetBuffer();

		/**@brief Clears the depth stencil buffer with the specified clear value.
		*/
		void ClearDepthStencilBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList, float clearValue);

	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> mDepthStencilBuffer;
		DXGI_FORMAT mDepthStencilFormat;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDSVHeap;

	};
}