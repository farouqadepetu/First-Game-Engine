#include "FABuffer.h"
#include "FADirectXException.h"
#include <d3dx12.h>

namespace FARender
{
	//-----------------------------------------------------------------------------------------------------------------------
	//RENDER TARGET BUFFER FUNCTION DEFINITIONS
	
	RenderTargetBuffer::RenderTargetBuffer(DXGI_FORMAT format) : mRenderTargetFormat{ format }
	{}

	DXGI_FORMAT RenderTargetBuffer::GetRenderTargetFormat() const
	{
		return mRenderTargetFormat;
	}

	Microsoft::WRL::ComPtr<ID3D12Resource>& RenderTargetBuffer::GetRenderTargetBuffer()
	{
		return mRenderTargetBuffer;
	}

	const Microsoft::WRL::ComPtr<ID3D12Resource>& RenderTargetBuffer::GetRenderTargetBuffer() const
	{
		return mRenderTargetBuffer;
	}

	void RenderTargetBuffer::CreateRenderTargetBufferAndView(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
		const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& rtvHeap, unsigned int indexOfWhereToStoreView, unsigned int rtvSize,
		unsigned int width, unsigned int height, unsigned int sampleCount)
	{
		D3D12_RESOURCE_DESC renderTargetBufferDesc{};
		renderTargetBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		renderTargetBufferDesc.Alignment = 0;
		renderTargetBufferDesc.Width = width;
		renderTargetBufferDesc.Height = height;
		renderTargetBufferDesc.DepthOrArraySize = 1;
		renderTargetBufferDesc.MipLevels = 1;
		renderTargetBufferDesc.Format = mRenderTargetFormat;
		renderTargetBufferDesc.SampleDesc.Count = sampleCount;
		renderTargetBufferDesc.SampleDesc.Quality = 0;
		renderTargetBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		renderTargetBufferDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

		D3D12_CLEAR_VALUE rtBufferClearValue{};
		rtBufferClearValue.Format = mRenderTargetFormat;
		rtBufferClearValue.Color[0] = 0.0f;
		rtBufferClearValue.Color[1] = 0.0f;
		rtBufferClearValue.Color[2] = 0.0f;
		rtBufferClearValue.Color[3] = 1.0f;

		//Use this class to say which type of heap the buffer will be stored in.
		CD3DX12_HEAP_PROPERTIES rtHeapProp(D3D12_HEAP_TYPE_DEFAULT);

		//Create the RT buffer resouce
		ThrowIfFailed(device->CreateCommittedResource(&rtHeapProp, D3D12_HEAP_FLAG_NONE, &renderTargetBufferDesc,
			D3D12_RESOURCE_STATE_RESOLVE_SOURCE, &rtBufferClearValue, IID_PPV_ARGS(&mRenderTargetBuffer)));

		//Get the address of where you want to store the view in the RTV heap.
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(rtvHeap->GetCPUDescriptorHandleForHeapStart(), indexOfWhereToStoreView, rtvSize);

		//Create the RTV
		device->CreateRenderTargetView(mRenderTargetBuffer.Get(), nullptr, rtvHeapHandle);
	}

	void RenderTargetBuffer::ResetBuffer()
	{
		mRenderTargetBuffer.Reset();
	}

	void RenderTargetBuffer::ClearRenderTargetBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
		const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& rtvHeap, unsigned int indexOfView, unsigned int rtvSize,
		const float* clearValue)
	{
		//Get the address of where the view to the render target buffer is in the RTV heap.
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(rtvHeap->GetCPUDescriptorHandleForHeapStart(), indexOfView, rtvSize);

		commandList->ClearRenderTargetView(rtvHeapHandle, clearValue, 0, nullptr);
	}

	//-----------------------------------------------------------------------------------------------------------------------
	


	//-----------------------------------------------------------------------------------------------------------------------
	//DEPTH STENCIL BUFFER FUNCTION DEFINITIONS

	DepthStencilBuffer::DepthStencilBuffer(DXGI_FORMAT format) : mDepthStencilFormat{ format }
	{}

	DXGI_FORMAT DepthStencilBuffer::GetDepthStencilFormat() const
	{
		return mDepthStencilFormat;
	}

	void DepthStencilBuffer::CreateDepthStencilBufferAndView(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
		const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& dsvHeap, unsigned int indexOfWhereToStoreView, unsigned int dsvSize,
		unsigned int width, unsigned int height, unsigned int sampleCount)
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
		depthBufferDescription.SampleDesc.Count = sampleCount;
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

		if (sampleCount == 1)
			depthBufferViewDescription.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		else
			depthBufferViewDescription.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DMS;

		depthBufferViewDescription.Flags = D3D12_DSV_FLAG_NONE;

		//Get the address of where you want to store the view in the DSV heap.
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHeapHandle(dsvHeap->GetCPUDescriptorHandleForHeapStart(), indexOfWhereToStoreView, dsvSize);

		device->CreateDepthStencilView(mDepthStencilBuffer.Get(), &depthBufferViewDescription, dsvHeapHandle);
	}

	void DepthStencilBuffer::ResetBuffer()
	{
		mDepthStencilBuffer.Reset();
	}

	void DepthStencilBuffer::ClearDepthStencilBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
		const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& dsvHeap, unsigned int indexOfView, unsigned int dsvSize,
		float clearValue)
	{
		//Get the address of the depth stencil view in the DSV heap.
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHeapHandle(dsvHeap->GetCPUDescriptorHandleForHeapStart(), indexOfView, dsvSize);

		//Clear the DS buffer
		commandList->ClearDepthStencilView(dsvHeapHandle,
			D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, clearValue, 0, 0, nullptr);
	}

	//-----------------------------------------------------------------------------------------------------------------------


	//-----------------------------------------------------------------------------------------------------------------------
	//STATIC BUFFER FUNCTION DEFINITIONS

	void StaticBuffer::CreateStaticBuffer(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
		const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList, const void* data, UINT numBytes)
	{
		D3D12_RESOURCE_DESC staticBufferDescription{};
		staticBufferDescription.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		staticBufferDescription.Alignment = 0;
		staticBufferDescription.Width = numBytes;
		staticBufferDescription.Height = 1;
		staticBufferDescription.DepthOrArraySize = 1;
		staticBufferDescription.MipLevels = 1;
		staticBufferDescription.Format = DXGI_FORMAT_UNKNOWN;
		staticBufferDescription.SampleDesc.Count = 1;
		staticBufferDescription.SampleDesc.Quality = 0;
		staticBufferDescription.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		staticBufferDescription.Flags = D3D12_RESOURCE_FLAG_NONE;

		//Use this class to say which type of heap our buffer will be stored in.
		CD3DX12_HEAP_PROPERTIES staticDefaultHeapProp(D3D12_HEAP_TYPE_DEFAULT);

		CD3DX12_HEAP_PROPERTIES staticUploadHeapProp(D3D12_HEAP_TYPE_UPLOAD);

		//Creates the resource and allocates enough memory on the heap to contain the entire resource.
		//The resource is also mapped to the heap.
		ThrowIfFailed(device->CreateCommittedResource(&staticDefaultHeapProp,
			D3D12_HEAP_FLAG_NONE, &staticBufferDescription,
			D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&mStaticDefaultBuffer)));

		ThrowIfFailed(device->CreateCommittedResource(&staticUploadHeapProp,
			D3D12_HEAP_FLAG_NONE, &staticBufferDescription,
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mStaticUploadBuffer)));

		//this describes the data we want to copy to the default buffer
		D3D12_SUBRESOURCE_DATA staticData{};
		staticData.pData = data;
		staticData.RowPitch = numBytes;
		staticData.SlicePitch = staticData.RowPitch;

		CD3DX12_RESOURCE_BARRIER staticDefaultBufferTransitionToCopyState =
			CD3DX12_RESOURCE_BARRIER::Transition(mStaticDefaultBuffer.Get(),
				D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
		//Transistion the vertex default buffer to a copy state
		commandList->ResourceBarrier(1, &staticDefaultBufferTransitionToCopyState);

		//This helper function copies our vertex data into our upload buffer, then copies the data from the upload buffer
		//to the default buffer
		UpdateSubresources<1>(commandList.Get(), mStaticDefaultBuffer.Get(),
			mStaticUploadBuffer.Get(), 0, 0, 1, &staticData);

		CD3DX12_RESOURCE_BARRIER staticDefaultBufferTransitionToReadState =
			CD3DX12_RESOURCE_BARRIER::Transition(mStaticDefaultBuffer.Get(),
				D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
		//Transition the default buffer to a read state
		commandList->ResourceBarrier(1, &staticDefaultBufferTransitionToReadState);
	}

	void StaticBuffer::CreateVertexBufferView(UINT numBytes, UINT stride)
	{
		mVertexBufferView.BufferLocation = mStaticDefaultBuffer->GetGPUVirtualAddress();
		mVertexBufferView.SizeInBytes = numBytes;
		mVertexBufferView.StrideInBytes = stride;
	}

	void StaticBuffer::CreateIndexBufferView(UINT numBytes, DXGI_FORMAT format)
	{
		mIndexBufferView.BufferLocation = mStaticDefaultBuffer->GetGPUVirtualAddress();
		mIndexBufferView.SizeInBytes = numBytes;
		mIndexBufferView.Format = format;
	}

	const D3D12_VERTEX_BUFFER_VIEW& StaticBuffer::GetVertexBufferView()
	{
		return mVertexBufferView;
	}

	const D3D12_INDEX_BUFFER_VIEW& StaticBuffer::GetIndexBufferView()
	{
		return mIndexBufferView;
	}

	void StaticBuffer::ReleaseUploader()
	{
		mStaticUploadBuffer = nullptr;
	}


	//-----------------------------------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------------------------------
	//DYNAMIC BUFFER FUNCTION DEFINITIONS

	DynamicBuffer::~DynamicBuffer()
	{
		if (mDynamicBuffer != nullptr)
			mDynamicBuffer->Unmap(0, nullptr);

		mMappedData = nullptr;
	}

	D3D12_GPU_VIRTUAL_ADDRESS DynamicBuffer::GetGPUAddress() const
	{
		return mDynamicBuffer->GetGPUVirtualAddress();
	}

	const unsigned int& DynamicBuffer::GetStride() const
	{
		return mStride;
	}

	const DXGI_FORMAT& DynamicBuffer::GetFormat() const
	{
		return mFormat;
	}

	void DynamicBuffer::CreateDynamicBuffer(const Microsoft::WRL::ComPtr<ID3D12Device>& device,  UINT numOfBytes, UINT stride)
	{
		mStride = stride;

		//describe the constant buffer
		D3D12_RESOURCE_DESC dynamicBufferDescription{};
		dynamicBufferDescription.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		dynamicBufferDescription.Alignment = 0;
		dynamicBufferDescription.Width = numOfBytes;
		dynamicBufferDescription.Height = 1;
		dynamicBufferDescription.DepthOrArraySize = 1;
		dynamicBufferDescription.MipLevels = 1;
		dynamicBufferDescription.Format = DXGI_FORMAT_UNKNOWN;
		dynamicBufferDescription.SampleDesc.Count = 1;
		dynamicBufferDescription.SampleDesc.Quality = 0;
		dynamicBufferDescription.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		dynamicBufferDescription.Flags = D3D12_RESOURCE_FLAG_NONE;

		//Use this class to say which type of heap our buffer will be stored in.
		CD3DX12_HEAP_PROPERTIES dynamicBufferHeapProp(D3D12_HEAP_TYPE_UPLOAD);

		//allocate memory for the dynamic buffer
		ThrowIfFailed(device->CreateCommittedResource(&dynamicBufferHeapProp,
			D3D12_HEAP_FLAG_NONE, &dynamicBufferDescription,
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mDynamicBuffer)));

		//Map the dyanmic buffer
		ThrowIfFailed(mDynamicBuffer->Map(0, nullptr, (void**)&mMappedData));
	}

	void DynamicBuffer::CreateDynamicBuffer(const Microsoft::WRL::ComPtr<ID3D12Device>& device, UINT numOfBytes, DXGI_FORMAT format)
	{
		mFormat = format;

		//describe the constant buffer
		D3D12_RESOURCE_DESC dynamicBufferDescription{};
		dynamicBufferDescription.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		dynamicBufferDescription.Alignment = 0;
		dynamicBufferDescription.Width = numOfBytes;
		dynamicBufferDescription.Height = 1;
		dynamicBufferDescription.DepthOrArraySize = 1;
		dynamicBufferDescription.MipLevels = 1;
		dynamicBufferDescription.Format = DXGI_FORMAT_UNKNOWN;
		dynamicBufferDescription.SampleDesc.Count = 1;
		dynamicBufferDescription.SampleDesc.Quality = 0;
		dynamicBufferDescription.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		dynamicBufferDescription.Flags = D3D12_RESOURCE_FLAG_NONE;

		//Use this class to say which type of heap our buffer will be stored in.
		CD3DX12_HEAP_PROPERTIES dynamicBufferHeapProp(D3D12_HEAP_TYPE_UPLOAD);

		//allocate memory for the dynamic buffer
		ThrowIfFailed(device->CreateCommittedResource(&dynamicBufferHeapProp,
			D3D12_HEAP_FLAG_NONE, &dynamicBufferDescription,
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mDynamicBuffer)));

		//Map the dyanmic buffer
		ThrowIfFailed(mDynamicBuffer->Map(0, nullptr, (void**)&mMappedData));
	}

	void DynamicBuffer::CreateConstantBufferView(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
		const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& cbvHeap, UINT cbvSize, UINT cbvHeapIndex,
		UINT cBufferIndex)
	{
		//Get the GPU address of the dyanmic buffer.
		D3D12_GPU_VIRTUAL_ADDRESS cbAddress{ mDynamicBuffer->GetGPUVirtualAddress() };

		//describe the constant buffer.
		D3D12_CONSTANT_BUFFER_VIEW_DESC constBufferViewDescription{};

		//offset to the object's constant data in the consatnt buffer.
		constBufferViewDescription.BufferLocation = cbAddress + (UINT64)(cBufferIndex * mStride);

		//number of bytes the object's constant data has
		constBufferViewDescription.SizeInBytes = mStride;

		//Get the address of where we want to store the view in the descriptor heap
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle =
			CD3DX12_CPU_DESCRIPTOR_HANDLE(cbvHeap->GetCPUDescriptorHandleForHeapStart());
		handle.Offset(cbvHeapIndex, cbvSize);

		//create the constant buffer view
		device->CreateConstantBufferView(&constBufferViewDescription, handle);
	}

	void DynamicBuffer::CreateVertexBufferView(UINT numBytes)
	{
		mVertexBufferView.BufferLocation = mDynamicBuffer->GetGPUVirtualAddress();
		mVertexBufferView.SizeInBytes = numBytes;
		mVertexBufferView.StrideInBytes = mStride;
	}

	void DynamicBuffer::CreateIndexBufferView(UINT numBytes)
	{
		mIndexBufferView.BufferLocation = mDynamicBuffer->GetGPUVirtualAddress();
		mIndexBufferView.SizeInBytes = numBytes;
		mIndexBufferView.Format = mFormat;
	}

	const D3D12_VERTEX_BUFFER_VIEW& DynamicBuffer::GetVertexBufferView()
	{
		return mVertexBufferView;
	}

	const D3D12_INDEX_BUFFER_VIEW& DynamicBuffer::GetIndexBufferView()
	{
		return mIndexBufferView;
	}

	void DynamicBuffer::CopyData(UINT index, const void* data, UINT64 numOfBytes)
	{
		memcpy(&mMappedData[index * mStride], data, numOfBytes);
	}

	//----------------------------------------------------------------------------------------------------------------------


}