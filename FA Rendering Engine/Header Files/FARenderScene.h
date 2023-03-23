#pragma once

/** @file FARenderScene.h
*	@brief File has class RenderScene under namespace FARender.
*/

#include <d3dcompiler.h>
#include <unordered_map>
#include <string>
#include "FADeviceResources.h"
#include "FABuffer.h"
#include "FACamera.h"
#include "FAText.h"
#include "FAShapesUtility.h"

namespace FARender
{
	/** @struct DrawSettings ""
	*	@brief Holds a array of objects that use the same PSO, root signature and primitive.
	*/
	struct DrawSettings
	{
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSig;
		D3D_PRIMITIVE_TOPOLOGY prim = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		std::vector<FAShapes::DrawArguments> drawArgs;
	};


	/** @class RenderScene ""
	*	@brief This class is used to render a scene using Direct3D 12 API.
	*/
	class RenderScene
	{
	public:

		RenderScene(unsigned int width, unsigned int height, HWND windowHandle);

		RenderScene(const RenderScene&) = delete;
		RenderScene& operator=(const RenderScene&) = delete;

		/*@brief Returns a constant reference to the device resources object.
		*/
		const DeviceResources& GetDeviceResources() const;

		/*@brief Returns a constant reference to the shader with the specified name.
		* Throws an out_of_range exception if the shader does not exist.
		*/
		const Microsoft::WRL::ComPtr<ID3DBlob>& GetShader(const std::wstring& name) const;

		/*@brief Returns a constant reference to the specified array of input element layout descriptions.
		* Throws an out_of_range exception if the array of input element layout descriptions does not exist.
		*/
		const std::vector<D3D12_INPUT_ELEMENT_DESC>& GetInputElementLayout(const std::wstring& name) const;

		/*@brief Returns a constant reference to the specified rasterization description.
		* Throws an out_of_range exception if the rasterization description does not exist.
		*/
		const D3D12_RASTERIZER_DESC& GetRasterizationState(const std::wstring& name) const;

		/*@brief Returns a constant reference to the PSO in the specified DrawSettings.
		* Throws an out_of_range exception if the DrawSettings does not exist.
		*/
		const Microsoft::WRL::ComPtr<ID3D12PipelineState>& GetPSO(const std::wstring& drawSettingsName) const;

		/*@brief Returns a constant reference to the root signature in the specified DrawSettings structure.
		* Throws an out_of_range exception if the DrawSettings does not exist.
		*/
		const Microsoft::WRL::ComPtr<ID3D12RootSignature>& GetRootSignature(const std::wstring& drawSettingsName) const;

		/*@brief Returns a constant reference to the primitive in the specified DrawSettings structure.
		* Throws an out_of_range exception if the DrawSettings does not exist.
		*/
		const D3D_PRIMITIVE_TOPOLOGY& GetPrimitive(const std::wstring& drawSettingsName) const;

		/*@brief Returns a reference to the specified DrawArguments object in the specified DrawSettings structure.
		* Throws an out_of_range exception if the DrawSettings does not exist or if the index is out of range.
		*/
		FAShapes::DrawArguments& GetDrawArguments(const std::wstring& drawSettingsName, unsigned int index);

		/*@brief Returns a constant reference to the specified DrawArguments object in the specified DrawSettings structure.
		* Throws an out_of_range exception if the DrawSettings does not exist or if the index is out of range.
		*/
		const FAShapes::DrawArguments& GetDrawArguments(const std::wstring& drawSettingsName, unsigned int index) const;

		/*@brief Returns a reference to the this scene's camera;
		*/
		FACamera::Camera& GetCamera();

		/*@brief Returns a constant reference to the this scene's camera;
		*/
		const FACamera::Camera& GetCamera() const;

		/*@brief Returns a reference to the specified Text object.
		* If the Text object does not exist an out_of_range exception is thrown.
		*/
		FARender::Text& GetText(std::wstring textName);

		/*@brief Returns a constant reference to the specified Text object.
		* If the Text object does not exist an out_of_range exception is thrown.
		*/
		const FARender::Text& GetText(std::wstring textName) const;

		/*@brief Loads a shader's bytecode and stores it with the specified name.
		*/
		void LoadShader(const std::wstring& filename, const std::wstring& name);

		/*@brief Removes the specified shader.
		* If the specified shader does not exist an out_of_range exception is thrown.
		*/
		void RemoveShader(const std::wstring& shaderName);

		/*@brief Stores an array of input element descriptions with the specified name.
		*/
		void StoreInputElementDescriptions(const std::wstring& name, const std::vector<D3D12_INPUT_ELEMENT_DESC>& inputElementLayout);

		/*@brief Stores an array of input element descriptions with the specified name.
		*/
		void StoreInputElementDescriptions(const std::wstring& name, const D3D12_INPUT_ELEMENT_DESC* inputElementLayout, 
			UINT numElements);

		/*@brief Removes the specified input element description.
		* If the specified input element description does not exist an out_of_range exception is thrown.
		*/
		void RemoveInputElementDescription(const std::wstring& name);

		/*@brief Creates a rasterization state description and stores it with the specified name.
		*/
		void CreateRasterizationState(D3D12_FILL_MODE fillMode, BOOL enableMultisample, const std::wstring& name);

		/*@brief Removes the specified rasterization state.
		* If the specified rasterization state does not exist an out_of_range exception is thrown.
		*/
		void RemoveRasterizationState(const std::wstring& name);

		/*@brief Creates a PSO and stores it in the specified DrawSettings structure.
		* If the specifed DrawSettings structure, Rasterization State, Vertex Shader, Pixel Shader or Input Layout
		* does not exist an out_of_range exception is thrown.
		*/
		void CreatePSO(const std::wstring& drawSettingsName, const std::wstring& rStateName, 
			const std::wstring& vsName, const std::wstring& psName, const std::wstring& inputLayoutName,
			const D3D12_PRIMITIVE_TOPOLOGY_TYPE& primitiveType, UINT sampleCount);

		/*@brief Creates a root signature and stores it with the specified name.
		* If the specifed DrawSettings structure does not exist an out_of_range exception is thrown.
		*/
		void CreateRootSignature(const std::wstring& drawSettingsName);

		/*@brief Creates a vertex buffer with the specified name and stores all of the added vertices.
		* Also creates a view to the vertex buffer.\n
		* Execute commands and the flush command queue after calling createVertexBuffer() and createIndexBuffer().
		*/
		void CreateVertexBuffer();

		/**@brief Creates an index buffer with the specified name and stores all of the added indices.
		* Also creates a view to the index buffer.\n
		* Execute commands and flush the command queue after calling createVertexBuffer() and createIndexBuffer().
		*/
		void CreateIndexBuffer();

		/**@brief Creates the CBV heap.
		*/
		void CreateCBVHeap(UINT numDescriptors, UINT shaderRegister);

		/**@brief Creates a constant buffer for each frame.
		*/
		void CreateConstantBuffer(UINT numOfBytes);

		/**@brief Creates a constant buffer view for each frame and stores it in the CBV heap.
		*/
		void CreateConstantBufferView(UINT index, UINT numBytes);

		/**@brief Sets the PSO in the specified DrawSettings structure to the specified pso.
		* If the specifed DrawSettings structure does not exist an out_of_range exception is thrown.
		*/
		void SetPSO(const std::wstring& drawSettingsName, const Microsoft::WRL::ComPtr<ID3D12PipelineState>& pso);

		/**@brief Sets the root signature in the specified DrawSettings structure to the specified root signature.
		* If the specifed DrawSettings structure does not exist an out_of_range exception is thrown.
		*/
		void SetRootSignature(const std::wstring& drawSettingsName, const Microsoft::WRL::ComPtr<ID3D12RootSignature>& rootSignature);

		/**@brief Sets the Primitive in the specified DrawSettings structure to the specified primitive.
		* If the specifed DrawSettings structure does not exist an out_of_range exception is thrown.
		*/
		void SetPrimitive(const std::wstring& drawSettingsName, const D3D_PRIMITIVE_TOPOLOGY& primitive);

		/**@brief Adds the specified draw argument structure to the DrawArguments vector of the specified DrawSettings structure.
		* If the specifed DrawSettings structure does not exist an out_of_range exception is thrown.
		*/
		void AddDrawArgument(const std::wstring& drawSettingsName, const FAShapes::DrawArguments& drawArg);

		/**@brief Adds the specified draw arguments to the DrawArguments vector of the specified DrawSettings structure.
		* If the specifed DrawSettings structure does not exist an out_of_range exception is thrown.
		*/
		void AddDrawArgument(const std::wstring& drawSettingsName,
			unsigned int indexCount, unsigned int locationOfFirstIndex, int indexOfFirstVertex, int indexOfConstantData);

		/**@brief Removes the draw argument in the specified DrawSettings structure at the specified index.
		* If the DrawSettings does not exist or if the index is out of bounds an out_of_range exception is thrown.
		*/
		void RemoveDrawArgument(const std::wstring& drawSettingsName, unsigned int index);

		/**@brief Creates a DrawSettings structure with the specified name.
		*/
		void CreateDrawSettings(const std::wstring& drawSettingsName);
		
		/**@brief Removes the specified DrawSettings structure.
		* If the DrawSettings structure does not exist an out_of_range exception is thrown.
		*/
		void RemoveDrawSettings(const std::wstring& drawSettingsName);

		/**@brief Creates a Text object with the specified properties and stores it with the specified name.
		* For text location the first two values in the vector is the top-left location of the rectangle and
		* the last two values are the bottom-right location of the rectangle.
		*/
		void CreateText(const std::wstring& textName, FAMath::Vector4D textLocation, const std::wstring& textString, 
			float textSize, const FAColor::Color textColor);

		/**@brief Removes the specified text object with the specified name.
		* If the Text object does not exist an out_of_range exception is thrown.
		*/
		void RemoveText(const std::wstring& textName);

		/**@brief Adds the specified vertices to the vertex list.
		*/
		void AddVertices(const std::vector<FAShapes::Vertex>& vertices);

		/**@brief Adds the specified vertices to the vertex list.
		*/
		void AddVertices(const FAShapes::Vertex* vertices, unsigned int numVertices);

		/**@brief Adds the specified vertices to the index list.
		*/
		void AddIndices(const std::vector<unsigned int>& indices);

		/**@brief Adds the specified vertices to the index list.
		*/
		void AddIndices(const unsigned int* indices, unsigned int numIndices);

		/**@brief Puts all of the commands needed in the command list before drawing the objects of the scene.
		* Call before calling the first drawObjects function.
		*/
		void BeforeDrawObjects();

		/**@brief Draws all of the objects that use the same PSO, root signature and primitive.
		* Call in between a beforeDrawObjects function and a afterDrawObjects function.\n
		* 
		* Ex.\n
		* beforeDrawObjects()\n
		* drawObjects()\n
		* drawObjects()\n
		* afterDrawObjects()\n
		*
		* Throws an out_of_range exception if the specified DrawSettings structure does not exist.
		*/
		void DrawObjects(const std::wstring& drawSettingsName);

		/**@brief Transitions the render target buffer to the correct state and excutes all the beforeDrawObjects and drawObjects commands.
		* Pass in true if you are going to render text, false otherwise.
		* Call after calling all the drawObjects functions.
		*/
		void AfterDrawObjects(bool renderText);
		
		/**@brief Puts all of the commands needed in the command list before drawing the text of the scene.
		* Call before calling the first drawText function.
		*/
		void BeforeDrawText();

		/**@brief Draws the specified Text object.
		* Call in between a beforeDrawObjects function and a afterDrawObjects function.\n
		*
		* Ex.\n
		* beforeDrawText()\n
		* drawText()\n
		* drawText()\n
		* afterDrawText()\n
		*
		* Throws an out_of_range exception if the specified Text object does not exist.
		*/
		void RenderText(const std::wstring& textName);

		/**@brief Transitions the render target buffer and executes all of the text drawing commands.
		*/
		void AfterDrawText();

		/**@brief Presents and signals (puts a fence command in the command queue).
		* Call after drawing all your objects and text.
		*/
		void AfterDraw();

		/**@brief Executes the commands to fill the vertex and index buffer with data and flushes the queue.
		*/
		void ExecuteAndFlush();

		/**@brief Moves to next frame and waits for the GPU to finish executing the next frame's commands.
		*/
		void NextFrame();

		/**@brief Resizes the DeviceResources resources when the window gets resized.
		*/
		void Resize(unsigned int width, unsigned int height, HWND windowHandle);

		/**@brief Copies the specified data into the constant buffer.
		*/
		void CopyData(UINT index, UINT byteSize, const void* data, UINT64 numOfBytes);

	private:

		//The device resources object that all RenderScene objects share.
		DeviceResources& mDeviceResources;

		//Stores all of the shaders and input element descriptions for this scene.
		std::unordered_map<std::wstring, Microsoft::WRL::ComPtr<ID3DBlob>> mShaders;
		std::unordered_map < std::wstring, std::vector<D3D12_INPUT_ELEMENT_DESC>> mInputElementDescriptions;

		//Stores all of the rasterization states.
		std::unordered_map <std::wstring, D3D12_RASTERIZER_DESC> mRasterizationStates;

		//Stores all of the possible draw settings that the scene uses.
		std::unordered_map <std::wstring, DrawSettings> mSceneObjects;

		//Each scene gets a CBV heap.
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mCBVHeap;
		D3D12_DESCRIPTOR_RANGE mCBVHeapDescription{};
		D3D12_ROOT_PARAMETER mCBVHeapRootParameter;

		//Stores all of the constant buffers this scene uses. We can't update a constant buffer until the GPU
		//is done executing all the commands that reference it, so each frame needs its own constant buffer.
		ConstantBuffer mConstantBuffer[DeviceResources::NUM_OF_FRAMES];

		//The vertices and indicies for the scene.
		std::vector<FAShapes::Vertex> mVertexList;
		std::vector<unsigned int> mIndexList;

		//The vertex and index buffer for the scene.
		VertexBuffer mVertexBuffer;
		IndexBuffer mIndexBuffer;

		//All of the text that is rendered with the scene.
		std::unordered_map <std::wstring, Text> mTexts;

		//The camera for the scene.
		FACamera::Camera mCamera;
	};
}