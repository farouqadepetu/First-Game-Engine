#include "Multisampling.h"
#include "DirectXException.h"

namespace RenderingEngine
{
	MultiSampling::MultiSampling() : mSampleCount{ 0 }
	{}

	MultiSampling::MultiSampling(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
		DXGI_FORMAT rtFormat, unsigned int sampleCount)
	{
		CheckMultiSamplingSupport(device, rtFormat, sampleCount);
	}

	void MultiSampling::CheckMultiSamplingSupport(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
		DXGI_FORMAT rtFormat, unsigned int sampleCount)
	{
		//Describes the render target format and sample count we want to check to see if it is supported.
		D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS levels{};
		levels.Format = rtFormat;
		levels.SampleCount = sampleCount;

		ExitIfFailed(device->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
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

	void MultiSampling::ReleaseBuffers()
	{
		mMSAARenderTargetBuffer.ReleaseBuffer();
		mMSAADepthStencilBuffer.ReleaseBuffer();
	}

	void MultiSampling::CreateRenderTargetBufferAndView(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
		const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& rtvHeap, unsigned int indexOfWhereToStoreView, unsigned int rtvSize,
		unsigned int width, unsigned int height)
	{
		mMSAARenderTargetBuffer.CreateRenderTargetBufferAndView(device, rtvHeap, indexOfWhereToStoreView, rtvSize,
			width, height, DXGI_FORMAT_R8G8B8A8_UNORM, mSampleCount);
	}

	void MultiSampling::CreateDepthStencilBufferAndView(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
		const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& dsvHeap, unsigned int indexOfWhereToStoreView, unsigned int dsvSize,
		unsigned int width, unsigned int height)
	{
		mMSAADepthStencilBuffer.CreateDepthStencilBufferAndView(device, dsvHeap, indexOfWhereToStoreView, dsvSize,
			width, height, DXGI_FORMAT_D24_UNORM_S8_UINT, mSampleCount);
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