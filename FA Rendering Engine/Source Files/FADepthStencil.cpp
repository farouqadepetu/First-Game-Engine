#include "FADepthStencil.h"
#include "FADirectXException.h"

namespace FARender
{
	DepthStencil::DepthStencil(const Microsoft::WRL::ComPtr<ID3D12Device>& device) : mDepthStencilFormat{ DXGI_FORMAT_D24_UNORM_S8_UINT }
	{
		D3D12_DESCRIPTOR_HEAP_DESC dsvDescriptorHeapDescription{};
		dsvDescriptorHeapDescription.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		dsvDescriptorHeapDescription.NumDescriptors = 1;
		dsvDescriptorHeapDescription.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		dsvDescriptorHeapDescription.NodeMask = 0;
		ThrowIfFailed(device->CreateDescriptorHeap(&dsvDescriptorHeapDescription, IID_PPV_ARGS(&mDSVHeap)));
	}

	DXGI_FORMAT DepthStencil::GetDepthStencilFormat() const
	{
		return mDepthStencilFormat;
	}

	CD3DX12_CPU_DESCRIPTOR_HANDLE DepthStencil::GetDepthStencilView() const
	{
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(mDSVHeap->GetCPUDescriptorHandleForHeapStart());
	}

	void DepthStencil::CreateDepthStencilBufferAndView(const Microsoft::WRL::ComPtr<ID3D12Device>& device, 
		unsigned int width, unsigned int height)
	{
		//Create Depth/Stenicl Buffer and View (Descriptor)

		D3D12_RESOURCE_DESC depthBufferDescription{};
		depthBufferDescription.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		depthBufferDescription.Alignment = 0;
		depthBufferDescription.Width = width;
		depthBufferDescription.Height = height;
		depthBufferDescription.DepthOrArraySize = 1;
		depthBufferDescription.MipLevels = 1;
		depthBufferDescription.Format = mDepthStencilFormat;
		depthBufferDescription.SampleDesc.Count = 1;
		depthBufferDescription.SampleDesc.Quality = 0;
		depthBufferDescription.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		depthBufferDescription.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		D3D12_CLEAR_VALUE depthBufferClearValue{};
		depthBufferClearValue.Format = mDepthStencilFormat;
		depthBufferClearValue.DepthStencil.Depth = 1.0f;
		depthBufferClearValue.DepthStencil.Stencil = 0;

		//Use this class to say which type of heap our buffer will be stored in.
		CD3DX12_HEAP_PROPERTIES dHeapProp(D3D12_HEAP_TYPE_DEFAULT);

		ThrowIfFailed(device->CreateCommittedResource(&dHeapProp, D3D12_HEAP_FLAG_NONE, &depthBufferDescription,
			D3D12_RESOURCE_STATE_DEPTH_WRITE, &depthBufferClearValue, IID_PPV_ARGS(&mDepthStencilBuffer)));

		D3D12_DEPTH_STENCIL_VIEW_DESC depthBufferViewDescription{};
		depthBufferViewDescription.Format = mDepthStencilFormat;
		depthBufferViewDescription.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		depthBufferViewDescription.Flags = D3D12_DSV_FLAG_NONE;
		depthBufferViewDescription.Texture2D.MipSlice = 0;

		device->CreateDepthStencilView(mDepthStencilBuffer.Get(), &depthBufferViewDescription,
			mDSVHeap->GetCPUDescriptorHandleForHeapStart());
	}

	void DepthStencil::ResetBuffer()
	{
		mDepthStencilBuffer.Reset();
	}

	void DepthStencil::ClearDepthStencilBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList, 
		float clearValue)
	{
		//Clear the DS buffer
		commandList->ClearDepthStencilView(mDSVHeap->GetCPUDescriptorHandleForHeapStart(),
			D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, clearValue, 0, 0, nullptr);
	}
}