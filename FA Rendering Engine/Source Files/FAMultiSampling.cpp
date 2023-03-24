#include "FAMultiSampling.h"
#include "FADirectXException.h"

namespace FARender
{
	MultiSampling::MultiSampling(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
		DXGI_FORMAT rtFormat, DXGI_FORMAT dsFormat, unsigned int sampleCount) :
		mMSAARenderTargetBuffer{ rtFormat }, mMSAADepthStencilBuffer{ dsFormat }
	{
		//Describes the render target format and sample count we want to check to see if it is supported.
		D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS levels{};
		levels.Format = rtFormat;
		levels.SampleCount = sampleCount;

		ThrowIfFailed(device->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
			&levels, sizeof(levels)));

		//The speicifed render target format and sample count is supported by the GPU.
		if (levels.NumQualityLevels > 0)
			mSampleCount = sampleCount;
		else
			throw std::runtime_error("The specified format and/or sample count is not supported by the Direct3D 12 device");
	}

	const Microsoft::WRL::ComPtr<ID3D12Resource>& MultiSampling::GetRenderTargetBuffer()
	{
		return mMSAARenderTargetBuffer.GetRenderTargetBuffer();
	}

	DXGI_FORMAT MultiSampling::GetRenderTargetFormat()
	{
		return mMSAARenderTargetBuffer.GetRenderTargetFormat();
	}

	DXGI_FORMAT MultiSampling::GetDepthStencilFormat()
	{
		return mMSAADepthStencilBuffer.GetDepthStencilFormat();
	}

	void MultiSampling::ResetBuffers()
	{
		mMSAARenderTargetBuffer.ResetBuffer();
		mMSAADepthStencilBuffer.ResetBuffer();
	}

	void MultiSampling::CreateRenderTargetBufferAndView(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
		const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& rtvHeap, unsigned int indexOfWhereToStoreView, unsigned int rtvSize,
		unsigned int width, unsigned int height)
	{
		mMSAARenderTargetBuffer.CreateRenderTargetBufferAndView(device, rtvHeap, indexOfWhereToStoreView, rtvSize, 
			width, height, mSampleCount);

		/*D3D12_RESOURCE_DESC mMSAARenderTargetBufferDesc{};
		mMSAARenderTargetBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		mMSAARenderTargetBufferDesc.Alignment = 0;
		mMSAARenderTargetBufferDesc.Width = width;
		mMSAARenderTargetBufferDesc.Height = height;
		mMSAARenderTargetBufferDesc.DepthOrArraySize = 1;
		mMSAARenderTargetBufferDesc.MipLevels = 1;
		mMSAARenderTargetBufferDesc.Format = mMSAAFormat;
		mMSAARenderTargetBufferDesc.SampleDesc.Count = 4;
		mMSAARenderTargetBufferDesc.SampleDesc.Quality = 0;
		mMSAARenderTargetBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		mMSAARenderTargetBufferDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

		D3D12_CLEAR_VALUE msaaRTBufferClearValue{};
		msaaRTBufferClearValue.Format = mMSAAFormat;
		msaaRTBufferClearValue.Color[0] = 0.0f;
		msaaRTBufferClearValue.Color[1] = 0.0f;
		msaaRTBufferClearValue.Color[2] = 0.0f;
		msaaRTBufferClearValue.Color[3] = 1.0f;

		//Use this class to say which type of heap our buffer will be stored in.
		CD3DX12_HEAP_PROPERTIES msaaRTHeapProp(D3D12_HEAP_TYPE_DEFAULT);

		//Create the RT buffer resouce
		ThrowIfFailed(device->CreateCommittedResource(&msaaRTHeapProp, D3D12_HEAP_FLAG_NONE, &mMSAARenderTargetBufferDesc,
			D3D12_RESOURCE_STATE_RESOLVE_SOURCE, &msaaRTBufferClearValue, IID_PPV_ARGS(&mMSAARenderTargetBuffer.GetRenderTargetBuffer())));

		//Get the address of where you want to store the view in the RTV heap.
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(rtvHeap->GetCPUDescriptorHandleForHeapStart(), indexOfWhereToStoreView, rtvSize);

		//Create the RTV
		device->CreateRenderTargetView(mMSAARenderTargetBuffer.GetRenderTargetBuffer().Get(), nullptr, rtvHeapHandle);*/
	}

	void MultiSampling::CreateDepthStencilBufferAndView(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
		const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& dsvHeap, unsigned int indexOfWhereToStoreView, unsigned int dsvSize,
		unsigned int width, unsigned int height)
	{
		mMSAADepthStencilBuffer.CreateDepthStencilBufferAndView(device, dsvHeap, indexOfWhereToStoreView, dsvSize, 
			width, height, mSampleCount);

		/*D3D12_RESOURCE_DESC mMSAADepthStencilBufferDesc{};
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
			mMSAADSVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());*/
	}

	void MultiSampling::Transition(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
		D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
	{
		CD3DX12_RESOURCE_BARRIER renderTargetBufferTransition =
			CD3DX12_RESOURCE_BARRIER::Transition(mMSAARenderTargetBuffer.GetRenderTargetBuffer().Get(),
				before, after);

		commandList->ResourceBarrier(1, &renderTargetBufferTransition);
	}

	void MultiSampling::ClearRenderTargetBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
		const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& rtvHeap, unsigned int indexOfView, unsigned int rtvSize,
		const float* clearValue)
	{
		mMSAARenderTargetBuffer.ClearRenderTargetBuffer(commandList, rtvHeap, indexOfView, rtvSize, clearValue);
	}

	void MultiSampling::ClearDepthStencilBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
		const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& dsvHeap, unsigned int indexOfView, unsigned int dsvSize,
		float clearValue)
	{
		mMSAADepthStencilBuffer.ClearDepthStencilBuffer(commandList, dsvHeap, indexOfView, dsvSize, clearValue);
	}

}