#include <memory>
#include "GraphicsContextLibrary.h"
#include "../ShaderComponent/ShaderObjectLibrary.h"
#include "../Signature/RootSignature.h"
#include "../PipelineState/PipelineStateObject.h"
#include "GraphicsCommandList.h"
#include "CommandQueue.h"
#include "../AsyncComponent/Fence.h"
#include "../Resource/DepthStencilBuffer.h"

K3D12::GraphicsContextLibrary::GraphicsContextLibrary() :
	_commandListLibrary(), _shaderObjectLibrary()
{

}


K3D12::GraphicsContextLibrary::~GraphicsContextLibrary()
{
	Discard();
}


void K3D12::GraphicsContextLibrary::CloseAllCommandLists()
{
	for (auto itr = _commandListLibrary._library.begin(); itr != _commandListLibrary._library.end(); itr++) {
		itr->second->CloseCommandList();
	}
}

void K3D12::GraphicsContextLibrary::CloseCommandList(std::string commandListName)
{
	_commandListLibrary.Get(commandListName)->CloseCommandList();
}

void K3D12::GraphicsContextLibrary::ResetAllCommandList()
{
	for (auto itr = _commandListLibrary._library.begin(); itr != _commandListLibrary._library.end(); itr++) {
		itr->second->ResetAllocator();
		itr->second->ResetCommandList();
	}
}

void K3D12::GraphicsContextLibrary::ResetCommandList(std::string commandListName)
{
	_commandListLibrary.Get(commandListName)->ResetAllocator();
	_commandListLibrary.Get(commandListName)->ResetCommandList();

}

HRESULT K3D12::GraphicsContextLibrary::ExcutionCommandList(CommandQueue * queue, std::string commandListName, Fence* fence)
{

	this->_commandListLibrary.Get(commandListName)->ExcutionCommandList(queue, fence);
	if (fence != nullptr) {
		fence->Wait(queue);
	}
	return S_OK;
}

HRESULT K3D12::GraphicsContextLibrary::ExcutionCommandListAsync(CommandQueue * queue, std::vector<std::string> commandListNames, Fence * fence)
{

	std::vector<ID3D12CommandList*> lists(commandListNames.size());

	for (int i = 0; i < commandListNames.size(); ++i) {
		lists[i] = GetGraphicsCommandList(commandListNames[i])->GetCommandList().Get();
	}

	queue->GetQueue()->ExecuteCommandLists(static_cast<unsigned int>(lists.size()), &lists[0]);

	return S_OK;
}

HRESULT K3D12::GraphicsContextLibrary::ExcutionAllCommandLists(CommandQueue * queue, Fence* fence)
{

	std::vector< ID3D12CommandList *> lists;

	lists.reserve(_commandListLibrary._library.size());
	for (auto itr = _commandListLibrary._library.begin(); itr != _commandListLibrary._library.end(); itr++) {
		lists.push_back(itr->second->GetCommandList().Get());
	}
	queue->GetQueue()->ExecuteCommandLists(static_cast<unsigned int>(lists.size()), &lists[0]);
	if (fence != nullptr) {
		fence->Wait(queue);
	}
	return S_OK;
}

HRESULT K3D12::GraphicsContextLibrary::CreateCommandList(std::string commandListName, unsigned int nodeMask, D3D12_COMMAND_LIST_TYPE listType)
{
	return _commandListLibrary.Create(commandListName, nodeMask, listType);

}

HRESULT K3D12::GraphicsContextLibrary::CreatePSO(std::string psoName, D3D12_GRAPHICS_PIPELINE_STATE_DESC gps, ID3DBlob * rootSignature)
{
	return _shaderObjectLibrary.CreatePSO(psoName, gps, rootSignature);
}

HRESULT K3D12::GraphicsContextLibrary::CreatePSO(std::string psoName, D3D12_COMPUTE_PIPELINE_STATE_DESC cps, ID3DBlob * rootSignature)
{
	return _shaderObjectLibrary.CreatePSO(psoName, cps, rootSignature);
}

HRESULT K3D12::GraphicsContextLibrary::CreateRootSignature(std::string rsName, ID3DBlob * signature)
{
	return _shaderObjectLibrary.CreateRootSignature(rsName, signature);
}

void K3D12::GraphicsContextLibrary::SetCommandList(std::string commandListName, std::shared_ptr<GraphicsCommandList> commandList)
{
	_commandListLibrary.Set(commandListName, commandList);
}

void K3D12::GraphicsContextLibrary::SetPSO(std::string psoName, std::shared_ptr<PipelineStateObject> pso)
{
	_shaderObjectLibrary.SetPSO(psoName, pso);
}

void K3D12::GraphicsContextLibrary::SetRootSignature(std::string rsName, std::shared_ptr<RootSignature> rootSignature)
{
	_shaderObjectLibrary.SetRootSignature(rsName, rootSignature);
}

std::shared_ptr<K3D12::GraphicsCommandList> K3D12::GraphicsContextLibrary::GetGraphicsCommandList(std::string commandListName)
{
	return _commandListLibrary.Get(commandListName);
}

std::shared_ptr<K3D12::PipelineStateObject> K3D12::GraphicsContextLibrary::GetPSO(std::string psoName)
{
	return _shaderObjectLibrary.GetPSO(psoName).lock();
}

std::shared_ptr<K3D12::RootSignature> K3D12::GraphicsContextLibrary::GetRootSignature(std::string rsName)
{
	return _shaderObjectLibrary.GetRootSignature(rsName + "RootSignature").lock();
}

void K3D12::GraphicsContextLibrary::EraseCommandList(std::string commandListName)
{
	_commandListLibrary.Erase(commandListName);
}

void K3D12::GraphicsContextLibrary::ErasePSO(std::string psoName)
{
	_shaderObjectLibrary.ErasePSO(psoName);
}

void K3D12::GraphicsContextLibrary::EraseRootSignature(std::string rsName)
{
	_shaderObjectLibrary.EraseRootSignature(rsName);
}

HRESULT K3D12::GraphicsContextLibrary::Connecting(std::string commandList, std::string psoName)
{
	auto list = _commandListLibrary.Get(commandList);
	auto pso = _shaderObjectLibrary.GetPSO(psoName);

	if (list == nullptr || pso.lock() == nullptr) {
		return E_FAIL;
	}
	list->ConnectingPSO(pso.lock().get());
	return S_OK;

}

HRESULT K3D12::GraphicsContextLibrary::SetRenderTargetsToAllCommandList(unsigned int renderTargetNum, D3D12_CPU_DESCRIPTOR_HANDLE * const handles, bool isSingleRenderTarget, DepthStencil * depth)
{
	for (auto itr = _commandListLibrary._library.begin(); itr != _commandListLibrary._library.end(); itr++) {
		itr->second->GetCommandList()->OMSetRenderTargets(renderTargetNum, handles, isSingleRenderTarget, &depth->GetDSVHeapPtr()->GetCPUHandle(0));
	}

	return S_OK;
}

HRESULT K3D12::GraphicsContextLibrary::SetRenderTargetsToSelectedCommandList(std::string* commandListName, unsigned int numCommandList, unsigned int renderTargetNum, D3D12_CPU_DESCRIPTOR_HANDLE * const handles, bool isSingleRenderTarget, DepthStencil * depth)
{
	for (unsigned int i = 0; i < numCommandList; i++) {
		this->_commandListLibrary.Get(commandListName[i])->GetCommandList()->OMSetRenderTargets(renderTargetNum, handles, isSingleRenderTarget, &depth->GetDSVHeapPtr()->GetCPUHandle(0));
	}
	return S_OK;
}

HRESULT K3D12::GraphicsContextLibrary::SetRenderTargets(std::string commandListName, unsigned int renderTargetNum, D3D12_CPU_DESCRIPTOR_HANDLE * const handles, bool isSingleRenderTarget, DepthStencil * depth)
{
	this->_commandListLibrary.Get(commandListName)->GetCommandList()->OMSetRenderTargets(renderTargetNum, handles, isSingleRenderTarget, &depth->GetDSVHeapPtr()->GetCPUHandle(0));

	return S_OK;
}

void K3D12::GraphicsContextLibrary::Discard()
{
	_shaderObjectLibrary.Discard();
	_commandListLibrary.Discard();
}

K3D12::GraphicsContextLibrary & K3D12::GraphicsContextLibrary::GetInstance()
{
	static K3D12::GraphicsContextLibrary instance;
	return instance;
}
