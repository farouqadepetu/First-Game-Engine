#pragma once

/** @file FADeviceResources.h
*	@brief File has class DeviceResources under namespace FARender.
*/

#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include "FASwapChain.h"
#include "FAMultiSampling.h"
#include "FATextResources.h"

namespace FARender
{
	/** @class DeviceResources ""
	*	@brief A wrapper for resources that are needed to render objects and text using the Direct3D 12 API.
	*	The copy constructor and assignment operators are explicitly deleted. This makes this class non-copyable.
	*/
	class DeviceResources
	{
	public:

		//No copying
		DeviceResources(const DeviceResources&) = delete;
		DeviceResources& operator=(const DeviceResources&) = delete;

		/**@brief The number of frames in the ciruclar array.
		* 
		* Allows the CPU to produce the commands for future frames as the GPU is executing the commands for the current frame.
		*/
		static const unsigned int NUM_OF_FRAMES{ 3 };

		/**@brief Call to make an object of DeviceResources.
		* 
		* Only one instance of DeviceResources can exist in a program.
		* 
		* @param[in] width The width of a window.
		* @param[in] height The height of a window.
		* @param[in] windowHandle A handle to a window.
		* @param[in] isMSAAEnabled Pass in true if you want to have MSAA enabled for the initial frame, false otherwise.
		* @param[in] isTextEnabled Pass in true if you want to have text enabled for the initial frame, false otherwise.
		* 
		*/
		static DeviceResources& GetInstance(unsigned int width, unsigned int height, HWND windowHandle, bool isMSAAEnabled, bool isTextEnabled);

		/**@brief Flushes the command queue.
		*/
		~DeviceResources();

		/**@brief Returns a constant reference to the ID3D12Device objcet.
		*/
		const Microsoft::WRL::ComPtr<ID3D12Device>& GetDevice() const;

		/**@brief Returns a constant reference to the ID3D12GraphicsCommandList objcet.
		*/
		const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& GetCommandList() const;

		/**@brief Returns a constant reference to the back buffer format.
		*/
		DXGI_FORMAT GetBackBufferFormat() const;

		/**@brief Returns a constant reference to the depth stencil format.
		*/
		DXGI_FORMAT GetDepthStencilFormat() const;

		/**@brief The size of a constant buffer/shader resource/unordered access view.
		*/
		unsigned int GetCBVSRVUAVSize() const;

		/**@brief Returns the current frame.
		*/
		unsigned int GetCurrentFrame() const;

		/**@brief Returns a constant reference to the TextResources object.
		*/
		const TextResources& GetTextResources() const;

		/**@brief Updates the current frames fence value.
		*/
		void UpdateCurrentFrameFenceValue();

		/**@brief Synchronizes the CPU and GPU.
		* 
		*	Use this function to make sure all of the commands in command list are executed by the GPU
		*	before the CPU writes in new commands.
		*/
		void FlushCommandQueue();

		/**@brief Waits for the GPU to execute all of the commands of the current frame.
		* 
		* Signal should have been called before this function is called.
		*/
		void WaitForGPU() const;

		/**@brief Adds an instruction to the GPU to set the fence value to the current fence value.
		*/
		void Signal();

		/**@brief Call when the window gets resized.
		* 
		* Call when you initialize your program.
		* 
		* @param[in] width The width of a window.
		* @param[in] height The height of a window.
		* @param[in] handle A handle to a window.
		* @param[in] isMSAAEnabled Pass in true if MSAA enabled, false otherwise.
		* @param[in] isTextEnabled Pass in true if text enabled, false otherwise.
		*/
		void Resize(int width, int height, const HWND& handle, bool isMSAAEnabled, bool isTextEnabled);

		/**@brief Transistions the render target buffer.
		* 
		* @param[in] isMSAAEnabled Pass in true if MSAA enabled, false otherwise.
		* @param[in] isTextEnabled Pass in true if text enabled, false otherwise.
		*/
		void RTBufferTransition(bool isMSAAEnabled, bool isTextEnabled);

		/**@brief Prepares to render text.
		*/
		void BeforeTextDraw();

		/**@brief Executes the text commands.
		*/
		void AfterTextDraw();

		/**@brief Executes the command list.
		*/
		void Execute() const;

		/**@brief Swaps the front and back buffers.
		*/
		void Present();

		/*@brief Calls the necessary functions to let the user draw their objects.
		* 
		*  @param[in] isMSAAEnabled Pass in true if MSAA enabled, false otherwise.
		*/
		void Draw(bool isMSAAEnabled);

		/**@brief Updates the current frame value to go to the next frame.
		*/
		void NextFrame();

	private:

		/**@brief Initializes Direct3D.
		* 
		* Enables the debug layer if in debug mode.\n
		* Creates a Direct3D 12 device.\n
		* Creates a DXGI factory object.\n
		* Creates a fence.\n
		* Queries descriptor sizes.\n
		* Creates command objects.\n
		* Creates a swap chain.\n
		* Creates a render target view and a depth/stencil view heap.
		* Creates the initial render target buffers, depth stencil buffer, MSAA buffers and text buffers.
		*/
		DeviceResources(unsigned int width, unsigned int height, HWND windowHandle,
			bool isMSAAEnabled, bool isTextEnabled);

		unsigned int mCurrentFrameIndex;

		Microsoft::WRL::ComPtr<ID3D12Device> mDirect3DDevice;

		Microsoft::WRL::ComPtr<IDXGIFactory4> mDXGIFactory;

		Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
		UINT64 mFenceValue;
		UINT64 mCurrentFrameFenceValue[NUM_OF_FRAMES];

		Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mCommandAllocators[NUM_OF_FRAMES];
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mDirectCommandAllocator;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;

		UINT mRTVSize;
		UINT mDSVSize;
		UINT mCBVSize;

		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mRTVHeap;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDSVHeap;

		SwapChain mSwapChain;

		MultiSampling mMultiSampling;

		D3D12_VIEWPORT mViewport{};
		D3D12_RECT mScissor{};

		TextResources mTextResources;

		//Call all of these functions to initialize Direct3D
		void mEnableDebugLayer();
		void mCreateDirect3DDevice();
		void mCreateDXGIFactory();
		void mCreateFence();
		void mQueryDescriptorSizes();
		void mCreateRTVHeap();
		void mCreateDSVHeap();
		void mCreateCommandObjects();
	};
}