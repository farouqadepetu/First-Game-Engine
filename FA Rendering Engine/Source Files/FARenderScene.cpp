#include "FARenderScene.h"
#include "FADirectXException.h"
#include <fstream>
#include <d3dx12.h>

namespace FARender
{
	//-----------------------------------------------------------------------------------------------------------------------
	//RENDER SCENE FUNCITON DEFINTIONS

	RenderScene::RenderScene(unsigned int width, unsigned int height, HWND windowHandle, bool isMSAAEnabled, bool isTextEnabled) :
		mDeviceResources{ DeviceResources::GetInstance(width, height, windowHandle, isMSAAEnabled, isTextEnabled) }

	{}

	void RenderScene::LoadShader(unsigned int shaderKey, const std::wstring& filename)
	{
		//opens the shader file.
		std::ifstream fin(filename, std::ios::binary);

		//put the read pointer at the end of the file
		fin.seekg(0, std::ios_base::end);

		//retrieves the position of the read pointer, which gives us the size of the file
		std::ifstream::pos_type size = (int)fin.tellg();

		//put the read pointer at the beginning of the file
		fin.seekg(0, std::ios_base::beg);

		//create a buffer
		ThrowIfFailed(D3DCreateBlob(size, &mShaders[shaderKey]));

		//read the bytes and store the bytes in the buffer.
		fin.read((char*)mShaders[shaderKey]->GetBufferPointer(), size);
	}

	void RenderScene::CompileShader(unsigned int shaderKey,
		const std::wstring& filename, const std::string& entryPointName, const std::string& target)
	{
		unsigned int compileFlags{ 0 };
#if defined(_DEBUG)
		compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		Microsoft::WRL::ComPtr<ID3DBlob> errors{ nullptr };
		
		//Compile shader and store the bytecode.
		HRESULT compileErrorCode{ D3DCompileFromFile(filename.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, 
			entryPointName.c_str(), target.c_str(), compileFlags, 0, &mShaders[shaderKey], &errors) };

		//If there was an error compiling the shader, display debug message in debug window if in debug mode.
		if (errors != nullptr)
			OutputDebugStringA((char*)errors->GetBufferPointer());
		
		ThrowIfFailed(compileErrorCode);
	}

	void RenderScene::RemoveShader(unsigned int shaderKey)
	{
		mShaders.erase(shaderKey);
	}

	void RenderScene::CreateInputElementDescription(unsigned int key, const char* semanticName, unsigned int semanticIndex,
		DXGI_FORMAT format, unsigned int inputSlot, unsigned int byteOffset,
		D3D12_INPUT_CLASSIFICATION inputSlotClass,
		unsigned int instanceStepRate)
	{
		D3D12_INPUT_ELEMENT_DESC inputElementDescription{};
		inputElementDescription.SemanticName = semanticName;
		inputElementDescription.SemanticIndex = semanticIndex;
		inputElementDescription.Format = format;
		inputElementDescription.InputSlot = inputSlot;
		inputElementDescription.AlignedByteOffset = byteOffset;
		inputElementDescription.InputSlotClass = inputSlotClass;
		inputElementDescription.InstanceDataStepRate = instanceStepRate;

		mInputElementDescriptions[key].emplace_back(inputElementDescription);
	}

	void  RenderScene::CreateStaticBuffer(unsigned int bufferType, unsigned int staticBufferKey,
		const void* data, unsigned numBytes, unsigned int stride, DXGI_FORMAT format)
	{
		if (bufferType != 0 && bufferType != 1)
			throw std::runtime_error("The buffer type for creating a static buffer is not 0 or 1");

		if (bufferType == 0) //create a vertex buffer
		{
			mStaticBuffers[staticBufferKey].CreateStaticBuffer(mDeviceResources.GetDevice(), mDeviceResources.GetCommandList(), 
				data, numBytes);

			mStaticBuffers[staticBufferKey].CreateVertexBufferView(numBytes, stride);
		}
		else if (bufferType == 1)//create an index buffer
		{
			mStaticBuffers[staticBufferKey].CreateStaticBuffer(mDeviceResources.GetDevice(), mDeviceResources.GetCommandList(),
				data, numBytes);

			mStaticBuffers[staticBufferKey].CreateIndexBufferView(numBytes, format);
		}
	}

	void RenderScene::CreateDynamicBuffer(unsigned int bufferType, unsigned int dynamicBufferKey,
		unsigned numBytes, unsigned int stride, DXGI_FORMAT format)
	{
		if (bufferType != 0 && bufferType != 1 && bufferType != 2)
			throw std::runtime_error("The buffer type for creating a dynamic buffer is not 0, 1 or 2");

		mDynamicBuffers[dynamicBufferKey].reserve(DeviceResources::NUM_OF_FRAMES);

		//Each frame needs its own dynamic buffer.
		if (bufferType == 0) //create a vertex buffer
		{
			for (unsigned int i = 0; i < DeviceResources::NUM_OF_FRAMES; ++i)
			{
				mDynamicBuffers[dynamicBufferKey].emplace_back();
				mDynamicBuffers[dynamicBufferKey].at(i).CreateDynamicBuffer(mDeviceResources.GetDevice(), numBytes, stride);
				mDynamicBuffers[dynamicBufferKey].at(i).CreateVertexBufferView(numBytes);
			}
		}
		else if (bufferType == 1) //create an index buffer
		{
			for (unsigned int i = 0; i < DeviceResources::NUM_OF_FRAMES; ++i)
			{
				mDynamicBuffers[dynamicBufferKey].emplace_back();
				mDynamicBuffers[dynamicBufferKey].at(i).CreateDynamicBuffer(mDeviceResources.GetDevice(), numBytes, stride);
				mDynamicBuffers[dynamicBufferKey].at(i).CreateIndexBufferView(numBytes);
			}
		}
		else if (bufferType == 2) //create a constant buffer
		{
			for (unsigned int i = 0; i < DeviceResources::NUM_OF_FRAMES; ++i)
			{
				mDynamicBuffers[dynamicBufferKey].emplace_back();
				mDynamicBuffers[dynamicBufferKey].at(i).CreateDynamicBuffer(mDeviceResources.GetDevice(), numBytes, stride);
			}
		}
	}

	void RenderScene::CreateRootParameter(unsigned int rootParameterKey, unsigned int shaderRegister)
	{
		D3D12_ROOT_DESCRIPTOR description{};
		description.ShaderRegister = shaderRegister; //the register where the constant data will be stored in.
		description.RegisterSpace = 0;

		//This root paramter is a root descriptor.
		D3D12_ROOT_PARAMETER rootParameter{};
		rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rootParameter.Descriptor = description;
		rootParameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		mRootParameters[rootParameterKey].emplace_back(rootParameter);

	}

	void RenderScene::CreateRootSignature(unsigned int rootSigKey, unsigned int rootParametersKey)
	{
		//Describe a root signature to store all our root parameters.
		D3D12_ROOT_SIGNATURE_DESC rootSignatureDescription{};
		rootSignatureDescription.NumParameters = mRootParameters.at(rootParametersKey).size(); //number of root paramters
		rootSignatureDescription.pParameters = mRootParameters.at(rootParametersKey).data(); //the array of root parameters
		rootSignatureDescription.NumStaticSamplers = 0;
		rootSignatureDescription.pStaticSamplers = nullptr;
		rootSignatureDescription.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		//serializes the root signature
		Microsoft::WRL::ComPtr<ID3DBlob> seralizedRootSignature;
		Microsoft::WRL::ComPtr<ID3DBlob> seralizedRootSignatureError;
		HRESULT e = D3D12SerializeRootSignature(&rootSignatureDescription, D3D_ROOT_SIGNATURE_VERSION_1,
			seralizedRootSignature.GetAddressOf(), seralizedRootSignatureError.GetAddressOf());

		//error message will be stored in seralizedRootSignatureError if D3D12SerializeRootSignature fails.
		if (seralizedRootSignatureError != nullptr)
		{
			//sends a string to the debugger to be displayed
			::OutputDebugStringA((char*)seralizedRootSignatureError->GetBufferPointer());
		}
		ThrowIfFailed(e);

		ThrowIfFailed(mDeviceResources.GetDevice()->CreateRootSignature(0, seralizedRootSignature->GetBufferPointer(),
			seralizedRootSignature->GetBufferSize(), IID_PPV_ARGS(&mRootSignatures[rootSigKey])));
	}

	void RenderScene::CreatePSO(unsigned int psoKey, D3D12_FILL_MODE fillMode, BOOL enableMultisample,
		unsigned int vsKey, unsigned int psKey, unsigned int inputElementDescriptionsKey, 
		unsigned int rootSigKey,
		const D3D12_PRIMITIVE_TOPOLOGY_TYPE& primitiveType, UINT sampleCount)
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pState{};
		ZeroMemory(&pState, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC)); //fills the block of memory with zeros

		pState.pRootSignature = mRootSignatures.at(rootSigKey).Get();

		pState.VS.pShaderBytecode = mShaders.at(vsKey)->GetBufferPointer();
		pState.VS.BytecodeLength = mShaders.at(vsKey)->GetBufferSize();

		pState.PS.pShaderBytecode = mShaders.at(psKey)->GetBufferPointer();
		pState.PS.BytecodeLength = mShaders.at(psKey)->GetBufferSize();

		pState.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

		pState.SampleMask = UINT_MAX;

		D3D12_RASTERIZER_DESC rDescription{};
		rDescription.FillMode = fillMode;
		rDescription.CullMode = D3D12_CULL_MODE_BACK;
		rDescription.FrontCounterClockwise = FALSE;
		rDescription.DepthBias = 0;
		rDescription.DepthBiasClamp = 0.0f;
		rDescription.SlopeScaledDepthBias = 0.0f;
		rDescription.DepthClipEnable = TRUE;
		rDescription.MultisampleEnable = enableMultisample;
		rDescription.AntialiasedLineEnable = FALSE;
		rDescription.ForcedSampleCount = 0;
		rDescription.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

		pState.RasterizerState = rDescription;

		pState.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

		pState.InputLayout.pInputElementDescs = mInputElementDescriptions[inputElementDescriptionsKey].data();
		pState.InputLayout.NumElements = (UINT)mInputElementDescriptions[inputElementDescriptionsKey].size();

		pState.PrimitiveTopologyType = primitiveType;

		pState.NumRenderTargets = 1;
		pState.RTVFormats[0] = mDeviceResources.GetBackBufferFormat();

		pState.DSVFormat = mDeviceResources.GetDepthStencilFormat();

		pState.SampleDesc.Count = sampleCount;
		pState.SampleDesc.Quality = 0;

		ThrowIfFailed(mDeviceResources.GetDevice()->CreateGraphicsPipelineState(&pState, IID_PPV_ARGS(&mPSOs[psoKey])));
	}

	void RenderScene::BeforeRenderObjects(bool isMSAAEnabled)
	{
		mDeviceResources.Draw(isMSAAEnabled);
	}

	void RenderScene::SetPSOAndRootSignature(unsigned int psoKey, unsigned int rootSigKey)
	{
		mDeviceResources.GetCommandList()->SetPipelineState(mPSOs.at(psoKey).Get());

		mDeviceResources.GetCommandList()->SetGraphicsRootSignature(mRootSignatures.at(rootSigKey).Get());
	}

	void RenderScene::SetStaticBuffer(unsigned int bufferType, unsigned int staticBufferKey)
	{
		if (bufferType != 0 && bufferType != 1)
			throw std::runtime_error("The buffer type for linking a static buffer is not 0 or 1");

		if(bufferType == 0) //link a vertex buffer.
			mDeviceResources.GetCommandList()->IASetVertexBuffers(0, 1, &mStaticBuffers.at(staticBufferKey).GetVertexBufferView());
		else if(bufferType == 1) //link a index buffer.
			mDeviceResources.GetCommandList()->IASetIndexBuffer(&mStaticBuffers.at(staticBufferKey).GetIndexBufferView());
	}

	void RenderScene::SetDynamicBuffer(unsigned int bufferType, unsigned int dynamicBufferKey, unsigned int indexConstantData,
		unsigned int rootParameterIndex)
	{
		if (bufferType != 0 && bufferType != 1 && bufferType != 2)
			throw std::runtime_error("The buffer type for linking a dynamic buffer is not 0, 1 or 2");

		if (bufferType == 0) //link a vertex buffer.
		{
			mDeviceResources.GetCommandList()->IASetVertexBuffers(0, 1, &mStaticBuffers.at(dynamicBufferKey).GetVertexBufferView());
		}
		else if (bufferType == 1) //link a index buffer.
		{
			mDeviceResources.GetCommandList()->IASetIndexBuffer(&mStaticBuffers.at(dynamicBufferKey).GetIndexBufferView());
		}
		else if (bufferType == 2) //link a constant buffer.
		{
			//Get the GPU address of the first byte of the dynamic buffer.
			D3D12_GPU_VIRTUAL_ADDRESS cbAddress{
				mDynamicBuffers.at(dynamicBufferKey)[mDeviceResources.GetCurrentFrame()].GetGPUAddress() };

			//Calculate the offset of where the constant data is in the dynamic buffer.
			UINT64 offset = (UINT64)indexConstantData *
				mDynamicBuffers.at(dynamicBufferKey)[mDeviceResources.GetCurrentFrame()].GetStride();
			cbAddress += offset;

			//Link the constant data to the pipeline.
			mDeviceResources.GetCommandList()->SetGraphicsRootConstantBufferView(rootParameterIndex, cbAddress);
		}

	}

	void RenderScene::RenderObject(unsigned int indexCount, unsigned int locationFirstVertex, int indexOfFirstVertex,
		D3D_PRIMITIVE_TOPOLOGY primitive)
	{
		mDeviceResources.GetCommandList()->IASetPrimitiveTopology(primitive);

		mDeviceResources.GetCommandList()->DrawIndexedInstanced(indexCount, 1,
			locationFirstVertex, indexOfFirstVertex, 0);
	}

	void RenderScene::AfterRenderObjects(bool isMSAAEnabled, bool isTextEnabled)
	{
		mDeviceResources.RTBufferTransition(isMSAAEnabled, isTextEnabled);

		mDeviceResources.Execute();
	}

	void RenderScene::BeforeRenderText()
	{
		mDeviceResources.BeforeTextDraw();
	}

	void RenderScene::RenderText(const FAMath::Vector4D& textLocation, const FAColor::Color& textColor, float textSize,
		const std::wstring& textString)
	{
		D2D_RECT_F tLocation{ textLocation.GetX(), textLocation.GetY(), textLocation.GetZ(), textLocation.GetW() };

		D2D1_COLOR_F tColor{ textColor.GetRed(), textColor.GetGreen(), textColor.GetBlue(), textColor.GetAlpha() };

		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> mDirect2DBrush;
		Microsoft::WRL::ComPtr<IDWriteTextFormat> mDirectWriteFormat;

		ThrowIfFailed(mDeviceResources.GetTextResources().GetDirect2DDeviceContext()->CreateSolidColorBrush(tColor,
			mDirect2DBrush.GetAddressOf()));

		ThrowIfFailed(mDeviceResources.GetTextResources().GetDirectWriteFactory()->CreateTextFormat(
			L"Verdana",
			nullptr,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			textSize,
			L"en-us",
			&mDirectWriteFormat));

		ThrowIfFailed(mDirectWriteFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING));
		ThrowIfFailed(mDirectWriteFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER));

		mDeviceResources.GetTextResources().GetDirect2DDeviceContext()->DrawTextW(textString.c_str(),
			(UINT32)textString.size(), mDirectWriteFormat.Get(),
			&tLocation, mDirect2DBrush.Get());

	}

	void RenderScene::AfterRenderText()
	{
		mDeviceResources.AfterTextDraw();
	}

	void RenderScene::AfterRender()
	{
		mDeviceResources.Present();

		//Update the current fence value
		mDeviceResources.UpdateCurrentFrameFenceValue();

		//Add a fence instruction to the command queue.
		mDeviceResources.Signal();

		mDeviceResources.NextFrame();
		mDeviceResources.WaitForGPU();
	}

	void RenderScene::ExecuteAndFlush()
	{
		mDeviceResources.Execute();
		mDeviceResources.FlushCommandQueue();
	}

	void RenderScene::Resize(unsigned int width, unsigned int height, HWND windowHandle, bool isMSAAEnabled, bool isTextEnabled)
	{
		mDeviceResources.Resize(width, height, windowHandle, isMSAAEnabled, isTextEnabled);
	}

	void RenderScene::CopyDataIntoDynamicBuffer(unsigned int dynamicBufferKey, unsigned int index, const void* data, UINT64 numOfBytes)
	{
		mDynamicBuffers.at(dynamicBufferKey)[mDeviceResources.GetCurrentFrame()].CopyData(index, data, numOfBytes);
	}

	//-----------------------------------------------------------------------------------------------------------------------
}