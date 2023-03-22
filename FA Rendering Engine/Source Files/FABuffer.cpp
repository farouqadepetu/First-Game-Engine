#include "FABuffer.h"
#include "FADirectXException.h"
#include <d3dx12.h>

namespace FARender
{
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

	Microsoft::WRL::ComPtr<ID3D12Resource>& ConstantBuffer::GetConstantBuffer()
	{
		return mConstantBuffer;
	}

	const Microsoft::WRL::ComPtr<ID3D12Resource>& ConstantBuffer::GetConstantBuffer() const
	{
		return mConstantBuffer;
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

	void ConstantBuffer::CopyData(UINT index, UINT byteSize, const void* data, const UINT64& numOfBytes)
	{
		memcpy(&mMappedData[index * byteSize], data, numOfBytes);
	}
	//----------------------------------------------------------------------------------------------------------------------

}