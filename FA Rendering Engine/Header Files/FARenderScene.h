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
		D3D_PRIMITIVE_TOPOLOGY prim{ D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST };
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

		/*@brief Returns a reference to the specified DrawArguments structure in the specified DrawSettings structure.
		* Throws an out_of_range exception if the index to DrawSettings structure or index to the DrawArguments structure
		* is out of bounds.
		*/
		FAShapes::DrawArguments& GetDrawArguments(unsigned int drawSettingsIndex, unsigned int drawArgsIndex);

		/*@brief Returns a constant reference to the specified DrawArguments object in the specified DrawSettings structure.
		* Throws an out_of_range exception if the index to DrawSettings structure or index to the DrawArguments structure
		* is out of bounds.
		*/
		const FAShapes::DrawArguments& GetDrawArguments(unsigned int drawSettingsIndex, unsigned int drawArgsIndex) const;

		/*@brief Returns a reference to the this scene's camera;
		*/
		FACamera::Camera& GetCamera();

		/*@brief Returns a constant reference to the this scene's camera;
		*/
		const FACamera::Camera& GetCamera() const;

		/*@brief Returns a reference to the specified Text object.
		* If the index of the Text object is out of bounds an out_of_range exception is thrown.
		*/
		FARender::Text& GetText(unsigned int textIndex);

		/*@brief Returns a constant reference to the specified Text object.
		* If the index of the Text object is out of bounds an out_of_range exception is thrown.
		*/
		const FARender::Text& GetText(unsigned int textIndex) const;

		/*@brief Loads a shader's bytecode and stores it.
		*/
		void LoadShader(const std::wstring& filename);

		/*@brief Loads a shaders file, compiles it into bytecode and stores the bytecode.
		*/
		void LoadShaderAndCompile(const std::wstring& filename, const std::string& entryPointName, const std::string& target);

		/*@brief Removes the Shader structue at the specified index.
		* If the index to the specifed Shader structure is out of bounds, an out_of_range exception is thrown.
		*/
		void RemoveShader(unsigned int index);

		/*@brief Creates an input element description and stores it.
		*/
		void CreateInputElementDescription(const char* semanticName, unsigned int semanticIndex,
			DXGI_FORMAT format, unsigned int inputSlot, unsigned int byteOffset,
			D3D12_INPUT_CLASSIFICATION inputSlotClassifcation = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			unsigned int instanceStepRate = 0);

		/*@brief Removes the specified input element description.
		* If the index to the input element description is out of bounds an out_of_range exceptio is thrown.
		*/
		void RemoveInputElementDescription(unsigned int index);

		/*@brief Creates a PSO and stores it in the specified DrawSettings structure.
		* If the indices to the specified DrawSettings structure, Vertex Shader or Pixel Shader
		* is out of bounds an out_of_range exception is thrown.
		*/
		void CreatePSO(unsigned int drawSettingsIndex, D3D12_FILL_MODE fillMode, BOOL enableMultisample,
			unsigned int vsIndex, unsigned int psIndex,
			const D3D12_PRIMITIVE_TOPOLOGY_TYPE& primitiveType, UINT sampleCount);

		/*@brief Creates a root signature and stores it in the specified DrawSettings structure.
		* If the index to the specified DrawSettings structure is out of bounds an out_of_range exception is thrown.
		*/
		void CreateRootSignature(unsigned int drawSettingsIndex);

		/*@brief Creates a static vertex buffer and fills it with the specified data.
		* Also creates a view to the static vertex buffer.\n
		* Execute commands and the flush command queue after calling createStaticVertexBuffer() and createStaticIndexBuffer().
		*/
		void CreateStaticVertexBuffer(const void* data, unsigned int numBytes, unsigned int stride);

		/**@brief Creates a static index buffer and fills it with the specified data..
		* Also creates a view to the static index buffer.\n
		* Execute commands and flush the command queue after calling createStaticVertexBuffer() and createStaticIndexBuffer().
		*/
		void CreateStaticIndexBuffer(const void* data, unsigned int numBytes, DXGI_FORMAT format);

		/*@brief Creates a dynamic vertex buffer.
		* Also creates a view to the dynamic vertex buffer.\n
		*/
		void CreateDynamicVertexBuffer(unsigned int numBytes, unsigned int stride);

		/**@brief Creates a dynamic index buffer.
		* Also creates a view to the dynamic index buffer.\n
		*/
		void CreateDynamicIndexBuffer(unsigned int numBytes, DXGI_FORMAT format);

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
		* If the index to the specified DrawSettings structure is out of bounds an out_of_range exception is thrown.
		*/
		void SetPSO(unsigned int drawSettingsIndex, const Microsoft::WRL::ComPtr<ID3D12PipelineState>& pso);

		/**@brief Sets the root signature in the specified DrawSettings structure to the specified root signature.
		* If the index to the specified DrawSettings structure is out of bounds an out_of_range exception is thrown.
		*/
		void SetRootSignature(unsigned int drawSettingsIndex,
			const Microsoft::WRL::ComPtr<ID3D12RootSignature>& rootSignature);

		/**@brief Sets the Primitive in the specified DrawSettings structure to the specified primitive.
		* If the index to the specified DrawSettings structure is out of bounds an out_of_range exception is thrown.
		*/
		void SetPrimitive(unsigned int drawSettingsIndex, const D3D_PRIMITIVE_TOPOLOGY& primitive);

		/**@brief Adds the specified draw argument structure to the DrawArguments vector of the specified DrawSettings structure.
		* If the index to the specified DrawSettings structure is out of bounds an out_of_range exception is thrown.
		*/
		void AddDrawArgument(unsigned int drawSettingsIndex, const FAShapes::DrawArguments& drawArg);

		/**@brief Creates a DrawArgument structure with the specified values.
		* The created DrawArgument structure is stored in the DrawArguments vector of the specified DrawSettings structure.
		* If the index to the specified DrawSettings structure is out of bounds an out_of_range exception is thrown.
		*/
		void CreateDrawArgument(unsigned int drawSettingsIndex,
			unsigned int indexCount, unsigned int locationOfFirstIndex, int indexOfFirstVertex, int indexOfConstantData);

		/**@brief Removes the specified DrawArgument structure in the DrawArguments vector of the specified DrawSettings structure.
		* If the index to the specified DrawSettings structure or if the index to the specifed DrawArguments structure
		* is out of bounds an out_of_range exception is thrown.
		*/
		void RemoveDrawArgument(unsigned int drawSettingsIndex, unsigned int drawArgIndex);

		/**@brief Creates a DrawSettings.
		*/
		void CreateDrawSettings();
		
		/**@brief Removes the specified DrawSettings structure.
		* If the index to the specified DrawSettings structure is out of bounds an out_of_range exception is thrown.
		*/
		void RemoveDrawSettings(unsigned int drawSettingsIndex);

		/**@brief Creates a Text object with the specified properties and stores it.
		* For text location the first two values in the vector is the top-left location of the rectangle and
		* the last two values are the bottom-right location of the rectangle.
		*/
		void CreateText(FAMath::Vector4D textLocation, const std::wstring& textString, 
			float textSize, const FAColor::Color textColor);

		/**@brief Removes the specified Text object.
		* If the index to the specified DrawSettings structure is out of bounds an out_of_range exception is thrown.
		*/
		void RemoveText(unsigned int textIndex);

		/**@brief Puts all of the commands needed in the command list before drawing the objects of the scene.
		* Call before calling the first drawObjects function.
		*/
		void BeforeDrawObjects();

		/**@brief Draws objects in the static vertex buffer that use the same PSO, root signature and primitive.
		* Call in between a beforeDrawObjects function and a afterDrawObjects function.\n
		*
		* Ex.\n
		* beforeDrawObjects()\n
		* drawObjects()\n
		* drawObjects()\n
		* afterDrawObjects()\n
		*
		* Throws an out_of_range exception if the index of the specified DrawSettings structure is out of bounds.
		*/
		void DrawStatic(unsigned int drawSettingsIndex);

		/**@brief Draws objects in the dynamic vertex buffer that use the same PSO, root signature and primitive.
		* Call in between a beforeDrawObjects function and a afterDrawObjects function.\n
		*
		* Ex.\n
		* beforeDrawObjects()\n
		* drawObjects()\n
		* drawObjects()\n
		* afterDrawObjects()\n
		*
		* Throws an out_of_range exception if the index of the specified DrawSettings structure is out of bounds.
		*/
		void DrawDynamic(unsigned int drawSettingsIndex);

		/**@brief Transitions the render target buffer to the correct state and excutes commands.
		*/
		void AfterDrawObjects();
		
		/**@brief Puts all of the commands needed in the command list before drawing the text of the scene.
		* Call before calling the first RenderText function.
		*/
		void BeforeDrawText();

		/**@brief Draws the specified Text object.
		* Call in between a BeforeDrawText function and a AfterDrawText function.\n
		*
		* Ex.\n
		* beforeDrawText()\n
		* drawText()\n
		* drawText()\n
		* afterDrawText()\n
		*
		* Throws an out_of_range exception if the specified Text object does not exist.
		*/
		void RenderText(unsigned int textIndex);

		/**@brief Transitions the render target buffer and executes all of the text drawing commands.
		* Call after calling all the RenderText functions.
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

		/**@brief Copies the specified data into the dyanmic vertex buffer.
		*/
		void CopyDataIntoDyanmicVertexBuffer(UINT index, const void* data, UINT64 numOfBytes, UINT stride);

		/**@brief Copies the specified data into the dyanmic index buffer.
		*/
		void CopyDataIntoDyanmicIndexBuffer(UINT index, const void* data, UINT64 numOfBytes, UINT stride);

		/**@brief Copies the specified data into the constant buffer.
		*/
		void CopyDataIntoConstantBuffer(UINT index,  const void* data, UINT64 numOfBytes, UINT stride);

		/**@brief Returns true if MSAA is enabled, false otherwise.
		*/
		bool IsMSAAEnabled() const;

		/**@brief Disables MSAA.
		*/
		void DisableMSAA(unsigned int width, unsigned int height, HWND windowHandle);

		/**@brief Enables MSAA.
		*/
		void EnableMSAA(unsigned int width, unsigned int height, HWND windowHandle);

		/**@brief Returns true if text is enabled, false otherwise.
		*/
		bool IsTextEnabled() const;

		/**@brief Disables text.
		*/
		void DisableText(unsigned int width, unsigned int height, HWND windowHandle);

		/**@brief Enables text.
		*/
		void EnableText(unsigned int width, unsigned int height, HWND windowHandle);

	private:
		bool mIsMSAAEnabled;
		bool mIsTextEnabled;

		//The device resources object that all RenderScene objects share.
		DeviceResources& mDeviceResources;

		//Stores all of the shaders for this scene.
		std::vector<Microsoft::WRL::ComPtr<ID3DBlob>> mShaders;

		//Stores input element descriptions for the shaders.
		std::vector<D3D12_INPUT_ELEMENT_DESC> mInputElementDescriptions;

		//Stores draw settings that the scene uses.
		std::vector<DrawSettings> mSceneObjects;

		//The static vertex and index buffer for the scene.
		StaticBuffer mStaticVertexBuffer;
		StaticBuffer mStaticIndexBuffer;

		//The dynamic vertex and index buffer for the scene.
		//Stores all of the vertices and indices this scene uses. We can't update a dynamic buffer until the GPU
		//is done executing all the commands that reference it, so each frame needs its own constant buffer.
		DynamicBuffer mDynamicVertexBuffer[DeviceResources::NUM_OF_FRAMES];
		DynamicBuffer mDynamicIndexBuffer[DeviceResources::NUM_OF_FRAMES];

		//Each scene gets a CBV heap.
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mCBVHeap;
		D3D12_DESCRIPTOR_RANGE mCBVHeapDescription{};
		D3D12_ROOT_PARAMETER mCBVHeapRootParameter;

		//Stores all of the constant buffers this scene uses. We can't update a dynamic buffer until the GPU
		//is done executing all the commands that reference it, so each frame needs its own dynamic buffer.
		DynamicBuffer mConstantBuffer[DeviceResources::NUM_OF_FRAMES];

		//All of the text that is rendered with the scene.
		std::vector<Text> mTexts;

		//The camera for the scene.
		FACamera::Camera mCamera;
	};
}