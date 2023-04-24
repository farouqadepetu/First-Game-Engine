#pragma once

/** @file FARenderScene.h
*	@brief File has class RenderScene under namespace FARender.
*/

#include <d3dcompiler.h>
#include <unordered_map>
#include "FADeviceResources.h"
#include "FABuffer.h"
#include "FAColor.h"

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
		* @param[in] width The width of a window.
		* @param[in] height The height of a window.
		* @param[in] windowHandle A handle to a window.
		* @param[in, optional] isMSAAEnabled Pass in true if you want to have MSAA enabled for the initial frame, false otherwise.
		* @param[in, optional] isTextEnabled Pass in true if you want to have text enabled for the initial frame, false otherwise.
		*/
		RenderScene(unsigned int width, unsigned int height, HWND windowHandle, bool isMSAAEnabled = false, bool isTextEnabled = false);

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

		/**@brief Creates a static vertex buffer and stores the specified \a data in the buffer.
		* 
		* The user cannot update/change the data once it is stored in the buffer.\n
		* If the specified key is already mapped to a static buffer, this function does nothing.
		* 
		* @param[in] staticBufferKey The key to map the static buffer to.
		* 
		* @param[in] numBytes The number of bytes to allocate for the static buffer.
		* 
		* @param[in] stride The number of bytes to get from one element to the next element.
		*/
		void CreateStaticBuffer(unsigned int staticBufferKey, const void* data, unsigned numBytes, unsigned int stride);

		/**@brief Creates a static index buffer and stores the specified \a data in the buffer.
		*
		* The user cannot update/change the data once it is stored in the buffer.\n
		* If the specified key is already mapped to a static buffer, this function does nothing.
		*
		* @param[in] staticBufferKey The key to map the static buffer to.
		*
		* @param[in] numBytes The number of bytes to allocate for the static buffer.
		*
		* @param[in] format The number of bytes to get from one element to the next element.
		*/
		void CreateStaticBuffer(unsigned int staticBufferKey, const void* data, unsigned numBytes, DXGI_FORMAT format);

		/**@brief Creates a static texture buffer, stores the data from the file into the buffer and creates a view of the texture.
		*
		* The user cannot update/change the data once it is stored in the buffer.\n
		* If the specified key is already mapped to a static buffer, this function does nothing.
		*
		* @param[in] staticBufferKey The key to map the static buffer to.
		*
		* @param[in] numBytes The number of bytes to allocate for the static buffer.
		*
		*  @param[in] filename The filename of the texture.
		* 
		* @param[in] texType The type of texture. 
		* Pass in FARender::Tex2D for a 2D texture or FARender::Tex2D_MS for a multi-sampled 2D texture.
		* 
		* @param[in] index Where to store the description (view) of the texture in a shader resource view heap.
		*/
		void CreateStaticBuffer(unsigned int staticBufferKey, const wchar_t* filename, unsigned int texType, unsigned int index);

		/**@brief Creates a dynamic vertex buffer or a dynamic constant buffer.
		* 
		* The user can update the data on a per-frame basis.\n
		* If the specified key is already mapped to a dynamic buffer, this function does nothing.
		*
		* @param[in] dynamicBufferKey The key to map the dynamic buffer to.
		* 
		* @param[in] numBytes The number of bytes to allocate for the dynamic buffer.
		* 
		* @param[in] data The data you want to copy into the dynamic buffer. 
		* Pass in nullptr if you don't want to copy data into the buffer on creation.
		* 
		* @param[in] stride The number of bytes to get from one element to the next element.
		*
		*/
		void CreateDynamicBuffer(unsigned int dynamicBufferKey, unsigned numBytes, const void* data, unsigned int stride);

		/**@brief Creates a dynamic index buffer.
		*
		* The user can update the data on a per-frame basis.\n
		* If the specified key is already mapped to a dynamic buffer, this function does nothing.
		*
		* @param[in] dynamicBufferKey The key to map the dynamic buffer to.
		*
		* @param[in] numBytes The number of bytes to allocate for the dynamic buffer.
		*
		* @param[in] data The data you want to copy into the dynamic buffer.
		* Pass in nullptr if you don't want to copy data into the buffer on creation.
		*
		* @param[in] format The number of bytes to get from one element to the next element.
		*
		*/
		void CreateDynamicBuffer(unsigned int dynamicBufferKey, unsigned numBytes, const void* data, DXGI_FORMAT format);

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

		/**@brief Creates a root descriptor and stores it in the array mapped to the specified \a rootParameterKey.
		* 
		* @param[in] rootParameterKey The key to a mappped array to store the created root parameter in.
		* @param[in] shaderRegister The register where constant data will be stored.
		*/
		void CreateRootDescriptor(unsigned int rootParameterKey, unsigned int shaderRegister);

		/**@brief Creates a descriptor range and stores it in the array mapped to the specified \a descriptorRangeKey.
		* 
		* @param[in] descriptorRangeKey The key to an array of descriptor ranges to store the descriptor range in.
		* 
		* @param[in] type The type of descriptor range.
		* 
		* @param[in] numDescriptors The number of descriptors in the range.
		* 
		* @param[in] shaderRegister The shader register the views are mapped to.
		* 
		* @param[in] registerSpace The space of the shader register.
		*/
		void CreateDescriptorRange(unsigned int descriptorRangeKey,
			D3D12_DESCRIPTOR_RANGE_TYPE type, unsigned int numDescriptors, unsigned int shaderRegister, unsigned int registerSpace);

		/**@brief Creates a root descriptor table and stores it in the array mapped to the specified \a rootParameterKey.
		*
		* @param[in] rootParameterKey The key to a mappped array to store the created root parameter in.
		* @param[in] descriptorRangeKey The key to an array of descriptor ranges.
		*/
		void CreateDescriptorTable(unsigned int rootParameterKey, unsigned int descriptorRangeKey);

		/**@brief Creates a root constant and stores it in the array mapped to the specified \a rootParameterKey.
		*
		* @param[in] rootParameterKey The key to a mappped array to store the created root parameter in.
		* @param[in] shaderRegister The register where constant data will be stored.
		* @param[in] numValues The number of 32-bit values.
		*/
		void CreateRootConstants(unsigned int rootParameterKey, unsigned int shaderRegister, unsigned int numValues);

		/**@brief Creates a root signature and maps it to the specified \a rootSigKey.
		*
		* If the \a rootParameterKey does not have a mapped value an out_of_range excepetion is thrown.
		*
		* @param[in] rootSigKey The key to map the created root signature to.
		* @param[in] rootParameterKey The key to a mapped array of root parameters.
		*
		*/
		void CreateRootSignature(unsigned int rootSigKey, unsigned int rootParametersKey);

		/**@brief Creates a root signature and maps it to the specified \a rootSigKey.
		* 
		* If the \a rootParameterKey or staticsSamplerKey does not have a mapped value an out_of_range excepetion is thrown.
		* 
		* @param[in] rootSigKey The key to map the created root signature to.
		* @param[in] rootParameterKey The key to a mapped array of root parameters.
		* @param[in] numStaticSamplers The number of static samplers.
		* @param[in] staticsSamplerKey The key to an array of static samplers.
		* 
		*/
		void CreateRootSignature(unsigned int rootSigKey, unsigned int rootParametersKey, unsigned int numStaticSamplers,
			unsigned int staticsSamplerKey);

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
		* @param[in] dynamicBufferKey The key mapped to a dynamic buffer.
		* 
		* @param[in, optional] indexConstantData The index of where the constant data is in the dynamic buffer.
		* 
		* @param[in, optional] rootParameterIndex The index of the root parameter in the root signature
		* that has the register the constant data in the dynamic constant buffer will be stored in. 
		* 
		* The parameters indexConstantData rootParameterIndex are used if the dynamic buffer is a constant buffer.
		*/
		void SetDynamicBuffer(unsigned int bufferType, unsigned int dynamicBufferKey, unsigned int indexConstantData = 0,
			unsigned int rootParameterIndex = 0);

		/**@brief Links a texture to the pipeline.
		*
		* @param[in] rootParameterIndex The index of the root parameter in the root signature
		* that has the register the texture will be stored in.
		*
		*  @param[in] textureViewIndex The index of the view to the texture in a shader resource view heap.
		*/
		void SetTexture(unsigned int rootParameterIndex, unsigned int textureViewIndex);

		/**@brief Links an array of 32-bit values to the pipeline.
		*
		* @param[in] rootParameterIndex The index of the root parameter in the root signature
		* that has the register the texture will be stored in.
		*
		*  @param[in] numValues The number of 32-bit values.
		* 
		* @param[in] data Pointer to an array of 32-bit values.
		* 
		* @param[in] index The index of the the first 32-bit value in the hlsl constant buffer.
		*/
		void SetConstants(unsigned int rootParameterIndex, unsigned int numValues, void* data, unsigned int index);

		/**@brief Puts all of the commands needed in the command list before drawing the objects of the scene.
		* 
		* Call before calling the first RenderObjects function.
		* 
		* @param[in, optional] isMSAAEnabled Pass in true if MSAA is enabled.
		*/
		void BeforeRenderObjects(bool isMSAAEnabled = false);

		/**@brief Renders an object with the specified draw arguments.
		*
		* Call in between a BeforeRenderObjects function and a AfterRenderObjects function.\n\n
		*
		* Ex.\n
		* BeforeRenderObjects()\n
		* RenderObject()\n
		* RenderObject()\n
		* AfterRenderObjects()\n\n
		*
		* @param[in] indexCount The number of indices used to connect the vertices of the objects.
		*
		* @param[in] locationFirstIndex The location of the first index of the object in an index buffer.
		*
		* @param[in] indexOfFirstVertex The index of the first vertex of the object in a vertex buffer.
		* 
		* @param[in] primitive The primitve used to render the object.
		*/
		void RenderObject(unsigned int indexCount, unsigned int locationFirstIndex, int indexOfFirstVertex,
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
		* @param[in] textLocation The location of the text. 
		* The first 2 values are the top left corner and last two values are bottom right corner.
		* 
		* @param[in] textColor The color of the text.
		* 
		*  @param[in] textSize The size of the size.
		* 
		* @param[in] textString The text to render.
		* 
		* @param[in] alignment Where you want the text to start at in the rectangle.
		*/
		void RenderText(const FAMath::Vector4D& textLocation, const FAColor::Color& textColor, float textSize,
			const std::wstring& textString, DWRITE_PARAGRAPH_ALIGNMENT alignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

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

		/**@brief Frees the memory of the static buffers upload buffers.
		*/
		void ReleaseUploaders();

		/**@brief Creates a descriptor heap to store views of textures.
		* @param[in] numDescriptors The number of views to be stored in the heap.
		*/
		void CreateTextureViewHeap(unsigned int numDescriptors);

		/**@brief Creates a static sampler and stores in an an array mapped to the specified key.
		* 
		* @param[in] staticSamplerKey The key to an array of static samplers.
		* @param[in] filter The filtering method to use when sampling a texture.
		* @param[in] u The address mode for the u texture coordinate.
		* @param[in] v The address mode for the v texture coordinate.
		* @param[in] w The address mode for the w texture coordinate.
		* @param[in] shaderRegister The register the sampler is linked to.
		*/
		void CreateStaticSampler(unsigned int staticSamplerKey, D3D12_FILTER filter,
			D3D12_TEXTURE_ADDRESS_MODE u, D3D12_TEXTURE_ADDRESS_MODE v, D3D12_TEXTURE_ADDRESS_MODE w, unsigned int shaderRegister);

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

		//Stores data that will not be updated on a per-frame basis.
		std::unordered_map<unsigned int, StaticBuffer> mStaticBuffers;

		//Stores data that will be updated on a per-frame basis.
		//We can't update a dynamic buffer until the GPU
		//is done executing all the commands that reference it, so each frame needs its own dynamic buffer.
		std::unordered_map<unsigned int, std::vector<DynamicBuffer>> mDynamicBuffers;

		//Used to store descriptors of textures.
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mTextureViewHeap;

		//Stores descriptor ranges for descriptor tables.
		std::unordered_map<unsigned int, std::vector<D3D12_DESCRIPTOR_RANGE >> mDescriptorRanges;

		//Stores static samplers.
		std::unordered_map<unsigned int, std::vector<D3D12_STATIC_SAMPLER_DESC>> mStaticSamplers;
	};
}