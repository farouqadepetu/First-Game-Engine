#include "FARenderScene.h"
#include "FADirectXException.h"
#include <fstream>
#include <d3dx12.h>

namespace FARender
{
	//-----------------------------------------------------------------------------------------------------------------------
	//RENDER SCENE FUNCITON DEFINTIONS

	DeviceResources RenderScene::mDeviceResources;

	RenderScene::RenderScene(unsigned int width, unsigned int height, HWND handle)
	{
		mDeviceResources.InitializeDirect3D(width, height, handle);
	}

	DeviceResources& RenderScene::GetDeviceResources()
	{
		return mDeviceResources;
	}

	const DeviceResources& RenderScene::GetDeviceResources() const
	{
		return mDeviceResources;
	}

	const Microsoft::WRL::ComPtr<ID3DBlob>& RenderScene::GetShader(const std::wstring& name) const
	{
		return mShaders.at(name);
	}

	const std::vector<D3D12_INPUT_ELEMENT_DESC>& RenderScene::GetInputElementLayout(const std::wstring& name) const
	{
		return mInputElementDescriptions.at(name);
	}

	const D3D12_RASTERIZER_DESC& RenderScene::GetRasterizationState(const std::wstring& name) const
	{
		return mRasterizationStates.at(name);
	}

	const Microsoft::WRL::ComPtr<ID3D12PipelineState>& RenderScene::GetPSO(const std::wstring& drawSettingsName) const
	{
		return mSceneObjects.at(drawSettingsName).pipelineState;
	}

	const Microsoft::WRL::ComPtr<ID3D12RootSignature>& RenderScene::GetRootSignature(const std::wstring& drawSettingsName) const
	{
		return mSceneObjects.at(drawSettingsName).rootSig;
	}

	const D3D_PRIMITIVE_TOPOLOGY& RenderScene::GetPrimitive(const std::wstring& drawSettingsName) const
	{
		return mSceneObjects.at(drawSettingsName).prim;
	}

	FAShapes::DrawArguments& RenderScene::GetDrawArguments(const std::wstring& drawSettingsName, unsigned int index)
	{
		return mSceneObjects.at(drawSettingsName).drawArgs.at(index);
	}

	const FAShapes::DrawArguments& RenderScene::GetDrawArguments(const std::wstring& drawSettingsName, unsigned int index) const
	{
		return mSceneObjects.at(drawSettingsName).drawArgs.at(index);
	}

	ConstantBuffer& RenderScene::GetConstantBuffer()
	{
		return mConstantBuffer[currentFrame];
	}

	const ConstantBuffer& RenderScene::GetConstantBuffer() const
	{
		return mConstantBuffer[currentFrame];
	}

	const UINT& RenderScene::GetCBVSize() const
	{
		return mCBVSize;
	}

	const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& RenderScene::GetCBVHeap() const
	{
		return mCBVHeap;
	}

	const D3D12_ROOT_PARAMETER& RenderScene::GetCBVHeapRootParameter() const
	{
		return mCBVHeapRootParameter;
	}

	void RenderScene::LoadShader(const std::wstring& filename, const std::wstring& name)
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

		mShaders[name] = blob;
	}

	void RenderScene::RemoveShader(const std::wstring& shaderName)
	{
		mShaders.erase(shaderName);
	}

	void RenderScene::StoreInputElementDescriptions(const std::wstring& name,
		const std::vector<D3D12_INPUT_ELEMENT_DESC>& inputElementLayout)
	{
		mInputElementDescriptions[name] = inputElementLayout;
	}

	void RenderScene::StoreInputElementDescriptions(const std::wstring& name,
		const D3D12_INPUT_ELEMENT_DESC* inputElementLayout,
		UINT numElements)
	{
		mInputElementDescriptions[name] = std::vector<D3D12_INPUT_ELEMENT_DESC>(inputElementLayout,
			inputElementLayout + numElements);
	}

	void RenderScene::RemoveInputElementDescription(const std::wstring& name)
	{
		mInputElementDescriptions.erase(name);
	}

	void RenderScene::CreateRasterizationState(D3D12_FILL_MODE fillMode, BOOL enableMultisample, const std::wstring& name)
	{
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

		mRasterizationStates[name] = rDescription;
	}

	void RenderScene::RemoveRasterizationState(const std::wstring& name)
	{
		mRasterizationStates.erase(name);
	}

	void RenderScene::CreatePSO(const std::wstring& drawSettingsName, const std::wstring& rStateName,
		const std::wstring& vsName, const std::wstring& psName, const std::wstring& inputLayoutName,
		const D3D12_PRIMITIVE_TOPOLOGY_TYPE& primitiveType, UINT sampleCount)
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pState{};
		ZeroMemory(&pState, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC)); //fills the block of memory with zeros

		pState.pRootSignature = mSceneObjects.at(drawSettingsName).rootSig.Get();

		pState.VS.pShaderBytecode = (BYTE*)mShaders.at(vsName)->GetBufferPointer();
		pState.VS.BytecodeLength = mShaders.at(vsName)->GetBufferSize();

		pState.PS.pShaderBytecode = (BYTE*)mShaders.at(psName)->GetBufferPointer();
		pState.PS.BytecodeLength = mShaders.at(psName)->GetBufferSize();

		pState.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

		pState.SampleMask = UINT_MAX;

		pState.RasterizerState = mRasterizationStates.at(rStateName);

		pState.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

		pState.InputLayout.pInputElementDescs = mInputElementDescriptions.at(inputLayoutName).data();
		pState.InputLayout.NumElements = (UINT)mInputElementDescriptions.at(inputLayoutName).size();

		pState.PrimitiveTopologyType = primitiveType;

		pState.NumRenderTargets = 1;
		pState.RTVFormats[0] = mDeviceResources.GetBackBufferFormat();

		pState.DSVFormat = mDeviceResources.GetDepthStencilFormat();

		pState.SampleDesc.Count = sampleCount;
		pState.SampleDesc.Quality = 0;

		Microsoft::WRL::ComPtr<ID3D12PipelineState> tempPSO;
		ThrowIfFailed(mDeviceResources.GetDevice()->CreateGraphicsPipelineState(&pState, IID_PPV_ARGS(&tempPSO)));
		mSceneObjects.at(drawSettingsName).pipelineState = tempPSO;
	}

	void RenderScene::CreateRootSignature(const std::wstring& drawSettingsName, 
		const D3D12_ROOT_PARAMETER* rootParameters, UINT numParameters)
	{
		//Describe a root signature to store all our root parameters.
		D3D12_ROOT_SIGNATURE_DESC rootSignatureDescription{};
		rootSignatureDescription.NumParameters = numParameters; //number of root paramters
		rootSignatureDescription.pParameters = rootParameters; //the array of root parameters
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

		mSceneObjects.at(drawSettingsName).rootSig = rootSignature;
	}

	void RenderScene::CreateVertexBuffer(const void* data, UINT numBytes, UINT stride)
	{
		mVertexBuffer.CreateVertexBuffer(mDeviceResources.GetDevice(), mDeviceResources.GetCommandList(), data, numBytes);
		mVertexBuffer.CreateVertexBufferView(numBytes, stride);
	}

	void RenderScene::CreateIndexBuffer(const void* data, UINT numBytes, DXGI_FORMAT format)
	{
		mIndexBuffer.CreateIndexBuffer(mDeviceResources.GetDevice(), mDeviceResources.GetCommandList(), data, numBytes);
		mIndexBuffer.CreateIndexBufferView(numBytes, format);
	}

	void RenderScene::CreateCBVHeap(UINT numDescriptors, UINT shaderRegister)
	{
		mCBVSize = mDeviceResources.GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		//Need a CBV for each object for each frame.
		D3D12_DESCRIPTOR_HEAP_DESC cbvDescriptorHeapDescription{};
		cbvDescriptorHeapDescription.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		cbvDescriptorHeapDescription.NumDescriptors = numDescriptors * numFrames;
		cbvDescriptorHeapDescription.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		cbvDescriptorHeapDescription.NodeMask = 0;
		ThrowIfFailed(mDeviceResources.GetDevice()->CreateDescriptorHeap(&cbvDescriptorHeapDescription, IID_PPV_ARGS(&mCBVHeap)));

		//Describes the number of CBV's in the CB descriptor heap
		mCBVHeapDescription.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;

		//num of descriptors in my cbv descriptor heap that are type cbv
		mCBVHeapDescription.NumDescriptors = numDescriptors * numFrames;

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
		for (UINT i = 0; i < numFrames; ++i)
		{
			mConstantBuffer[i].CreateConstantBuffer(mDeviceResources.GetDevice(), numOfBytes);
		}
	}

	void RenderScene::CreateConstantBufferView( UINT index, UINT numBytes)
	{
		//Create a constant buffer view for each frame.
		for (UINT i = 0; i < numFrames; ++i)
		{
			mConstantBuffer[i].CreateConstantBufferView(mDeviceResources.GetDevice(), mCBVHeap, mCBVSize, (index * numFrames) + i,
				index, numBytes);
		}
	}

	void RenderScene::SetPSO(const std::wstring& drawSettingsName, 
		const Microsoft::WRL::ComPtr<ID3D12PipelineState>& pso)
	{
		mSceneObjects.at(drawSettingsName).pipelineState = pso;
	}

	void RenderScene::SetRootSignature(const std::wstring& drawSettingsName, 
		const Microsoft::WRL::ComPtr<ID3D12RootSignature>& rootSignature)
	{
		mSceneObjects.at(drawSettingsName).rootSig = rootSignature;
	}

	void RenderScene::SetPrimitive(const std::wstring& drawSettingsName, 
		const D3D_PRIMITIVE_TOPOLOGY& primitive)
	{
		mSceneObjects.at(drawSettingsName).prim = primitive;
	}

	void RenderScene::AddDrawArgument(const std::wstring& drawSettingsName,
		const FAShapes::DrawArguments& drawArg)
	{
		mSceneObjects.at(drawSettingsName).drawArgs.push_back(drawArg);
	}

	void RenderScene::AddDrawArgument(const std::wstring& drawSettingsName,
		unsigned int indexCount, unsigned int locationOfFirstIndex, int indexOfFirstVertex, int indexOfConstantData)
	{
		FAShapes::DrawArguments drawArgs{ indexCount, locationOfFirstIndex, indexOfFirstVertex, indexOfConstantData };
		mSceneObjects.at(drawSettingsName).drawArgs.push_back(drawArgs);
	}

	void RenderScene::RemoveDrawArgument(const std::wstring& drawSettingsName, unsigned int index)
	{
		std::vector<FAShapes::DrawArguments>::iterator it = mSceneObjects.at(drawSettingsName).drawArgs.begin();
		mSceneObjects.at(drawSettingsName).drawArgs.erase(it + index);
	}

	void RenderScene::CreateDrawSettings(const std::wstring& drawSettingsName)
	{
		mSceneObjects[drawSettingsName];
	}

	void RenderScene::RemoveDrawSettings(const std::wstring& drawSettingsName)
	{
		mSceneObjects.erase(drawSettingsName);
	}

	void RenderScene::CreateText(const std::wstring& textName, FAMath::Vector4D textLocation, const std::wstring& textString,
		float textSize, const FAColor::Color textColor)
	{
		mSceneText[textName].Initialize(mDeviceResources, textLocation, textString, textSize, textColor);
	}

	void RenderScene::RemoveText(const std::wstring& textName)
	{
		mSceneText.erase(textName);
	}

	void RenderScene::ChangeTextLocation(const std::wstring& textName, FAMath::Vector4D textLocation)
	{
		mSceneText.at(textName).SetTextLocation(textLocation);
	}

	void RenderScene::ChangeTextString(const std::wstring& textName, const std::wstring& textString)
	{
		mSceneText.at(textName).SetTextString(textString);
	}

	void RenderScene::ChangeTextSize(const std::wstring& textName, float textSize)
	{
		mSceneText.at(textName).SetTextSize(mDeviceResources, textSize);
	}

	void RenderScene::ChangeTextColor(const std::wstring& textName, const FAColor::Color textColor)
	{
		mSceneText.at(textName).SetTextColor(textColor);
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

	void RenderScene::DrawObjects(const std::wstring& drawSettingsName)
	{
		mDeviceResources.GetCommandList()->SetPipelineState(mSceneObjects.at(drawSettingsName).pipelineState.Get());

		mDeviceResources.GetCommandList()->SetGraphicsRootSignature(mSceneObjects.at(drawSettingsName).rootSig.Get());

		mDeviceResources.GetCommandList()->IASetPrimitiveTopology(mSceneObjects.at(drawSettingsName).prim);

		//draw all the objects the share the same PSO, root signature and primitive
		for (const FAShapes::DrawArguments& i : mSceneObjects.at(drawSettingsName).drawArgs)
		{
			//Get the address of the first view in the constant buffer view heap
			CD3DX12_GPU_DESCRIPTOR_HANDLE handle =
				CD3DX12_GPU_DESCRIPTOR_HANDLE(mCBVHeap->GetGPUDescriptorHandleForHeapStart());

			handle.Offset((i.indexOfConstantData * numFrames) + currentFrame, mCBVSize);

			mDeviceResources.GetCommandList()->SetGraphicsRootDescriptorTable(0, handle);

			mDeviceResources.GetCommandList()->DrawIndexedInstanced(i.indexCount, 1,
				i.locationOfFirstIndex, i.indexOfFirstVertex, 0);
		}
	}

	void RenderScene::AfterDrawObjects(bool renderText)
	{
		mDeviceResources.RTBufferTransition(renderText);

		mDeviceResources.Execute();
	}

	void RenderScene::BeforeDrawText()
	{
		mDeviceResources.BeforeTextDraw();
	}

	void RenderScene::RenderText(const std::wstring& textName)
	{
		D2D_RECT_F r{ mSceneText.at(textName).GetTextLocation().GetX(), mSceneText.at(textName).GetTextLocation().GetY(),
			mSceneText.at(textName).GetTextLocation().GetZ(), mSceneText.at(textName).GetTextLocation().GetW() };

		mDeviceResources.GetDevice2DContext()->DrawTextW(mSceneText.at(textName).GetTextString().c_str(),
			(UINT32)mSceneText.at(textName).GetTextString().size(), mSceneText.at(textName).GetFormat().Get(),
			&r, mSceneText.at(textName).GetBrush().Get());
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

	//-----------------------------------------------------------------------------------------------------------------------
}