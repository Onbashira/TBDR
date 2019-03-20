#include <string>
#include "RootSignature.h"
#include "../Util/D3D12Common.h"
#include "../System/D3D12System.h"
#include "../Util/Utility.h"
#include "../Util/Logger.h"


K3D12::RootSignature::RootSignature() :
	_name("UnNamed RootSignature")
{
}


K3D12::RootSignature::~RootSignature()
{
	Discard();
}


HRESULT K3D12::RootSignature::CreateFromShader(ID3DBlob * shader)
{
	Microsoft::WRL::ComPtr<ID3DBlob> signature;

	if (FAILED(D3DGetBlobPart(shader->GetBufferPointer(), shader->GetBufferSize(), D3D_BLOB_ROOT_SIGNATURE, 0, &signature)))
	{
		return E_FAIL;
	}
	auto hr = GET_DEVICE->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&_rootSignature));
	if (FAILED(hr)) {
		return hr;
	};
	signature.Reset();
	return S_OK;
}

HRESULT K3D12::RootSignature::CreateFromDesc(D3D12_ROOT_SIGNATURE_DESC * desc)
{
	Microsoft::WRL::ComPtr<ID3DBlob> signature;
	Microsoft::WRL::ComPtr<ID3DBlob> error;

	auto hr = D3D12SerializeRootSignature(desc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
	if (FAILED(hr)) {
		OutputDebugStringA((char*)error->GetBufferPointer());
		return E_FAIL;
	}

	GET_DEVICE->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&_rootSignature));
	signature.Reset();
	error.Reset();
	return S_OK;
}

Microsoft::WRL::ComPtr<ID3D12RootSignature> K3D12::RootSignature::GetSignature()
{
	return this->_rootSignature;
}

void K3D12::RootSignature::SetName(std::string name)
{
	if (_rootSignature.Get() != nullptr) {
		this->_rootSignature->SetName(Util::StringToWString(name).c_str());
		_name = name;
	}
}

void K3D12::RootSignature::Discard()
{
	if (_rootSignature.Get() != nullptr) {
		_rootSignature.Reset();
		SystemLogger::GetInstance().Log(LogLevel::Debug, _name + " is  Reset\n");
	}
}