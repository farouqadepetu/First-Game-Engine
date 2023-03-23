#pragma once

/** @file FADeviceResources.h
*	@brief File has class DeviceResources under namespace FARender.
*/

#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include "FATextResources.h"
#include "FASwapChain.h"

namespace FARender
{
	/** @class DeviceResources ""
	*	@brief A wrapper for resources that are needed to render objects and text using the Direct3D 12 API.
	*/
	class DeviceResources
	{
	public:
		static const unsigned int NUM_OF_FRAMES{ 3 };

		/**@brief Call to make an object of DeviceResources.
		* This only allows one instance to exist.
		*/
		static DeviceResources& GetInstance(unsigned int width, unsigned int height, HWND windowHandle);

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
		const DXGI_FORMAT& GetDepthStencilFormat() const;

		/**@brief The size of a constant buffer view.
		*/
		UINT GetCBVSize() const;

		/**@brief Returns the current frame.
		*/
		unsigned int GetCurrentFrame() const;

		/**@brief Returns a constant reference to the TextResources object.
		*/
		const TextResources& GetTextResources() const;

		/**@brief Returns true if MSAA is enabled, false otherwise.
		*/
		bool IsMSAAEnabled();

		/**@brief Disables MSAA.
		*/
		void DisableMSAA();

		/**@brief Enables MSAA.
		*/
		void EnableMSAA();

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
		void Resize(int width, int height, const HWND& handle);

		/**@brief Transistions the render target buffer.
		*/
		void RTBufferTransition(bool renderText);

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
		void Draw();

		/**@brief Update our current frame value to go to the next frame.
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
		DeviceResources(unsigned int width, unsigned int height, HWND windowHandle);

		unsigned int mCurrentFrameIndex{ 0 };

		Microsoft::WRL::ComPtr<ID3D12Device> mDirect3DDevice;

		Microsoft::WRL::ComPtr<IDXGIFactory4> mDXGIFactory;

		Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
		UINT64 mFenceValue{ 0 };
		UINT64 mCurrentFrameFenceValue[NUM_OF_FRAMES];

		Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mCommandAllocator[NUM_OF_FRAMES];
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mDirectCommandAllocator;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;

		SwapChain mSwapChain;

		Microsoft::WRL::ComPtr<ID3D12Resource> mDepthStencilBuffer;
		DXGI_FORMAT mDepthStencilFormat{ DXGI_FORMAT_D24_UNORM_S8_UINT };

		UINT mRTVSize;
		UINT mDSVSize;
		UINT mCBVSize;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDSVHeap;

		D3D12_VIEWPORT mViewport{};
		D3D12_RECT mScissor{};

		bool mMSAA4xSupported = false;
		bool mIsMSAAEnabled = false;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mMSAARTVDescriptorHeap;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mMSAADSVDescriptorHeap;
		Microsoft::WRL::ComPtr<ID3D12Resource> mMSAARenderTargetBuffer;
		Microsoft::WRL::ComPtr<ID3D12Resource> mMSAADepthStencilBuffer;

		TextResources mTextResources;

		//Call all of these functions to initialize Direct3D
		void mEnableDebugLayer();
		void mCreateDirect3DDevice();
		void mCreateDXGIFactory();
		void mCreateFence();
		void mQueryDescriptorSizes();
		void mCreateCommandObjects();

		void mCreateDSVHeap();

		//if MSAA is supported, creates a MSAA RTV and DSV heap.
		void mCheckMSAASupport();
		void mCreateMSAARTVHeap();
		void mCreateMSAADSVHeap();

		void mCreateDepthStencilBufferAndView(int width, int height);

		//These functions are for creating a MSAA render target buffer, MSAA depth/stencil buffer, 
		//MSAA render target view, and a MSAA depth/stencil view.
		//They are called in the resize function.
		void mCreateMSAARenderTargetBufferAndView(int width, int height);
		void mCreateMSAADepthStencilBufferAndView(int width, int height);
	};
}