#pragma once
#include <memory>
#include "../Util/D3D12Common.h"
#include "../CommandContext/GraphicsCommandList.h"
#include "../DescriptorHeap/DescriptorHeap.h"

namespace K3D12 {
	class PipelineStateObject;
	class RootSignature;

	class DrawableComponent
	{
	private:
	protected:

		std::unique_ptr<K3D12::PipelineStateObject>		_pipelineState;
		std::unique_ptr<K3D12::RootSignature>			_rootSignature;
		K3D12::GraphicsCommandList						_bundleList;
		std::weak_ptr<K3D12::GraphicsCommandList>		_commandList;

	public:
	private:
	protected:
	public:

		virtual void SetPipelineState(K3D12::PipelineStateObject* pipelineState) = 0;
		virtual void SetRootSignature(K3D12::RootSignature* rootSignature) = 0;
		virtual void SetMasterCommandList(std::shared_ptr<K3D12::GraphicsCommandList> masterCommandList);

		//PSOとRSを設定されたマスターコマンドリストにセットする
		virtual void BindingShaderObject();
		virtual void BindingShaderObjectToBundle();

		//バンドルリストに継承先特有のドローコールを積む作業を強制
		virtual void RegistBundle() = 0;

		DrawableComponent();
		virtual ~DrawableComponent();
	};
}