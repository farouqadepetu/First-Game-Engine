#include "FARenderScene.h"
#include "FADirectXException.h"
#include <fstream>
#include <d3dx12.h>

namespace FARender
{
	//-----------------------------------------------------------------------------------------------------------------------
	//RENDER SCENE FUNCITON DEFINTIONS

	const Microsoft::WRL::ComPtr<ID3DBlob>& RenderScene::shader(const std::wstring& name) const
	{
		return mShaders.at(name);
	}

	const std::vector<D3D12_INPUT_ELEMENT_DESC>& RenderScene::inputElementLayout(const std::wstring& name) const
	{
		return mInputElementDescriptions.at(name);
	}

	const D3D12_RASTERIZER_DESC& RenderScene::rasterizationState(const std::wstring& name) const
	{
		return mRasterizationStates.at(name);
	}

	const Microsoft::WRL::ComPtr<ID3D12PipelineState>& RenderScene::pso(const std::wstring& name) const
	{
		return mPSOs.at(name);
	}

	const Microsoft::WRL::ComPtr<ID3D12RootSignature>& RenderScene::rootSignature(const std::wstring& name) const
	{
		return mRootSignatures.at(name);
	}

	ConstantBuffer& RenderScene::cBuffer(const std::wstring& name)
	{
		return mConstantBuffers[currentFrame].at(name);
	}

	const ConstantBuffer& RenderScene::cBuffer(const std::wstring& name) const
	{
		return mConstantBuffers[currentFrame].at(name);
	}

	const UINT& RenderScene::cbvSize() const
	{
		return mCBVSize;
	}

	const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& RenderScene::cbvHeap() const
	{
		return mCBVHeap;
	}

	const D3D12_ROOT_PARAMETER& RenderScene::cbvHeapRootParameter() const
	{
		return mCBVHeapRootParameter;
	}
	
	const FAShapes::DrawArguments& RenderScene::drawArgument(const std::wstring& groupName, const std::wstring& objectName) const
	{
		return mDrawArgs.at(groupName).at(objectName);
	}

	void RenderScene::loadShader(const std::wstring& filename, const std::wstring& name)
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

	void RenderScene::storeInputElementDescriptions(const std::wstring& name,
		const std::vector<D3D12_INPUT_ELEMENT_DESC>& inputElementLayout)
	{
		mInputElementDescriptions[name] = inputElementLayout;
	}

	void RenderScene::storeInputElementDescriptions(const std::wstring& name,
		const D3D12_INPUT_ELEMENT_DESC* inputElementLayout,
		UINT numElements)
	{
		mInputElementDescriptions[name] = std::vector<D3D12_INPUT_ELEMENT_DESC>(inputElementLayout,
			inputElementLayout + numElements);
	}

	void RenderScene::createRasterizationState(D3D12_FILL_MODE fillMode, BOOL enableMultisample, const std::wstring& name)
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

	void RenderScene::createPSO(const Microsoft::WRL::ComPtr<ID3D12Device>& device, const std::wstring& psoName,
		const std::wstring& rootSignatureName, const std::wstring& rStateName, 
		const std::wstring& vsName, const std::wstring& psName, const std::wstring& inputLayoutName,
		const D3D12_PRIMITIVE_TOPOLOGY_TYPE& primitiveType, DXGI_FORMAT rtvFormat, DXGI_FORMAT dsvFormat, UINT sampleCount)
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pState{};
		ZeroMemory(&pState, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC)); //fills the block of memory with zeros

		pState.pRootSignature = mRootSignatures.at(rootSignatureName).Get();

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
		pState.RTVFormats[0] = rtvFormat;

		pState.DSVFormat = dsvFormat;

		pState.SampleDesc.Count = sampleCount;
		pState.SampleDesc.Quality = 0;

		Microsoft::WRL::ComPtr<ID3D12PipelineState> tempPSO;
		ThrowIfFailed(device->CreateGraphicsPipelineState(&pState, IID_PPV_ARGS(&tempPSO)));
		mPSOs[psoName] = tempPSO;
	}

	void RenderScene::createRootSignature(const Microsoft::WRL::ComPtr<ID3D12Device>& device, const std::wstring& name,
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
		ThrowIfFailed(device->CreateRootSignature(0, seralizedRootSignature->GetBufferPointer(),
			seralizedRootSignature->GetBufferSize(), IID_PPV_ARGS(&rootSignature)));

		mRootSignatures[name] = rootSignature;
	}

	void RenderScene::storeDrawArgument(const std::wstring& groupName, const std::wstring& objectName, 
		const FAShapes::DrawArguments& drawArgs)
	{
		FAShapes::DrawArguments temp{};
		temp.indexCount = drawArgs.indexCount;
		temp.locationOfFirstIndex = drawArgs.locationOfFirstIndex;
		temp.indexOfFirstVertex = drawArgs.indexOfFirstVertex;
		temp.indexOfConstantData = drawArgs.indexOfConstantData;

		mDrawArgs[groupName][objectName] = temp;
	}

	void RenderScene::createVertexBuffer(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
		const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
		const std::wstring& vbName, const void* data, UINT numBytes)
	{
		mVertexBuffers[vbName].createVertexBuffer(device, commandList, data, numBytes);
	}

	void RenderScene::createVertexBufferView(const std::wstring& vbName, UINT numBytes, UINT stride)
	{
		mVertexBuffers[vbName].createVertexBufferView(numBytes, stride);
	}

	void RenderScene::createIndexBuffer(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
		const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
		const std::wstring& ibName, const void* data, UINT numBytes)
	{
		mIndexBuffers[ibName].createIndexBuffer(device, commandList, data, numBytes);
	}

	void RenderScene::createIndexBufferView(const std::wstring& ibName, UINT numBytes, DXGI_FORMAT format)
	{
		mIndexBuffers[ibName].createIndexBufferView(numBytes, format);
	}

	void RenderScene::createCBVHeap(const Microsoft::WRL::ComPtr<ID3D12Device>& device, UINT numDescriptors, UINT shaderRegister)
	{
		mCBVSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		//Need a CBV for each object for each frame.
		D3D12_DESCRIPTOR_HEAP_DESC cbvDescriptorHeapDescription{};
		cbvDescriptorHeapDescription.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		cbvDescriptorHeapDescription.NumDescriptors = numDescriptors * numFrames;
		cbvDescriptorHeapDescription.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		cbvDescriptorHeapDescription.NodeMask = 0;
		ThrowIfFailed(device->CreateDescriptorHeap(&cbvDescriptorHeapDescription, IID_PPV_ARGS(&mCBVHeap)));

		//Describes the number of CBV's in the CB descriptor heap
		mCBVHeapDescription.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		mCBVHeapDescription.NumDescriptors = numDescriptors * numFrames; //num of descriptors in my cbv descriptor heap that are type cbv
		mCBVHeapDescription.BaseShaderRegister = shaderRegister; //which register does your vertex shader expect the buffer to be in
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

	void RenderScene::createConstantBuffer(const Microsoft::WRL::ComPtr<ID3D12Device>& device, const std::wstring& name,
		const UINT& numOfBytes)
	{
		for (UINT i = 0; i < numFrames; ++i)
		{
			mConstantBuffers[i][name].createConstantBuffer(device, numOfBytes);
		}
	}

	void RenderScene::createConstantBufferView(const Microsoft::WRL::ComPtr<ID3D12Device>& device,
		const std::wstring& name, UINT index, UINT numBytes)
	{
		//Create a constant buffer view for each frame.
		for (UINT i = 0; i < numFrames; ++i)
		{
			mConstantBuffers[i][name].createConstantBufferView(device, mCBVHeap, mCBVSize, (index * numFrames) + i,
				index, numBytes);
		}
	}

	void RenderScene::beforeDraw(DeviceResources& deviceResource)
	{
		deviceResource.draw();

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList{ deviceResource.commandList() };

		//Link the CBV descriptor heaps to the pipeline
		ID3D12DescriptorHeap* dH[] = { mCBVHeap.Get() };
		commandList->SetDescriptorHeaps(1, dH);

	}

	void RenderScene::drawObjects(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
		const std::wstring& drawArgsGroupName, const std::wstring& vbName, const std::wstring& ibName,
		const std::wstring& psoName, const std::wstring& rootSignatureName,
		const D3D_PRIMITIVE_TOPOLOGY& primitive)
	{
		commandList->IASetVertexBuffers(0, 1, &mVertexBuffers.at(vbName).vertexBufferView());

		commandList->IASetIndexBuffer(&mIndexBuffers.at(ibName).indexBufferView());

		commandList->SetPipelineState(mPSOs.at(psoName).Get());

		commandList->SetGraphicsRootSignature(mRootSignatures.at(rootSignatureName).Get());

		commandList->IASetPrimitiveTopology(primitive);

		//draw all the objects the share the same PSO, root signature and primitive
		for (const std::pair<std::wstring, FAShapes::DrawArguments>& i : mDrawArgs.at(drawArgsGroupName))
		{
			//Get the address of the first view in the constant buffer view heap
			CD3DX12_GPU_DESCRIPTOR_HANDLE handle =
				CD3DX12_GPU_DESCRIPTOR_HANDLE(mCBVHeap->GetGPUDescriptorHandleForHeapStart());

			handle.Offset((i.second.indexOfConstantData * numFrames) + currentFrame, mCBVSize);

			commandList->SetGraphicsRootDescriptorTable(0, handle);

			commandList->DrawIndexedInstanced(i.second.indexCount, 1,
				i.second.locationOfFirstIndex, i.second.indexOfFirstVertex, 0);
		}
	}

	void RenderScene::afterDraw(DeviceResources& deviceResource, Text* textToRender, UINT numText)
	{
		deviceResource.rtBufferTransition(textToRender);

		deviceResource.execute();

		deviceResource.textDraw(textToRender, numText);

		deviceResource.present();

		//Update the current fence value
		deviceResource.updateCurrentFrameFenceValue();

		//Add a fence instruction to the command queue.
		deviceResource.signal();
	}

	//-----------------------------------------------------------------------------------------------------------------------
}