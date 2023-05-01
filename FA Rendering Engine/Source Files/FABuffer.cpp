#include "FABuffer.h"
#include "FADirectXException.h"
#include <d3dx12.h>
#include <DDSTextureLoader.h>

namespace FARender
{
	//-----------------------------------------------------------------------------------------------------------------------
	//RENDER TARGET BUFFER FUNCTION DEFINITIONS

	RenderTargetBuffer::RenderTargetBuffer() : mRenderTargetBuffer{ nullptr }
	{}

	RenderTargetBuffer::RenderTargetBuffer(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
		const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& rtvHeap, unsigned int index, unsigned int rtvSize,
		unsigned int width, unsigned int height, DXGI_FORMAT format, unsigned int sampleCount)
	{
		CreateRenderTargetBufferAndView(device, rtvHeap, index, rtvSize, width, height, format, sampleCount);
	}

	DXGI_FORMAT RenderTargetBuffer::GetRenderTargetFormat() const
	{
		return mRenderTargetBuffer->GetDesc().Format;
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
		const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& rtvHeap, unsigned int index, unsigned int rtvSize,
		unsigned int width, unsigned int height, DXGI_FORMAT format, unsigned int sampleCount)
	{
		D3D12_RESOURCE_DESC renderTargetBufferDesc{};
		renderTargetBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		renderTargetBufferDesc.Alignment = 0;
		renderTargetBufferDesc.Width = width;
		renderTargetBufferDesc.Height = height;
		renderTargetBufferDesc.DepthOrArraySize = 1;
		renderTargetBufferDesc.MipLevels = 1;
		renderTargetBufferDesc.Format = format;
		renderTargetBufferDesc.SampleDesc.Count = sampleCount;
		renderTargetBufferDesc.SampleDesc.Quality = 0;
		renderTargetBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		renderTargetBufferDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

		D3D12_CLEAR_VALUE rtBufferClearValue{};
		rtBufferClearValue.Format = format;
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
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(rtvHeap->GetCPUDescriptorHandleForHeapStart(), index, rtvSize);

		//Create the RTV
		device->CreateRenderTargetView(mRenderTargetBuffer.Get(), nullptr, rtvHeapHandle);
	}

	void RenderTargetBuffer::ReleaseBuffer()
	{
		mRenderTargetBuffer = nullptr;
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

	DepthStencilBuffer::DepthStencilBuffer() : mDepthStencilBuffer{ nullptr }
	{}

	DepthStencilBuffer::DepthStencilBuffer(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
		const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& dsvHeap, unsigned int index, unsigned int dsvSize,
		unsigned int width, unsigned int height, DXGI_FORMAT format, unsigned int sampleCount)
	{
		CreateDepthStencilBufferAndView(device, dsvHeap, index, dsvSize, width, height, format, sampleCount);
	}

	DXGI_FORMAT DepthStencilBuffer::GetDepthStencilFormat() const
	{
		return mDepthStencilBuffer->GetDesc().Format;
	}

	void DepthStencilBuffer::CreateDepthStencilBufferAndView(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
		const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& dsvHeap, unsigned int index, unsigned int dsvSize,
		unsigned int width, unsigned int height, DXGI_FORMAT format, unsigned int sampleCount)
	{
		//Create Depth/Stenicl Buffer and View (Descriptor)

		D3D12_RESOURCE_DESC depthBufferDescription{};
		depthBufferDescription.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		depthBufferDescription.Alignment = 0;
		depthBufferDescription.Width = width;
		depthBufferDescription.Height = height;
		depthBufferDescription.DepthOrArraySize = 1;
		depthBufferDescription.MipLevels = 1;
		depthBufferDescription.Format = format;
		depthBufferDescription.SampleDesc.Count = sampleCount;
		depthBufferDescription.SampleDesc.Quality = 0;
		depthBufferDescription.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		depthBufferDescription.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		D3D12_CLEAR_VALUE depthBufferClearValue{};
		depthBufferClearValue.Format = format;
		depthBufferClearValue.DepthStencil.Depth = 1.0f;
		depthBufferClearValue.DepthStencil.Stencil = 0;

		//Use this class to say which type of heap our buffer will be stored in.
		CD3DX12_HEAP_PROPERTIES dHeapProp(D3D12_HEAP_TYPE_DEFAULT);

		ThrowIfFailed(device->CreateCommittedResource(&dHeapProp, D3D12_HEAP_FLAG_NONE, &depthBufferDescription,
			D3D12_RESOURCE_STATE_DEPTH_WRITE, &depthBufferClearValue, IID_PPV_ARGS(&mDepthStencilBuffer)));

		D3D12_DEPTH_STENCIL_VIEW_DESC depthBufferViewDescription{};
		depthBufferViewDescription.Format = format;

		if (sampleCount == 1)
			depthBufferViewDescription.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		else
			depthBufferViewDescription.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DMS;

		depthBufferViewDescription.Flags = D3D12_DSV_FLAG_NONE;

		//Get the address of where you want to store the view in the DSV heap.
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHeapHandle(dsvHeap->GetCPUDescriptorHandleForHeapStart(), index, dsvSize);

		device->CreateDepthStencilView(mDepthStencilBuffer.Get(), &depthBufferViewDescription, dsvHeapHandle);
	}

	void DepthStencilBuffer::ReleaseBuffer()
	{
		mDepthStencilBuffer = nullptr;
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

	StaticBuffer::StaticBuffer() : mStaticDefaultBuffer{ nullptr }, mStaticUploadBuffer{ nullptr }, mStride{ 0 }
	{}

	StaticBuffer::StaticBuffer(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
		const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList, const void* data,
		unsigned int numBytes, unsigned int stride)
	{
		CreateStaticBuffer(device, commandList, data, numBytes, stride);
	}

	StaticBuffer::StaticBuffer(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
		const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList, const void* data,
		unsigned int numBytes, DXGI_FORMAT format)
	{
		CreateStaticBuffer(device, commandList, data, numBytes, format);
	}

	StaticBuffer::StaticBuffer(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
		const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList, const wchar_t* filename)
	{
		CreateStaticBuffer(device, commandList, filename);
	}

	 void StaticBuffer::CreateStaticBuffer(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
		const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList, const void* data,
		unsigned int numBytes, unsigned int stride)
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

		 mStride = stride;
	}

	 void StaticBuffer::CreateStaticBuffer(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
		 const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList, const void* data,
		 unsigned int numBytes, DXGI_FORMAT format)
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

		 mFormat = format;
	 }

	 void StaticBuffer::CreateStaticBuffer(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
		 const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList, const wchar_t* filename)
	 {
		 DirectX::CreateDDSTextureFromFile12(device.Get(), commandList.Get(), filename, mStaticDefaultBuffer, mStaticUploadBuffer);

		 mStride = 0;
	 }

	const D3D12_VERTEX_BUFFER_VIEW StaticBuffer::GetVertexBufferView() const
	{
		D3D12_VERTEX_BUFFER_VIEW vBView{};
		vBView.BufferLocation = mStaticDefaultBuffer->GetGPUVirtualAddress();
		vBView.SizeInBytes = mStaticDefaultBuffer->GetDesc().Width;
		vBView.StrideInBytes = mStride;

		return vBView;
	}

	const D3D12_INDEX_BUFFER_VIEW StaticBuffer::GetIndexBufferView() const
	{
		D3D12_INDEX_BUFFER_VIEW ibView{};
		ibView.BufferLocation = mStaticDefaultBuffer->GetGPUVirtualAddress();
		ibView.SizeInBytes = mStaticDefaultBuffer->GetDesc().Width;
		ibView.Format = mFormat;

		return ibView;
	}

	void StaticBuffer::CreateTexture2DView(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
		const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& srvHeap, unsigned int srvSize, unsigned int index)
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC tex2DView{};
		tex2DView.Format = mStaticDefaultBuffer->GetDesc().Format;
		tex2DView.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		tex2DView.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		tex2DView.Texture2D.MostDetailedMip = 0;
		tex2DView.Texture2D.MipLevels = mStaticDefaultBuffer->GetDesc().MipLevels;
		tex2DView.Texture2D.PlaneSlice = 0;
		tex2DView.Texture2D.ResourceMinLODClamp = 0.0f;

		//Offset to where to store the view in the view heap.
		CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(srvHeap->GetCPUDescriptorHandleForHeapStart());
		hDescriptor.Offset(index, srvSize);

		device->CreateShaderResourceView(mStaticDefaultBuffer.Get(), &tex2DView, hDescriptor);
	}

	void StaticBuffer::CreateTexture2DMSView(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
		const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& srvHeap, unsigned int srvSize, unsigned int index)
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC tex2DMSView{};
		tex2DMSView.Format = mStaticDefaultBuffer->GetDesc().Format;
		tex2DMSView.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMS;
		tex2DMSView.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

		//Offset to where to store the view in the view heap.
		CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(srvHeap->GetCPUDescriptorHandleForHeapStart());
		hDescriptor.Offset(index, srvSize);

		device->CreateShaderResourceView(mStaticDefaultBuffer.Get(), &tex2DMSView, hDescriptor);
	}

	void StaticBuffer::ReleaseBuffer()
	{
		mStaticDefaultBuffer = nullptr;
		mStaticUploadBuffer = nullptr;
	}
	//-----------------------------------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------------------------------
	//DYNAMIC BUFFER FUNCTION DEFINITIONS

	DynamicBuffer::DynamicBuffer() : mDynamicBuffer{ nullptr }, mStride{ 0 }
	{}

	DynamicBuffer::DynamicBuffer(const Microsoft::WRL::ComPtr<ID3D12Device>& device, unsigned int numOfBytes, unsigned int stride)
	{
		CreateDynamicBuffer(device, numOfBytes, stride);
	}

	DynamicBuffer::DynamicBuffer(const Microsoft::WRL::ComPtr<ID3D12Device>& device, unsigned int numOfBytes, DXGI_FORMAT format)
	{
		CreateDynamicBuffer(device, numOfBytes, format);
	}

	DynamicBuffer::~DynamicBuffer()
	{
		if (mDynamicBuffer != nullptr)
			mDynamicBuffer->Unmap(0, nullptr);

		mMappedData = nullptr;
	}

	void DynamicBuffer::CreateDynamicBuffer(const Microsoft::WRL::ComPtr<ID3D12Device>& device, 
		unsigned int numOfBytes, unsigned int stride)
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

	void DynamicBuffer::CreateDynamicBuffer(const Microsoft::WRL::ComPtr<ID3D12Device>& device, 
		unsigned int numOfBytes, DXGI_FORMAT format)
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

	const D3D12_GPU_VIRTUAL_ADDRESS DynamicBuffer::GetGPUAddress(unsigned int index) const
	{
		D3D12_GPU_VIRTUAL_ADDRESS address{ mDynamicBuffer->GetGPUVirtualAddress() };

		unsigned long long offset = (unsigned long long)(index * mStride);

		return address + offset;
	}

	void DynamicBuffer::CreateConstantBufferView(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
		const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& cbvHeap, unsigned int cbvSize, unsigned int cbvHeapIndex,
		unsigned int cBufferIndex)
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

	const D3D12_VERTEX_BUFFER_VIEW DynamicBuffer::GetVertexBufferView()
	{
		D3D12_VERTEX_BUFFER_VIEW vBView{};
		vBView.BufferLocation = mDynamicBuffer->GetGPUVirtualAddress();
		vBView.SizeInBytes = mDynamicBuffer->GetDesc().Width;
		vBView.StrideInBytes = mStride;

		return vBView;
	}

	const D3D12_INDEX_BUFFER_VIEW DynamicBuffer::GetIndexBufferView()
	{
		D3D12_INDEX_BUFFER_VIEW ibView{};
		ibView.BufferLocation = mDynamicBuffer->GetGPUVirtualAddress();
		ibView.SizeInBytes = mDynamicBuffer->GetDesc().Width;
		ibView.Format = mFormat;

		return ibView;
	}

	void DynamicBuffer::CopyData(unsigned int index, const void* data, unsigned long long numOfBytes)
	{
		memcpy(&mMappedData[index * mStride], data, numOfBytes);
	}

	void DynamicBuffer::ReleaseBuffer()
	{
		mDynamicBuffer = nullptr;
	}

	//----------------------------------------------------------------------------------------------------------------------


}