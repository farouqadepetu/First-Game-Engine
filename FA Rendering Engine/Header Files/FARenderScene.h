#pragma once

/** @file FARenderScene.h
*	@brief File has class RenderScene under namespace FARender.
*/

#include <wrl.h>
#include <d3d12.h>
#include <d3dcompiler.h>
#include <unordered_map>
#include <string>
#include "FARenderingUtility.h"
#include "FADeviceResources.h"
#include "FABuffer.h"

namespace FARender
{
	/** @class RenderScene ""
	*	@brief This class is used to render a scene using Direct3D 12 API.
	*/
	class RenderScene
	{
	public:

		RenderScene() = default;

		RenderScene(const RenderScene&) = delete;
		RenderScene& operator=(const RenderScene&) = delete;

		/*@brief Returns a constant reference to the shader with the specified name.
		* Throws an out_of_range exception if the shader does not exist.
		*/
		const Microsoft::WRL::ComPtr<ID3DBlob>& shader(const std::wstring& name) const;

		/*@brief Returns a constant reference to an array of input element layout descriptions.
		* Throws an out_of_range exception if the array of input element layout descriptions does not exist.
		*/
		const std::vector<D3D12_INPUT_ELEMENT_DESC>& inputElementLayout(const std::wstring& name) const;

		/*@brief Returns a constant reference to the rasterization description with the specified name.
		* Throws an out_of_range exception if the rasterization description does not exist.
		*/
		const D3D12_RASTERIZER_DESC& rasterizationState(const std::wstring& name) const;

		/*@brief Returns a constant reference to the PSO with the specified name.
		* Throws an out_of_range exception if the PSO does not exist.
		*/
		const Microsoft::WRL::ComPtr<ID3D12PipelineState>& pso(const std::wstring& name) const;

		/*@brief Returns a constant reference to the root signature with the specified name.
		* Throws an out_of_range exception if the root signature does not exist.
		*/
		const Microsoft::WRL::ComPtr<ID3D12RootSignature>& rootSignature(const std::wstring& name) const;

		/*@brief Returns a reference to the constant buffer with the specified name.
		* Throws an out_of_range exception if the root signature does not exist.
		*/
		ConstantBuffer& cBuffer(const std::wstring& name);

		/*@brief Returns a constant reference to the constant buffer with the specified name.
		* Throws an out_of_range exception if the root signature does not exist.
		*/
		const ConstantBuffer& cBuffer(const std::wstring& name) const;

		/**@brief Returns a constant reference to the CBV/SRV/UAV descriptor size.
		*/
		const UINT& cbvSize() const;

		/**@brief Returns a constant reference to the CBV descriptor heap.
		*/
		const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& cbvHeap() const;

		/**@brief Returns a constant reference to the CBV's heap root parameter.
		*/
		const D3D12_ROOT_PARAMETER& cbvHeapRootParameter() const;

		/*@brief Returns a constant reference to the draw argument with the specifed name in the specified group.
		* Throws an out_of_range exception if the draw argument does not exist.
		*/
		const DrawArguments& drawArgument(const std::wstring& groupName, const std::wstring& objectName) const;

		/*@brief Loads a shader's bytecode and stores it with the specified name.
		*/
		void loadShader(const std::wstring& filename, const std::wstring& name);

		/*@brief Stores an array of input element descriptions with the specified name.
		*/
		void storeInputElementDescriptions(const std::wstring& name, const std::vector<D3D12_INPUT_ELEMENT_DESC>& inputElementLayout);

		/*@brief Stores an array of input element descriptions with the specified name.
		*/
		void storeInputElementDescriptions(const std::wstring& name, const D3D12_INPUT_ELEMENT_DESC* inputElementLayout, UINT numElements);

		/*@brief Creates a rasterization description and stores it with the specified name.
		*/
		void createRasterizationState(D3D12_FILL_MODE fillMode, BOOL enableMultisample, const std::wstring& name);

		/*@brief Creates a PSO and stores it with the specified name.
		*/
		void createPSO(const Microsoft::WRL::ComPtr<ID3D12Device>& device, const std::wstring& psoName,
			const std::wstring& rsName, const std::wstring& rStateName, const std::wstring& vsName, const std::wstring& psName,
			const std::wstring& inputLayoutName,
			const D3D12_PRIMITIVE_TOPOLOGY_TYPE& primitiveType, DXGI_FORMAT rtvFormat, DXGI_FORMAT dsvFormat, UINT sampleCount);

		/*@brief Creates a root signature and stores it with the specified name.
		*/
		void createRootSignature(const Microsoft::WRL::ComPtr<ID3D12Device>& device, const std::wstring& name,
			const D3D12_ROOT_PARAMETER* rootParameters, UINT numParameters);

		/*@brief Stores a DrawArgument object with the specified name in the specified group.
		*/
		void storeDrawArgument(const std::wstring& groupName, const std::wstring& objectName, const DrawArguments& drawArgs);

		/*@brief Creates a vertex buffer with the specified name and stores all of given data in the vertex buffer.
		* Execute commands and the flush command queue after calling createVertexBuffer() and createIndexBuffer().
		*/
		void createVertexBuffer(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
			const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
			const std::wstring& vbName, const void* data, UINT numBytes);

		/*@brief Creates a vertex buffer view for the vertex buffer with the specified name.
		*/
		void createVertexBufferView(const std::wstring& vbName, UINT numBytes, UINT stride);

		/**@brief Creates an indexbuffer with the specified name and stores all of given data in the index buffer.
		* Execute commands and flush the command queue after calling createVertexBuffer() and createIndexBuffer().
		*/
		void createIndexBuffer(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
			const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
			const std::wstring& ibName, const void* data, UINT numBytes);

		/*@brief Creates an index buffer view for the index buffer with the specified name.
		*/
		void createIndexBufferView(const std::wstring& ibName, UINT numBytes, DXGI_FORMAT format);

		/**@brief Creates the CBV heap.
		*/
		void createCBVHeap(const Microsoft::WRL::ComPtr<ID3D12Device>& device, UINT numDescriptors, UINT shaderRegister);

		/**@brief Creates a constant buffer for each frame and stores it with the specified name.
		*/
		void createConstantBuffer(const Microsoft::WRL::ComPtr<ID3D12Device>& device, const std::wstring& name,
			const UINT& numOfBytes);

		/**@brief Creates a constant buffer view for eacg frame and stores it in the CBV heap.
		*/
		void createConstantBufferView(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
			const std::wstring& name, UINT index, UINT numBytes);

		/**@brief Puts all of the commands needed in the command list before drawing the objects of the scene.
		* Call before calling the first drawObjects function.
		*/
		void beforeDraw(DeviceResources& deviceResource);

		/**@brief Draws all of the objects that are in the same vertex and index buffers and
		* use the same PSO and primitive.
		* Call in between a beforeDraw function and a afterDraw function.\n
		*
		* Ex.\n
		* beforeDraw()\n
		* drawObjects()\n
		* drawObjects()\n
		* afterDraw()\n
		*
		* Throws an out_of_range exception if the vertex buffer, index buffer, draw argument group,
		* PSO, or root signature does not exist.
		*/
		void drawObjects(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
			const std::wstring& drawArgsGroupName, const std::wstring& vbName, const std::wstring& ibName,
			const std::wstring& psoName, const std::wstring& rootSignatureName,
			const D3D_PRIMITIVE_TOPOLOGY& primitive);

		/**@brief Puts all of the commands needed in the command list after drawing the objects of the scene.
		* Call after calling all the drawObjects functions.
		*/
		void afterDraw(DeviceResources& deviceResource, Text* textToRender = nullptr, UINT numText = 0);

	private:
		//Stores all of the shaders and input element descriptions for this scene.
		std::unordered_map<std::wstring, Microsoft::WRL::ComPtr<ID3DBlob>> mShaders;
		std::unordered_map < std::wstring, std::vector<D3D12_INPUT_ELEMENT_DESC>> mInputElementDescriptions;

		//Stores all of the rasterization states, PSOs, and root signatures for this scene.
		std::unordered_map <std::wstring, D3D12_RASTERIZER_DESC> mRasterizationStates;
		std::unordered_map<std::wstring, Microsoft::WRL::ComPtr<ID3D12PipelineState>> mPSOs;
		std::unordered_map<std::wstring, Microsoft::WRL::ComPtr<ID3D12RootSignature>> mRootSignatures;

		//Each scene gets one CBV heap.
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mCBVHeap;
		UINT mCBVSize;
		D3D12_DESCRIPTOR_RANGE mCBVHeapDescription{};
		D3D12_ROOT_PARAMETER mCBVHeapRootParameter;

		//Stores all of the constant buffers this scene uses. We can't update a constant buffer until the GPU
		//is done executing all the commands that reference it, so each frame needs its own constant buffers.
		std::unordered_map<std::wstring, ConstantBuffer> mConstantBuffers[numFrames];

		//Groups all of the objects draw arguments that are in the same vertex buffer and index buffer,
		//and uses the same shaders, rasterization states, PSO, and root signatures.
		std::unordered_map<std::wstring, std::unordered_map<std::wstring, DrawArguments> > mDrawArgs;

		//Stores all of the vertex buffers and index buffers for this scene.
		std::unordered_map<std::wstring, VertexBuffer> mVertexBuffers;
		std::unordered_map<std::wstring, IndexBuffer> mIndexBuffers;
	};
}