#include "FARenderScene.h"
#include "FADirectXException.h"
#include <fstream>
#include <d3dx12.h>

namespace FARender
{
	//-----------------------------------------------------------------------------------------------------------------------
	//RENDER SCENE FUNCITON DEFINTIONS

	RenderScene::RenderScene(unsigned int width, unsigned int height, HWND windowHandle) :
		mIsMSAAEnabled{ false }, mIsTextEnabled{ false },
		mDeviceResources{ DeviceResources::GetInstance(width, height, windowHandle, mIsMSAAEnabled, mIsTextEnabled) }

	{
		D3D12_ROOT_PARAMETER root{};
		mRootParameters[0] = root;
		mRootParameters[1] = root;

		mCamera.SetAspectRatio((float)width / height);
	}

	const DeviceResources& RenderScene::GetDeviceResources() const
	{
		return mDeviceResources;
	}

	FAShapes::DrawArguments& RenderScene::GetDrawArguments(unsigned int drawSettingsIndex, unsigned int drawArgsIndex)
	{
		return mObjects.at(drawSettingsIndex).drawArgs.at(drawArgsIndex);
	}

	const FAShapes::DrawArguments& RenderScene::GetDrawArguments(unsigned int drawSettingsIndex, unsigned int drawArgsIndex) const
	{
		return mObjects.at(drawSettingsIndex).drawArgs.at(drawArgsIndex);
	}

	FACamera::Camera& RenderScene::GetCamera()
	{
		return mCamera;
	}

	const FACamera::Camera& RenderScene::GetCamera() const
	{
		return mCamera;
	}

	FARender::Text& RenderScene::GetText(unsigned int textIndex)
	{
		return mTexts.at(textIndex);
	}

	const FARender::Text& RenderScene::GetText(unsigned int textIndex) const
	{
		return mTexts.at(textIndex);
	}

	void RenderScene::LoadShader(const std::wstring& filename)
	{
		//opens the file
		std::ifstream fin(filename, std::ios::binary);

		//put the read pointer at the end of the file
		fin.seekg(0, std::ios_base::end);

		//retrieves the position of the read pointer, which gives us the size of the file
		std::ifstream::pos_type size = (int)fin.tellg();

		//put the read pointer at the beginning of the file
		fin.seekg(0, std::ios_base::beg);

		//create a buffer
		Microsoft::WRL::ComPtr<ID3DBlob> blob;
		ThrowIfFailed(D3DCreateBlob(size, blob.GetAddressOf()));

		//read the bytes and store the bytes in the buffer
		fin.read((char*)blob->GetBufferPointer(), size);
		
		mShaders.push_back(blob);
	}

	void RenderScene::LoadShaderAndCompile(const std::wstring& filename, const std::string& entryPointName, 
		const std::string& target)
	{
		unsigned int compileFlags{ 0 };
#if defined(_DEBUG)
		compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		Microsoft::WRL::ComPtr<ID3DBlob> byteCode{ nullptr };
		Microsoft::WRL::ComPtr<ID3DBlob> errors{ nullptr };
		
		//Compile shader.
		HRESULT compileErrorCode{ D3DCompileFromFile(filename.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, 
			entryPointName.c_str(),target.c_str(), compileFlags, 0, &byteCode, &errors) };

		//If there was an error compiling the shader, display debug message in debug window if in debug mode.
		if (errors != nullptr)
			OutputDebugStringA((char*)errors->GetBufferPointer());
		else
			mShaders.push_back(byteCode);
		
		ThrowIfFailed(compileErrorCode);
	}

	void RenderScene::RemoveShader(unsigned int index)
	{
		if (index >= mShaders.size())
			throw std::out_of_range("Index is out of bounds");

		mShaders.erase(mShaders.begin() + index);
	}

	void RenderScene::CreateInputElementDescription(const char* semanticName, unsigned int semanticIndex,
		DXGI_FORMAT format, unsigned int inputSlot, unsigned int byteOffset,
		D3D12_INPUT_CLASSIFICATION inputSlotClassifcation,
		unsigned int instanceStepRate)
	{
		D3D12_INPUT_ELEMENT_DESC inputElementDescription{};
		inputElementDescription.SemanticName = semanticName;
		inputElementDescription.SemanticIndex = semanticIndex;
		inputElementDescription.Format = format;
		inputElementDescription.InputSlot = inputSlot;
		inputElementDescription.AlignedByteOffset = byteOffset;
		inputElementDescription.InputSlotClass = inputSlotClassifcation;
		inputElementDescription.InstanceDataStepRate = instanceStepRate;

		mInputElementDescriptions.push_back(inputElementDescription);
	}

	void RenderScene::RemoveInputElementDescription(unsigned int index)
	{
		if (index>= mInputElementDescriptions.size())
			throw std::out_of_range("Index is out of bounds");
		
		mInputElementDescriptions.erase(mInputElementDescriptions.begin() + index);
	}

	void RenderScene::CreatePSO(unsigned int drawSettingsIndex, D3D12_FILL_MODE fillMode, BOOL enableMultisample,
		unsigned int vsIndex, unsigned int psIndex, const D3D12_PRIMITIVE_TOPOLOGY_TYPE& primitiveType, UINT sampleCount)
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pState{};
		ZeroMemory(&pState, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC)); //fills the block of memory with zeros

		pState.pRootSignature = mObjects.at(drawSettingsIndex).rootSig.Get();

		pState.VS.pShaderBytecode = (BYTE*)mShaders.at(vsIndex)->GetBufferPointer();
		pState.VS.BytecodeLength = mShaders.at(vsIndex)->GetBufferSize();

		pState.PS.pShaderBytecode = (BYTE*)mShaders.at(psIndex)->GetBufferPointer();
		pState.PS.BytecodeLength = mShaders.at(psIndex)->GetBufferSize();

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

		pState.InputLayout.pInputElementDescs = mInputElementDescriptions.data();
		pState.InputLayout.NumElements = (UINT)mInputElementDescriptions.size();

		pState.PrimitiveTopologyType = primitiveType;

		pState.NumRenderTargets = 1;
		pState.RTVFormats[0] = mDeviceResources.GetBackBufferFormat();

		pState.DSVFormat = mDeviceResources.GetDepthStencilFormat();

		pState.SampleDesc.Count = sampleCount;
		pState.SampleDesc.Quality = 0;

		Microsoft::WRL::ComPtr<ID3D12PipelineState> tempPSO;
		ThrowIfFailed(mDeviceResources.GetDevice()->CreateGraphicsPipelineState(&pState, IID_PPV_ARGS(&tempPSO)));
		mObjects.at(drawSettingsIndex).pipelineState = tempPSO;
	}

	void RenderScene::CreateRootSignature(unsigned int drawSettingsIndex)
	{
		//Describe a root signature to store all our root parameters.
		D3D12_ROOT_SIGNATURE_DESC rootSignatureDescription{};
		rootSignatureDescription.NumParameters = 2; //number of root paramters
		rootSignatureDescription.pParameters = mRootParameters; //the array of root parameters
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

		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
		ThrowIfFailed(mDeviceResources.GetDevice()->CreateRootSignature(0, seralizedRootSignature->GetBufferPointer(),
			seralizedRootSignature->GetBufferSize(), IID_PPV_ARGS(&rootSignature)));

		mObjects.at(drawSettingsIndex).rootSig = rootSignature;
	}

	void RenderScene::CreateStaticVertexBuffer(const void* data, unsigned int numBytes, unsigned int stride)
	{
		mStaticVertexBuffer.CreateStaticBuffer(mDeviceResources.GetDevice(), mDeviceResources.GetCommandList(), data, numBytes);

		mStaticVertexBuffer.CreateVertexBufferView(numBytes, stride);
	}

	void RenderScene::CreateStaticIndexBuffer(const void* data, unsigned int numBytes, DXGI_FORMAT format)
	{
		mStaticIndexBuffer.CreateStaticBuffer(mDeviceResources.GetDevice(), mDeviceResources.GetCommandList(), data, numBytes);

		mStaticIndexBuffer.CreateIndexBufferView(numBytes, format);
	}

	void RenderScene::CreateDynamicVertexBuffer(unsigned int numBytes, unsigned int stride)
	{
		for (int i = 0; i < DeviceResources::NUM_OF_FRAMES; ++i)
		{
			mDynamicVertexBuffer[i].CreateDynamicBuffer(mDeviceResources.GetDevice(), numBytes, stride);
			mDynamicVertexBuffer[i].CreateVertexBufferView(numBytes);
		}
	}

	void RenderScene::CreateDynamicIndexBuffer(unsigned int numBytes, DXGI_FORMAT format)
	{
		for (int i = 0; i < DeviceResources::NUM_OF_FRAMES; ++i)
		{
			mDynamicIndexBuffer[i].CreateDynamicBuffer(mDeviceResources.GetDevice(), numBytes, format);
			mDynamicIndexBuffer[i].CreateIndexBufferView(numBytes);
		}
	}

	void RenderScene::CreateObjectConstantBuffer(UINT numOfBytes, UINT stride)
	{
		for (UINT i = 0; i < DeviceResources::NUM_OF_FRAMES; ++i)
		{
			mObjectConstantBuffer[i].CreateDynamicBuffer(mDeviceResources.GetDevice(), numOfBytes, stride);
		}

		D3D12_ROOT_DESCRIPTOR objectCBDescription{};
		objectCBDescription.ShaderRegister = 0; //the register where the scenes objects constant data will be stored.
		objectCBDescription.RegisterSpace = 0;

		//This root paramter is a root descriptor.
		mRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		mRootParameters[0].Descriptor = objectCBDescription;
		mRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	}

	void RenderScene::CreatePassConstantBuffer(UINT numOfBytes, UINT stride)
	{
		for (UINT i = 0; i < DeviceResources::NUM_OF_FRAMES; ++i)
		{
			mPassConstantBuffer[i].CreateDynamicBuffer(mDeviceResources.GetDevice(), numOfBytes, stride);
		}

		D3D12_ROOT_DESCRIPTOR passCBDescription{};
		passCBDescription.ShaderRegister = 1; //the register where the scenes objects constant data will be stored.
		passCBDescription.RegisterSpace = 0;

		//This root paramter is a root descriptor.
		mRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		mRootParameters[1].Descriptor = passCBDescription;
		mRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	}

	void RenderScene::SetPSO(unsigned int drawSettingsIndex,
		const Microsoft::WRL::ComPtr<ID3D12PipelineState>& pso)
	{
		mObjects.at(drawSettingsIndex).pipelineState = pso;
	}

	void RenderScene::SetRootSignature(unsigned int drawSettingsIndex,
		const Microsoft::WRL::ComPtr<ID3D12RootSignature>& rootSignature)
	{
		mObjects.at(drawSettingsIndex).rootSig = rootSignature;
	}

	void RenderScene::SetPrimitive(unsigned int drawSettingsIndex,
		const D3D_PRIMITIVE_TOPOLOGY& primitive)
	{
		mObjects.at(drawSettingsIndex).prim = primitive;
	}

	void RenderScene::AddDrawArgument(unsigned int drawSettingsIndex,
		const FAShapes::DrawArguments& drawArg)
	{
		mObjects.at(drawSettingsIndex).drawArgs.push_back(drawArg);
	}

	void RenderScene::CreateDrawArgument(unsigned int drawSettingsIndex,
		unsigned int indexCount, unsigned int locationOfFirstIndex, int indexOfFirstVertex, int indexOfConstantData)
	{
		FAShapes::DrawArguments drawArgs{ indexCount, locationOfFirstIndex, indexOfFirstVertex, indexOfConstantData };
		mObjects.at(drawSettingsIndex).drawArgs.push_back(drawArgs);
	}

	void RenderScene::RemoveDrawArgument(unsigned int drawSettingsIndex, unsigned int drawArgIndex)
	{
		std::vector<FAShapes::DrawArguments>::iterator it = mObjects.at(drawSettingsIndex).drawArgs.begin();
		mObjects.at(drawSettingsIndex).drawArgs.erase(it + drawArgIndex);
	}

	void RenderScene::CreateDrawSettings()
	{
		mObjects.push_back(DrawSettings());
	}

	void RenderScene::RemoveDrawSettings(unsigned int drawSettingsIndex)
	{
		if(drawSettingsIndex >= mObjects.size())
			throw std::out_of_range("Index is out of bounds");

		mObjects.erase(mObjects.begin() + drawSettingsIndex);
	}

	void RenderScene::CreateText(FAMath::Vector4D textLocation, const std::wstring& textString,
		float textSize, const FAColor::Color textColor)
	{
		mTexts.push_back(Text(textLocation, textString, textSize, textColor));
	}

	void RenderScene::RemoveText(unsigned int textIndex)
	{
		if(textIndex >= mTexts.size())
			throw std::out_of_range("Index is out of bounds");

		mTexts.erase(mTexts.begin() + textIndex);
	}

	void RenderScene::BeforeDrawObjects()
	{
		mDeviceResources.Draw(mIsMSAAEnabled);
	}

	void RenderScene::DrawStatic(unsigned int drawSettingsIndex)
	{
		mDeviceResources.GetCommandList()->IASetVertexBuffers(0, 1, &mStaticVertexBuffer.GetVertexBufferView());

		mDeviceResources.GetCommandList()->IASetIndexBuffer(&mStaticIndexBuffer.GetIndexBufferView());

		mDeviceResources.GetCommandList()->SetPipelineState(mObjects.at(drawSettingsIndex).pipelineState.Get());

		mDeviceResources.GetCommandList()->SetGraphicsRootSignature(mObjects.at(drawSettingsIndex).rootSig.Get());

		//Set the GPU address of the pass constants data.
		mDeviceResources.GetCommandList()->SetGraphicsRootConstantBufferView(1,
			mPassConstantBuffer[mDeviceResources.GetCurrentFrame()].GetGPUAddress());

		mDeviceResources.GetCommandList()->IASetPrimitiveTopology(mObjects.at(drawSettingsIndex).prim);

		//draw all the objects the share the same PSO, root signature and primitive
		for (const FAShapes::DrawArguments& i : mObjects.at(drawSettingsIndex).drawArgs)
		{
			//Get the GPU address of the first byte of the object constant buffer.
			D3D12_GPU_VIRTUAL_ADDRESS cbAddress{ mObjectConstantBuffer[mDeviceResources.GetCurrentFrame()].GetGPUAddress() };

			//Calculate the offset of where the objects constant data is in the constant buffer.
			UINT64 offset = i.indexOfConstantData * mObjectConstantBuffer[mDeviceResources.GetCurrentFrame()].GetStride();
			cbAddress += offset;

			//Set the GPU address of the objects constant data.
			mDeviceResources.GetCommandList()->SetGraphicsRootConstantBufferView(0, cbAddress);

			//Draw object
			mDeviceResources.GetCommandList()->DrawIndexedInstanced(i.indexCount, 1,
				i.locationOfFirstIndex, i.indexOfFirstVertex, 0);
		}
	}

	void RenderScene::DrawDynamic(unsigned int drawSettingsIndex)
	{
		mDeviceResources.GetCommandList()->IASetVertexBuffers(0, 1, 
			&mDynamicVertexBuffer[mDeviceResources.GetCurrentFrame()].GetVertexBufferView());

		mDeviceResources.GetCommandList()->IASetIndexBuffer(
			&mDynamicIndexBuffer[mDeviceResources.GetCurrentFrame()].GetIndexBufferView());

		mDeviceResources.GetCommandList()->SetPipelineState(mObjects.at(drawSettingsIndex).pipelineState.Get());

		mDeviceResources.GetCommandList()->SetGraphicsRootSignature(mObjects.at(drawSettingsIndex).rootSig.Get());

		//Set the GPU address of the pass constants data.
		mDeviceResources.GetCommandList()->SetGraphicsRootConstantBufferView(1,
			mPassConstantBuffer[mDeviceResources.GetCurrentFrame()].GetGPUAddress());

		mDeviceResources.GetCommandList()->IASetPrimitiveTopology(mObjects.at(drawSettingsIndex).prim);

		//draw all the objects the share the same PSO, root signature and primitive
		for (const FAShapes::DrawArguments& i : mObjects.at(drawSettingsIndex).drawArgs)
		{
			//Get the GPU address of the first byte of the object constant buffer.
			D3D12_GPU_VIRTUAL_ADDRESS cbAddress{ mObjectConstantBuffer[mDeviceResources.GetCurrentFrame()].GetGPUAddress() };

			//Calculate the offset of where the objects constant data is in the constant buffer.
			UINT64 offset = i.indexOfConstantData * mObjectConstantBuffer[mDeviceResources.GetCurrentFrame()].GetStride();
			cbAddress += offset;

			//Set the GPU address of the objects constant data.
			mDeviceResources.GetCommandList()->SetGraphicsRootConstantBufferView(0, cbAddress);

			mDeviceResources.GetCommandList()->DrawIndexedInstanced(i.indexCount, 1,
				i.locationOfFirstIndex, i.indexOfFirstVertex, 0);
		}
	}

	void RenderScene::AfterDrawObjects()
	{
		mDeviceResources.RTBufferTransition(mIsMSAAEnabled, mIsTextEnabled);

		mDeviceResources.Execute();
	}

	void RenderScene::BeforeDrawText()
	{
		if(mIsTextEnabled)
			mDeviceResources.BeforeTextDraw();
	}

	void RenderScene::RenderText(unsigned int textIndex)
	{
		if (mIsTextEnabled)
		{
			Text* textToRender{ &mTexts.at(textIndex) };

			D2D_RECT_F textLocation{ textToRender->GetTextLocation().GetX(), textToRender->GetTextLocation().GetY(),
				textToRender->GetTextLocation().GetZ(), textToRender->GetTextLocation().GetW() };

			D2D1_COLOR_F textColor{ textToRender->GetTextColor().GetRed(), textToRender->GetTextColor().GetGreen(),
				textToRender->GetTextColor().GetBlue(), textToRender->GetTextColor().GetAlpha() };

			Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> mDirect2DBrush;
			Microsoft::WRL::ComPtr<IDWriteTextFormat> mDirectWriteFormat;

			ThrowIfFailed(mDeviceResources.GetTextResources().GetDirect2DDeviceContext()->CreateSolidColorBrush(textColor,
				mDirect2DBrush.GetAddressOf()));

			ThrowIfFailed(mDeviceResources.GetTextResources().GetDirectWriteFactory()->CreateTextFormat(
				L"Verdana",
				nullptr,
				DWRITE_FONT_WEIGHT_NORMAL,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				textToRender->GetTextSize(),
				L"en-us",
				&mDirectWriteFormat));

			ThrowIfFailed(mDirectWriteFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING));
			ThrowIfFailed(mDirectWriteFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER));

			mDeviceResources.GetTextResources().GetDirect2DDeviceContext()->DrawTextW(textToRender->GetTextString().c_str(),
				(UINT32)textToRender->GetTextString().size(), mDirectWriteFormat.Get(),
				&textLocation, mDirect2DBrush.Get());
		}
	}

	void RenderScene::AfterDrawText()
	{
		if (mIsTextEnabled)
			mDeviceResources.AfterTextDraw();
	}

	void RenderScene::AfterDraw()
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

	void RenderScene::NextFrame()
	{
		mDeviceResources.NextFrame();
		mDeviceResources.WaitForGPU();
	}

	void RenderScene::Resize(unsigned int width, unsigned int height, HWND windowHandle)
	{
		mDeviceResources.Resize(width, height, windowHandle, mIsMSAAEnabled, mIsTextEnabled);

		mCamera.SetAspectRatio((float)width / height);
	}

	void RenderScene::CopyDataIntoDyanmicVertexBuffer(UINT index, const void* data, UINT64 numOfBytes)
	{
		mDynamicVertexBuffer[mDeviceResources.GetCurrentFrame()].CopyData(index, data, numOfBytes);
	}

	void RenderScene::CopyDataIntoDyanmicIndexBuffer(UINT index,  const void* data, UINT64 numOfBytes)
	{
		mDynamicIndexBuffer[mDeviceResources.GetCurrentFrame()].CopyData(index, data, numOfBytes);
	}

	void RenderScene::CopyDataIntoObjectConstantBuffer(UINT index, const void* data, UINT64 numOfBytes)
	{
		mObjectConstantBuffer[mDeviceResources.GetCurrentFrame()].CopyData(index, data, numOfBytes);
	}

	void RenderScene::CopyDataIntoPassConstantBuffer(UINT index, const void* data, UINT64 numOfBytes)
	{
		mPassConstantBuffer[mDeviceResources.GetCurrentFrame()].CopyData(index, data, numOfBytes);
	}

	bool RenderScene::IsMSAAEnabled() const
	{
		return mIsMSAAEnabled;
	}

	void RenderScene::DisableMSAA(unsigned int width, unsigned int height, HWND windowHandle)
	{
		mIsMSAAEnabled = false;
		mDeviceResources.Resize(width, height, windowHandle, mIsMSAAEnabled, mIsTextEnabled);
	}

	void RenderScene::EnableMSAA(unsigned int width, unsigned int height, HWND windowHandle)
	{
		mIsMSAAEnabled = true;
		mDeviceResources.Resize(width, height, windowHandle, mIsMSAAEnabled, mIsTextEnabled);
	}

	bool RenderScene::IsTextEnabled() const
	{
		return mIsTextEnabled;
	}

	void RenderScene::DisableText(unsigned int width, unsigned int height, HWND windowHandle)
	{
		mIsTextEnabled = false;
		mDeviceResources.Resize(width, height, windowHandle, mIsMSAAEnabled, mIsTextEnabled);
	}

	void RenderScene::EnableText(unsigned int width, unsigned int height, HWND windowHandle)
	{
		mIsTextEnabled = true;
		mDeviceResources.Resize(width, height, windowHandle, mIsMSAAEnabled, mIsTextEnabled);
	}

	//-----------------------------------------------------------------------------------------------------------------------
}