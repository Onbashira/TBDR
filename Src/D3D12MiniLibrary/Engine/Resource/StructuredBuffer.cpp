#include "StructuredBuffer.h"
#include "Resource.h"
#include "../System/D3D12System.h"
#include "../Util/Utility.h"



enum StructuredBufferLocation {
	LOCATION_UAV = 0,
	LOCATION_CBV,
	LOCATION_SRV,
	LOCATION_MAX
};

K3D12::StructuredBuffer::StructuredBuffer()
{
}


K3D12::StructuredBuffer::~StructuredBuffer()
{
}


HRESULT K3D12::StructuredBuffer::Create(unsigned int elementSize, unsigned int numElements, ID3D12Resource* counterResource)
{
	if (numElements == 0 || elementSize <= 0) {
		return E_FAIL;
	}

	_uavBuffer.FirstElement = 0;
	_uavBuffer.NumElements = numElements;
	_uavBuffer.StructureByteStride = elementSize;
	_uavBuffer.CounterOffsetInBytes = 0;
	_uavBuffer.Flags = D3D12_BUFFER_UAV_FLAGS::D3D12_BUFFER_UAV_FLAG_NONE;


	_elementNum = numElements;
	_elementSize = elementSize;

	unsigned int alignmentSize = static_cast<unsigned int >(Util::Alignment256Bytes(static_cast<unsigned int>(elementSize * numElements)));

	D3D12_HEAP_PROPERTIES prop = {};
	prop.Type = D3D12_HEAP_TYPE_UPLOAD;
	prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	prop.CreationNodeMask = 1;
	prop.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Alignment = 0;
	desc.Width = alignmentSize;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.Flags = D3D12_RESOURCE_FLAG_NONE;

	auto hr = GET_DEVICE->CreateCommittedResource(&prop, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(GetAddressOf()));

	if (FAILED(hr)) {
		return E_FAIL;
	}

	//システム終了までUnmapしない（永続マップ）

	hr = Map(0, nullptr);
	if (FAILED(hr)) {
		return E_FAIL;
	}


	return S_OK;
}

void K3D12::StructuredBuffer::CreateView(D3D12_SHADER_RESOURCE_VIEW_DESC & srv)
{
	GET_DEVICE->CreateShaderResourceView(this->GetResource(), &srv, this->_heap.GetCPUHandle(StructuredBufferLocation::LOCATION_SRV));
}

void K3D12::StructuredBuffer::CreateView(D3D12_UNORDERED_ACCESS_VIEW_DESC & uav, ID3D12Resource* counterResource)
{
	GET_DEVICE->CreateUnorderedAccessView(this->GetResource(), counterResource, &uav, this->_heap.GetCPUHandle(StructuredBufferLocation::LOCATION_UAV));
}

D3D12_CPU_DESCRIPTOR_HANDLE K3D12::StructuredBuffer::GetUAVCPUHandle()
{
	return this->_heap.GetCPUHandle(StructuredBufferLocation::LOCATION_UAV);
}

D3D12_CPU_DESCRIPTOR_HANDLE K3D12::StructuredBuffer::GetSRVCPUHandle()
{
	return this->_heap.GetCPUHandle(StructuredBufferLocation::LOCATION_SRV);
}

D3D12_GPU_DESCRIPTOR_HANDLE K3D12::StructuredBuffer::GetUAVGPUHandle()
{
	return this->_heap.GetGPUHandle(StructuredBufferLocation::LOCATION_UAV);
}

D3D12_GPU_DESCRIPTOR_HANDLE K3D12::StructuredBuffer::GetSRVGPUHandle()
{
	return this->_heap.GetGPUHandle(StructuredBufferLocation::LOCATION_SRV);
}

unsigned int K3D12::StructuredBuffer::GetElementSize()
{
	return _elementSize;
}

unsigned int K3D12::StructuredBuffer::GetElementNum()
{
	return _elementNum;
}

