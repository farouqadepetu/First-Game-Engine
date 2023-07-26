#pragma once

#include <wrl.h>
#include <d3d12.h>

/** @namespace RenderingEngine
*	@brief Has classes that are used for rendering objects and text through the Direct3D 12 API.
*/
namespace RenderingEngine
{
	enum BufferTypes { VERTEX_BUFFER, INDEX_BUFFER, CONSTANT_BUFFER, TEXTURE_BUFFER };
	enum TextureTypes { TEX2D, TEX2D_MS };

	/** @class RenderTargetBuffer ""
	*	@brief A wrapper for render target buffer resources. Uses DirectD 12 API.
	*	The copy constructor and assignment operators are explicitly deleted. This makes this class non-copyable.
	*/
	class RenderTargetBuffer
	{
	public:

		//No copying
		RenderTargetBuffer(const RenderTargetBuffer&) = delete;
		RenderTargetBuffer& operator=(const RenderTargetBuffer&) = delete;

		/**@brief Creates a render target buffer object. No memory is allocated.
		* Called the CreateRenderTargetBufferAndView() function to allocate memory for the buffer.
		*/
		RenderTargetBuffer();

		/**@brief Creates the render target buffer and view.
		*
		* @param[in] device A Direct3D 12 device.
		* @param[in] rtvHeap A descriptor heap for storing render target descriptors.
		* @param[in] index The index of where to store the created descriptor in the descriptor heap.
		* @param[in] rtvSize The size of a render target descriptor.
		* @param[in] width The width of the render target buffer.
		* @param[in] height The height of the render target buffer.
		* @param[in] sampleCount The sample count of the render target buffer.
		*/
		RenderTargetBuffer(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
			const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& rtvHeap, unsigned int index, unsigned int rtvSize,
			unsigned int width, unsigned int height, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM, unsigned int sampleCount = 1);

		/**@brief Returns the format of the render target buffer.
		*/
		DXGI_FORMAT GetRenderTargetFormat() const;

		/**@brief Returns a reference to the render target buffer.
		*/
		Microsoft::WRL::ComPtr<ID3D12Resource>& GetRenderTargetBuffer();

		/**@brief Returns a constant reference to the render target buffer.
		*/
		const Microsoft::WRL::ComPtr<ID3D12Resource>& GetRenderTargetBuffer() const;

		/**@brief Creates the render target buffer and view.
		*
		* @param[in] device A Direct3D 12 device.
		* @param[in] rtvHeap A descriptor heap for storing render target descriptors.
		* @param[in] index The index of where to store the created descriptor in the descriptor heap.
		* @param[in] rtvSize The size of a render target descriptor.
		* @param[in] width The width of the render target buffer.
		* @param[in] height The height of the render target buffer.
		* @param[in] sampleCount The sample count of the render target buffer.
		*/
		void CreateRenderTargetBufferAndView(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
			const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& rtvHeap, unsigned int index, unsigned int rtvSize,
			unsigned int width, unsigned int height, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM, unsigned int sampleCount = 1);

		/**@brief Frees the memory of the buffer.
		*/
		void ReleaseBuffer();

		/**@brief Clears the render target buffer with the specified clear value.
		*
		* @param[in] commadList A Direct3D 12 graphics command list.
		* @param[in] rtvHeap A render target descriptor heap.
		* @param[in] indexOfView The index of where the render target descriptor of the
		* render target buffer is stored in the descriptor heap.
		*
		* @param[in] rtvSize The size of a render target descriptor.
		* @param[in] clearValue The RGBA values of what to set every element in the render target buffer to.
		*/
		void ClearRenderTargetBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
			const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& rtvHeap, unsigned int indexOfView, unsigned int rtvSize,
			const float* clearValue);

	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> mRenderTargetBuffer;
	};

	/** @class DepthStencilBuffer ""
	*	@brief A wrapper for depth stencil buffer resources. Uses DirectD 12 API.
	*   The copy constructor and assignment operators are explicitly deleted. This makes this class non-copyable.
	*/
	class DepthStencilBuffer
	{
	public:

		//No copying
		DepthStencilBuffer(const DepthStencilBuffer&) = delete;
		DepthStencilBuffer& operator=(const DepthStencilBuffer&) = delete;

		/**@brief Creates a depth stencil buffer object.
		* Call the CreateDepthStencilBufferAndView() to allocate memory for the buffer.
		*/
		DepthStencilBuffer();

		/**@brief Creates the depth stencil buffer and view.
		*
		* @param[in] device A Direct3D 12 device.
		* @param[in] dsvHeap A descriptor heap for storing depth stencil descriptors.
		* @param[in] indexOfWhereToStoreView The index of where to store the created descriptor in the descriptor heap.
		* @param[in] dsvSize The size of a depth stenicl descriptor.
		* @param[in] width The width of the depth stenicl buffer.
		* @param[in] height The height of the depth stenicl buffer.
		* @param[in] sampleCount The sample count of the depth stenicl buffer.
		*/
		DepthStencilBuffer(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
			const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& dsvHeap, unsigned int index, unsigned int dsvSize,
			unsigned int width, unsigned int height, DXGI_FORMAT format = DXGI_FORMAT_D24_UNORM_S8_UINT, unsigned int sampleCount = 1);

		/**@brief Returns the format of the depth stencil buffer.
		*/
		DXGI_FORMAT GetDepthStencilFormat() const;

		/**@brief Creates the depth stencil buffer and view.
		*
		* @param[in] device A Direct3D 12 device.
		* @param[in] dsvHeap A descriptor heap for storing depth stencil descriptors.
		* @param[in] indexOfWhereToStoreView The index of where to store the created descriptor in the descriptor heap.
		* @param[in] dsvSize The size of a depth stenicl descriptor.
		* @param[in] width The width of the depth stenicl buffer.
		* @param[in] height The height of the depth stenicl buffer.
		* @param[in] sampleCount The sample count of the depth stenicl buffer.
		*/
		void CreateDepthStencilBufferAndView(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
			const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& dsvHeap, unsigned int index, unsigned int dsvSize,
			unsigned int width, unsigned int height, DXGI_FORMAT format = DXGI_FORMAT_D24_UNORM_S8_UINT, unsigned int sampleCount = 1);

		/**@brief Frees the memory of the buffer.
		*/
		void ReleaseBuffer();

		/**@brief Clears the depth stencil buffer with the specified clear value.
		*
		* @param[in] commadList A Direct3D 12 graphics command list.
		* @param[in] dsvHeap A depth stencil descriptor heap.
		* @param[in] indexOfView The index of where the depth stencil descriptor of the
		* depth stencil buffer is stored in the descriptor heap.
		*
		* @param[in] dsvSize The size of a depth stencil descriptor.
		* @param[in] clearValue The value of what to set every element in the depth stencil buffer to.
		*/
		void ClearDepthStencilBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
			const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& dsvHeap, unsigned int indexOfView, unsigned int dsvSize,
			float clearValue);

	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> mDepthStencilBuffer;
	};


	/** @class StaticBuffer ""
	*	@brief This class stores data in a Direct3D 12 default buffer.
	*   The copy constructor and assignment operators are explicitly deleted. This makes this class non-copyable.
	*/
	class StaticBuffer
	{
	public:

		//No copying
		StaticBuffer(const StaticBuffer&) = delete;
		StaticBuffer& operator=(const StaticBuffer&) = delete;

		/**@brief Creates a static buffer object. No memory is allocated for the buffer.
		* Call one of the CreateStaticBuffer() functions to allocate memory for the buffer and store data in the buffer.
		*/
		StaticBuffer();

		/**@brief Creates a static vertex buffer and stores all of the specified data in the buffer.
		*
		* @param[in] device A Direct3D 12 device.
		*
		* @param[in] commadList A Direct3D 12 graphics command list.
		*
		* @param[in] data The data to store in the static vertex buffer.
		*
		* @param[in] numBytes The number of bytes to store in the static vertex buffer.
		*
		* @param[in] stride The number of bytes to get from one element to the next element.
		*/
		StaticBuffer(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
			const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList, const void* data,
			unsigned int numBytes, unsigned int stride);

		/**@brief Creates a static index buffer and stores all of the specified data in the buffer.
		*
		* @param[in] device A Direct3D 12 device.
		*
		* @param[in] commadList A Direct3D 12 graphics command list.
		*
		* @param[in] data The data to store in the static index buffer.
		*
		* @param[in] numBytes The number of bytes to store in the static index buffer.
		*
		* @param[in] format The number of bytes to get from one element to the next element.
		*/
		StaticBuffer(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
			const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList, const void* data,
			unsigned int numBytes, DXGI_FORMAT format);

		/**@brief Creates a static texture buffer and stores all of the data from the file in the buffer.
		*
		* @param[in] device A Direct3D 12 device.
		*
		* @param[in] commadList A Direct3D 12 graphics command list.
		*
		* @param[in] data The data to store in the static texture buffer.
		*
		* @param[in] numBytes The number of bytes to store in the static texture buffer.
		*
		* @param[in] filename The name of the texture file.
		*/
		StaticBuffer(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
			const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList, const wchar_t* filename);

		/**@brief Creates a static vertex buffer and stores all of the specified data in the buffer.
		*
		* @param[in] device A Direct3D 12 device.
		*
		* @param[in] commadList A Direct3D 12 graphics command list.
		*
		* @param[in] data The data to store in the static vertex buffer.
		*
		* @param[in] numBytes The number of bytes to store in the static vertex buffer.
		*
		* @param[in] stride The number of bytes to get from one element to the next element.
		*/
		void CreateStaticBuffer(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
			const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList, const void* data,
			unsigned int numBytes, unsigned int stride);

		/**@brief Creates a static index buffer and stores all of the specified data in the buffer.
		*
		* @param[in] device A Direct3D 12 device.
		*
		* @param[in] commadList A Direct3D 12 graphics command list.
		*
		* @param[in] data The data to store in the static index buffer.
		*
		* @param[in] numBytes The number of bytes to store in the static index buffer.
		*
		* @param[in] format The number of bytes to get from one element to the next element.
		*/
		void CreateStaticBuffer(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
			const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList, const void* data,
			unsigned int numBytes, DXGI_FORMAT format);

		/**@brief Creates a static texture buffer and stores all of the data from the file in the buffer.
		*
		* @param[in] device A Direct3D 12 device.
		*
		* @param[in] commadList A Direct3D 12 graphics command list.
		*
		* @param[in] data The data to store in the static texture buffer.
		*
		* @param[in] numBytes The number of bytes to store in the static texture buffer.
		*
		* @param[in] filename The name of the texture file.
		*/
		void CreateStaticBuffer(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
			const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList, const wchar_t* filename);

		/**@brief Returns the vertex buffer view of the static buffer.
		*/
		const D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView() const;

		/**@brief Returns the index buffer view of the static buffers.
		*/
		const D3D12_INDEX_BUFFER_VIEW GetIndexBufferView() const;

		/**@brief Creates a 2D texture view and stores it in the specified heap.
		*
		* @param[in] device A Direct3D 12 device.
		*
		* @param[in] srvHeap A shader resource view heap.
		*
		* @param[in] srvSize The size of a shader resource view.
		*
		* @param[in] index The index of where to store the texture view in the shader resource view heap.
		*/
		void CreateTexture2DView(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
			const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& srvHeap, unsigned int srvSize, unsigned int index);

		/**@brief Creates a multi-sampled 2D texture view and stores it in the specified heap.
		*
		* @param[in] device A Direct3D 12 device.
		*
		* @param[in] srvHeap A shader resource view heap.
		*
		* @param[in] srvSize The size of a shader resource view.
		*
		* @param[in] index The index of where to store the texture view in the shader resource view heap.
		*/
		void CreateTexture2DMSView(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
			const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& srvHeap, unsigned int srvSize, unsigned int index);

		/**@brief Frees the static buffer memory.
		*/
		void ReleaseBuffer();

	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> mStaticDefaultBuffer;
		Microsoft::WRL::ComPtr<ID3D12Resource> mStaticUploadBuffer;

		union
		{
			unsigned int mStride;
			DXGI_FORMAT mFormat;
		};
	};

	/** @class DynamicBuffer ""
	*	@brief This class stores data in a Direct3D 12 upload buffer.
	*	The copy constructor and assignment operators are explicitly deleted. This makes this class non-copyable.
	*/
	class DynamicBuffer
	{
	public:

		//No copying
		DynamicBuffer(const DynamicBuffer&) = delete;
		DynamicBuffer& operator=(const DynamicBuffer&) = delete;

		/**@brief Creates a dynamic buffer object. No memory is allocated for the buffer.
		* Call one of the CreateDynamicBuffer() functions to allocate memory for the buffer.
		*/
		DynamicBuffer();

		/**@brief Creates and maps a dynamic vertex buffer or a dynamic constant buffer.
		*
		* @param[in] device A Direct3D 12 device.
		*
		* @param[in] numOfBytes The number of bytes you want to allocate for the dynamic buffer.
		*
		* @param[in] stride The number of bytes to get from one element to another in the dynamic buffer.
		*/
		DynamicBuffer(const Microsoft::WRL::ComPtr<ID3D12Device>& device, unsigned int numOfBytes, unsigned int stride);

		/**@brief Creates and maps a dynamic index buffer.
		*
		* @param[in] device A Direct3D 12 device.
		*
		* @param[in] numOfBytes The number of bytes you want to allocate for the dynamic buffer.
		*
		* @param[in] format The number of bytes to get from one element to another in the dynamic buffer.
		*/
		DynamicBuffer(const Microsoft::WRL::ComPtr<ID3D12Device>& device, unsigned int numOfBytes, DXGI_FORMAT format);

		/**@brief Unmaps the pointer to the dynamic buffer.
		*/
		~DynamicBuffer();

		/**@brief Creates and maps a dynamic vertex buffer or a dynamic constant buffer.
		*
		* @param[in] device A Direct3D 12 device.
		*
		* @param[in] numOfBytes The number of bytes you want to allocate for the dynamic buffer.
		*
		* @param[in] stride The number of bytes to get from one element to another in the dynamic buffer.
		*/
		void CreateDynamicBuffer(const Microsoft::WRL::ComPtr<ID3D12Device>& device, unsigned int numOfBytes, unsigned int stride);

		/**@brief Creates and maps a dynamic index buffer.
		*
		* @param[in] device A Direct3D 12 device.
		*
		* @param[in] numOfBytes The number of bytes you want to allocate for the dynamic buffer.
		*
		* @param[in] format The number of bytes to get from one element to another in the dynamic buffer.
		*/
		void CreateDynamicBuffer(const Microsoft::WRL::ComPtr<ID3D12Device>& device, unsigned int numOfBytes, DXGI_FORMAT format);

		/**@brief Returns the GPU address of the data at the specified index.
		*/
		const D3D12_GPU_VIRTUAL_ADDRESS GetGPUAddress(unsigned int index) const;

		/**@brief Creates the constant buffer view and stores it in the specified descriptor heap.
		*
		* @param[in] device A Direct3D 12 device.
		* @param[in] cbvHeap A descriptor heap for storing constant buffer descriptors.
		* @param[in] cbvSize The size of a depth stenicl descriptor.
		* @param[in] cbvHeapIndex The index of where to store the created descriptor in the descriptor heap.
		* @param[in] cBufferIndex The index of the constant data in the constant buffer you want to describe.
		*/
		void CreateConstantBufferView(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
			const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& cbvHeap, unsigned int cbvSize, unsigned int cbvHeapIndex,
			unsigned int cBufferIndex);

		/**@brief Returns a the vertex buffer view of the dynamic buffer.
		*/
		const D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView();

		/**@brief Returns the index buffer view of the dynamic buffer.
		*/
		const D3D12_INDEX_BUFFER_VIEW GetIndexBufferView();

		/**@brief Copies data from the given data into the dynamic buffer.
		* Uses 0-indexing.
		*
		* @param[in] data The data to copy in the dynamic buffer.
		* @param[in] numOfBytes The number of bytes to copy.
		*/
		void CopyData(unsigned int index, const void* data, unsigned long long numOfBytes);

		/**@brief Frees the dynamic buffer memory.
		*/
		void ReleaseBuffer();

	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> mDynamicBuffer;
		BYTE* mMappedData{ nullptr };

		union
		{
			UINT mStride;
			DXGI_FORMAT mFormat;
		};
	};
}