#include "FARenderScene.h"
#include "FADirectXException.h"
#include <fstream>
#include <d3dx12.h>

namespace FARender
{
	//-----------------------------------------------------------------------------------------------------------------------
	//RENDER SCENE FUNCITON DEFINTIONS

	RenderScene::RenderScene(unsigned int width, unsigned int height, HWND windowHandle) : 
		mDeviceResources{ DeviceResources::GetInstance(width, height, windowHandle, NUM_OF_FRAMES) }
	{
		mCamera.SetAspectRatio((float)width / height);
	}

	const DeviceResources& RenderScene::GetDeviceResources() const
	{
		return mDeviceResources;
	}

	FAShapes::DrawArguments& RenderScene::GetDrawArguments(unsigned int drawSettingsIndex, unsigned int drawArgsIndex)
	{
		return mSceneObjects.at(drawSettingsIndex).drawArgs.at(drawArgsIndex);
	}

	const FAShapes::DrawArguments& RenderScene::GetDrawArguments(unsigned int drawSettingsIndex, unsigned int drawArgsIndex) const
	{
		return mSceneObjects.at(drawSettingsIndex).drawArgs.at(drawArgsIndex);
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

		pState.pRootSignature = mSceneObjects.at(drawSettingsIndex).rootSig.Get();

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
		mSceneObjects.at(drawSettingsIndex).pipelineState = tempPSO;
	}

	void RenderScene::CreateRootSignature(unsigned int drawSettingsIndex)
	{
		//Describe a root signature to store all our root parameters.
		D3D12_ROOT_SIGNATURE_DESC rootSignatureDescription{};
		rootSignatureDescription.NumParameters = 1; //number of root paramters
		rootSignatureDescription.pParameters = &mCBVHeapRootParameter; //the array of root parameters
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

		mSceneObjects.at(drawSettingsIndex).rootSig = rootSignature;
	}

	void RenderScene::CreateVertexBuffer()
	{
		mVertexBuffer.CreateVertexBuffer(mDeviceResources.GetDevice(), mDeviceResources.GetCommandList(), mVertexList.data(), 
			static_cast<UINT>(mVertexList.size() * sizeof(FAShapes::Vertex)));

		mVertexBuffer.CreateVertexBufferView(static_cast<UINT>(mVertexList.size() * sizeof(FAShapes::Vertex)), sizeof(FAShapes::Vertex));
	}

	void RenderScene::CreateIndexBuffer()
	{
		mIndexBuffer.CreateIndexBuffer(mDeviceResources.GetDevice(), mDeviceResources.GetCommandList(), mIndexList.data(), 
			static_cast<UINT>(mIndexList.size() * sizeof(unsigned int)));

		mIndexBuffer.CreateIndexBufferView(static_cast<UINT>(mIndexList.size() * sizeof(unsigned int)), DXGI_FORMAT_R32_UINT);
	}

	void RenderScene::CreateCBVHeap(UINT numDescriptors, UINT shaderRegister)
	{
		//Need a CBV for each object for each frame.
		D3D12_DESCRIPTOR_HEAP_DESC cbvDescriptorHeapDescription{};
		cbvDescriptorHeapDescription.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		cbvDescriptorHeapDescription.NumDescriptors = numDescriptors * mDeviceResources.GetNumFrames();
		cbvDescriptorHeapDescription.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		cbvDescriptorHeapDescription.NodeMask = 0;
		ThrowIfFailed(mDeviceResources.GetDevice()->CreateDescriptorHeap(&cbvDescriptorHeapDescription, IID_PPV_ARGS(&mCBVHeap)));

		//Describes the number of CBV's in the CB descriptor heap
		mCBVHeapDescription.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;

		//num of descriptors in my cbv descriptor heap that are type cbv
		mCBVHeapDescription.NumDescriptors = numDescriptors * mDeviceResources.GetNumFrames();

		//which register does your vertex shader expect the buffer to be in
		mCBVHeapDescription.BaseShaderRegister = shaderRegister;

		mCBVHeapDescription.RegisterSpace = 0;
		mCBVHeapDescription.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		//describe the descriptor range
		D3D12_ROOT_DESCRIPTOR_TABLE cbvTable{};
		cbvTable.NumDescriptorRanges = 1;
		cbvTable.pDescriptorRanges = &mCBVHeapDescription;

		//describe the CBV root parameter.
		mCBVHeapRootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		mCBVHeapRootParameter.DescriptorTable = cbvTable;
		mCBVHeapRootParameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	}

	void RenderScene::CreateConstantBuffer(UINT numOfBytes)
	{
		unsigned int numFrames{ mDeviceResources.GetNumFrames() };
		for (UINT i = 0; i < numFrames; ++i)
		{
			mConstantBuffer[i].CreateConstantBuffer(mDeviceResources.GetDevice(), numOfBytes);
		}
	}

	void RenderScene::CreateConstantBufferView(UINT index, UINT numBytes)
	{
		unsigned int numFrames{ mDeviceResources.GetNumFrames() };

		//Create a constant buffer view for each frame.
		for (UINT i = 0; i < numFrames; ++i)
		{
			mConstantBuffer[i].CreateConstantBufferView(mDeviceResources.GetDevice(), mCBVHeap, 
				mDeviceResources.GetCBVSize(), (index * numFrames) + i, index, numBytes);
		}
	}

	void RenderScene::SetPSO(unsigned int drawSettingsIndex,
		const Microsoft::WRL::ComPtr<ID3D12PipelineState>& pso)
	{
		mSceneObjects.at(drawSettingsIndex).pipelineState = pso;
	}

	void RenderScene::SetRootSignature(unsigned int drawSettingsIndex,
		const Microsoft::WRL::ComPtr<ID3D12RootSignature>& rootSignature)
	{
		mSceneObjects.at(drawSettingsIndex).rootSig = rootSignature;
	}

	void RenderScene::SetPrimitive(unsigned int drawSettingsIndex,
		const D3D_PRIMITIVE_TOPOLOGY& primitive)
	{
		mSceneObjects.at(drawSettingsIndex).prim = primitive;
	}

	void RenderScene::AddDrawArgument(unsigned int drawSettingsIndex,
		const FAShapes::DrawArguments& drawArg)
	{
		mSceneObjects.at(drawSettingsIndex).drawArgs.push_back(drawArg);
	}

	void RenderScene::CreateDrawArgument(unsigned int drawSettingsIndex,
		unsigned int indexCount, unsigned int locationOfFirstIndex, int indexOfFirstVertex, int indexOfConstantData)
	{
		FAShapes::DrawArguments drawArgs{ indexCount, locationOfFirstIndex, indexOfFirstVertex, indexOfConstantData };
		mSceneObjects.at(drawSettingsIndex).drawArgs.push_back(drawArgs);
	}

	void RenderScene::RemoveDrawArgument(unsigned int drawSettingsIndex, unsigned int drawArgIndex)
	{
		std::vector<FAShapes::DrawArguments>::iterator it = mSceneObjects.at(drawSettingsIndex).drawArgs.begin();
		mSceneObjects.at(drawSettingsIndex).drawArgs.erase(it + drawArgIndex);
	}

	void RenderScene::CreateDrawSettings()
	{
		mSceneObjects.push_back(DrawSettings());
	}

	void RenderScene::RemoveDrawSettings(unsigned int drawSettingsIndex)
	{
		if(drawSettingsIndex >= mSceneObjects.size())
			throw std::out_of_range("Index is out of bounds");

		mSceneObjects.erase(mSceneObjects.begin() + drawSettingsIndex);
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

	void RenderScene::AddVertices(const std::vector<FAShapes::Vertex>& vertices)
	{
		mVertexList.insert(mVertexList.end(), vertices.begin(), vertices.end());
	}

	void RenderScene::AddVertices(const FAShapes::Vertex* vertices, unsigned int numVertices)
	{
		mVertexList.insert(mVertexList.end(), vertices, vertices + numVertices);
	}

	void RenderScene::AddIndices(const std::vector<unsigned int>& indices)
	{
		mIndexList.insert(mIndexList.end(), indices.begin(), indices.end());
	}

	void RenderScene::AddIndices(const unsigned int* indices, unsigned int numIndices)
	{
		mIndexList.insert(mIndexList.end(), indices, indices + numIndices);
	}

	void RenderScene::BeforeDrawObjects()
	{
		mDeviceResources.Draw();

		//Link the CBV descriptor heaps to the pipeline
		ID3D12DescriptorHeap* dH[] = { mCBVHeap.Get() };
		mDeviceResources.GetCommandList()->SetDescriptorHeaps(1, dH);

		mDeviceResources.GetCommandList()->IASetVertexBuffers(0, 1, &mVertexBuffer.GetVertexBufferView());

		mDeviceResources.GetCommandList()->IASetIndexBuffer(&mIndexBuffer.GetIndexBufferView());
	}

	void RenderScene::DrawObjects(unsigned int drawSettingsIndex)
	{
		unsigned int numFrames{ mDeviceResources.GetNumFrames() };

		mDeviceResources.GetCommandList()->SetPipelineState(mSceneObjects.at(drawSettingsIndex).pipelineState.Get());

		mDeviceResources.GetCommandList()->SetGraphicsRootSignature(mSceneObjects.at(drawSettingsIndex).rootSig.Get());

		mDeviceResources.GetCommandList()->IASetPrimitiveTopology(mSceneObjects.at(drawSettingsIndex).prim);

		//draw all the objects the share the same PSO, root signature and primitive
		for (const FAShapes::DrawArguments& i : mSceneObjects.at(drawSettingsIndex).drawArgs)
		{
			//Get the address of the first view in the constant buffer view heap
			CD3DX12_GPU_DESCRIPTOR_HANDLE handle =
				CD3DX12_GPU_DESCRIPTOR_HANDLE(mCBVHeap->GetGPUDescriptorHandleForHeapStart());

			handle.Offset((i.indexOfConstantData * numFrames) + mDeviceResources.GetCurrentFrame(),
				mDeviceResources.GetCBVSize());

			mDeviceResources.GetCommandList()->SetGraphicsRootDescriptorTable(0, handle);

			mDeviceResources.GetCommandList()->DrawIndexedInstanced(i.indexCount, 1,
				i.locationOfFirstIndex, i.indexOfFirstVertex, 0);
		}
	}

	void RenderScene::AfterDrawObjects()
	{
		mDeviceResources.RTBufferTransition();

		mDeviceResources.Execute();
	}

	void RenderScene::BeforeDrawText()
	{
		mDeviceResources.BeforeTextDraw();
	}

	void RenderScene::RenderText(unsigned int textIndex)
	{
		if (mDeviceResources.IsTextEnabled())
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
		mDeviceResources.AfterTextDraw();
	}

	void RenderScene::AfterDraw()
	{
		mDeviceResources.Present();

		//Update the current fence value
		mDeviceResources.UpdateCurrentFrameFenceValue();

		//Add a fence instruction to the command queue.
		mDeviceResources.Signal();
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
		mDeviceResources.Resize(width, height, windowHandle);

		mCamera.SetAspectRatio((float)width / height);
	}

	void RenderScene::CopyData(UINT index, UINT byteSize, const void* data, UINT64 numOfBytes)
	{
		mConstantBuffer[mDeviceResources.GetCurrentFrame()].CopyData(index, byteSize, data, numOfBytes);
	}

	bool RenderScene::IsMSAAEnabled() const
	{
		return mDeviceResources.IsMSAAEnabled();
	}

	void RenderScene::DisableMSAA(unsigned int width, unsigned int height, HWND windowHandle)
	{
		mDeviceResources.DisableMSAA(width, height, windowHandle);
	}

	void RenderScene::EnableMSAA(unsigned int width, unsigned int height, HWND windowHandle)
	{
		mDeviceResources.EnableMSAA(width, height, windowHandle);
	}

	bool RenderScene::IsTextEnabled() const
	{
		return mDeviceResources.IsTextEnabled();
	}

	void RenderScene::DisableText(unsigned int width, unsigned int height, HWND windowHandle)
	{
		mDeviceResources.DisableText(width, height, windowHandle);
	}

	void RenderScene::EnableText(unsigned int width, unsigned int height, HWND windowHandle)
	{
		mDeviceResources.EnableText(width, height, windowHandle);
	}

	//-----------------------------------------------------------------------------------------------------------------------
}