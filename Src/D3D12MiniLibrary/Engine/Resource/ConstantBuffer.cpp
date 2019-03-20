#include "ConstantBuffer.h"
#include "../System/D3D12System.h"


K3D12::ConstantBuffer::ConstantBuffer()
{

}


K3D12::ConstantBuffer::~ConstantBuffer()
{
	Resource::Discard();
}


HRESULT K3D12::ConstantBuffer::Create( ULONG64 size)
{
	if (size == 0) {
		return E_FAIL;
	}
	if ((size % 256) != 0) {
		return E_FAIL;
	}
	if (_resource.Get() != nullptr) {
		Resource::Discard();
	}

	D3D12_HEAP_PROPERTIES prop = {};
	prop.Type = D3D12_HEAP_TYPE_UPLOAD;
	prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	prop.CreationNodeMask = 1;
	prop.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Alignment = 0;
	desc.Width = size;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.Flags = D3D12_RESOURCE_FLAG_NONE;

	auto hr = GET_DEVICE->CreateCommittedResource(&prop, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,IID_PPV_ARGS(GetAddressOf()));

	if (FAILED(hr)) {
		return E_FAIL;
	}

	//システム終了までUnmapしない（しなくていいらしい）

	hr = Map(0, nullptr);
	if (FAILED(hr)) {
		return E_FAIL;
	}
	return S_OK;
}

HRESULT K3D12::ConstantBuffer::CreateView(D3D12_CONSTANT_BUFFER_VIEW_DESC& cbvDesc, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle)
{
	GET_DEVICE->CreateConstantBufferView(&cbvDesc, cpuDescriptorHandle);
	return S_OK;
}
