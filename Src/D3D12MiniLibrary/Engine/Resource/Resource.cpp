#include <string>
#include <tuple>
#include "Resource.h"
#include "../Util/Utility.h"
#include "../System/D3D12System.h"
#include "../Util/Logger.h"


K3D12::Resource::Resource() :
	_resource(), _pDst(nullptr),
	_clearValue({}), _currentResourceState(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON)
	, _name("UnNamedResource ")
{
}


K3D12::Resource::~Resource()
{
	Discard();
}


HRESULT K3D12::Resource::Create(D3D12_HEAP_PROPERTIES heapProps, D3D12_HEAP_FLAGS flags, D3D12_RESOURCE_DESC resourceDesc, D3D12_RESOURCE_STATES state, D3D12_CLEAR_VALUE* clearValue)
{

	_currentResourceState = state;
	if (clearValue != nullptr) {
		_clearValue = *clearValue;
	}

	if (_resource.Get() != nullptr) {
		Discard();
	}

	auto hr = GET_DEVICE->CreateCommittedResource(&heapProps, flags, &resourceDesc, state, clearValue, IID_PPV_ARGS(&this->_resource));

	if (FAILED(hr)) {
		SystemLogger::GetInstance().Log(LogLevel::Error, hr);

		return E_FAIL;
	}
	return S_OK;
}

HRESULT K3D12::Resource::Map(UINT subResource, D3D12_RANGE * readRange)
{
	auto hr = _resource->Map(subResource, readRange, reinterpret_cast<void**>(&_pDst));
	if (FAILED(hr)) {
		return E_FAIL;
	}
	return S_OK;
}

void K3D12::Resource::Unmap(UINT subResource, D3D12_RANGE * writtenRange)
{
	_resource->Unmap(subResource, writtenRange);
	_pDst = nullptr;
}

UCHAR* K3D12::Resource::GetMappedPointer()
{
	return _pDst;
}

void K3D12::Resource::Discard()
{

	//インターフェースへのデタッチを行う。これはポインタの参照カウントのデクリメントを行う操作のはず
	if (_resource.Get() != nullptr) {
		if (_pDst != nullptr) {
			Unmap(0, nullptr);
		}
		_resource.Reset();
		SystemLogger::GetInstance().Log(LogLevel::Debug, _name + " is  Reset \n");
	}
}

void K3D12::Resource::Update(const void * pSrc, ULONG64 size, const UINT dstOffset)
{
	assert(pSrc != nullptr);
	assert(size != 0);
	memcpy(_pDst + dstOffset, pSrc, size_t(size));
}

void K3D12::Resource::Update(const void * pSrc, ULONG64 size, const UINT dstOffset, UINT subResource, D3D12_RANGE * readRange, D3D12_RANGE * writtenRange)
{
	Map(subResource, readRange);
	Update(pSrc, size, dstOffset);
	Unmap(subResource, writtenRange);
}

void K3D12::Resource::Alignment256ByteUpdate(const void * pSrc, unsigned int elementSize, unsigned int elementCount)
{
	UINT offset = 0;
	for (unsigned int i = 0; i < elementCount; i++) {
		Update(static_cast<void*>((char*)(pSrc)+i * elementSize), Util::Alignment256Bytes(elementSize), offset);
		offset += static_cast<unsigned int>(Util::Alignment256Bytes(elementSize));

	}
}

ID3D12Resource * K3D12::Resource::GetResource()
{
	return _resource.Get();
}

ID3D12Resource ** K3D12::Resource::GetAddressOf()
{
	return _resource.GetAddressOf();
}

const std::tuple<D3D12_HEAP_PROPERTIES, D3D12_HEAP_FLAGS> K3D12::Resource::GetHeapPropaties()
{
	D3D12_HEAP_PROPERTIES props;
	D3D12_HEAP_FLAGS flags;
	_resource->GetHeapProperties(&props, &flags);
	return { props, flags };
}

const D3D12_RESOURCE_DESC* K3D12::Resource::GetResourceDesc()
{
	return &_resource->GetDesc();
}

const D3D12_RESOURCE_STATES & K3D12::Resource::GetResourceState()
{
	return   _currentResourceState;
}

void K3D12::Resource::SetResourceState(D3D12_RESOURCE_STATES state)
{
	_currentResourceState = state;
}

HRESULT K3D12::Resource::ResourceTransition(ID3D12GraphicsCommandList * list, D3D12_RESOURCE_STATES nextState)
{
	D3D12_RESOURCE_BARRIER resource_barrier{};

	resource_barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	resource_barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	resource_barrier.Transition.pResource = this->_resource.Get();
	resource_barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	resource_barrier.Transition.StateBefore = _currentResourceState;
	resource_barrier.Transition.StateAfter = nextState;
	_currentResourceState = nextState;
	list->ResourceBarrier(1, &resource_barrier);

	return S_OK;

}

D3D12_CLEAR_VALUE K3D12::Resource::GetClearValue()
{
	return _clearValue;
}

void K3D12::Resource::SetName(std::string name)
{
	_name = name;
	_resource->SetName(Util::StringToWString(_name).c_str());
}


