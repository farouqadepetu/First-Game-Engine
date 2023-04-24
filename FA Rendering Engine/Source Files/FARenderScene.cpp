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

	void RenderScene::CreateStaticBuffer(unsigned int staticBufferKey, const void* data, unsigned numBytes, unsigned int stride)
	{
		mStaticBuffers.insert({ staticBufferKey, StaticBuffer(mDeviceResources.GetDevice(), mDeviceResources.GetCommandList(),
				data, numBytes, stride) });
	}

	void RenderScene::CreateStaticBuffer(unsigned int staticBufferKey, const void* data, unsigned numBytes, DXGI_FORMAT format)
	{
		mStaticBuffers.insert({ staticBufferKey, StaticBuffer(mDeviceResources.GetDevice(), mDeviceResources.GetCommandList(),
				data, numBytes, format) });
	}

	void  RenderScene::CreateStaticBuffer(unsigned int staticBufferKey, 
		const wchar_t* filename, unsigned int texType, unsigned int index)
	{
		mStaticBuffers.insert({ staticBufferKey, StaticBuffer(mDeviceResources.GetDevice(), mDeviceResources.GetCommandList(),
			filename) });

		if (texType == TEX2D)
		{
			mStaticBuffers.at(staticBufferKey).CreateTexture2DView(mDeviceResources.GetDevice(), mTextureViewHeap,
				mDeviceResources.GetCBVSRVUAVSize(), index);
		}
		else if (texType == TEX2D_MS)
		{
			mStaticBuffers.at(staticBufferKey).CreateTexture2DMSView(mDeviceResources.GetDevice(), mTextureViewHeap,
				mDeviceResources.GetCBVSRVUAVSize(), index);
		}
	}

	void RenderScene::CreateDynamicBuffer(unsigned int dynamicBufferKey, unsigned numBytes, const void* data, unsigned int stride)
	{
		for (unsigned int i = 0; i < DeviceResources::NUM_OF_FRAMES; ++i)
		{
			mDynamicBuffers[dynamicBufferKey].emplace_back(mDeviceResources.GetDevice(), numBytes, stride);

			if (data != nullptr)
			{
				mDynamicBuffers[dynamicBufferKey].at(i).CopyData(0, data, numBytes);
			}
		}
	}

	void RenderScene::CreateDynamicBuffer(unsigned int dynamicBufferKey, unsigned numBytes, const void* data, DXGI_FORMAT format)
	{
		for (unsigned int i = 0; i < DeviceResources::NUM_OF_FRAMES; ++i)
		{
			mDynamicBuffers[dynamicBufferKey].emplace_back(mDeviceResources.GetDevice(), numBytes, format);

			if (data != nullptr)
			{
				mDynamicBuffers[dynamicBufferKey].at(i).CopyData(0, data, numBytes);
			}
		}
	}

	void RenderScene::CreateRootDescriptor(unsigned int rootParameterKey, unsigned int shaderRegister)
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

	void RenderScene::CreateDescriptorRange(unsigned int descriptorRangeKey,
		D3D12_DESCRIPTOR_RANGE_TYPE type, unsigned int numDescriptors, unsigned int shaderRegister, unsigned int registerSpace)
	{
		D3D12_DESCRIPTOR_RANGE dRange{};
		dRange.RangeType = type;
		dRange.NumDescriptors = numDescriptors;
		dRange.BaseShaderRegister = shaderRegister;
		dRange.RegisterSpace = registerSpace;
		dRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		mDescriptorRanges[descriptorRangeKey].emplace_back(dRange);
	}

	void RenderScene::CreateDescriptorTable(unsigned int rootParameterKey, unsigned int descriptorRangeKey)
	{
		D3D12_ROOT_DESCRIPTOR_TABLE dTable{};
		dTable.NumDescriptorRanges = mDescriptorRanges.at(descriptorRangeKey).size();
		dTable.pDescriptorRanges = mDescriptorRanges.at(descriptorRangeKey).data();

		//This root paramter is descriptor table
		D3D12_ROOT_PARAMETER rootParameter{};
		rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameter.DescriptorTable = dTable;
		rootParameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		mRootParameters[rootParameterKey].emplace_back(rootParameter);
	}

	void RenderScene::CreateRootConstants(unsigned int rootParameterKey, unsigned int shaderRegister, unsigned int numValues)
	{
		D3D12_ROOT_CONSTANTS rDesc{};
		rDesc.ShaderRegister = shaderRegister;
		rDesc.RegisterSpace = 0;
		rDesc.Num32BitValues = numValues;

		//This root paramter is root constant
		D3D12_ROOT_PARAMETER rootParameter{};
		rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		rootParameter.Constants = rDesc;
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

	void RenderScene::CreateRootSignature(unsigned int rootSigKey, unsigned int rootParametersKey, unsigned int numStaticSamplers,
		unsigned int staticsSamplerKey)
	{
		//Describe a root signature to store all our root parameters.
		D3D12_ROOT_SIGNATURE_DESC rootSignatureDescription{};
		rootSignatureDescription.NumParameters = mRootParameters.at(rootParametersKey).size(); //number of root paramters
		rootSignatureDescription.pParameters = mRootParameters.at(rootParametersKey).data(); //the array of root parameters
		rootSignatureDescription.NumStaticSamplers = numStaticSamplers;
		rootSignatureDescription.pStaticSamplers = mStaticSamplers.at(staticsSamplerKey).data();
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
		if (bufferType != VERTEX_BUFFER && bufferType != INDEX_BUFFER)
			throw std::runtime_error("The buffer type for linking a static buffer to the pipeline is not "
				" FARender::VERTEX_BUFFER or FARender::INDEX_BUFFER");

		if (bufferType == VERTEX_BUFFER) //link a vertex buffer.
		{
			D3D12_VERTEX_BUFFER_VIEW vbView{ mStaticBuffers.at(staticBufferKey).GetVertexBufferView() };
			mDeviceResources.GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
		}
		else if (bufferType == INDEX_BUFFER) //link a index buffer.
		{
			D3D12_INDEX_BUFFER_VIEW ibView{ mStaticBuffers.at(staticBufferKey).GetIndexBufferView() };
			mDeviceResources.GetCommandList()->IASetIndexBuffer(&ibView);
		}
	}

	void RenderScene::SetDynamicBuffer(unsigned int bufferType, unsigned int dynamicBufferKey, unsigned int indexConstantData,
		unsigned int rootParameterIndex)
	{
		if (bufferType != VERTEX_BUFFER && bufferType != INDEX_BUFFER && bufferType != CONSTANT_BUFFER)
			throw std::runtime_error("The buffer type for linking a static buffer to the pipeline is not "
				" FARender::VERTEX_BUFFER, FARender::INDEX_BUFFER or FARender::CONSTANT_BUFFER");

		if (bufferType == VERTEX_BUFFER) //link a vertex buffer.
		{
			D3D12_VERTEX_BUFFER_VIEW vbView
			{ mDynamicBuffers.at(dynamicBufferKey).at(mDeviceResources.GetCurrentFrame()).GetVertexBufferView() };

			mDeviceResources.GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
		}
		else if (bufferType == INDEX_BUFFER) //link a index buffer.
		{
			D3D12_INDEX_BUFFER_VIEW ibView
			{ mDynamicBuffers.at(dynamicBufferKey).at(mDeviceResources.GetCurrentFrame()).GetIndexBufferView() };

			mDeviceResources.GetCommandList()->IASetIndexBuffer(&ibView);
		}
		else if (bufferType == CONSTANT_BUFFER) //link a constant buffer.
		{
			//Link the constant data to the pipeline.
			mDeviceResources.GetCommandList()->SetGraphicsRootConstantBufferView(rootParameterIndex, 
				mDynamicBuffers.at(dynamicBufferKey).at(mDeviceResources.GetCurrentFrame()).GetGPUAddress(indexConstantData));
		}
	}

	void RenderScene::SetTexture(unsigned int rootParameterIndex, unsigned int textureViewIndex)
	{
		//offset to the texture view at the specified index in the shader resource view heap.
		CD3DX12_GPU_DESCRIPTOR_HANDLE texHandle(mTextureViewHeap->GetGPUDescriptorHandleForHeapStart());
		texHandle.Offset(textureViewIndex, mDeviceResources.GetCBVSRVUAVSize());

		mDeviceResources.GetCommandList()->SetGraphicsRootDescriptorTable(rootParameterIndex, texHandle);
	}

	void RenderScene::SetConstants(unsigned int rootParameterIndex, unsigned int numValues, void* data, unsigned int index)
	{
		mDeviceResources.GetCommandList()->SetGraphicsRoot32BitConstants(rootParameterIndex, numValues, data, index);
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
		const std::wstring& textString, DWRITE_PARAGRAPH_ALIGNMENT alignment)
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
		ThrowIfFailed(mDirectWriteFormat->SetParagraphAlignment(alignment));

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

	void RenderScene::ReleaseUploaders()
	{
		if (!mStaticBuffers.empty())
		{
			for (auto& i : mStaticBuffers)
			{
				i.second.ReleaseUploader();
			}
		}
	}

	void RenderScene::CreateTextureViewHeap(unsigned int numDescriptors)
	{
		D3D12_DESCRIPTOR_HEAP_DESC textureViewDescription{};
		textureViewDescription.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		textureViewDescription.NumDescriptors = numDescriptors;
		textureViewDescription.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		textureViewDescription.NodeMask = 0;

		ThrowIfFailed(mDeviceResources.GetDevice()->CreateDescriptorHeap(&textureViewDescription, IID_PPV_ARGS(&mTextureViewHeap)));
	}

	void RenderScene::CreateStaticSampler(unsigned int staticSamplerKey, D3D12_FILTER filter, 
		D3D12_TEXTURE_ADDRESS_MODE u, D3D12_TEXTURE_ADDRESS_MODE v, D3D12_TEXTURE_ADDRESS_MODE w, unsigned int shaderRegister)
	{
		D3D12_STATIC_SAMPLER_DESC sSampler{};
		sSampler.Filter = filter;
		sSampler.AddressU = u;
		sSampler.AddressV = v;
		sSampler.AddressW = w;
		sSampler.MipLODBias = 0;
		sSampler.MaxAnisotropy = 16;
		sSampler.ComparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		sSampler.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
		sSampler.MinLOD = 0.0f;
		sSampler.MaxLOD = D3D12_FLOAT32_MAX;
		sSampler.ShaderRegister = shaderRegister;
		sSampler.RegisterSpace = 0;
		sSampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		mStaticSamplers[staticSamplerKey].emplace_back(sSampler);
	}
	//-----------------------------------------------------------------------------------------------------------------------
}