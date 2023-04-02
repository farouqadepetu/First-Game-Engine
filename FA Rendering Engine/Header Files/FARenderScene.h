#pragma once

/** @file FARenderScene.h
*	@brief File has class RenderScene under namespace FARender.
*/

#include <d3dcompiler.h>
#include <unordered_map>
#include "FADeviceResources.h"
#include "FABuffer.h"
#include "FACamera.h"
#include "FAText.h"
#include "FAShapesUtility.h"

namespace FARender
{
	/** @class RenderScene ""
	*	@brief This class is used to render a scene using Direct3D 12 API.
	*/
	class RenderScene
	{
	public:

		/*@brief Constructor.
		* *Initializes all necessary resources.
		* Sets the cameras aspect ratio the given width / height.
		*/
		RenderScene(unsigned int width, unsigned int height, HWND windowHandle, bool isMSAAEnabled = false, bool isTextEnabled = false);

		RenderScene(const RenderScene&) = delete;
		RenderScene& operator=(const RenderScene&) = delete;

		RenderScene(RenderScene&&) = default;

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

		/*@brief Loads a shaders bytecode and maps it to the specified shaderKey.
		*/
		void LoadShader(unsigned int shaderKey, const std::wstring& filename);

		/*@brief Loads a shader file, compiles it into bytecode and and maps the bytecode to the specified shaderKey.
		*/
		void CompileShader(unsigned int shaderKey, const std::wstring& filename, 
			const std::string& entryPointName, const std::string& target);

		/*@brief Removes the shader bytecode mapped to the specified shaderKey.
		* If the shaderKey is not mapped to a value, an out_of_range exception is thrown.
		*/
		void RemoveShader(unsigned int shaderKey);

		/**@brief Creates a static buffer and stores the specified data in the buffer.
		* The user cannot update/changed the data once it is stored in the buffer.\n
		* bufferType must be the values 0 or 1. If it isn't one of those values a runtime_error exception is thrown.
		* If 0 a static vertex buffer is created. If 1 a static index buffer is created.\n
		* If 1 pass in 0 for the stride.\n
		* Make sure to pass in different keys to store the static buffers with to prevent replacing the static buffer
		* at that key with the newly created static buffer.
		*/
		void CreateStaticBuffer(unsigned int bufferType, unsigned int staticBufferKey,
			const void* data, unsigned numBytes, unsigned int stride = 0, DXGI_FORMAT format = DXGI_FORMAT_R32_UINT);

		/**@brief Creates a dynamic buffer.
		* The user can update the data on a per-frame basis.
		* bufferType must be the values 0, 1, or 2. If it isn't one of those values a runtime_error exception is thrown.
		* If 0 a dymanic vertex buffer is created. If 1 a dynamic index buffer is created. If 2 a dynamic constant buffer is created.\n
		* If 1 pass in 0 for the stride.\n
		* Make sure to pass in different keys to store the dynamic buffers with to prevent replacing the dynamic buffer
		* at that key with the newly created dynamic buffer.
		*/
		void CreateDynamicBuffer(unsigned int bufferType, unsigned int dynamicBufferKey,
			unsigned numBytes, unsigned int stride = 0, DXGI_FORMAT format = DXGI_FORMAT_R32_UINT);

		/**@brief Creates a constant buffer to store constant data that is related to objects.
		* E.g. Local to world transformation matrices.
		*/
		//void CreateObjectConstantBuffer(UINT numOfBytes, UINT stride);

		/**@brief Creates a constant buffer to store constant data that is not related to objects.
		* E.g. view matrix.
		*/
		//void CreatePassConstantBuffer(UINT numOfBytes, UINT stride);

		/*@brief Creates an input element description and stores in an array mapped to the specified key.
		*/
		void CreateInputElementDescription(unsigned int key, const char* semanticName, unsigned int semanticIndex,
			DXGI_FORMAT format, unsigned int inputSlot, unsigned int byteOffset,
			D3D12_INPUT_CLASSIFICATION inputSlotClassifcation,
			unsigned int instanceStepRate);

		/*@brief Creates a root parameter and stores it in the array mapped to the specified rootParameterKey.
		*/
		void CreateRootParameter(unsigned int rootParameterKey, unsigned int shaderRegister);

		/*@brief Creates a root signature and maps it to the specified rootSigKey.
		* If the root parameter key does not have a mapped value an out_of_range excepetion is thrown.
		*/
		void CreateRootSignature(unsigned int rootSigKey, unsigned int rootParametersKey);

		/*@brief Creates a PSO and maps it to the specified psoKey.
		* If the shader keys or the input element descripton key or the root signature key
		* does not have a mapped value an out_of_range exception is thrown.
		*/
		void CreatePSO(unsigned int psoKey, D3D12_FILL_MODE fillMode, BOOL enableMultisample,
			unsigned int vsKey, unsigned int psKey, unsigned int inputElementDescriptionsKey,
			unsigned int rootSigKey,
			const D3D12_PRIMITIVE_TOPOLOGY_TYPE& primitiveType, UINT sampleCount = 1);

		/**@brief Stores the specified draw argument structure in an array mapped to the specified key.
		*/
		void AddDrawArgument(unsigned int key, const FAShapes::DrawArguments& drawArg);

		/**@brief Creates a DrawArgument structure with the specified values and stores it in an array mapped to the specified key.
		*/
		void CreateDrawArgument(unsigned int key,
			unsigned int indexCount, unsigned int locationOfFirstIndex, int indexOfFirstVertex, int indexOfConstantData);

		/**@brief Removes the specified DrawArgument structure in the array mapped to the specified key.
		* If the key to the specified array of DrawArguments or if the index to the specifed DrawArguments structure in the array
		* is out of bounds an out_of_range exception is thrown.
		*/
		void RemoveDrawArgument(unsigned int key, unsigned int drawArgIndex);

		/**@brief Creates a Text object with the specified properties and maps it to the specified textKey.
		* For text location the first two values in the vector is the top-left location of the rectangle and
		* the last two values are the bottom-right location of the rectangle.
		*/
		void CreateText(unsigned int textKey, FAMath::Vector4D textLocation, const std::wstring& textString,
			float textSize, const FAColor::Color textColor);

		/**@brief Removes the Text object mapped to the specified textKey.
		* If the textKey is not mapped to a value, an out_of_range exception is thrown.
		*/
		void RemoveText(unsigned int textKey);

		/**@brief Puts all of the commands needed in the command list before drawing the objects of the scene.
		* Call before calling the first drawObjects function.
		*/
		void BeforeRenderObjects(bool isMSAAEnabled = false);

		/**@brief Sets the PSO and its associated root signature to indicate what settings you want to use to render objects.
		* An out_of_range exception is thrown if any of the keys don't have a mapped values.
		*/
		void SetPSOAndRootSignature(unsigned int psoKey, unsigned int rootSigKey);

		/**@brief Links the static buffer mapped to the static buffer key to the pipeline.
		* bufferType must be the values 0 or 1. If it isn't one of those values a runtime_error exception is thrown.
		* If 0 the mapped static vertex buffer is linked. If 1 the mapped static index buffer is linked.\n
		* An out_of_range exception is thrown if the static buffer key does not have a mapped static buffer.
		*/
		void SetStaticBuffer(unsigned int bufferType, unsigned int staticBufferKey);

		/**@brief Links the dynamic buffer mapped to the dynamic buffer key to the pipeline.
		* bufferType must be the values 0, 1 or 2. If it isn't one of those values a runtime_error exception is thrown.\n
		* If 0 the mapped dynamic vertex buffer is linked. If 1 the mapped dynamic index buffer is linked.
		* If 2 the mapped dynamic constant buffer is linked.\n
		* The root parameter index is the index of the root parameter in the root signature
		* that has the register the constant data in the dynamic constant buffer will be stored in.
		* An out_of_range exception is thrown if the dynamic buffer key does not have a mapped dynamic buffer.
		*/
		void SetDynamicBuffer(unsigned int bufferType, unsigned int dynamicBufferKey, unsigned int rootParameterIndex);

		/**@brief Renders the objects in the array mapped to the specified draw arguments key.
		* The constant buffer key is the mapped constant buffer that has the constant data for the specified objects.\n
		* The root parameter index is the index of the root parameter in the root signature
		* that has the register the constant data will be stored in.\n
		* The primitive is the primitve you want to use to render your objects.
		* Call in between a BeforeRenderObjects function and a AfterRenderObjects function.\n
		*
		* Ex.\n
		* BeforeRenderObjects()\n
		* RenderObjects()\n
		* RenderObjects()\n
		* AfterRenderObjects()\n
		*
		* Throws an out_of_range exception if any of the keys don't have mapped values.
		*/
		void RenderObjects(unsigned int drawArgsKey, unsigned int objectconstantBufferKey, unsigned int rootParamterIndex,
			D3D_PRIMITIVE_TOPOLOGY primitive);

		/**@brief Transitions the render target buffer to the correct state and excutes commands.
		*/
		void AfterRenderObjects(bool isMSAAEnabled = false, bool isTextEnabled = false);
		
		/**@brief Puts all of the commands needed in the command list before drawing the text of the scene.
		* Call before calling the first RenderText function.
		*/
		void BeforeRenderText();

		/**@brief Draws the specified Text object.
		* Call in between a BeforeRenderText function and a AfterRenderText function.\n
		*
		* Ex.\n
		* BeforeRenderText()\n
		* RenderText()\n
		* RenderText()\n
		* AfterRenderText()\n
		*
		* Throws an out_of_range exception if the textKey is not mapped to a Text object.
		*/
		void RenderText(unsigned int textKey);

		/**@brief Transitions the render target buffer and executes all of the text drawing commands.
		* Call after calling all the RenderText functions.
		*/
		void AfterRenderText();

		/**@brief Presents and signals (puts a fence command in the command queue).
		* Call after rendering all your objects and text.
		*/
		void AfterRender();

		/**@brief Executes the commands to fill the vertex and index buffer with data and flushes the queue.
		*/
		void ExecuteAndFlush();

		/**@brief Resizes the window-dependent resources when the window gets resized.
		*/
		void Resize(unsigned int width, unsigned int height, HWND windowHandle, bool isMSAAEnabled = false, bool isTextEnabled = false);

		/**@brief Copies the specified data into the dyanmic buffer mapped to the dynamic buffer key.
		*/
		void CopyDataIntoDynamicBuffer(unsigned int dynamicBufferKey, unsigned int index, const void* data, UINT64 numOfBytes);

	private:
		//The device resources object that all RenderScene objects share.
		DeviceResources& mDeviceResources;

		//Stores all of the shaders for this scene.
		std::unordered_map<unsigned int, Microsoft::WRL::ComPtr<ID3DBlob>> mShaders;

		//Stores input element descriptions for a set of shaders.
		std::unordered_map<unsigned int, std::vector<D3D12_INPUT_ELEMENT_DESC>> mInputElementDescriptions;

		//Stores root parameters for root signatures.
		std::unordered_map<unsigned int, std::vector<D3D12_ROOT_PARAMETER>> mRootParameters;

		//The root signatures for the scene.
		//Describes all of the constant data that is expected in a set of shaders.
		//Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature;
		std::unordered_map<unsigned int, Microsoft::WRL::ComPtr<ID3D12RootSignature>> mRootSignatures;

		//Stores pipeline state objects.
		std::unordered_map<unsigned int, Microsoft::WRL::ComPtr<ID3D12PipelineState>> mPSOs;

		//Stores the objects the scene is going to render.
		//Group objects together that have their vertices in the same vertex buffer.
		std::unordered_map<unsigned int, std::vector<FAShapes::DrawArguments>> mObjects;

		//Stores data that will not be updated on a per-frame basis.
		std::unordered_map<unsigned int, StaticBuffer> mStaticBuffers;

		//Stores data that will be updated on a per-frame basis.
		//We can't update a dynamic buffer until the GPU
		//is done executing all the commands that reference it, so each frame needs its own dynamic buffer.
		std::unordered_map<unsigned int, std::vector<DynamicBuffer>> mDynamicBuffers;

		//All of the text that is rendered with the scene.
		std::unordered_map<unsigned int, Text> mTexts;

		//The camera for the scene.
		FACamera::Camera mCamera;
	};
}