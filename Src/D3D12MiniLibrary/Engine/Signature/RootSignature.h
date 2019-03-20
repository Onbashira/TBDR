#pragma once
#include <string>

#include "../Util/D3D12Common.h"
namespace K3D12 {

	class RootSignature
	{
	private:
		Microsoft::WRL::ComPtr<ID3D12RootSignature> _rootSignature;
		std::string									_name;
	public:
	private:
	public:
		HRESULT CreateFromShader(ID3DBlob* shader);
		HRESULT CreateFromDesc(D3D12_ROOT_SIGNATURE_DESC* desc);
		Microsoft::WRL::ComPtr<ID3D12RootSignature> GetSignature();
		void SetName(std::string name);
		void Discard();
		RootSignature();
		~RootSignature();
	};
}