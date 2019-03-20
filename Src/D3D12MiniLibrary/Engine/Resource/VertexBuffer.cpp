#include "VertexBuffer.h"
#include "../System/D3D12System.h"


K3D12::VertexBuffer::VertexBuffer() :
	_view({}), _size(0), _stride(0)
{
	_view.BufferLocation = 0;
	_view.SizeInBytes = 0;
	_view.StrideInBytes = 0;
}


K3D12::VertexBuffer::~VertexBuffer()
{
	Discard();
}

HRESULT K3D12::VertexBuffer::Create(ULONG64 size, UINT stride, const void * pVertices)
{
	if (pVertices == nullptr || size == 0 || stride == 0) {
		return E_FAIL;
	}
	_size = size;
	_stride = stride;

	D3D12_HEAP_PROPERTIES prop = {};
	prop.Type = D3D12_HEAP_TYPE_UPLOAD;
	prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	prop.CreationNodeMask = 1;
	prop.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Alignment = 0;
	desc.Width = _size;
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


	hr = Map(0, nullptr);
	if (FAILED(hr)) {
		return E_FAIL;
	}
	Update(pVertices, size_t(_size), 0);
	Unmap(0, nullptr);

	_view.BufferLocation = GetResource()->GetGPUVirtualAddress();
	_view.SizeInBytes = static_cast<UINT>(_size);
	_view.StrideInBytes = _stride;


	return S_OK;

}

void K3D12::VertexBuffer::Discard()
{
	_view.BufferLocation = 0;
	_view.SizeInBytes = 0;
	_view.StrideInBytes = 0;
	Resource::Discard();

}

D3D12_VERTEX_BUFFER_VIEW K3D12::VertexBuffer::GetView() const
{
	return this->_view;
}


