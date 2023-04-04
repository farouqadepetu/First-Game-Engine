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

		/*@brief Initializes all necessary resources.
		* 
		* Sets the cameras aspect ratio the specified \a width / \a height.
		* 
		* @param[in] width The width of a window.
		* @param[in] height The height of a window.
		* @param[in] windowHandle A handle to a window.
		* @param[in, optional] isMSAAEnabled Pass in true if you want to have MSAA enabled for the initial frame, false otherwise.
		* @param[in, optional] isTextEnabled Pass in true if you want to have text enabled for the initial frame, false otherwise.
		*/
		RenderScene(unsigned int width, unsigned int height, HWND windowHandle, bool isMSAAEnabled = false, bool isTextEnabled = false);

		RenderScene(const RenderScene&) = delete;
		RenderScene& operator=(const RenderScene&) = delete;

		RenderScene(RenderScene&&) = default;

		/**@brief Returns a reference to the this scene's camera;
		*/
		FACamera::Camera& GetCamera();

		/**@brief Returns a constant reference to the this scene's camera;
		*/
		const FACamera::Camera& GetCamera() const;

		/**@brief Returns a reference to the specified Text object.
		* 
		* If the \a textKey does not have a mapped value an out_of_range exception is thrown.
		*/
		FARender::Text& GetText(unsigned int textKey);

		/**@brief Returns a constant reference to the specified Text object.
		* 
		* If the \a textKey does not have a mapped value an out_of_range exception is thrown.
		*/
		const FARender::Text& GetText(unsigned int textKey) const;

		/**@brief Loads a shaders bytecode and maps it to the specified \a shaderKey.
		* 
		* @param[in] shaderKey The key to map the bytecode to.
		* @param[in] filename The name of the .cso file.
		*/
		void LoadShader(unsigned int shaderKey, const std::wstring& filename);

		/**@brief Loads a shader file, compiles it into bytecode and and maps the bytecode to the specified \a shaderKey.
		* 
		* @param[in] shaderKey The key to map the bytecode to.
		* @param[in] filename The name of the .hlsl file.
		* @param[in] entryPointName The name of the entry point in the .hlsl file.
		* @param[in] target The name of the shader target to compile with.
		*/
		void CompileShader(unsigned int shaderKey, const std::wstring& filename, 
			const std::string& entryPointName, const std::string& target);

		/**@brief Removes the shader bytecode mapped to the specified \a shaderKey.
		* 
		* If the \a shaderKey is not mapped to a value, an out_of_range exception is thrown.
		*/
		void RemoveShader(unsigned int shaderKey);

		/**@brief Creates a static buffer and stores the specified \a data in the buffer.
		* The user cannot update/changed the data once it is stored in the buffer.\n
		* Make sure to pass in different keys to store the static buffers with to prevent replacing the static buffer
		* at that key with the newly created static buffer.
		* 
		* @param[in] bufferType The type of buffer. Must be the values 0 or 1. If it isn't one of those values a 
		* runtime_error exception is thrown. If 0 a static vertex buffer is created. If 1 a static index buffer is created.
		* If 1 pass in 0 for the stride.
		* 
		* @param[in] staticBufferKey The key to map the dynamic buffer to.
		* @param[in] numBytes The number of bytes to allocate for the dynamic buffer.
		* @param[in] stride The number of bytes to get from one element to the next element. Used for vertex and constant buffers.
		* @param[in, optional] format The number of bytes to get from one element to the next element. Used for index buffers.
		*/
		void CreateStaticBuffer(unsigned int bufferType, unsigned int staticBufferKey,
			const void* data, unsigned numBytes, unsigned int stride = 0, DXGI_FORMAT format = DXGI_FORMAT_R32_UINT);

		/**@brief Creates a dynamic buffer.
		* The user can update the data on a per-frame basis.\n
		*  Make sure to pass in different keys to store the dynamic buffers with to prevent replacing the dynamic buffer
		*  at that key with the newly created dynamic buffer.
		* 
		* @param[in] bufferType The type of buffer. Must be the values 0, 1, or 2. If it isn't one of those values a 
		* runtime_error exception is thrown. If 0 a dymanic vertex buffer is created. If 1 a dynamic index buffer is created. 
		* If 2 a dynamic constant buffer is created. If 1 pass in 0 for the stride.
		* 
		* @param[in] dynamicBufferKey The key to map the dynamic buffer to.
		* @param[in] numBytes The number of bytes to allocate for the dynamic buffer.
		* @param[in] stride The number of bytes to get from one element to the next element. Used for vertex and constant buffers.
		* @param[in, optional] format The number of bytes to get from one element to the next element. Used for index buffers.
		* 
		*/
		void CreateDynamicBuffer(unsigned int bufferType, unsigned int dynamicBufferKey,
			unsigned numBytes, unsigned int stride = 0, DXGI_FORMAT format = DXGI_FORMAT_R32_UINT);

		/**@brief Creates an input element description and stores in an array mapped to the specified \a key.
		* 
		* @param[in] key The key to a mapped array to store the created input element description.
		* @param[in] semanticName The name of the application variable linked to a shader variable.
		* @param[in] semanticIndex The index to attach to the semanticName.
		* @param[in] format The data type of input element being described.
		* @param[in] inputSlot The input slot the input element will come from.
		* @param[in] byteOffset The offset in bytes to get to the input element being described.
		* @param[in, optional] inputSlotClass The data class for an input slot. Used for instancing.
		* @param[in, optional] instanceStepRate The number of instances to render. Used for instancing.
		*/
		void CreateInputElementDescription(unsigned int key, const char* semanticName, unsigned int semanticIndex,
			DXGI_FORMAT format, unsigned int inputSlot, unsigned int byteOffset,
			D3D12_INPUT_CLASSIFICATION inputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			unsigned int instanceStepRate = 0);

		/**@brief Creates a root parameter and stores it in the array mapped to the specified \a rootParameterKey.
		* 
		* @param[in] rootParameterKey The key to a mappped array to store the created root parameter in.
		* @param[in] shaderRegister The register where constant data will be stored.
		*/
		void CreateRootParameter(unsigned int rootParameterKey, unsigned int shaderRegister);

		/**@brief Creates a root signature and maps it to the specified \a rootSigKey.
		* 
		* If the \a rootParameterKey does not have a mapped value an out_of_range excepetion is thrown.
		* 
		* @param[in] rootSigKey The key to map the created root signature to.
		* @param[in] rootParameterKey The key to a mapped array of root parameters.
		*/
		void CreateRootSignature(unsigned int rootSigKey, unsigned int rootParametersKey);

		/**@brief Creates a PSO and maps it to the specified \a psoKey.
		* 
		* If any of the shader keys or the input element descripton key or the root signature key
		* does not have a mapped value an out_of_range exception is thrown.
		* 
		* @param[in] psoKey The key to map the created PSO to.
		* @param[in] fillMode The fill mode to use when rendering triangles.
		* Use D3D12_FILL_MODE_WIREFRAME for wireframe and D3D12_FILL_MODE_SOLID for solid.
		* 
		* @param[in] enableMultisample Pass in TRUE to use multi-sampling, FALSE otherwise.
		* @param[in] vsKey A key to a mapped vertex shader.
		* @param[in] psKey A key to a mapped pixel shader.
		* @param[in] inputElementDescriptionsKey A key to a mapped array of input element descriptions for the specified
		* vertex and pixel shaders.
		* 
		* @param[in] rootSigKey A key to a mapped root signature.
		* @param[in] primitiveType The type of primitive to connect vertices into.
		* @param[in, optional] sampleCount The number of samples. 
		* If enableMultiSample is TRUE pass in 4. All other values will cause an error.
		* 
		*/
		void CreatePSO(unsigned int psoKey, D3D12_FILL_MODE fillMode, BOOL enableMultisample,
			unsigned int vsKey, unsigned int psKey, unsigned int inputElementDescriptionsKey,
			unsigned int rootSigKey,
			const D3D12_PRIMITIVE_TOPOLOGY_TYPE& primitiveType, UINT sampleCount = 1);

		/**@brief Stores the specified \a DrawArgument structure in an array mapped to the specified \a key.
		*/
		void AddDrawArgument(unsigned int key, const FAShapes::DrawArguments& drawArg);

		/**@brief Creates a DrawArgument structure with the specified values and stores it in an array mapped to the specified \a key.
		* 
		* @param[in] key The key to a mappped array to store the created DrawArguments structure in.
		* @param[in] indexCount The number of indicies used to connect the vertices of an object in an index buffer.
		* @param[in] locationOfFirstIndex The location of the first index of the indices
		* used to connect the vertices of an object in an index buffer.
		* 
		* @param[in] indexOfFirstVertex The index of the first vertex of the vertices of an object in a vertex buffer.
		* @param[in] indexOfConstantData The index of the objects constant data in a constant buffer.
		*/
		void CreateDrawArgument(unsigned int key,
			unsigned int indexCount, unsigned int locationOfFirstIndex, int indexOfFirstVertex, int indexOfConstantData);

		/**@brief Removes the specified DrawArgument structure in the array mapped to the specified key.
		* 
		* If the \a key to the specified array of DrawArgument structure or if 
		* the index to the specifed DrawArguments structure in the array is out of bounds an out_of_range exception is thrown.
		* 
		* @param[in] key The key to a mapped array of where the DrawArguments structure is stored.
		* @param[in] drawArgIndex The index of where the DrawArguments structure is stored.
		*/
		void RemoveDrawArgument(unsigned int key, unsigned int drawArgIndex);

		/**@brief Creates a Text object with the specified properties and maps it to the specified \a textKey.
		* 
		* For text location the first two values in the vector is the top-left location of the rectangle and
		* the last two values are the bottom-right location of the rectangle.
		* 
		* @param[in] textKey The key to a mapped Text object.
		* @param[in] textLocation The location of where to render the text on the window.
		* @param[in] textString The string to render.
		* @param[in] textSize How big the text is.
		* @param[in] textColor The color of the text.
		*/
		void CreateText(unsigned int textKey, FAMath::Vector4D textLocation, const std::wstring& textString,
			float textSize, const FAColor::Color textColor);

		/**@brief Removes the Text object mapped to the specified \a textKey.
		* 
		* If the \a textKey is not mapped to a value, an out_of_range exception is thrown.
		* 
		* @param[in] textKey The key to a mapped Text object.
		*/
		void RemoveText(unsigned int textKey);

		/**@brief Sets the PSO and its associated root signature to indicate what settings you want to use to render objects.
		* An out_of_range exception is thrown if any of the keys don't have a mapped values.
		* 
		* @param[in] psoKey The key to a mapped PSO.
		* @param[in] rootSigKey The key to a mapped root signature.
		*/
		void SetPSOAndRootSignature(unsigned int psoKey, unsigned int rootSigKey);

		/**@brief Links the static buffer mapped to the static buffer key to the pipeline.
		* 
		* An out_of_range exception is thrown if the static buffer key does not have a mapped static buffer.
		* 
		* @param[in] bufferType The type of buffer. Must be the values 0 or 1.\n 
		* If it isn't one of those values a runtime_error exception is thrown.
		* If 0 the mapped static vertex buffer is linked. If 1 the mapped static index buffer is linked.
		* 
		* @param[in] staticBufferKey The key to a mapped static buffer.
		*/
		void SetStaticBuffer(unsigned int bufferType, unsigned int staticBufferKey);

		/**@brief Links the dynamic buffer mapped to the dynamic buffer key to the pipeline.
		* 
		* An out_of_range exception is thrown if the dynamic buffer key does not have a mapped dynamic buffer.
		* 
		* @param[in]bufferType The type of buffer. Must be the values 0, 1 or 2. 
		* If it isn't one of those values a runtime_error exception is thrown.
		* If 0 the mapped dynamic vertex buffer is linked. If 1 the mapped dynamic index buffer is linked.
		* If 2 the mapped dynamic constant buffer is linked.
		* 
		*  @param[in] dynamicBufferKey The key mapped to a dynamic buffer.
		* 
		* @param[in, optional] rootParameterIndex The index of the root parameter in the root signature
		* that has the register the constant data in the dynamic constant buffer will be stored in. 
		* Used if the dynamic buffer is a constant buffer.
		*/
		void SetDynamicBuffer(unsigned int bufferType, unsigned int dynamicBufferKey, unsigned int rootParameterIndex = 0);

		/**@brief Puts all of the commands needed in the command list before drawing the objects of the scene.
		* 
		* Call before calling the first RenderObjects function.
		* 
		* @param[in, optional] isMSAAEnabled Pass in true if MSAA is enabled.
		*/
		void BeforeRenderObjects(bool isMSAAEnabled = false);

		/**@brief Renders the objects in the array mapped to the specified \a drawArgsKey.
		* 
		* Call in between a BeforeRenderObjects function and a AfterRenderObjects function.\n\n
		*
		* Ex.\n
		* BeforeRenderObjects()\n
		* RenderObjects()\n
		* RenderObjects()\n
		* AfterRenderObjects()\n\n
		*
		* Throws an out_of_range exception if any of the keys don't have mapped values.
		* 
		* @param[in] objectConstantBufferKey The key mapped to a constant buffer that has the constant data for the specified objects.
		* 
		* @param[in] rootParameterIndex The index of the root parameter in the root signature
		* that has the register the constant data in the dynamic constant buffer will be stored in. 
		* 
		* @param[in] primitive The primitve used to render the objects.
		*/
		void RenderObjects(unsigned int drawArgsKey, unsigned int objectConstantBufferKey, unsigned int rootParamterIndex,
			D3D_PRIMITIVE_TOPOLOGY primitive);

		/**@brief Transitions the render target buffer to the correct state and executes commands.
		* 
		* @param[in, optional] isMSAAEnabled Pass in true if MSAA is enabled.
		* @param[in, optional] isTextEnabled Pass in true of text is enabled.
		*/
		void AfterRenderObjects(bool isMSAAEnabled = false, bool isTextEnabled = false);
		
		/**@brief Puts all of the commands needed in the command list before drawing the text of the scene.
		* Call before calling the first RenderText function.
		*/
		void BeforeRenderText();

		/**@brief Draws the Text object mapped to the specified \a textKey.
		* Call in between a BeforeRenderText function and a AfterRenderText function.\n
		*
		* Ex.\n
		* BeforeRenderText()\n
		* RenderText()\n
		* RenderText()\n
		* AfterRenderText()\n
		*
		* Throws an out_of_range exception if the textKey is not mapped to a Text object.
		* 
		* @param[in] textKey The key mapped to a Text object.
		*/
		void RenderText(unsigned int textKey);

		/**@brief Transitions the render target buffer and executes all of the text drawing commands.
		* 
		* Call after calling all the RenderText functions.
		*/
		void AfterRenderText();

		/**@brief Presents and signals (puts a fence command in the command queue).
		* 
		* Call after rendering all your objects and text.
		*/
		void AfterRender();

		/**@brief Executes the commands to fill the vertex and index buffer with data and flushes the queue.
		*/
		void ExecuteAndFlush();

		/**@brief Resizes the window-dependent resources when the window gets resized.
		* 
		* @param[in] width The width of a window.
		* @param[in] height The height of a window.
		* @param[in] handle A handle to a window.
		* @param[in, optional] isMSAAEnabled Pass in true if MSAA enabled, false otherwise.
		* @param[in, optional] isTextEnabled Pass in true if text enabled, false otherwise.
		*/
		void Resize(unsigned int width, unsigned int height, HWND windowHandle, bool isMSAAEnabled = false, bool isTextEnabled = false);

		/**@brief Copies the specified data into the dyanmic buffer mapped to the dynamic buffer key.
		* 
		* @param[in] dynamicBufferKey The key mapped to a dynamic buffer.
		* @param[in] index The index of where to copy the data to.
		* @param[in] data The data to copy.
		* @param[in] numOfBytes The number of bytes to copy.
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