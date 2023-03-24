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
	//VERTEX BUFFER FUNCTION DEFINITIONS

	const D3D12_VERTEX_BUFFER_VIEW& VertexBuffer::GetVertexBufferView()
	{
		return mVertexBufferView;
	}

	void VertexBuffer::CreateVertexBuffer(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
		const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList, const void* data, UINT numBytes)
	{
		D3D12_RESOURCE_DESC vertexBufferDescription{};
		vertexBufferDescription.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		vertexBufferDescription.Alignment = 0;
		vertexBufferDescription.Width = numBytes;
		vertexBufferDescription.Height = 1;
		vertexBufferDescription.DepthOrArraySize = 1;
		vertexBufferDescription.MipLevels = 1;
		vertexBufferDescription.Format = DXGI_FORMAT_UNKNOWN;
		vertexBufferDescription.SampleDesc.Count = 1;
		vertexBufferDescription.SampleDesc.Quality = 0;
		vertexBufferDescription.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		vertexBufferDescription.Flags = D3D12_RESOURCE_FLAG_NONE;

		//Use this class to say which type of heap our buffer will be stored in.
		CD3DX12_HEAP_PROPERTIES vertexDefaultHeapProp(D3D12_HEAP_TYPE_DEFAULT);

		CD3DX12_HEAP_PROPERTIES vertexUploadHeapProp(D3D12_HEAP_TYPE_UPLOAD);

		//Creates the resource and allocates enough memory on the heap to contain the entire resource.
		//The resource is also mapped to the heap.
		ThrowIfFailed(device->CreateCommittedResource(&vertexDefaultHeapProp,
			D3D12_HEAP_FLAG_NONE, &vertexBufferDescription,
			D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&mVertexDefaultBuffer)));

		ThrowIfFailed(device->CreateCommittedResource(&vertexUploadHeapProp,
			D3D12_HEAP_FLAG_NONE, &vertexBufferDescription,
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mVertexUploadBuffer)));

		//this describes the data we want to copy to the default buffer
		D3D12_SUBRESOURCE_DATA vertexData{};
		vertexData.pData = data;
		vertexData.RowPitch = numBytes;
		vertexData.SlicePitch = vertexData.RowPitch;

		CD3DX12_RESOURCE_BARRIER vertexDefaultBufferTransitionToCopyState =
			CD3DX12_RESOURCE_BARRIER::Transition(mVertexDefaultBuffer.Get(),
				D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
		//Transistion the vertex default buffer to a copy state
		commandList->ResourceBarrier(1, &vertexDefaultBufferTransitionToCopyState);

		//This helper function copies our vertex data into our upload buffer, then copies the data from the upload buffer
		//to the default buffer
		UpdateSubresources<1>(commandList.Get(), mVertexDefaultBuffer.Get(),
			mVertexUploadBuffer.Get(), 0, 0, 1, &vertexData);

		CD3DX12_RESOURCE_BARRIER vertexDefaultBufferTransitionToReadState =
			CD3DX12_RESOURCE_BARRIER::Transition(mVertexDefaultBuffer.Get(),
				D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
		//Transition the default buffer to a read state
		commandList->ResourceBarrier(1, &vertexDefaultBufferTransitionToReadState);
	}

	void VertexBuffer::CreateVertexBufferView(UINT numBytes, UINT stride)
	{
		mVertexBufferView.BufferLocation = mVertexDefaultBuffer->GetGPUVirtualAddress();
		mVertexBufferView.SizeInBytes = numBytes;
		mVertexBufferView.StrideInBytes = stride;
	}
	//-----------------------------------------------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------------------------------------------------
	//Index BUFFER FUNCTION DEFINITIONS

	const D3D12_INDEX_BUFFER_VIEW& IndexBuffer::GetIndexBufferView()
	{
		return mIndexBufferView;
	}

	void IndexBuffer::CreateIndexBuffer(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
		const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList, const void* data, UINT numBytes)
	{
		D3D12_RESOURCE_DESC indexBufferDescription{};
		indexBufferDescription.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		indexBufferDescription.Alignment = 0;
		indexBufferDescription.Width = numBytes;
		indexBufferDescription.Height = 1;
		indexBufferDescription.DepthOrArraySize = 1;
		indexBufferDescription.MipLevels = 1;
		indexBufferDescription.Format = DXGI_FORMAT_UNKNOWN;
		indexBufferDescription.SampleDesc.Count = 1;
		indexBufferDescription.SampleDesc.Quality = 0;
		indexBufferDescription.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		indexBufferDescription.Flags = D3D12_RESOURCE_FLAG_NONE;

		//Use this class to say which type of heap our buffer will be stored in.
		CD3DX12_HEAP_PROPERTIES indexDefaultHeapProp(D3D12_HEAP_TYPE_DEFAULT);

		CD3DX12_HEAP_PROPERTIES indexUploadHeapProp(D3D12_HEAP_TYPE_UPLOAD);

		//Creates the resource and allocates enough memory on the heap to contain the entire resource.
		//The resource is also mapped to the heap.
		ThrowIfFailed(device->CreateCommittedResource(&indexDefaultHeapProp,
			D3D12_HEAP_FLAG_NONE, &indexBufferDescription,
			D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&mIndexDefaultBuffer)));

		ThrowIfFailed(device->CreateCommittedResource(&indexUploadHeapProp,
			D3D12_HEAP_FLAG_NONE, &indexBufferDescription,
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mIndexUploadBuffer)));

		//this describes the data we want to copy to the default buffer
		D3D12_SUBRESOURCE_DATA indexData{};
		indexData.pData = data;
		indexData.RowPitch = numBytes;
		indexData.SlicePitch = indexData.RowPitch;

		CD3DX12_RESOURCE_BARRIER indexDefaultBufferTransitionToCopyState =
			CD3DX12_RESOURCE_BARRIER::Transition(mIndexDefaultBuffer.Get(), D3D12_RESOURCE_STATE_COMMON,
				D3D12_RESOURCE_STATE_COPY_DEST);
		//Transistion the vertex default buffer to a copy state
		commandList->ResourceBarrier(1, &indexDefaultBufferTransitionToCopyState);

		//This helper function copies our vertex data into our upload buffer, then copies the data from the upload buffer
		//to the default buffer
		UpdateSubresources<1>(commandList.Get(), mIndexDefaultBuffer.Get(),
			mIndexUploadBuffer.Get(), 0, 0, 1, &indexData);

		CD3DX12_RESOURCE_BARRIER indexDefaultBufferTransitionToReadState =
			CD3DX12_RESOURCE_BARRIER::Transition(mIndexDefaultBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST,
				D3D12_RESOURCE_STATE_GENERIC_READ);
		//Transition the default buffer to a read state
		commandList->ResourceBarrier(1, &indexDefaultBufferTransitionToReadState);
	}

	void IndexBuffer::CreateIndexBufferView(UINT numBytes, DXGI_FORMAT format)
	{
		mIndexBufferView.BufferLocation = mIndexDefaultBuffer->GetGPUVirtualAddress();
		mIndexBufferView.SizeInBytes = numBytes;
		mIndexBufferView.Format = format;
	}
	//-----------------------------------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------------------------------
	//CONSTANT BUFFER FUNCTION DEFINITIONS

	ConstantBuffer::~ConstantBuffer()
	{
		if (mConstantBuffer != nullptr)
			mConstantBuffer->Unmap(0, nullptr);

		mMappedData = nullptr;
	}

	void ConstantBuffer::CreateConstantBuffer(const Microsoft::WRL::ComPtr<ID3D12Device>& device, const UINT& numOfBytes)
	{
		//describe the constant buffer
		D3D12_RESOURCE_DESC constantBufferDescription{};
		constantBufferDescription.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		constantBufferDescription.Alignment = 0;
		constantBufferDescription.Width = numOfBytes;
		constantBufferDescription.Height = 1;
		constantBufferDescription.DepthOrArraySize = 1;
		constantBufferDescription.MipLevels = 1;
		constantBufferDescription.Format = DXGI_FORMAT_UNKNOWN;
		constantBufferDescription.SampleDesc.Count = 1;
		constantBufferDescription.SampleDesc.Quality = 0;
		constantBufferDescription.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		constantBufferDescription.Flags = D3D12_RESOURCE_FLAG_NONE;

		//Use this class to say which type of heap our buffer will be stored in.
		CD3DX12_HEAP_PROPERTIES constantBufferHeapProp(D3D12_HEAP_TYPE_UPLOAD);

		//allocate memory for the constant buffer
		ThrowIfFailed(device->CreateCommittedResource(&constantBufferHeapProp,
			D3D12_HEAP_FLAG_NONE, &constantBufferDescription,
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mConstantBuffer)));

		//Map the constant buffer
		ThrowIfFailed(mConstantBuffer->Map(0, nullptr, (void**)&mMappedData));
	}

	void ConstantBuffer::CreateConstantBufferView(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
		const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& cbvHeap, UINT cbvSize, UINT cbvHeapIndex,
		UINT cBufferIndex, UINT numBytes)
	{
		D3D12_GPU_VIRTUAL_ADDRESS cbAddress{ mConstantBuffer->GetGPUVirtualAddress() };

		//describe the constant buffer view
		D3D12_CONSTANT_BUFFER_VIEW_DESC constBufferViewDescription{};

		//offset to the object's constant data in the consatnt buffer
		constBufferViewDescription.BufferLocation = cbAddress + (UINT64)(cBufferIndex * numBytes);

		//number of bytes the object's constant data has
		constBufferViewDescription.SizeInBytes = numBytes;

		//Get the address of where we want to store the view in the descriptor heap
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle =
			CD3DX12_CPU_DESCRIPTOR_HANDLE(cbvHeap->GetCPUDescriptorHandleForHeapStart());
		handle.Offset(cbvHeapIndex, cbvSize);

		//create the constant buffer view
		device->CreateConstantBufferView(&constBufferViewDescription, handle);
	}

	void ConstantBuffer::CopyData(UINT index, UINT byteSize, const void* data, UINT64 numOfBytes)
	{
		memcpy(&mMappedData[index * byteSize], data, numOfBytes);
	}
	//----------------------------------------------------------------------------------------------------------------------

}