#pragma once

/** @file FABuffer.h
*	@brief File has classes VertexBuffer, IndexBuffer and ConstantBuffer under namespace FARender.
*/

#include <wrl.h>
#include <d3d12.h>

namespace FARender
{
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
		D3D12_INDEX_BUFFER_VIEW mIndexBufferView;
	};

	/** @class ConstantBuffer ""
	*	@brief This class stores constant data in a Direct3D 12 upload buffers.
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