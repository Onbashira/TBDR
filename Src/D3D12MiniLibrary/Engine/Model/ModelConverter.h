#pragma once

#include <memory>

namespace K3D12 {
	class PMDModel;
	class PMXModel;
	struct MMDModelResourceData;
	class MMDModel;
	class PipelineStateObject;
	class RootSignature;
	class GraphicsCommandList;

	class ModelConverter
	{
	private:

	public:

	private:
	public:

		static std::shared_ptr<MMDModelResourceData>	ConvertPMDModel(std::shared_ptr<PMDModel> model);
		static std::shared_ptr<MMDModelResourceData>	ConvertPMXModel(std::shared_ptr<PMXModel> model);
		//汎用モデル形式に抽出
		static std::shared_ptr<MMDModel>				ExtructMMDModel(std::weak_ptr<PipelineStateObject> pso, std::weak_ptr<RootSignature> rootSignature, std::weak_ptr<GraphicsCommandList> list, std::shared_ptr<MMDModelResourceData> modelResource);
		ModelConverter();
		~ModelConverter();
	};
}