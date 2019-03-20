#include <string>
#include "PipelineStateObject.h"
#include "../Util/D3D12Common.h"
#include "../System/D3D12System.h"
#include "../Util/Utility.h"
#include "../ShaderComponent/ShaderObjectLibrary.h"
#include "../CommandContext/GraphicsContextLibrary.h"
#include "../Signature/RootSignature.h"
#include  "../Util/Logger.h"

K3D12::PipelineStateObject::PipelineStateObject() :
	_pipelineState(), _name("UnNamed PipelineState")
{

}

K3D12::PipelineStateObject::~PipelineStateObject()
{
	Discard();
}


HRESULT K3D12::PipelineStateObject::Create(std::string name, D3D12_GRAPHICS_PIPELINE_STATE_DESC & gpsDesc, ID3DBlob * shader)
{
	if (shader != nullptr) {
		GraphicsContextLibrary::GetInstance().CreateRootSignature(name + "RootSignature", shader);
		_rootSignature = GraphicsContextLibrary::GetInstance().GetRootSignature(name + "RootSignature");
		gpsDesc.pRootSignature = _rootSignature.lock()->GetSignature().Get();
	}
	auto hr = GET_DEVICE->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(&_pipelineState));
	return hr;
}

HRESULT K3D12::PipelineStateObject::Create(std::string name, D3D12_COMPUTE_PIPELINE_STATE_DESC & cpsDesc, ID3DBlob * shader)
{
	if (shader != nullptr) {
		GraphicsContextLibrary::GetInstance().CreateRootSignature(name + "RootSignature", shader);
		_rootSignature = GraphicsContextLibrary::GetInstance().GetRootSignature(name + "RootSignature");
		cpsDesc.pRootSignature = _rootSignature.lock()->GetSignature().Get();
	}
	auto hr = GET_DEVICE->CreateComputePipelineState(&cpsDesc, IID_PPV_ARGS(&_pipelineState));
	SetName(name);

	return hr;
}

HRESULT K3D12::PipelineStateObject::Create(std::string name, D3D12_GRAPHICS_PIPELINE_STATE_DESC & gpsDesc, std::weak_ptr<RootSignature> rootSignature)
{
	_rootSignature = rootSignature;
	gpsDesc.pRootSignature = _rootSignature.lock()->GetSignature().Get();
	auto hr = GET_DEVICE->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(&_pipelineState));
	SetName(name);

	return hr;
}

HRESULT K3D12::PipelineStateObject::Create(std::string name, D3D12_COMPUTE_PIPELINE_STATE_DESC & cpsDesc, std::weak_ptr<RootSignature> rootSignature)
{
	_rootSignature = rootSignature;
	cpsDesc.pRootSignature = _rootSignature.lock()->GetSignature().Get();

	auto hr = GET_DEVICE->CreateComputePipelineState(&cpsDesc, IID_PPV_ARGS(&_pipelineState));
	SetName(name);

	return hr;
}

void K3D12::PipelineStateObject::SetName(std::string name)
{
	_pipelineState->SetName(Util::StringToWString(name).c_str());
	_name = name;
}

void K3D12::PipelineStateObject::BindingRootSignature(std::weak_ptr<K3D12::RootSignature> rootSignature)
{

}

std::weak_ptr<K3D12::RootSignature> K3D12::PipelineStateObject::GetRootSignature()
{
	return this->_rootSignature;
}

Microsoft::WRL::ComPtr<ID3D12PipelineState> K3D12::PipelineStateObject::GetPSO()
{
	return this->_pipelineState;
}

void K3D12::PipelineStateObject::Discard()
{
	if (_pipelineState.Get() != nullptr) {
		_pipelineState.Reset();
		SystemLogger::GetInstance().Log(LogLevel::Debug, _name + " is  Reset\n");
	}
	_rootSignature.reset();
}
