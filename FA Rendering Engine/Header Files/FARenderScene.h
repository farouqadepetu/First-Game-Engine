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
		RenderScene(unsigned int width, unsigned int height, HWND windowHandle);

		RenderScene(const RenderScene&) = delete;
		RenderScene& operator=(const RenderScene&) = delete;

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

		/*@brief Loads a shaders bytecode and stores it.
		*/
		void LoadShader(const std::wstring& filename);

		/*@brief Loads a shader file, compiles it into bytecode and stores the bytecodes.
		*/
		void CompileShader(const std::wstring& filename, const std::string& entryPointName, const std::string& target);

		/*@brief Removes the Shader structue at the specified index.
		* If the index to the specifed shader is out of bounds, an out_of_range exception is thrown.
		*/
		void RemoveShader(unsigned int index);

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

		/**@brief Creates a constant buffer to store constant data that is related to objects.
		* E.g. Local to world transformation matrices.
		*/
		void CreateObjectConstantBuffer(UINT numOfBytes, UINT stride);

		/**@brief Creates a constant buffer to store constant data that is not related to objects.
		* E.g. view matrix.
		*/
		void CreatePassConstantBuffer(UINT numOfBytes, UINT stride);

		/*@brief Creates an input element description and stores it in an array at the specified index.
		*/
		void CreateInputElementDescription(unsigned int index, const char* semanticName, unsigned int semanticIndex,
			DXGI_FORMAT format, unsigned int inputSlot, unsigned int byteOffset,
			D3D12_INPUT_CLASSIFICATION inputSlotClassifcation,
			unsigned int instanceStepRate);

		/*@brief Creates the scenes root signature.
		*/
		void CreateRootSignature();

		/*@brief Creates a PSO and stores it in an array.
		*/
		void CreatePSO(D3D12_FILL_MODE fillMode, BOOL enableMultisample,
			unsigned int vsIndex, unsigned int psIndex, unsigned int inputElementDescriptionsIndex,
			const D3D12_PRIMITIVE_TOPOLOGY_TYPE& primitiveType, UINT sampleCount = 1);

		/**@brief Stores the specified draw argument structure in an array at the specified index.
		*/
		void AddDrawArgument(unsigned int index, const FAShapes::DrawArguments& drawArg);

		/**@brief Creates a DrawArgument structure with the specified values and stores it in an array at the specified index.
		*/
		void CreateDrawArgument(unsigned int index,
			unsigned int indexCount, unsigned int locationOfFirstIndex, int indexOfFirstVertex, int indexOfConstantData);

		/**@brief Removes the specified DrawArgument structure in the array at the specified index.
		* If the index to the specified array of DrawArguments or if the index to the specifed DrawArguments structure
		* is out of bounds an out_of_range exception is thrown.
		*/
		void RemoveDrawArgument(unsigned int index, unsigned int drawArgIndex);

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
		* Call in between a BeforeDrawObjects function and a AfterDrawObjects function.\n
		*
		* Ex.\n
		* BeforeDrawObjects()\n
		* DrawStatic()\n
		* DrawStatic()\n
		* AfterDrawObjects()\n
		*
		* Throws an out_of_range exception if the index of the specified DrawSettings structure is out of bounds.
		*/
		void DrawStatic(unsigned int psoIndex, unsigned int drawArgsIndex, D3D_PRIMITIVE_TOPOLOGY primitive);

		/**@brief Draws objects in the dynamic vertex buffer that use the same PSO, root signature and primitive.
		* Call in between a BeforeDrawObjects function and a AfterDrawObjects function.\n
		*
		* Ex.\n
		* BeforeDrawObjects()\n
		* DrawDynamic()\n
		* DrawDynamic()\n
		* AfterDrawObjects()\n
		*
		* Throws an out_of_range exception if the index of the specified DrawSettings structure is out of bounds.
		*/
		void DrawDynamic(unsigned int psoIndex, unsigned int drawArgsIndex, D3D_PRIMITIVE_TOPOLOGY primitive);

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
		* BeforeDrawText()\n
		* RenderText()\n
		* RenderText()\n
		* AfterDrawText()\n
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

		/**@brief Resizes the DeviceResources resources when the window gets resized.
		*/
		void Resize(unsigned int width, unsigned int height, HWND windowHandle);

		/**@brief Copies the specified data into the dyanmic vertex buffer.
		*/
		void CopyDataIntoDyanmicVertexBuffer(UINT index, const void* data, UINT64 numOfBytes);

		/**@brief Copies the specified data into the dyanmic index buffer.
		*/
		void CopyDataIntoDyanmicIndexBuffer(UINT index, const void* data, UINT64 numOfBytes);

		/**@brief Copies the specified data into the object constant buffer.
		*/
		void CopyDataIntoObjectConstantBuffer(UINT index,  const void* data, UINT64 numOfBytes);

		/**@brief Copies the specified data into the pass constant buffer.
		*/
		void CopyDataIntoPassConstantBuffer(UINT index, const void* data, UINT64 numOfBytes);

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
		std::unordered_map<unsigned int, std::vector<D3D12_INPUT_ELEMENT_DESC>> mInputElementDescriptions;

		//The root signature for the scene.
		//Describes all of the constant data that is expected in a set of shaders.
		Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature;

		D3D12_ROOT_PARAMETER mRootParameters[2];

		//Stores pipeline state objects.
		std::vector<Microsoft::WRL::ComPtr<ID3D12PipelineState>> mPSOs;

		//Stores the objects the scene is going to render.
		std::unordered_map<unsigned int, std::vector<FAShapes::DrawArguments>> mObjects;

		//The static vertex and index buffer for the scene.
		//Stores vertices and indices that don't need to be updated after they are stored in the buffers.
		StaticBuffer mStaticVertexBuffer;
		StaticBuffer mStaticIndexBuffer;

		//The dynamic vertex and index buffer for the scene.
		//Stores vertices and indices that are going to be updated on a per-frame basis.
		// We can't update a dynamic buffer until the GPU
		//is done executing all the commands that reference it, so each frame needs its own constant buffer.
		DynamicBuffer mDynamicVertexBuffer[DeviceResources::NUM_OF_FRAMES];
		DynamicBuffer mDynamicIndexBuffer[DeviceResources::NUM_OF_FRAMES];

		//Stores constant data that is related to objects.
		//e.g. model matrices.
		//We can't update a dynamic buffer until the GPU
		//is done executing all the commands that reference it, so each frame needs its own dynamic buffer.
		DynamicBuffer mObjectConstantBuffer[DeviceResources::NUM_OF_FRAMES];

		//Stores constant data that is not related to objects.
		//e.g. view matrix.
		//We can't update a dynamic buffer until the GPU
		//is done executing all the commands that reference it, so each frame needs its own dynamic buffer.
		DynamicBuffer mPassConstantBuffer[DeviceResources::NUM_OF_FRAMES];

		//All of the text that is rendered with the scene.
		std::vector<Text> mTexts;

		//The camera for the scene.
		FACamera::Camera mCamera;
	};
}