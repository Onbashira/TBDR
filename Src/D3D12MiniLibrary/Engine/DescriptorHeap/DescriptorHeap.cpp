#include <string>
#include "DescriptorHeap.h"
#include "../System/D3D12System.h"
#include "../Util/Utility.h"
#include "../Util/Logger.h"

K3D12::DescriptorHeap::DescriptorHeap() :
	_heap(), _incrementSize(0), _gpuHandle(), _cpuHandle(), _heapName("UnNamed Heap ")
{
}


K3D12::DescriptorHeap::~DescriptorHeap()
{
	Discard();
}

D3D12_CPU_DESCRIPTOR_HANDLE K3D12::DescriptorHeap::GetCPUHandle(UINT indexCount)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = _heap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += (_incrementSize)* indexCount;
	return handle;
}

D3D12_GPU_DESCRIPTOR_HANDLE K3D12::DescriptorHeap::GetGPUHandle(UINT indexCount)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handle = _heap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += (_incrementSize)* indexCount;
	return handle;
}

UINT K3D12::DescriptorHeap::GetIncrementSize()
{
	return _incrementSize;
}

ID3D12DescriptorHeap* const K3D12::DescriptorHeap::GetPtr()const
{
	return _heap.Get();
}

const D3D12_DESCRIPTOR_HEAP_DESC & K3D12::DescriptorHeap::GetDesc()
{
	return _heapDesc;
}

ID3D12DescriptorHeap* K3D12::DescriptorHeap::operator->()
{
	return _heap.Get();
}

HRESULT K3D12::DescriptorHeap::Create(D3D12_DESCRIPTOR_HEAP_DESC* desc)
{
	if (desc == nullptr) {
		return E_FAIL;
	}
	auto hr = GET_DEVICE->CreateDescriptorHeap(desc, IID_PPV_ARGS(&_heap));
	if (FAILED(hr)) {
		return  E_FAIL;
	}
	_cpuHandle = this->_heap->GetCPUDescriptorHandleForHeapStart();
	_gpuHandle = this->_heap->GetGPUDescriptorHandleForHeapStart();
	_heapDesc = *desc;
	_type = desc->Type;
	_incrementSize = GET_DEVICE->GetDescriptorHandleIncrementSize(desc->Type);
	return S_OK;
}

HRESULT K3D12::DescriptorHeap::Create(D3D12_DESCRIPTOR_HEAP_TYPE type, unsigned int numDescriptors, unsigned int nodeMask)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc;
	switch (type)
	{
	case D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV:
		desc.NumDescriptors = numDescriptors;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		desc.NodeMask = nodeMask;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		break;
	case D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER:
		desc.NumDescriptors = numDescriptors;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		desc.NodeMask = nodeMask;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
		break;
	case D3D12_DESCRIPTOR_HEAP_TYPE_RTV:
		desc.NumDescriptors = numDescriptors;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		desc.NodeMask = nodeMask;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		break;
	case D3D12_DESCRIPTOR_HEAP_TYPE_DSV:
		desc.NumDescriptors = numDescriptors;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		desc.NodeMask = nodeMask;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		break;
	case D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES:
		desc.NumDescriptors = numDescriptors;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		desc.NodeMask = nodeMask;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES;
		break;
	default:
		break;
	}

	auto hr = Create(&desc);
	CHECK_RESULT(hr);
	return hr;
}

void K3D12::DescriptorHeap::SetName(std::string name)
{
	_heap->SetName(Util::StringToWString(name).c_str());
	_heapName = name;
}

void K3D12::DescriptorHeap::Discard()
{
	if (_heap.Get() != nullptr) {
		_heap.Reset();

		_incrementSize = 0;
		SystemLogger::GetInstance().Log(LogLevel::Details, _heapName + "is Released\n");
	}
}
