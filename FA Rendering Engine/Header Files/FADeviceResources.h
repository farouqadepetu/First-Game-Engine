#pragma once

/** @file FADeviceResources.h
*	@brief File has class DeviceResources under namespace FARender.
*/

#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <vector>
#include "FARenderingUtility.h"
#include "FAText.h"

namespace FARender
{
	/** @class DeviceResources ""
		*	@brief A wrapper for a Direct3D 12 device, swapchain, depth buffer, MSAA buffers and command objects.
		*/
	class DeviceResources
	{
	public:

		DeviceResources(unsigned int width, unsigned int height, HWND windowHandle);

		DeviceResources(const DeviceResources&) = delete;
		DeviceResources& operator=(const DeviceResources&) = delete;

		/**@brief Flushes the command queue.
		*/
		~DeviceResources();

		/**@brief Returns a constant reference to the ID3D12Device objcet.
		*/
		const Microsoft::WRL::ComPtr<ID3D12Device>& device() const;

		/**@brief Returns a constant reference to the ID3D12CommandQueue objcet.
		*/
		const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& commandQueue() const;

		/**@brief Returns a constant reference to the current ID3D12CommandAllocator objcet.
		*/
		const Microsoft::WRL::ComPtr<ID3D12CommandAllocator>& commandAllocator() const;

		/**@brief Returns a constant reference to the ID3D12GraphicsCommandList objcet.
		*/
		const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList() const;

		/**@brief Returns a constant reference to the back buffer format.
		*/
		const DXGI_FORMAT& backBufferFormat() const;

		/**@brief Returns a constant reference to the number of swap chains.
		*/
		const UINT numOfSwapChainBuffers() const;

		/**@brief Returns a constant reference to the IDXGISwapChain1 object.
		*/
		const Microsoft::WRL::ComPtr<IDXGISwapChain1>& swapChain() const;

		/**@brief Returns a constant reference to the render target view descriptor size.
		*/
		const UINT& rtvDescriptorSize() const;

		/**@brief Returns a constant reference to the depth/stencil view descriptor size.
		*/
		const UINT& dsvDescriptorSize() const;

		/**@brief Returns a constant reference to the render target descriptor heap.
		*/
		const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& rtvDescriptorHeap() const;

		/**@brief Returns a constant reference to the depth/stencil descriptor heap.
		*/
		const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& dsvDescriptorHeap() const;

		/**@brief Returns a constant reference to the current back buffer.
		*/
		const UINT& currentBackBuffer() const;

		/**@brief Returns a pointer to the swap chain buffers.\n
		* There are two swap chain buffers.\n
		* To access each buffer do swapChainBuffers()[i], where i is the index of the buffer you want to access.
		*/
		const Microsoft::WRL::ComPtr<ID3D12Resource>* swapChainBuffers() const;

		/**@brief Returns a constant reference to the depth stencil buffer.
		*/
		const Microsoft::WRL::ComPtr<ID3D12Resource>& depthStencilBuffer() const;

		/**@brief Returns a constant reference to the depth stencil format.
		*/
		const DXGI_FORMAT& depthStencilFormat() const;

		/**@brief Returns a constant reference to the D3D12_VIEWPORT objcet.
		*/
		const D3D12_VIEWPORT& viewport() const;

		/**@brief Returns a constant reference to the D3D12_RECT scissor objcet.
		*/
		const D3D12_RECT& scissor() const;

		/**@brief Returns a reference to check if MSAA is enabled or not.
		*/
		bool& isMSAAEnabled();

		/**@brief Returns a constant reference to check if MSAA is enabled or not.
		*/
		const bool& isMSAAEnabled() const;

		/**@brief Returns a reference to the sample count.
		*/
		UINT& sampleCount();

		/**@brief Returns a constant reference to the sample count.
		*/
		const UINT& sampleCount() const;

		/**@brief Returns a reference to the current fence value.
		*/
		UINT64& currentFenceValue();

		/**@brief Returns a constant reference to the current fence value.
		*/
		const UINT64& currentFenceValue() const;

		/**@brief Returns a constant reference to the direct 2D device context.
		*/
		const Microsoft::WRL::ComPtr<ID2D1DeviceContext>& device2DContext();

		/**@brief Returns a constant reference to the direct direct write factory.
		*/
		const Microsoft::WRL::ComPtr<IDWriteFactory>& directWriteFactory();

		/**@brief Updates the current frames fence value.
		*/
		void updateCurrentFrameFenceValue();

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
		void initializeDirect3D(unsigned int width, unsigned int height, HWND handle);

		/**@brief Synchronizes the CPU and GPU.
		*	Use this function to make sure all of the commands in command list are executed by the GPU
		*	before the CPU writes in new commands.
		*/
		void flushCommandQueue();

		/**@brief Waits for the GPU to execute all of the commands of the current frame.
		* Signal should have been called before this function is called.
		*/
		void waitForGPU();

		/**@brief Adds an instruction to the GPU to set the fence value to the current fence value.
		*/
		void signal();

		/**@brief Call when the window gets resized.
		*	Call when you initialize your program.
		*/
		void resize(int width, int height, const HWND& handle);

		/**@brief Resets the command list to open it with a current frame command allocator.
		*/
		void resetCommandList();

		/*@brief Resets the command list to open it with the direct command allocator.
		*/
		void resetDirectCommandList();

		/**@brief Resets command allocator to allow reuse of the memory.
		*/
		void resetCommandAllocator();

		/**@briefTransistions the render target buffer.
		*/
		void rtBufferTransition(Text* text);

		/*@brief Renders the text.
		*/
		void textDraw(Text* textToRender = nullptr, UINT numText = 0);

		/**@brief Executes the command list.
		*/
		void execute();

		/**@brief Swaps the front and back buffers.
		*/
		void present();

		/*@brief Calls the necessary functions to let the user draw their objects.
		*/
		void draw();

	private:
		Microsoft::WRL::ComPtr<ID3D12Device> mDirect3DDevice;

		Microsoft::WRL::ComPtr<IDXGIFactory4> mDXGIFactory;

		Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
		UINT64 mFenceValue{ 0 };
		UINT64 mCurrentFrameFenceValue[numFrames];

		Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mCommandAllocator[numFrames];
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mDirectCommandAllocator;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;

		DXGI_FORMAT mBackBufferFormat{ DXGI_FORMAT_R8G8B8A8_UNORM };
		static const UINT mNumOfSwapChainBuffers{ 2 };
		UINT mCurrentBackBuffer{ 0 };
		Microsoft::WRL::ComPtr<IDXGISwapChain1> mSwapChain;
		Microsoft::WRL::ComPtr<ID3D12Resource> mSwapChainBuffers[mNumOfSwapChainBuffers];

		Microsoft::WRL::ComPtr<ID3D12Resource> mDepthStencilBuffer;
		DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

		UINT mRTVSize;
		UINT mDSVSize;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mRTVHeap;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDSVHeap;

		D3D12_VIEWPORT mViewport;
		D3D12_RECT mScissor;

		bool mMSAA4xSupported = false;
		bool mIsMSAAEnabled = false;
		UINT mSampleCount{ 4 };
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mMSAARTVDescriptorHeap;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mMSAADSVDescriptorHeap;
		Microsoft::WRL::ComPtr<ID3D12Resource> mMSAARenderTargetBuffer;
		Microsoft::WRL::ComPtr<ID3D12Resource> mMSAADepthStencilBuffer;

		Microsoft::WRL::ComPtr<ID3D11Device> mDevice11;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDevice11Context;
		Microsoft::WRL::ComPtr<ID3D11On12Device> mDevice11on12;

		Microsoft::WRL::ComPtr<ID2D1Device2> mDirect2DDevice;
		Microsoft::WRL::ComPtr<ID2D1Factory3> mDirect2DFactory;
		Microsoft::WRL::ComPtr<ID2D1DeviceContext> mDirect2DDeviceContext;

		Microsoft::WRL::ComPtr<IDWriteFactory> mDirectWriteFactory;

		std::vector<Microsoft::WRL::ComPtr<ID3D11Resource>> mWrappedBuffers;
		std::vector<Microsoft::WRL::ComPtr<ID2D1Bitmap1>> mDirect2DBuffers;
		std::vector<Microsoft::WRL::ComPtr<IDXGISurface>> mSurfaces;

		//Call all of these functions to initialize Direct3D
		void enableDebugLayer();
		void createDirect3DDevice();
		void createDXGIFactory();
		void createFence();
		void queryDescriptorSizes();
		void createCommandObjects();
		void createSwapChain(HWND handle);
		void createRTVHeap();
		void createDSVHeap();

		//if MSAA is supported, creates a MSAA RTV and DSV heap.
		void checkMSAASupport();
		void createMSAARTVHeap();
		void createMSAADSVHeap();

		//Creates and initializes everything needed to render text.
		void initializeText();

		//These functions are for creating swap chain buffers, depth/stencil buffer, render target views and depth/stencil view.
		//They are called in the resize function.
		void createRenderTargetBufferAndView();
		void createDepthStencilBufferAndView(int width, int height);

		//These functions are for creating a MSAA render target buffer, MSAA depth/stencil buffer, 
		//MSAA render target view, and a MSAA depth/stencil view.
		//They are called in the resize function.
		void createMSAARenderTargetBufferAndView(int width, int height);
		void createMSAADepthStencilBufferAndView(int width, int height);

		/* Resets the text buffers.
		* Gets called in the resize function.
		*/
		void resetTextBuffers();

		/*Resizes the necessary text buffers.
		* Gets called in the resize function.
		*/
		void textResize(const HWND& handle);
	};
}