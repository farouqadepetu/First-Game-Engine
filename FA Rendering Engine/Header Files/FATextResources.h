#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <d3d11on12.h>
#include <d2d1_3.h>
#include <dwrite.h>
#include <vector>

namespace FARender
{
	/** @class TextResources ""
	*	@brief A wrapper for resources that are needed to render text using Direct3D 11on12, Direct2D and DirectWrite.
	*/
	class TextResources
	{
	public:
		TextResources() = default;

		/**@brief Constructor.
		* Initializes the resources.
		*/
		TextResources(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
			const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& commandQueue, unsigned int numSwapChainBuffers);

		/**@brief Returns a constant reference to the direct 2D device context.
		*/
		const Microsoft::WRL::ComPtr<ID2D1DeviceContext>& GetDirect2DDeviceContext() const;

		/**@brief Returns a constant reference to the direct direct write factory.
		*/
		const Microsoft::WRL::ComPtr<IDWriteFactory>& GetDirectWriteFactory() const;

		/**@brief Resets the text buffers.
		*/
		void ResetBuffers();

		/**@brief Resizes the buffers.
		*/
		void ResizeBuffers(const Microsoft::WRL::ComPtr<ID3D12Resource>* swapChainBuffers, HWND windowHandle);

		/**@brief Prepares to render text.
		*/
		void BeforeRenderText(unsigned int currentBackBuffer);

		/**@brief Executes text commands.
		*/
		void AfterRenderText(unsigned int currentBackBuffer);

	private:
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice11;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDevice11Context;
		Microsoft::WRL::ComPtr<ID3D11On12Device> mDevice11on12;

		Microsoft::WRL::ComPtr<ID2D1Device2> mDirect2DDevice;
		Microsoft::WRL::ComPtr<ID2D1Factory3> mDirect2DFactory;
		Microsoft::WRL::ComPtr<ID2D1DeviceContext> mDirect2DDeviceContext;

		Microsoft::WRL::ComPtr<IDWriteFactory> mDirectWriteFactory;

		std::vector<Microsoft::WRL::ComPtr<ID3D11Resource>> mWrappedBuffers;
		std::vector<Microsoft::WRL::ComPtr<ID2D1Bitmap1>> mDirect2DBuffers;
		std::vector<Microsoft::WRL::ComPtr<IDXGISurface>> mSurfaces;
	};
}