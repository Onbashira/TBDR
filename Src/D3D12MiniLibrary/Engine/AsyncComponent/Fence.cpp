#include "Fence.h"
#include "../System/D3D12System.h"
#include "../CommandContext/CommandQueue.h"

K3D12::Fence::Fence():_fence(),_fenceValue(), _fenceEvent(nullptr)
{
}


K3D12::Fence::~Fence()
{
	Discard();
}


HRESULT K3D12::Fence::Create(UINT64 initialFenceValue, D3D12_FENCE_FLAGS flags)
{
	auto hresult = GET_DEVICE->CreateFence(0,flags,IID_PPV_ARGS(&_fence));
	if (FAILED(hresult)) {
		return hresult;
	}
	_fenceValue = 1;
	_fenceEvent = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);
	if (_fenceEvent == nullptr) {
		return ERROR_INVALID_HANDLE;
	}
	return hresult;
}

void K3D12::Fence::Discard()
{
	CloseHandle(_fenceEvent);
	_fenceEvent = nullptr;
	_fenceValue = 0;
	_fence.Reset();
}

Microsoft::WRL::ComPtr<ID3D12Fence> K3D12::Fence::GetFence()
{
	return _fence;
}

UINT64 K3D12::Fence::GetFenceValue()
{
	return _fenceValue;
}

void * K3D12::Fence::GetFenceEvent()
{
	return _fenceEvent;
}

HRESULT K3D12::Fence::Wait(K3D12::CommandQueue * commandQueue)
{
	const UINT64 fence = _fenceValue;
	auto hr = commandQueue->GetQueue()->Signal(_fence.Get(), fence);
	if (FAILED(hr))
		return hr;
	_fenceValue++;

	// Š®—¹‚ð‘Ò‹@.
	if (_fence->GetCompletedValue() < fence)
	{
		hr = _fence->SetEventOnCompletion(fence, _fenceEvent);
		if (FAILED(hr))
			return hr;

		WaitForSingleObject(_fenceEvent, INFINITE);
	}
	return S_OK;
}

