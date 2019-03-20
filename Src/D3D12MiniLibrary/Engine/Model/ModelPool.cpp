#include <memory>
#include <string>
#include <assert.h>
#include <functional>

#include "ModelPool.h"
#include "./MMDModel/MMDModelResourceDataManager.h"
#include "ModelConverter.h"
#include "../Util/Utility.h"
#include "../AsyncComponent/ThreadPool.h"
#include "./PMX/PMXLoader.h"
#include "./PMD/PMDLoader.h"
#include "./MMDModel/MMDModel.h"
#include "./MMDModel/MMDModelResourceData.h"
#include "./MMDModel/MMDModelManager.h"
#include "../CommandContext/GraphicsCommandList.h"
#include "../PipelineState/PipelineStateObject.h"

constexpr size_t ThreadNum = 5;

K3D12::ModelPool::ModelPool() :
	_pmdLoader(std::make_shared<PMDLoader>()), _pmxLoader(std::make_shared<PMXLoader>()),
	_modelResourcePool(std::make_shared<MMDModelResourceDataManager>()),
	_modelPool(std::make_shared<MMDModelManager>()),
	_list()
{

}


K3D12::ModelPool::~ModelPool()
{
	Discard();
}


void K3D12::ModelPool::SetGraphicsCommandList(std::shared_ptr<GraphicsCommandList> list)
{
	_list = list;
}

void K3D12::ModelPool::SetPipelineState(std::weak_ptr<PipelineStateObject> pso)
{
	_pso = pso;
}

void K3D12::ModelPool::SetRootSignature(std::weak_ptr<RootSignature> rootSignature)
{
	_rootSignature = rootSignature;
}

std::weak_ptr<K3D12::MMDModel> K3D12::ModelPool::LoadModel(std::string modelPath)
{
	std::shared_ptr<K3D12::MMDModelResourceData> convertedModel;

	if (!_modelResourcePool->IsLoadedModel(modelPath)) {
		std::string extension = Util::ExtensionExtruction(modelPath);
		if (extension == "pmx") {
			std::shared_ptr<PMXModel> model;
			std::string tempPath = modelPath;

			model = this->_pmxLoader->Load(modelPath);

			if (model == nullptr) {
				MessageBox(nullptr, L"モデルの読み込みに失敗しました", L"不正パス", MB_OK);
				return std::weak_ptr<MMDModel>();
			}
			convertedModel = ModelConverter::ConvertPMXModel(model);
			_modelResourcePool->SetConvertedModelData(modelPath, convertedModel);
		}
		else if (extension == "pmd") {
			std::shared_ptr<PMDModel> model;
			std::string tempPath = modelPath;

			model = this->_pmdLoader->Load(modelPath);
			if (model == nullptr) {
				MessageBox(nullptr, L"モデルの読み込みに失敗しました", L"不正パス", MB_OK);
				return std::weak_ptr<MMDModel>();
			}
			convertedModel = ModelConverter::ConvertPMDModel(model);
			_modelResourcePool->SetConvertedModelData(modelPath, convertedModel);
		}
		else {
			MessageBox(nullptr, L"モデルの読み込みに失敗しました", L"不正拡張子", MB_OK);
			return std::weak_ptr<MMDModel>();
		}
	}
	else {

		convertedModel = _modelResourcePool->GetConvertedModelData(modelPath).lock();

	}

	auto ptr = ModelConverter::ExtructMMDModel(_pso, _rootSignature, _list, convertedModel);

	ptr->SetPos(Vector3(0.0f, 0.0f, 0.0f));
	ptr->SetScale(Vector3::one);
	ptr->SetRotation(Quaternion::CreateIdentity());

	_modelPool->SetMMDModel(modelPath, ptr);

	return _modelPool->GetMMDModel(modelPath);

}

void K3D12::ModelPool::Discard()
{
	_pmdLoader.reset();
	_pmxLoader.reset();
	_modelPool.reset();
	_modelResourcePool.reset();
}

