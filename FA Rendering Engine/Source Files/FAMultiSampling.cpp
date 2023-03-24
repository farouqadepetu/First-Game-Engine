#include "FAMultiSampling.h"
#include "FADirectXException.h"

namespace FARender
{
	MultiSampling::MultiSampling(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
		DXGI_FORMAT format, unsigned int sampleCount)
	{
		//Describes the format and sample count we want to check to see if it is supported.
		D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS levels{};
		levels.Format = format;
		levels.SampleCount = sampleCount;

		ThrowIfFailed(device->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
			&levels, sizeof(levels)));

		//The speicifed format and sample count is supported by the GPU.
		if (levels.NumQualityLevels > 0)
		{
			//Create the MSAA render target view heap
			D3D12_DESCRIPTOR_HEAP_DESC msaaRTVHeapDescription{};
			msaaRTVHeapDescription.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			msaaRTVHeapDescription.NumDescriptors = 1;
			msaaRTVHeapDescription.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			msaaRTVHeapDescription.NodeMask = 0;
			ThrowIfFailed(device->CreateDescriptorHeap(&msaaRTVHeapDescription,
				IID_PPV_ARGS(&mMSAARTVDescriptorHeap)));

			//Create the MSAA depth stencil view heap
			D3D12_DESCRIPTOR_HEAP_DESC msaaDSVHeapDescription{};
			msaaDSVHeapDescription.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
			msaaDSVHeapDescription.NumDescriptors = 1;
			msaaDSVHeapDescription.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			msaaDSVHeapDescription.NodeMask = 0;
			ThrowIfFailed(device->CreateDescriptorHeap(&msaaDSVHeapDescription,
				IID_PPV_ARGS(&mMSAADSVDescriptorHeap)));
		}
		else
			throw std::runtime_error("The specified format and/or sample count is not supported by the Direct3D 12 device");
	}

	const Microsoft::WRL::ComPtr<ID3D12Resource>& MultiSampling::GetRenderTargetBuffer()
	{
		return mMSAARenderTargetBuffer;
	}

	CD3DX12_CPU_DESCRIPTOR_HANDLE MultiSampling::GetRenderTargetBufferView() const
	{
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(mMSAARTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
			
	}

	CD3DX12_CPU_DESCRIPTOR_HANDLE MultiSampling::GetDepthStencilBufferView() const
	{
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(mMSAADSVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	}

	void MultiSampling::ResetBuffers()
	{
		mMSAARenderTargetBuffer.Reset();
		mMSAADepthStencilBuffer.Reset();
	}

	void MultiSampling::CreateRenderTargetBufferAndView(const Microsoft::WRL::ComPtr<ID3D12Device>& device, 
		DXGI_FORMAT format, unsigned int width, unsigned int height)
	{
		D3D12_RESOURCE_DESC mMSAARenderTargetBufferDesc{};
		mMSAARenderTargetBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		mMSAARenderTargetBufferDesc.Alignment = 0;
		mMSAARenderTargetBufferDesc.Width = width;
		mMSAARenderTargetBufferDesc.Height = height;
		mMSAARenderTargetBufferDesc.DepthOrArraySize = 1;
		mMSAARenderTargetBufferDesc.MipLevels = 1;
		mMSAARenderTargetBufferDesc.Format = format;
		mMSAARenderTargetBufferDesc.SampleDesc.Count = 4;
		mMSAARenderTargetBufferDesc.SampleDesc.Quality = 0;
		mMSAARenderTargetBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		mMSAARenderTargetBufferDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

		D3D12_CLEAR_VALUE msaaRTBufferClearValue{};
		msaaRTBufferClearValue.Format = format;
		msaaRTBufferClearValue.Color[0] = 0.0f;
		msaaRTBufferClearValue.Color[1] = 0.0f;
		msaaRTBufferClearValue.Color[2] = 0.0f;
		msaaRTBufferClearValue.Color[3] = 1.0f;

		//Use this class to say which type of heap our buffer will be stored in.
		CD3DX12_HEAP_PROPERTIES msaaRTHeapProp(D3D12_HEAP_TYPE_DEFAULT);

		//Create the RT buffer resouce
		ThrowIfFailed(device->CreateCommittedResource(&msaaRTHeapProp, D3D12_HEAP_FLAG_NONE, &mMSAARenderTargetBufferDesc,
			D3D12_RESOURCE_STATE_RESOLVE_SOURCE, &msaaRTBufferClearValue, IID_PPV_ARGS(&mMSAARenderTargetBuffer)));

		//Create the RTV
		device->CreateRenderTargetView(mMSAARenderTargetBuffer.Get(), nullptr,
			mMSAARTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	}

	void MultiSampling::CreateDepthStencilBufferAndView(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
		DXGI_FORMAT format, unsigned int width, unsigned int height)
	{
		D3D12_RESOURCE_DESC mMSAADepthStencilBufferDesc{};
		mMSAADepthStencilBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		mMSAADepthStencilBufferDesc.Alignment = 0;
		mMSAADepthStencilBufferDesc.Width = width;
		mMSAADepthStencilBufferDesc.Height = height;
		mMSAADepthStencilBufferDesc.DepthOrArraySize = 1;
		mMSAADepthStencilBufferDesc.MipLevels = 1;
		mMSAADepthStencilBufferDesc.Format = format;
		mMSAADepthStencilBufferDesc.SampleDesc.Count = 4;
		mMSAADepthStencilBufferDesc.SampleDesc.Quality = 0;
		mMSAADepthStencilBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		mMSAADepthStencilBufferDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		D3D12_CLEAR_VALUE msaaDSBufferClearValue{};
		msaaDSBufferClearValue.Format = format;
		msaaDSBufferClearValue.DepthStencil.Depth = 1.0f;
		msaaDSBufferClearValue.DepthStencil.Stencil = 0;

		//Use this class to say which type of heap our buffer will be stored in.
		CD3DX12_HEAP_PROPERTIES msaaDSHeapProp(D3D12_HEAP_TYPE_DEFAULT);

		//Create the msaa DS buffer resouce
		ThrowIfFailed(device->CreateCommittedResource(&msaaDSHeapProp, D3D12_HEAP_FLAG_NONE, &mMSAADepthStencilBufferDesc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE, &msaaDSBufferClearValue, IID_PPV_ARGS(&mMSAADepthStencilBuffer)));

		//Describe the msaa DSV
		D3D12_DEPTH_STENCIL_VIEW_DESC dsViewDescription{};
		dsViewDescription.Format = format;
		dsViewDescription.Flags = D3D12_DSV_FLAG_NONE;
		dsViewDescription.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DMS;

		//Create the msaa DSV
		device->CreateDepthStencilView(mMSAADepthStencilBuffer.Get(), &dsViewDescription,
			mMSAADSVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	}

	void MultiSampling::Transition(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
		D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
	{
		CD3DX12_RESOURCE_BARRIER renderTargetBufferTransition =
			CD3DX12_RESOURCE_BARRIER::Transition(mMSAARenderTargetBuffer.Get(),
				before, after);

		commandList->ResourceBarrier(1, &renderTargetBufferTransition);
	}

	void MultiSampling::ClearRenderTargetBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList, 
		const float* clearValue)
	{
		commandList->ClearRenderTargetView(mMSAARTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			clearValue, 0, nullptr);
	}

	void MultiSampling::ClearDepthStencilBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
		float clearValue)
	{
		//clear the msaa DS buffer
		commandList->ClearDepthStencilView(mMSAADSVDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, clearValue, 0, 0, nullptr);
	}

}