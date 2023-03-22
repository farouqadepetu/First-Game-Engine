#pragma once

/** @file FADeviceResources.h
*	@brief File has class DeviceResources under namespace FARender.
*/

#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3d11.h>
#include <d3d11on12.h>
#include <d2d1_3.h>
#include <dwrite.h>
#include <vector>
#include "FARenderingUtility.h"

namespace FARender
{
	/** @class DeviceResources ""
	*	@brief A wrapper for a Direct3D 12 device, swapchain, depth buffer, MSAA buffers and command objects.
	*/
	class DeviceResources
	{
	public:
		DeviceResources() = default;

		DeviceResources(unsigned int width, unsigned int height, HWND windowHandle);

		DeviceResources(const DeviceResources&) = delete;
		DeviceResources& operator=(const DeviceResources&) = delete;

		/**@brief Flushes the command queue.
		*/
		~DeviceResources();

		/**@brief Returns a constant reference to the ID3D12Device objcet.
		*/
		const Microsoft::WRL::ComPtr<ID3D12Device>& GetDevice() const;

		/**@brief Returns a constant reference to the ID3D12CommandQueue objcet.
		*/
		const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& GetCommandQueue() const;

		/**@brief Returns a constant reference to the current ID3D12CommandAllocator objcet.
		*/
		const Microsoft::WRL::ComPtr<ID3D12CommandAllocator>& GetCommandAllocator() const;

		/**@brief Returns a constant reference to the ID3D12GraphicsCommandList objcet.
		*/
		const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& GetCommandList() const;

		/**@brief Returns a constant reference to the back buffer format.
		*/
		const DXGI_FORMAT& GetBackBufferFormat() const;

		/**@brief Returns a constant reference to the number of swap chains.
		*/
		const UINT GetNumOfSwapChainBuffers() const;

		/**@brief Returns a constant reference to the IDXGISwapChain1 object.
		*/
		const Microsoft::WRL::ComPtr<IDXGISwapChain1>& GetSwapChain() const;

		/**@brief Returns a constant reference to the render target view descriptor size.
		*/
		const UINT& GetRTVDescriptorSize() const;

		/**@brief Returns a constant reference to the depth/stencil view descriptor size.
		*/
		const UINT& GetDSVDescriptorSize() const;

		/**@brief Returns a constant reference to the render target descriptor heap.
		*/
		const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& GetRTVDescriptorHeap() const;

		/**@brief Returns a constant reference to the depth/stencil descriptor heap.
		*/
		const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& GetDSVDescriptorHeap() const;

		/**@brief Returns a constant reference to the current back buffer.
		*/
		const UINT& GetCurrentBackBuffer() const;

		/**@brief Returns a pointer to the swap chain buffers.\n
		* There are two swap chain buffers.\n
		* To access each buffer do swapChainBuffers()[i], where i is the index of the buffer you want to access.
		*/
		const Microsoft::WRL::ComPtr<ID3D12Resource>* GetSwapChainBuffers() const;

		/**@brief Returns a constant reference to the depth stencil buffer.
		*/
		const Microsoft::WRL::ComPtr<ID3D12Resource>& GetDepthStencilBuffer() const;

		/**@brief Returns a constant reference to the depth stencil format.
		*/
		const DXGI_FORMAT& GetDepthStencilFormat() const;

		/**@brief Returns a constant reference to the D3D12_VIEWPORT objcet.
		*/
		const D3D12_VIEWPORT& GetViewport() const;

		/**@brief Returns a constant reference to the D3D12_RECT scissor objcet.
		*/
		const D3D12_RECT& GetScissor() const;

		/**@brief Returns true if MSAA is enabled, false otherwise.
		*/
		bool IsMSAAEnabled();

		/**@brief Disables MSAA.
		*/
		void DisableMSAA();

		/**@brief Enables MSAA.
		*/
		void EnableMSAA();

		/**@brief Returns a reference to the sample count.
		*/
		UINT& GetSampleCount();

		/**@brief Returns a constant reference to the sample count.
		*/
		const UINT& GetSampleCount() const;

		/**@brief Returns a constant reference to the current fence value.
		*/
		const UINT64& GetCurrentFenceValue() const;

		/**@brief Returns a constant reference to the direct 2D device context.
		*/
		const Microsoft::WRL::ComPtr<ID2D1DeviceContext>& GetDevice2DContext() const;

		/**@brief Returns a constant reference to the direct direct write factory.
		*/
		const Microsoft::WRL::ComPtr<IDWriteFactory>& GetDirectWriteFactory() const;

		/**@brief Updates the current frames fence value.
		*/
		void UpdateCurrentFrameFenceValue();

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
		void InitializeDirect3D(unsigned int width, unsigned int height, HWND handle);

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

		/**@brief Resets the command list to open it with a current frame command allocator.
		*/
		void ResetCommandList();

		/*@brief Resets the command list to open it with the direct command allocator.
		*/
		void ResetDirectCommandList();

		/**@brief Resets command allocator to allow reuse of the memory.
		*/
		void ResetCommandAllocator();

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
		DXGI_FORMAT mDepthStencilFormat{ DXGI_FORMAT_D24_UNORM_S8_UINT };

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
		void EnableDebugLayer();
		void CreateDirect3DDevice();
		void CreateDXGIFactory();
		void CreateFence();
		void QueryDescriptorSizes();
		void CreateCommandObjects();
		void CreateSwapChain(HWND handle);
		void CreateRTVHeap();
		void CreateDSVHeap();

		//if MSAA is supported, creates a MSAA RTV and DSV heap.
		void CheckMSAASupport();
		void CreateMSAARTVHeap();
		void CreateMSAADSVHeap();

		//Creates and initializes everything needed to render text.
		void InitializeText();

		//These functions are for creating swap chain buffers, depth/stencil buffer, render target views and depth/stencil view.
		//They are called in the resize function.
		void CreateRenderTargetBufferAndView();
		void CreateDepthStencilBufferAndView(int width, int height);

		//These functions are for creating a MSAA render target buffer, MSAA depth/stencil buffer, 
		//MSAA render target view, and a MSAA depth/stencil view.
		//They are called in the resize function.
		void CreateMSAARenderTargetBufferAndView(int width, int height);
		void CreateMSAADepthStencilBufferAndView(int width, int height);

		/* Resets the text buffers.
		* Gets called in the resize function.
		*/
		void ResetTextBuffers();

		/*Resizes the necessary text buffers.
		* Gets called in the resize function.
		*/
		void TextResize(const HWND& handle);
	};
}