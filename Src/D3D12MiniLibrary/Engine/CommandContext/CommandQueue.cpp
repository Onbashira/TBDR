#include "CommandQueue.h"
#include "../System/D3D12System.h"


K3D12::CommandQueue::CommandQueue() : _commandQueue()
{
}


K3D12::CommandQueue::~CommandQueue()
{
	Discard();
}


HRESULT K3D12::CommandQueue::Create(D3D12_COMMAND_QUEUE_DESC desc)
{

	if (FAILED(GET_DEVICE->CreateCommandQueue(&desc, IID_PPV_ARGS(&_commandQueue))))
		return FALSE;
	return S_OK;
}

Microsoft::WRL::ComPtr<ID3D12CommandQueue> K3D12::CommandQueue::GetQueue()
{

	return _commandQueue;
}

UINT64 K3D12::CommandQueue::GetTimestampFrequency()
{
	if (_commandQueue == nullptr) {
		return 1;
	}

	UINT64 ret;
	auto hr = _commandQueue->GetTimestampFrequency(&ret);
	if (FAILED(hr)) {
		return 1;
	}

	return ret;
}

D3D12_COMMAND_QUEUE_DESC K3D12::CommandQueue::GetDesc()
{
	return _commandQueue->GetDesc();
}

void K3D12::CommandQueue::Discard()
{
	_commandQueue.Reset();
}


