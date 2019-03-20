#include <memory>
#include <map>
#include "GBufferSprite.h"
#include "../Resource/VertexBuffer.h"
#include "../Resource/ConstantBuffer.h"
#include "../DescriptorHeap/DescriptorHeap.h"
#include "../PipelineState/PipelineStateObject.h"
#include "../Resource/ShaderResource.h"
#include "../Resource/DepthStencilBuffer.h"
#include "../Window/Window.h"
#include "../Util/Math.h"
#include "../Util/Utility.h"
#include "../CommandContext/GraphicsContextLibrary.h"
#include "../GameConponent/Camera/Camera.h"
#include "../Resource/UnorderdAccessView.h"
#include "../CommandContext/GraphicsCommandList.h"
#include "../Signature/RootSignature.h"

void K3D12::GBufferSprite::BindingDepthStencill(K3D12::DepthStencil * depthStencil)
{
	this->_bindedDepthStencil = depthStencil;
}

void K3D12::GBufferSprite::Discard()
{
	_vertexBuffer.Discard();
	_geometryShaderHeap.Discard();
	_rtvHeap.Discard();
	_uavHeap.Discard();
	_bindedDepthStencil = nullptr;
	_cameraConstantBuffer.Discard();
	_geometryRenderTargets.clear();
	_geometryResource.clear();
	_list.reset();
}

K3D12::GBufferSprite::GBufferSprite()
{
}


K3D12::GBufferSprite::~GBufferSprite()
{
	Discard();
}

void K3D12::GBufferSprite::SetHeaps()
{
	ID3D12DescriptorHeap* heaps[] = { _geometryShaderHeap.GetPtr() };
	_list->GetCommandList().Get()->SetDescriptorHeaps(1, heaps);
}

K3D12::VertexBuffer & K3D12::GBufferSprite::GetVertexBuffer()
{
	return this->_vertexBuffer;
}

HRESULT K3D12::GBufferSprite::InitializeVertex()
{
	_vertex[0].normal = Vector3::back;
	_vertex[1].normal = Vector3::back;
	_vertex[2].normal = Vector3::back;
	_vertex[3].normal = Vector3::back;

	_vertex[0].pos = Vector3(-1.0f, 1.0f, 0.0f);
	_vertex[1].pos = Vector3(1.0f, 1.0f, 0.0f);
	_vertex[2].pos = Vector3(-1.0f, -1.0f, 0.0f);
	_vertex[3].pos = Vector3(1.0f, -1.0f, 0.0f);

	_vertex[0].texCoord = Vector2(0.0f, 0.0f);
	_vertex[1].texCoord = Vector2(1.0f, 0.0f);
	_vertex[2].texCoord = Vector2(0.0f, 1.0f);
	_vertex[3].texCoord = Vector2(1.0f, 1.0f);

	this->_vertexBuffer.Create(sizeof(_vertex), sizeof(Vertex3D), &_vertex[0]);
	_vertexBuffer.SetName("GBuffferVertex");
	this->_cameraInfo.projection = Matrix::CreateIdentity();
	this->_cameraInfo.view = Matrix::CreateLookTo({ 0.f,10.0f,-10.0f }, { 0.0f,0.0f,1.0f }, { 0.0f,1.0f,0.0f });

	auto hr = this->_cameraConstantBuffer.Create(Util::Alignment256Bytes(sizeof(CameraInfo)));
	_cameraConstantBuffer.Update(&_cameraInfo, sizeof(CameraInfo), 0);
	_cameraConstantBuffer.SetName("GBufferCameraInfo");
	if (FAILED(hr)) {
		return hr;
	}
	return S_OK;
}

HRESULT K3D12::GBufferSprite::InitializeResource(unsigned int spriteWidth, unsigned int spriteHeight)
{

	D3D12_HEAP_PROPERTIES props = {};
	props.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
	props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	props.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	props.CreationNodeMask = 0;
	props.VisibleNodeMask = 0;

	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resDesc.Width = spriteWidth;
	resDesc.Height = spriteHeight;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resDesc.SampleDesc.Count = 1;
	resDesc.SampleDesc.Quality = 0;
	resDesc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET | D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;


	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	clearValue.Color[0] = 0.0f;
	clearValue.Color[1] = 0.0f;
	clearValue.Color[2] = 0.0f;
	clearValue.Color[3] = 1.0f;



	auto hr = this->_geometryResource[GEOMETRY_TYPE::NORMAL].Create(props, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, resDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, &clearValue);
	if (FAILED(hr)) {
		return hr;
	}
	this->_geometryResource[GEOMETRY_TYPE::NORMAL].SetName("NORMAL_TEXTURE");
	clearValue.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;

	resDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;

	hr = this->_geometryResource[GEOMETRY_TYPE::SPECULAR].Create(props, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, resDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, &clearValue);
	if (FAILED(hr)) {
		return hr;
	}
	this->_geometryResource[GEOMETRY_TYPE::SPECULAR].SetName("SPECULAR_TEXTURE");

	hr = this->_geometryResource[GEOMETRY_TYPE::ALBEDO].Create(props, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, resDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, &clearValue);
	if (FAILED(hr)) {
		return hr;
	}
	this->_geometryResource[GEOMETRY_TYPE::ALBEDO].SetName("ALBEDO_TEXTURE");

	hr = this->_geometryResource[GEOMETRY_TYPE::SPECULER_MAP].Create(props, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, resDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, &clearValue);
	if (FAILED(hr)) {
		return hr;
	}
	this->_geometryResource[GEOMETRY_TYPE::SPECULER_MAP].SetName("SPECULER_MAP_TEXTURE");

	hr = this->_geometryResource[GEOMETRY_TYPE::DIFFUSE_MAP].Create(props, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, resDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, &clearValue);
	if (FAILED(hr)) {
		return hr;
	}
	this->_geometryResource[GEOMETRY_TYPE::DIFFUSE_MAP].SetName("DIFFUSE_MAP_TEXTURE");

	clearValue.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	resDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	hr = this->_geometryResource[GEOMETRY_TYPE::RESULT].Create(props, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, resDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, &clearValue);
	if (FAILED(hr)) {
		return hr;
	}	
	this->_geometryResource[GEOMETRY_TYPE::RESULT].SetName("RESULT_TEXTURE");
	hr = this->_geometryResource[GEOMETRY_TYPE::RESULT2].Create(props, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, resDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, &clearValue);
	if (FAILED(hr)) {
		return hr;
	}
	this->_geometryResource[GEOMETRY_TYPE::RESULT2].SetName("RESULT_TEXTURE2");

	return hr;
}

HRESULT K3D12::GBufferSprite::InitializeDescriptorHeap()
{

	D3D12_DESCRIPTOR_HEAP_DESC desc;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask = 0;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = static_cast<unsigned int>(GEOMETRY_TYPE::GEOMETRY_TYPE_MAX);

	//SRV用ヒープ
	auto hr = this->_geometryShaderHeap.Create(&desc);
	if (FAILED(hr)) {
		return hr;
	}
	_geometryShaderHeap.SetName("GeometrySRV");

	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

	//RTV用ヒープ
	hr = this->_rtvHeap.Create(&desc);
	if (FAILED(hr)) {
		return hr;
	}
	_rtvHeap.SetName("GeometryRTV");
	//uav用ヒープ
	//とりあえずバッファ内のすべてのリソースに対してUAVアクセス可能にしておく

	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NodeMask = 0;
	hr = this->_uavHeap.Create(&desc);
	if (FAILED(hr)) {
		return hr;
	}
	_uavHeap.SetName("GeometryUAV");

	return S_OK;
}

HRESULT K3D12::GBufferSprite::InitializeGeometryView(DepthStencil* depth)
{
	{

		D3D12_RENDER_TARGET_VIEW_DESC rtDesc = {};
		rtDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		rtDesc.ViewDimension = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2D;
		rtDesc.Texture2D.MipSlice = 0;
		rtDesc.Texture2D.PlaneSlice = 0;
		// テクスチャのレンダターゲットの作成
		_geometryResource.at(GEOMETRY_TYPE::NORMAL).CreateView(rtDesc, _rtvHeap.GetCPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::NORMAL)));
		rtDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		_geometryResource.at(GEOMETRY_TYPE::SPECULAR).CreateView(rtDesc, _rtvHeap.GetCPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::SPECULAR)));
		_geometryResource.at(GEOMETRY_TYPE::ALBEDO).CreateView(rtDesc, _rtvHeap.GetCPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::ALBEDO)));
		_geometryResource.at(GEOMETRY_TYPE::SPECULER_MAP).CreateView(rtDesc, _rtvHeap.GetCPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::SPECULER_MAP)));
		_geometryResource.at(GEOMETRY_TYPE::DIFFUSE_MAP).CreateView(rtDesc, _rtvHeap.GetCPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::DIFFUSE_MAP)));
		
		rtDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		_geometryResource.at(GEOMETRY_TYPE::RESULT).CreateView(rtDesc, _rtvHeap.GetCPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::RESULT)));
		_geometryResource.at(GEOMETRY_TYPE::RESULT2).CreateView(rtDesc, _rtvHeap.GetCPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::RESULT2)));


	}

	{
		D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
		uavDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
		uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
		{
			uavDesc.Texture2D.MipSlice = 0;
			uavDesc.Texture2D.PlaneSlice = 0;
		}
		_geometryResource.at(GEOMETRY_TYPE::NORMAL).CreateView(uavDesc, this->GetUAVCPUHandle(GEOMETRY_TYPE::NORMAL));
		_geometryResource.at(GEOMETRY_TYPE::SPECULAR).CreateView(uavDesc, this->GetUAVCPUHandle(GEOMETRY_TYPE::SPECULAR));
		_geometryResource.at(GEOMETRY_TYPE::ALBEDO).CreateView(uavDesc, this->GetUAVCPUHandle(GEOMETRY_TYPE::ALBEDO));
		_geometryResource.at(GEOMETRY_TYPE::SPECULER_MAP).CreateView(uavDesc, this->GetUAVCPUHandle(GEOMETRY_TYPE::SPECULER_MAP));
		_geometryResource.at(GEOMETRY_TYPE::DIFFUSE_MAP).CreateView(uavDesc, this->GetUAVCPUHandle(GEOMETRY_TYPE::DIFFUSE_MAP));
		_geometryResource.at(GEOMETRY_TYPE::RESULT).CreateView(uavDesc, this->GetUAVCPUHandle(GEOMETRY_TYPE::RESULT));
		_geometryResource.at(GEOMETRY_TYPE::RESULT).CreateView(uavDesc, this->GetUAVCPUHandle(GEOMETRY_TYPE::RESULT2));

	}
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC srv;
		srv.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		srv.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srv.Texture2D.MipLevels = 1;
		srv.Texture2D.MostDetailedMip = 0;
		srv.Texture2D.PlaneSlice = 0;
		srv.Texture2D.ResourceMinLODClamp = 0.0F;
		srv.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;


		_geometryResource.at(GEOMETRY_TYPE::NORMAL).CreateView(srv, _geometryShaderHeap.GetCPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::NORMAL)));
		srv.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		_geometryResource.at(GEOMETRY_TYPE::SPECULAR).CreateView(srv, _geometryShaderHeap.GetCPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::SPECULAR)));
		_geometryResource.at(GEOMETRY_TYPE::ALBEDO).CreateView(srv, _geometryShaderHeap.GetCPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::ALBEDO)));
		_geometryResource.at(GEOMETRY_TYPE::SPECULER_MAP).CreateView(srv, _geometryShaderHeap.GetCPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::SPECULER_MAP)));
		_geometryResource.at(GEOMETRY_TYPE::DIFFUSE_MAP).CreateView(srv, _geometryShaderHeap.GetCPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::DIFFUSE_MAP)));
		srv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		_geometryResource.at(GEOMETRY_TYPE::RESULT).CreateView(srv, _geometryShaderHeap.GetCPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::RESULT)));
		_geometryResource.at(GEOMETRY_TYPE::RESULT2).CreateView(srv, _geometryShaderHeap.GetCPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::RESULT2)));


		srv.Format = DXGI_FORMAT_R32_FLOAT;
		depth->CreateView(srv, _geometryShaderHeap.GetCPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::DEPTH)));
	}

	{
		_geometryRenderTargets.push_back(GetRTVCPUHandle(GEOMETRY_TYPE::NORMAL));
		_geometryRenderTargets.push_back(GetRTVCPUHandle(GEOMETRY_TYPE::SPECULAR));
		_geometryRenderTargets.push_back(GetRTVCPUHandle(GEOMETRY_TYPE::ALBEDO));

	}
	return S_OK;
}

HRESULT K3D12::GBufferSprite::CreateLightingPassPso()
{
	D3D12_COMPUTE_PIPELINE_STATE_DESC desc = {};
	Microsoft::WRL::ComPtr<ID3DBlob> computeShader;
	Microsoft::WRL::ComPtr<ID3DBlob> error = {};

#if defined(_DEBUG)
	//グラフィックデバッグツールによるシェーダーのデバッグの有効化処理
	UINT compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION 
		;

#else
	UINT compileFlag = 0;
#endif

	if (FAILED(D3DCompileFromFile(L"./Engine/Shader/LightingPassShader.hlsl", nullptr, nullptr, "CSMain", "cs_5_0", compileFlag, 0, &computeShader, &error))) {
		OutputDebugStringA((char*)error->GetBufferPointer());
		return E_FAIL;
	}
	if (error != nullptr) {
		OutputDebugStringA((char*)error->GetBufferPointer());
	}

	desc.CS.BytecodeLength = computeShader.Get()->GetBufferSize();
	desc.CS.pShaderBytecode = computeShader.Get()->GetBufferPointer();
	desc.Flags = D3D12_PIPELINE_STATE_FLAGS::D3D12_PIPELINE_STATE_FLAG_NONE;
	desc.NodeMask = 0;
	desc.pRootSignature = nullptr;

	auto result = GraphicsContextLibrary::GetInstance().CreatePSO("lightingPassPSO", desc, computeShader.Get());

	if (FAILED(result)) {
		return result;
	}
	computeShader.Reset();
	error.Reset();


	return S_OK;
}

HRESULT K3D12::GBufferSprite::CreateGBuffer(unsigned int spriteWidth, unsigned int spriteHeight, Window* window, std::shared_ptr<GraphicsCommandList> list, DepthStencil* depth)
{
	_list = list;
	_windowWidth = spriteWidth;
	_windowHeight = spriteHeight;
	_window = window;
	_bindedDepthStencil = depth;
	auto hr = InitializeResource(spriteWidth, spriteHeight);
	if (FAILED(hr)) {
		return hr;
	}
	hr = InitializeDescriptorHeap();
	if (FAILED(hr)) {
		return hr;
	}
	hr = InitializeGeometryView(depth);
	if (FAILED(hr)) {
		return hr;
	}
	hr = InitializeVertex();
	if (FAILED(hr)) {
		return hr;
	}
	hr = CreateLightingPassPso();
	if (FAILED(hr)) {
		return hr;
	}
	return hr;
}

D3D12_GPU_DESCRIPTOR_HANDLE K3D12::GBufferSprite::GetRTVGPUHandle(K3D12::GEOMETRY_TYPE type)
{
	switch (type)
	{
	case GEOMETRY_TYPE::NORMAL:
		return this->_rtvHeap.GetGPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::NORMAL));
		break;
	case GEOMETRY_TYPE::SPECULAR:
		return this->_rtvHeap.GetGPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::SPECULAR));

		break;
	case GEOMETRY_TYPE::ALBEDO:
		return this->_rtvHeap.GetGPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::ALBEDO));
		break;
	case GEOMETRY_TYPE::SPECULER_MAP:
		return this->_rtvHeap.GetGPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::SPECULER_MAP));
		break;
	case GEOMETRY_TYPE::DIFFUSE_MAP:
		return this->_rtvHeap.GetGPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::DIFFUSE_MAP));
		break;
	case GEOMETRY_TYPE::RESULT:
		return this->_rtvHeap.GetGPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::RESULT));
		break;
	case GEOMETRY_TYPE::RESULT2:
		return this->_rtvHeap.GetGPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::RESULT2));
		break;
	case GEOMETRY_TYPE::GEOMETRY_TYPE_MAX:
		break;
	default:
		break;
	}
	return D3D12_GPU_DESCRIPTOR_HANDLE();
}

D3D12_GPU_DESCRIPTOR_HANDLE K3D12::GBufferSprite::GetSRVGPUHandle(GEOMETRY_TYPE type)
{
	switch (type)
	{
	case GEOMETRY_TYPE::NORMAL:
		return this->_geometryShaderHeap.GetGPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::NORMAL));

		break;
	case GEOMETRY_TYPE::SPECULAR:
		return this->_geometryShaderHeap.GetGPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::SPECULAR));

		break;
	case GEOMETRY_TYPE::ALBEDO:
		return this->_geometryShaderHeap.GetGPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::ALBEDO));

		break;
	case GEOMETRY_TYPE::DEPTH:
		return this->_geometryShaderHeap.GetGPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::DEPTH));

		break;
	case GEOMETRY_TYPE::SPECULER_MAP:
		return this->_geometryShaderHeap.GetGPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::SPECULER_MAP));

		break;
	case GEOMETRY_TYPE::DIFFUSE_MAP:
		return this->_geometryShaderHeap.GetGPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::DIFFUSE_MAP));
		break;
	case GEOMETRY_TYPE::RESULT:
		return this->_geometryShaderHeap.GetGPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::RESULT));
		break;
	case GEOMETRY_TYPE::RESULT2:
		return this->_geometryShaderHeap.GetGPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::RESULT2));
		break;
	case GEOMETRY_TYPE::GEOMETRY_TYPE_MAX:
		break;
	default:
		break;
	}
	return D3D12_GPU_DESCRIPTOR_HANDLE();

}

D3D12_GPU_DESCRIPTOR_HANDLE K3D12::GBufferSprite::GetUAVGPUHandle(GEOMETRY_TYPE type)
{
	switch (type)
	{
	case GEOMETRY_TYPE::NORMAL:
		return this->_uavHeap.GetGPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::NORMAL));
		break;
	case GEOMETRY_TYPE::SPECULAR:
		return this->_uavHeap.GetGPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::SPECULAR));
		break;
	case GEOMETRY_TYPE::ALBEDO:
		return this->_uavHeap.GetGPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::ALBEDO));
		break;
	case GEOMETRY_TYPE::SPECULER_MAP:
		return this->_uavHeap.GetGPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::SPECULER_MAP));
		break;
	case GEOMETRY_TYPE::DIFFUSE_MAP:
		return this->_uavHeap.GetGPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::DIFFUSE_MAP));
		break;
	case GEOMETRY_TYPE::RESULT:
		return this->_uavHeap.GetGPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::RESULT));
		break;
	case GEOMETRY_TYPE::RESULT2:
		return this->_uavHeap.GetGPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::RESULT2));
		break;
	case GEOMETRY_TYPE::GEOMETRY_TYPE_MAX:
		break;
	default:
		break;
	}
	return D3D12_GPU_DESCRIPTOR_HANDLE();

}

D3D12_CPU_DESCRIPTOR_HANDLE K3D12::GBufferSprite::GetRTVCPUHandle(GEOMETRY_TYPE type)
{
	switch (type)
	{
	case GEOMETRY_TYPE::NORMAL:
		return	this->_rtvHeap.GetCPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::NORMAL));
		break;
	case GEOMETRY_TYPE::SPECULAR:
		return	this->_rtvHeap.GetCPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::SPECULAR));
		break;
	case GEOMETRY_TYPE::ALBEDO:
		return	this->_rtvHeap.GetCPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::ALBEDO));
		break;
	case GEOMETRY_TYPE::SPECULER_MAP:
		return	this->_rtvHeap.GetCPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::SPECULER_MAP));
		break;
	case GEOMETRY_TYPE::DIFFUSE_MAP:
		return	this->_rtvHeap.GetCPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::DIFFUSE_MAP));
		break;
	case GEOMETRY_TYPE::RESULT:
		return	this->_rtvHeap.GetCPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::RESULT));
		break;
	case GEOMETRY_TYPE::RESULT2:
		return this->_rtvHeap.GetCPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::RESULT2));
		break;
	case GEOMETRY_TYPE::GEOMETRY_TYPE_MAX:
		break;
	default:
		break;
	}
	return D3D12_CPU_DESCRIPTOR_HANDLE();

}

D3D12_CPU_DESCRIPTOR_HANDLE K3D12::GBufferSprite::GetSRVCPUHandle(GEOMETRY_TYPE type)
{
	switch (type)
	{
	case GEOMETRY_TYPE::NORMAL:
		return	this->_geometryShaderHeap.GetCPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::NORMAL));
		break;
	case GEOMETRY_TYPE::SPECULAR:
		return	this->_geometryShaderHeap.GetCPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::SPECULAR));
		break;
	case GEOMETRY_TYPE::ALBEDO:
		return	this->_geometryShaderHeap.GetCPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::ALBEDO));
		break;
	case GEOMETRY_TYPE::DEPTH:
		return this->_geometryShaderHeap.GetCPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::DEPTH));
		break;
	case GEOMETRY_TYPE::SPECULER_MAP:
		return	this->_geometryShaderHeap.GetCPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::SPECULER_MAP));
		break;
	case GEOMETRY_TYPE::DIFFUSE_MAP:
		return	this->_geometryShaderHeap.GetCPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::DIFFUSE_MAP));
		break;
	case GEOMETRY_TYPE::RESULT:
		return	this->_geometryShaderHeap.GetCPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::RESULT));
		break;
	case GEOMETRY_TYPE::RESULT2:
		return this->_geometryShaderHeap.GetCPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::RESULT2));
		break;
	case GEOMETRY_TYPE::GEOMETRY_TYPE_MAX:
		break;
	default:
		break;
	}
	return D3D12_CPU_DESCRIPTOR_HANDLE();
}

D3D12_CPU_DESCRIPTOR_HANDLE K3D12::GBufferSprite::GetUAVCPUHandle(GEOMETRY_TYPE type)
{
	switch (type)
	{
	case GEOMETRY_TYPE::NORMAL:
		return this->_uavHeap.GetCPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::NORMAL));
		break;
	case GEOMETRY_TYPE::SPECULAR:
		return this->_uavHeap.GetCPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::SPECULAR));
		break;
	case GEOMETRY_TYPE::ALBEDO:
		return this->_uavHeap.GetCPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::ALBEDO));
		break;
	case GEOMETRY_TYPE::SPECULER_MAP:
		return this->_uavHeap.GetCPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::SPECULER_MAP));
		break;
	case GEOMETRY_TYPE::DIFFUSE_MAP:
		return this->_uavHeap.GetCPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::DIFFUSE_MAP));
		break;
	case GEOMETRY_TYPE::RESULT:
		return this->_uavHeap.GetCPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::RESULT));
		break;
	case GEOMETRY_TYPE::RESULT2:
		return this->_uavHeap.GetCPUHandle(static_cast<unsigned int>(GEOMETRY_TYPE::RESULT2));
		break;
	case GEOMETRY_TYPE::GEOMETRY_TYPE_MAX:
		break;
	default:
		break;
	}
	return D3D12_CPU_DESCRIPTOR_HANDLE();
}

K3D12::Resource * K3D12::GBufferSprite::GetGBufferResource(K3D12::GEOMETRY_TYPE type)
{
	return &this->_geometryResource[type];
}

void K3D12::GBufferSprite::SetGeometryRenderTarget(K3D12::DepthStencil * depth)
{

	_list->GetCommandList().Get()->RSSetViewports(1, &_window->GetViewPort());
	_list->GetCommandList().Get()->RSSetScissorRects(1, &_window->GetScissorRect());

	if (depth == nullptr) {
		_list->GetCommandList().Get()->OMSetRenderTargets(static_cast<unsigned int>(_geometryRenderTargets.size()), &_geometryRenderTargets[0], FALSE, &this->_bindedDepthStencil->GetDSVHeapPtr()->GetCPUHandle(0));
		return;
	}
	_list->GetCommandList().Get()->OMSetRenderTargets(static_cast<unsigned int>(_geometryRenderTargets.size()), &_geometryRenderTargets[0], FALSE, &depth->GetDSVHeapPtr()->GetCPUHandle(0));
}

void K3D12::GBufferSprite::SetCameraInfo(K3D12::CameraInfo & cameraInfo)
{
	_cameraInfo = cameraInfo;
	_cameraConstantBuffer.Update(&_cameraInfo, sizeof(CameraInfo), 0);
}

void K3D12::GBufferSprite::SetGeometryResultTexture(K3D12::DepthStencil * depth)
{

	ID3D12DescriptorHeap* heaps[] = { _geometryShaderHeap.GetPtr() };
	_list->GetCommandList().Get()->SetDescriptorHeaps(1, heaps);

	_list->GetCommandList().Get()->SetComputeRootDescriptorTable(static_cast<unsigned int>(SHADER_PARAMATER_INDEX_LIGHTING::NORMAL_MAP), GetSRVGPUHandle(GEOMETRY_TYPE::NORMAL));
	_list->GetCommandList().Get()->SetComputeRootDescriptorTable(static_cast<unsigned int>(SHADER_PARAMATER_INDEX_LIGHTING::SPECULAR_POWER_MAP), GetSRVGPUHandle(GEOMETRY_TYPE::SPECULAR));
	_list->GetCommandList().Get()->SetComputeRootDescriptorTable(static_cast<unsigned int>(SHADER_PARAMATER_INDEX_LIGHTING::ABLEDO_MAP), GetSRVGPUHandle(GEOMETRY_TYPE::ALBEDO));
	_list->GetCommandList().Get()->SetComputeRootDescriptorTable(static_cast<unsigned int>(SHADER_PARAMATER_INDEX_LIGHTING::DEPTH_MAP), GetSRVGPUHandle(GEOMETRY_TYPE::DEPTH));
}

void K3D12::GBufferSprite::SetLightingRenderUAV()
{
	ID3D12DescriptorHeap* heaps[] = { _uavHeap.GetPtr() };
	_list->GetCommandList().Get()->SetDescriptorHeaps(1, heaps);

	/*_list->GetCommandList().Get()->SetComputeRootDescriptorTable(static_cast<unsigned int>(SHADER_PARAMATER_INDEX_LIGHTING::DIFFUSE_MAP), GetUAVGPUHandle(GEOMETRY_TYPE::DIFFUSE_MAP));
	_list->GetCommandList().Get()->SetComputeRootDescriptorTable(static_cast<unsigned int>(SHADER_PARAMATER_INDEX_LIGHTING::SPECULAR_MAP), GetUAVGPUHandle(GEOMETRY_TYPE::SPECULER_MAP));*/
	_list->GetCommandList().Get()->SetComputeRootDescriptorTable(static_cast<unsigned int>(SHADER_PARAMATER_INDEX_LIGHTING::RESULT), GetUAVGPUHandle(GEOMETRY_TYPE::RESULT));
}

void K3D12::GBufferSprite::SetLightStructuredBuffer(UnorderedAccessValue * lightUav)
{
	ID3D12DescriptorHeap* heap[] = { lightUav->GetHeap()->GetPtr() };

	_list->GetCommandList().Get()->SetDescriptorHeaps(1, heap);
	_list->GetCommandList().Get()->SetComputeRootDescriptorTable(static_cast<unsigned int>(SHADER_PARAMATER_INDEX_LIGHTING::LIGHT_BUFFER), lightUav->GetSRVGPUHandle());
}

D3D12_CPU_DESCRIPTOR_HANDLE K3D12::GBufferSprite::GetResultTextureCPUHandle()
{
	return GetSRVCPUHandle(GEOMETRY_TYPE::RESULT);
}

void K3D12::GBufferSprite::SetLightingResult()
{

	ID3D12DescriptorHeap* heaps[] = { _geometryShaderHeap.GetPtr() };
	_list->GetCommandList().Get()->SetDescriptorHeaps(1, heaps);

	_list->GetCommandList().Get()->SetComputeRootDescriptorTable(static_cast<unsigned int>(SHADER_PARAMATER_INDEX_RESULT::DIFFUSE_MAP), GetSRVGPUHandle(GEOMETRY_TYPE::DIFFUSE_MAP));
	_list->GetCommandList().Get()->SetComputeRootDescriptorTable(static_cast<unsigned int>(SHADER_PARAMATER_INDEX_RESULT::ALBEDO_MAP), GetSRVGPUHandle(GEOMETRY_TYPE::ALBEDO));
	_list->GetCommandList().Get()->SetComputeRootDescriptorTable(static_cast<unsigned int>(SHADER_PARAMATER_INDEX_RESULT::SPECULAR_MAP), GetSRVGPUHandle(GEOMETRY_TYPE::SPECULER_MAP));

}

void K3D12::GBufferSprite::StartLightingPass(K3D12::UnorderedAccessValue* lightsBuffer,DepthStencil * depthResource)
{
	//シェーダーセット
	_list->GetCommandList().Get()->SetPipelineState(GraphicsContextLibrary::GetInstance().GetPSO("lightingPassPSO")->GetPSO().Get());
	_list->GetCommandList().Get()->SetComputeRootSignature(GraphicsContextLibrary::GetInstance().GetRootSignature("lightingPassPSO")->GetSignature().Get());
	//

	//カメラのセット
	_list->GetCommandList().Get()->SetComputeRootConstantBufferView(static_cast<unsigned int>(SHADER_PARAMATER_INDEX_LIGHTING::CAMERA_INFO), _cameraConstantBuffer.GetResource()->GetGPUVirtualAddress());
	//

	//SRVのセット
	this->SetGeometryResultTexture(depthResource);
	//

	//ライトのセット
	SetLightStructuredBuffer(lightsBuffer);
	//

	//UAVのセット
	this->SetLightingRenderUAV();
	//

	//6次元構造を作りそこにスレッドを立てる 16Pix*16Pixで
	_list->GetCommandList()->Dispatch(80, 45, 1);
	//SystemLogger::GetInstance().Log(LogLevel::Warning, "DisPatch\n");

}

void K3D12::GBufferSprite::TransitionGeometryPassStart()
{
	_geometryResource[GEOMETRY_TYPE::NORMAL].ResourceTransition(_list->GetCommandList().Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET);
	_geometryResource[GEOMETRY_TYPE::SPECULAR].ResourceTransition(_list->GetCommandList().Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET);
	_geometryResource[GEOMETRY_TYPE::ALBEDO].ResourceTransition(_list->GetCommandList().Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET);
	_list->GetCommandList().Get()->ClearRenderTargetView(GetRTVCPUHandle(GEOMETRY_TYPE::NORMAL), _geometryResource[GEOMETRY_TYPE::NORMAL].GetClearValue().Color, 0, nullptr);
	_list->GetCommandList().Get()->ClearRenderTargetView(GetRTVCPUHandle(GEOMETRY_TYPE::SPECULAR), _geometryResource[GEOMETRY_TYPE::SPECULAR].GetClearValue().Color, 0, nullptr);
	_list->GetCommandList().Get()->ClearRenderTargetView(GetRTVCPUHandle(GEOMETRY_TYPE::ALBEDO), _geometryResource[GEOMETRY_TYPE::ALBEDO].GetClearValue().Color, 0, nullptr);

}

void K3D12::GBufferSprite::TransitionGeometryPassEnd()
{
	_geometryResource[GEOMETRY_TYPE::NORMAL].ResourceTransition(_list->GetCommandList().Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
	_geometryResource[GEOMETRY_TYPE::SPECULAR].ResourceTransition(_list->GetCommandList().Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
	_geometryResource[GEOMETRY_TYPE::ALBEDO].ResourceTransition(_list->GetCommandList().Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
}

void K3D12::GBufferSprite::TransitionLightingPassStart(DepthStencil* depthResource)
{
	_geometryResource[GEOMETRY_TYPE::NORMAL].ResourceTransition(_list->GetCommandList().Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
	_geometryResource[GEOMETRY_TYPE::SPECULAR].ResourceTransition(_list->GetCommandList().Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
	_geometryResource[GEOMETRY_TYPE::ALBEDO].ResourceTransition(_list->GetCommandList().Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
	if (depthResource != nullptr) {
		depthResource->ResourceTransition(_list->GetCommandList().Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
	}
	else {
		this->_bindedDepthStencil->ResourceTransition(_list->GetCommandList().Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
	}
	//このテクスチャをUAVにする
	//_geometryResource[GEOMETRY_TYPE::SPECULER_MAP].ResourceTransition(_list->GetCommandList().Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	//_geometryResource[GEOMETRY_TYPE::DIFFUSE_MAP].ResourceTransition(_list->GetCommandList().Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	_geometryResource[GEOMETRY_TYPE::RESULT].ResourceTransition(_list->GetCommandList().Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	//書き込まれる前にクリアしておく
}

//ユーザー側でこれらのリソースがどう使われるかは未知数なので非常に危険だがGenericRreadに設定する。（後でそれぞれのステートに流れができるように変更）
void K3D12::GBufferSprite::TransitionLightingPassEnd(DepthStencil* depthResource)
{
	_geometryResource[GEOMETRY_TYPE::NORMAL].ResourceTransition(_list->GetCommandList().Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
	_geometryResource[GEOMETRY_TYPE::SPECULAR].ResourceTransition(_list->GetCommandList().Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
	_geometryResource[GEOMETRY_TYPE::ALBEDO].ResourceTransition(_list->GetCommandList().Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
	if (depthResource != nullptr) {
		depthResource->ResourceTransition(_list->GetCommandList().Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_DEPTH_WRITE);
	}
	else {
		this->_bindedDepthStencil->ResourceTransition(_list->GetCommandList().Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_DEPTH_WRITE);
	}
	//_geometryResource[GEOMETRY_TYPE::SPECULER_MAP].ResourceTransition(_list->GetCommandList().Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
	//_geometryResource[GEOMETRY_TYPE::DIFFUSE_MAP].ResourceTransition(_list->GetCommandList().Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
	_geometryResource[GEOMETRY_TYPE::RESULT].ResourceTransition(_list->GetCommandList().Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);

}

void K3D12::GBufferSprite::TransitionResultTextureToSR()
{
	_geometryResource[GEOMETRY_TYPE::RESULT].ResourceTransition(_list->GetCommandList().Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}

void K3D12::GBufferSprite::TransitionResultTextureToUAV()
{
	_geometryResource[GEOMETRY_TYPE::RESULT].ResourceTransition(_list->GetCommandList().Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
}

void K3D12::GBufferSprite::TransitionResultTextureToCopySource()
{
	_geometryResource[GEOMETRY_TYPE::RESULT].ResourceTransition(_list->GetCommandList().Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE);

}

void K3D12::GBufferSprite::TransitionResultTextureToGeneric()
{
	_geometryResource[GEOMETRY_TYPE::RESULT].ResourceTransition(_list->GetCommandList().Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
}

void K3D12::GBufferSprite::TransitionResultTextureToRT()
{
	_geometryResource[GEOMETRY_TYPE::RESULT].ResourceTransition(_list->GetCommandList().Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET);

}

void K3D12::GBufferSprite::TransitionResultTexture2ToSR()
{
	_geometryResource[GEOMETRY_TYPE::RESULT2].ResourceTransition(_list->GetCommandList().Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}

void K3D12::GBufferSprite::TransitionResultTexture2ToUAV()
{
	_geometryResource[GEOMETRY_TYPE::RESULT2].ResourceTransition(_list->GetCommandList().Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
}

void K3D12::GBufferSprite::TransitionResultTexture2ToCopySource()
{
	_geometryResource[GEOMETRY_TYPE::RESULT2].ResourceTransition(_list->GetCommandList().Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE);

}

void K3D12::GBufferSprite::TransitionResultTexture2ToRT()
{
	_geometryResource[GEOMETRY_TYPE::RESULT2].ResourceTransition(_list->GetCommandList().Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET);

}

void K3D12::GBufferSprite::TransitionResultTexture2ToGeneric()
{
	_geometryResource[GEOMETRY_TYPE::RESULT2].ResourceTransition(_list->GetCommandList().Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
}

void K3D12::GBufferSprite::BindingCommandList(std::shared_ptr<K3D12::GraphicsCommandList> newCommandList)
{
	this->_list.reset();
	this->_list = newCommandList;
}