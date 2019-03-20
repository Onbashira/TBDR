#include "IndexBuffer.h"
#include "../System/D3D12System.h"
#include <assert.h>

enum class IndexListElementType {
	CHAR = 1,
	SHORT = 2,
	UINT = 4,
};

HRESULT K3D12::IndexBuffer::Create(ULONG64 size, DXGI_FORMAT format, const void * pIndices)
{
	if (pIndices == nullptr || size == 0)
	{
		return E_FAIL;
	}

	_size = size;

	D3D12_HEAP_PROPERTIES props = {
		D3D12_HEAP_TYPE_UPLOAD,
		D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
		D3D12_MEMORY_POOL_UNKNOWN,
		1,
		1
	};

	D3D12_RESOURCE_DESC desc = {
		D3D12_RESOURCE_DIMENSION_BUFFER,
		0,
		_size,
		1,
		1,
		1,
		DXGI_FORMAT_UNKNOWN,
		{ 1, 0 },
		D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
		D3D12_RESOURCE_FLAG_NONE
	};

	auto hr = GET_DEVICE->CreateCommittedResource(
		&props,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_resource));

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	UCHAR* pDst = {};
	hr = Map(0, nullptr);

	if (FAILED(hr))
	{
		return E_FAIL;
	}
	Update(pIndices, static_cast<size_t>(_size), 0);
	Unmap(0, nullptr);

	_view.BufferLocation = GetResource()->GetGPUVirtualAddress();
	_view.SizeInBytes = static_cast<UINT>(_size);
	_view.Format = format;

	return S_OK;
}

HRESULT K3D12::IndexBuffer::Create(ULONG64 size, size_t elementSize, const void * pIndices)
{
	DXGI_FORMAT format;
	switch (static_cast<IndexListElementType>(elementSize))
	{
	case IndexListElementType::CHAR:
		format = DXGI_FORMAT::DXGI_FORMAT_R8_UINT;
		break;
	case IndexListElementType::SHORT:
		format = DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
		break;
	case IndexListElementType::UINT:
		format = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
		break;
	default:
		format = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
		break;
	}
	auto hr = Create(size, format, pIndices);
	CHECK_RESULT(hr);
	return hr;
}

void K3D12::IndexBuffer::Discard()
{

	_view.BufferLocation = 0;
	_view.SizeInBytes = 0;
	_view.Format = DXGI_FORMAT_UNKNOWN;
	Resource::Discard();
}

D3D12_INDEX_BUFFER_VIEW K3D12::IndexBuffer::GetView() const
{
	return _view;
}

K3D12::IndexBuffer::IndexBuffer()
{
	_view.BufferLocation = 0;
	_view.SizeInBytes = 0;
	_view.Format = DXGI_FORMAT_UNKNOWN;
}


K3D12::IndexBuffer::~IndexBuffer()
{
	Discard();
}
