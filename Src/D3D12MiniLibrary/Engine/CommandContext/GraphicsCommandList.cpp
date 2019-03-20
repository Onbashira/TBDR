#include "GraphicsCommandList.h"
#include "../PipelineState/PipelineStateObject.h"
#include "../Signature/RootSignature.h"
#include "../System/D3D12System.h"
#include "../AsyncComponent/Fence.h"
#include "../CommandContext/CommandQueue.h"
#include "../Resource/DepthStencilBuffer.h"
#include "../Util/Utility.h"
#include "../Util/Logger.h"



K3D12::GraphicsCommandList::GraphicsCommandList() :
	_commandList(), _commandAllocator(), _bindedDepthStencil(nullptr),
	_bindedRenderTargets(), _commandListName("UnNamed_CommandList"), _commandAllocatorName("UnNamed_CommandAllocator")
{

}


K3D12::GraphicsCommandList::~GraphicsCommandList()
{
	Discard();
}


HRESULT K3D12::GraphicsCommandList::Create(unsigned int nodeMask, D3D12_COMMAND_LIST_TYPE listType)
{
	HRESULT result;
	result = D3D12System::GetInstance().GetDevice()->CreateCommandAllocator(listType,IID_PPV_ARGS(&_commandAllocator));
	if (result != S_OK) {
		return E_FAIL;
	}

	result = GET_DEVICE->CreateCommandList(nodeMask, listType, _commandAllocator.Get(), nullptr, IID_PPV_ARGS(&_commandList));
	if (result != S_OK) {
		return E_FAIL;
	}


	return S_OK;
}

HRESULT K3D12::GraphicsCommandList::SetResourceBarrie(ID3D12Resource * resource, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState)
{
	D3D12_RESOURCE_BARRIER resource_barrier{};

	resource_barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	resource_barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	resource_barrier.Transition.pResource = resource;
	resource_barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	resource_barrier.Transition.StateBefore = beforeState;
	resource_barrier.Transition.StateAfter = afterState;

	_commandList->ResourceBarrier(1, &resource_barrier);
	return S_OK;
}

HRESULT K3D12::GraphicsCommandList::UAVBarrie(ID3D12Resource * resource)
{
	return E_NOTIMPL;
}

HRESULT K3D12::GraphicsCommandList::ExcutionCommandList(CommandQueue* queue, Fence* fence)
{
	CHECK_RESULT(CloseCommandList());

	HRESULT hr = {};

	ID3D12CommandList *const command_lists = _commandList.Get();
	queue->GetQueue()->ExecuteCommandLists(1, &command_lists);

	if (fence != nullptr) {
		fence->Wait(queue);
	}
	return S_OK;
}

Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> K3D12::GraphicsCommandList::GetCommandList()
{
	return this->_commandList;
}

Microsoft::WRL::ComPtr<ID3D12CommandAllocator> K3D12::GraphicsCommandList::GetAllocator()
{
	return this->_commandAllocator;
}

HRESULT K3D12::GraphicsCommandList::ResetCommandList(ID3D12PipelineState * pInitialState)
{
	auto hr = _commandList->Reset(_commandAllocator.Get(), pInitialState);
	return hr;
}

HRESULT K3D12::GraphicsCommandList::ResetAllocator()
{
	auto  hr = _commandAllocator->Reset();
	return hr;
}

HRESULT K3D12::GraphicsCommandList::CloseCommandList()
{
	auto hr = _commandList->Close();
	return hr;
}

HRESULT K3D12::GraphicsCommandList::ConnectingPSO(PipelineStateObject * pso)
{
	this->_commandList->SetGraphicsRootSignature(pso->GetRootSignature().lock()->GetSignature().Get());
	this->_commandList->SetPipelineState(pso->GetPSO().Get());

	return S_OK;
}

HRESULT K3D12::GraphicsCommandList::BindingDepthStencil(DepthStencil * depth)
{
	this->_bindedDepthStencil = depth;
	return S_OK;
}

void K3D12::GraphicsCommandList::AddRenderTargets(D3D12_CPU_DESCRIPTOR_HANDLE handles[])
{
	//unsigned int size = _countof(&handles);
	unsigned int size = sizeof(handles) / sizeof(D3D12_CPU_DESCRIPTOR_HANDLE);
	for (unsigned int index = 0; index < size;index++) {
		this->_bindedRenderTargets[index] = handles[index];
	}
}

void K3D12::GraphicsCommandList::AddRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE handle, unsigned int index)
{
	if (_bindedRenderTargets.count(index) == 0) {
		this->_bindedRenderTargets[index] = handle;
	}
}

void K3D12::GraphicsCommandList::UnBindingRenderTargets()
{
	for (unsigned int index = 0; index < _bindedRenderTargets.size(); index++) {
		_bindedRenderTargets.erase(index);
	}
}

void K3D12::GraphicsCommandList::UnBindingRenderTarget(unsigned int renderTargetIndex)
{
	_bindedRenderTargets.erase(renderTargetIndex);
}

void K3D12::GraphicsCommandList::OMSetBindingRenderTargets()
{
	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> handles;
	for (auto& ref : _bindedRenderTargets) {
		handles.push_back(ref.second);
	}
	if (handles.size()== 0 || _bindedDepthStencil == nullptr) {
		return;
	}
	_commandList->OMSetRenderTargets(static_cast<unsigned int>(_bindedRenderTargets.size()), &handles[0], (handles.size() > 1 ? TRUE : FALSE), &_bindedDepthStencil->GetDSVHeapPtr()->GetCPUHandle(0));
}

void K3D12::GraphicsCommandList::OMSetRenderTargets(unsigned int numRenderTargets, D3D12_CPU_DESCRIPTOR_HANDLE * rtvHandles, bool RTsSingleHandleToDescriptorRange, D3D12_CPU_DESCRIPTOR_HANDLE* dsvHandle)
{
	_commandList->OMSetRenderTargets(numRenderTargets, rtvHandles, RTsSingleHandleToDescriptorRange, dsvHandle);
}

void K3D12::GraphicsCommandList::Discard()
{

	if (_commandList.Get() != nullptr) {
		if (_commandListName == "UnNamed_CommandList") {
			assert(true);

		}
		this->_commandAllocator.Reset();
		this->_commandList.Reset();
		SystemLogger::GetInstance().Log(LogLevel::Details, _commandListName + " is Reset\n");
		SystemLogger::GetInstance().Log(LogLevel::Details, _commandAllocatorName + " is Reset\n");
		_bindedDepthStencil = nullptr;
		_bindedRenderTargets.clear();
	}

}

void K3D12::GraphicsCommandList::SetName(std::string objectName)
{
	_commandListName = objectName + "List";
	SetCommandListName(_commandListName);
	_commandAllocatorName = objectName + "Allocator";
	SetCommandAllocatorName(_commandAllocatorName);
}

void K3D12::GraphicsCommandList::SetCommandListName(std::string name)
{
	_commandListName = name;
	this->_commandList->SetName(Util::StringToWString(_commandListName).c_str());
}

void K3D12::GraphicsCommandList::SetCommandAllocatorName(std::string name)
{
	_commandAllocatorName = name;
	this->_commandAllocator->SetName(Util::StringToWString(_commandAllocatorName).c_str());
}