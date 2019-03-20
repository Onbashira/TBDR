#pragma once
#include <string>
#include <memory>

namespace K3D12 {

	class PMDLoader;
	class PMXLoader;
	class MMDModelResourceDataManager;
	class MMDModel;
	class MMDModelManager;
	class GraphicsCommandList;
	class PipelineStateObject;
	class RootSignature;

	class ModelPool
	{

	private:
		std::shared_ptr<PMDLoader>						_pmdLoader;
		std::shared_ptr<PMXLoader>						_pmxLoader;
		std::shared_ptr<MMDModelResourceDataManager>	_modelResourcePool;
		std::shared_ptr<MMDModelManager>				_modelPool;
		std::weak_ptr<GraphicsCommandList>				_list;
		std::weak_ptr<PipelineStateObject>				_pso;
		std::weak_ptr<RootSignature>					_rootSignature;

	public:

	private:

	public:

		void SetGraphicsCommandList(std::shared_ptr<GraphicsCommandList> list);

		void SetPipelineState(std::weak_ptr<PipelineStateObject> pso);

		void SetRootSignature(std::weak_ptr<RootSignature> rootSignature);

		std::weak_ptr<MMDModel> LoadModel(std::string modelPath);

		void Discard();

		ModelPool();
		~ModelPool();
	};
}