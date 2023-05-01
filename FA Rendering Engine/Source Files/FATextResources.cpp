#include "FATextResources.h"
#include "FADirectXException.h"

namespace FARender
{
	TextResources::TextResources(const Microsoft::WRL::ComPtr<ID3D12Device>& device, 
		const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& commandQueue, unsigned int numSwapChainBuffers)
	{
		//-----------------------------------------------------------------------------------------------------------------------------
		//Create a D3D11On12 Device

		ThrowIfFailed(D3D11On12CreateDevice(device.Get(),
			D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT,
			nullptr, 0, (IUnknown**)commandQueue.GetAddressOf(), 1, 0, &mDevice11, &mDevice11Context, nullptr));

		//Query the 11On12 device from the 11 device.
		ThrowIfFailed(mDevice11.As(&mDevice11on12));
		//-----------------------------------------------------------------------------------------------------------------------------


		//-----------------------------------------------------------------------------------------------------------------------------
		//Create Direct2D factory, Direct2D device, Direct2D device context and DirectWrite factory.

		D2D1_FACTORY_OPTIONS factoryOptions{ D2D1_DEBUG_LEVEL_INFORMATION };
		ThrowIfFailed(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, factoryOptions, mDirect2DFactory.GetAddressOf()));

		Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
		ThrowIfFailed(mDevice11on12.As(&dxgiDevice));

		ThrowIfFailed(mDirect2DFactory->CreateDevice(dxgiDevice.Get(), &mDirect2DDevice));
		ThrowIfFailed(mDirect2DDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &mDirect2DDeviceContext));

		ThrowIfFailed(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &mDirectWriteFactory));
		//-----------------------------------------------------------------------------------------------------------------------------

		//-----------------------------------------------------------------------------------------------------------------------------
		//Resize the buffers to match the number of swap chain buffers

		mWrappedBuffers.resize(numSwapChainBuffers);
		mDirect2DBuffers.resize(numSwapChainBuffers);
		mSurfaces.resize(numSwapChainBuffers);

		//-----------------------------------------------------------------------------------------------------------------------------
	}

	const Microsoft::WRL::ComPtr<ID2D1DeviceContext>& TextResources::GetDirect2DDeviceContext() const
	{
		return mDirect2DDeviceContext;
	}

	const Microsoft::WRL::ComPtr<IDWriteFactory>& TextResources::GetDirectWriteFactory() const
	{
		return mDirectWriteFactory;
	}

	void TextResources::ResetBuffers()
	{
		mDirect2DDeviceContext->SetTarget(nullptr);

		int numBuffers = (int)mWrappedBuffers.size();

		for (int i = 0; i < numBuffers; ++i)
		{
			mWrappedBuffers[i].Reset();
			mDirect2DBuffers[i].Reset();
			mSurfaces[i].Reset();
		}

		mDevice11Context->Flush();
	}

	void TextResources::ResizeBuffers(const std::vector<std::unique_ptr<RenderTargetBuffer>>& renderTargetBuffers, HWND windowHandle)
	{
		int numBuffers = (int)mWrappedBuffers.size();

		//-----------------------------------------------------------------------------------------------------------------------------
		//Create a wrapped 11on12 resource to the back buffers

		D3D11_RESOURCE_FLAGS direct11ResourceFlags{ D3D11_BIND_RENDER_TARGET };

		//creates a wrapped resource to each of our swap chain buffers
		for (int i = 0; i < numBuffers; ++i)
		{
			ThrowIfFailed(mDevice11on12->CreateWrappedResource(renderTargetBuffers[i]->GetRenderTargetBuffer().Get(), 
				&direct11ResourceFlags,
				D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT, IID_PPV_ARGS(mWrappedBuffers[i].GetAddressOf())));
		}
		//-----------------------------------------------------------------------------------------------------------------------------


		//-----------------------------------------------------------------------------------------------------------------------------
		//Create a render target for D2D to draw directly to the back buffers.

		// Query the window's dpi settings, which will be used to create
		// D2D's render targets.
		float dpi = (float)GetDpiForWindow(windowHandle);

		D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
			D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
			dpi, dpi);

		for (int i = 0; i < numBuffers; ++i)
		{
			ThrowIfFailed(mWrappedBuffers[i].As(&mSurfaces[i]));

			ThrowIfFailed(mDirect2DDeviceContext->CreateBitmapFromDxgiSurface(mSurfaces[i].Get(), &bitmapProperties,
				mDirect2DBuffers[i].GetAddressOf()));
		}
		//-----------------------------------------------------------------------------------------------------------------------------
	}

	void TextResources::BeforeRenderText(unsigned int currentBackBuffer)
	{
		//gives direct2D access to our back buffer
		mDevice11on12->AcquireWrappedResources(mWrappedBuffers[currentBackBuffer].GetAddressOf(), 1);

		//Render text to back buffer
		mDirect2DDeviceContext->SetTarget(mDirect2DBuffers[currentBackBuffer].Get());
		mDirect2DDeviceContext->BeginDraw();
		mDirect2DDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
	}

	void TextResources::AfterRenderText(unsigned int currentBackBuffer)
	{
		ThrowIfFailed(mDirect2DDeviceContext->EndDraw());

		//Releasing the wrapped render target resource transitions the back buffer to present state.
		mDevice11on12->ReleaseWrappedResources(mWrappedBuffers[currentBackBuffer].GetAddressOf(), 1);

		//Submit the commands to the shared D3D12 command queue.
		mDevice11Context->Flush();
	}
}