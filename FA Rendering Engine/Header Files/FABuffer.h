#pragma once

/** @file FABuffer.h
*	@brief File has classes VertexBuffer, IndexBuffer and ConstantBuffer under namespace FARender.
*/

#include <wrl.h>
#include <d3d12.h>

/** @namespace FARender
*	@brief Has classes that are used for rendering objects and text through the Direct3D 12 API.
*/
namespace FARender
{
	/** @class RenderTargetBuffer ""
	*	@brief A wrapper for render target buffer resources. Uses DirectD 12 API.
	*/
	class RenderTargetBuffer
	{
	public:
		/**@brief Default Constructor.
		*/
		RenderTargetBuffer(DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);

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
		*/
		void CreateRenderTargetBufferAndView(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
			const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& rtvHeap, unsigned int indexOfWhereToStoreView, unsigned int rtvSize,
			unsigned int width, unsigned int height, unsigned int sampleCount = 1);

		/**@brief Resest the render target buffer.
		*/
		void ResetBuffer();

		/**@brief Clears the render target buffer with the specified clear value.
		*/
		void ClearRenderTargetBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
			const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& rtvHeap, unsigned int indexOfView, unsigned int rtvSize,
			const float* clearValue);

	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> mRenderTargetBuffer;
		DXGI_FORMAT mRenderTargetFormat;

	};

	/** @class DepthStencilBuffer ""
	*	@brief A wrapper for depth stencil buffer resources. Uses DirectD 12 API.
	*/
	class DepthStencilBuffer
	{
	public:

		/**@brief Default Constructor.
		*/
		DepthStencilBuffer(DXGI_FORMAT format = DXGI_FORMAT_D24_UNORM_S8_UINT);

		/**@brief Returns the format of the depth stencil buffer.
		*/
		DXGI_FORMAT GetDepthStencilFormat() const;

		/**@brief Creates the depth stencil buffer and view.
		*/
		void CreateDepthStencilBufferAndView(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
			const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& dsvHeap, unsigned int indexOfWhereToStoreView, unsigned int dsvSize,
			unsigned int width, unsigned int height, unsigned int sampleCount = 1);

		/**@brief Resest the depth stencil buffer.
		*/
		void ResetBuffer();

		/**@brief Clears the depth stencil buffer with the specified clear value.
		*/
		void ClearDepthStencilBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
			const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& dsvHeap, unsigned int indexOfView, unsigned int dsvSize,
			float clearValue);

	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> mDepthStencilBuffer;
		DXGI_FORMAT mDepthStencilFormat;
	};


	/** @class VertexBuffer ""
	*	@brief This class stores vertices in a Direct3D 12 default buffer.
	*
	*/
	class VertexBuffer
	{
	public:
		VertexBuffer() = default;
		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;

		/**@brief Creates the vertex buffer and stores all of the specified vertices in the vertex buffer.
		*/
		void CreateVertexBuffer(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
			const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList, const void* data, UINT numBytes);

		/**@brief Creates the vertex buffer view and stores it.
		*/
		void CreateVertexBufferView(UINT numBytes, UINT stride);

		/**@brief Returns a constant reference to the vertex buffer view.
		*/
		const D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferView();

	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> mVertexDefaultBuffer;
		Microsoft::WRL::ComPtr<ID3D12Resource> mVertexUploadBuffer;
		D3D12_VERTEX_BUFFER_VIEW mVertexBufferView{};
	};

	/** @class IndexBuffer ""
	*	@brief This class stores indices in a Direct3D 12 default buffer.
	*
	*/
	class IndexBuffer
	{
	public:
		IndexBuffer() = default;
		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator=(const IndexBuffer&) = delete;

		/**@brief Returns a constant reference to the vertex buffer view.
		*/
		const D3D12_INDEX_BUFFER_VIEW& GetIndexBufferView();

		/**@brief Creates the vertex buffer and stores all of the specified vertices in the vertex buffer.
		*/
		void CreateIndexBuffer(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
			const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList, const void* data, UINT numBytes);

		/**@brief Creates the vertex buffer view and stores it.
		*/
		void CreateIndexBufferView(UINT numBytes, DXGI_FORMAT format);

	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> mIndexDefaultBuffer;
		Microsoft::WRL::ComPtr<ID3D12Resource> mIndexUploadBuffer;
		D3D12_INDEX_BUFFER_VIEW mIndexBufferView{};
	};

	/** @class ConstantBuffer ""
	*	@brief This class stores constant data in a Direct3D 12 upload buffer.
	*
	*/
	class ConstantBuffer
	{
	public:
		ConstantBuffer() = default;

		ConstantBuffer(const ConstantBuffer&) = delete;
		ConstantBuffer& operator=(const ConstantBuffer&) = delete;

		/**@brief Unmaps the pointer to the constant buffer.
		*/
		~ConstantBuffer();

		/**@brief Creates and maps the constant buffer.
		* The number of bytes allocated should be a multiple of 256 bytes.
		*/
		void CreateConstantBuffer(const Microsoft::WRL::ComPtr<ID3D12Device>& device, const UINT& numOfBytes);

		/**@brief Creates and maps the constant buffer view and stores it in the specified descriptor heap.
		*/
		void CreateConstantBufferView(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
			const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& cbvHeap, UINT cbvSize, UINT cBufferIndex,
			UINT cbvHeapIndex, UINT numBytes);

		/**@brief Copies data from the given data into the constant buffer.
		* Uses 0-indexing.
		*/
		void CopyData(UINT index, UINT byteSize, const void* data, UINT64 numOfBytes);

	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> mConstantBuffer;
		BYTE* mMappedData{ nullptr };
	};
}