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
	*/
	class DeviceResources
	{
	public:

		/**@brief Call to make an object of DeviceResources.
		* This only allows one instance to exist.
		*/
		static DeviceResources& GetInstance(unsigned int width, unsigned int height, HWND windowHandle, unsigned int numFrames,
			bool isMSAAEnabled, bool isTextEnabled);

		DeviceResources(const DeviceResources&) = delete;
		DeviceResources& operator=(const DeviceResources&) = delete;

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

		/**@brief The size of a constant buffer view.
		*/
		unsigned int GetCBVSize() const;

		/**@brief Returns the number of frames.
		*/
		unsigned int GetNumFrames() const;

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
		*	Use this function to make sure all of the commands in command list are executed by the GPU
		*	before the CPU writes in new commands.
		*/
		void FlushCommandQueue();

		/**@brief Waits for the GPU to execute all of the commands of the current frame.
		* Signal should have been called before this function is called.
		*/
		void WaitForGPU() const;

		/**@brief Adds an instruction to the GPU to set the fence value to the current fence value.
		*/
		void Signal();

		/**@brief Call when the window gets resized.
		*	Call when you initialize your program.
		*/
		void Resize(int width, int height, const HWND& handle, bool isMSAAEnabled, bool isTextEnabled);

		/**@brief Transistions the render target buffer.
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
		*/
		void Draw(bool isMSAAEnabled);

		/**@brief Updates the current frame value to go to the next frame.
		*/
		void NextFrame();

	private:

		/**@brief Initializes Direct3D.
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
		DeviceResources(unsigned int width, unsigned int height, HWND windowHandle, unsigned int numFrames,
			bool isMSAAEnabled, bool isTextEnabled);

		unsigned int mNumFrames;
		unsigned int mCurrentFrameIndex;

		Microsoft::WRL::ComPtr<ID3D12Device> mDirect3DDevice;

		Microsoft::WRL::ComPtr<IDXGIFactory4> mDXGIFactory;

		Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
		UINT64 mFenceValue;
		std::vector<UINT64> mCurrentFrameFenceValue;

		Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;
		std::vector<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>> mCommandAllocators;
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