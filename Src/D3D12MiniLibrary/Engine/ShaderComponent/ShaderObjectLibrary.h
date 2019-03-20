#pragma once
#include <d3d12.h>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace K3D12 {

	class PipelineStateObject;
	class RootSignature;

	class ShaderObjectLibrary
	{
	private:
		std::map <std::string, std::shared_ptr<K3D12::PipelineStateObject>> _psolibrary;
		std::map <std::string, std::shared_ptr<K3D12::RootSignature>> _rootSignaturelibrary;
	public:
	private:
	public:

		//描画パイプラインの作成
		HRESULT CreatePSO(std::string psoName, D3D12_GRAPHICS_PIPELINE_STATE_DESC gps, ID3DBlob* signature = nullptr);
		HRESULT CreatePSO(std::string psoName, D3D12_COMPUTE_PIPELINE_STATE_DESC cps, ID3DBlob* signature = nullptr);

		//ルートシグネチャの作成
		HRESULT CreateRootSignature(std::string rsName, ID3DBlob* signature);

		//パイプラインステートの登録
		void SetPSO(std::string psoName, std::shared_ptr<K3D12::PipelineStateObject> pso);
		
		//ロートシグネチャの登録
		void SetRootSignature(std::string rsName, std::shared_ptr<RootSignature> rootSignature);

		std::weak_ptr<K3D12::PipelineStateObject> GetPSO(std::string psoName);
		std::weak_ptr<K3D12::RootSignature> GetRootSignature(std::string rsName);

		void ErasePSO(std::string psoName);
		void EraseRootSignature(std::string psoName);

		void Discard();

		ShaderObjectLibrary();
		~ShaderObjectLibrary();
	};
}