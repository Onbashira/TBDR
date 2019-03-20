#include "MMDModelManager.h"
#include "MMDModel.h"


K3D12::MMDModelManager::MMDModelManager()
{
}


K3D12::MMDModelManager::~MMDModelManager()
{
	Discard();
}


void K3D12::MMDModelManager::SetMMDModel(std::string modelPath, std::shared_ptr<MMDModel> model)
{
	_pool.Set(modelPath, model);
}

std::weak_ptr<K3D12::MMDModel> K3D12::MMDModelManager::GetMMDModel(std::string modelPath)
{
	return _pool.Get(modelPath);
}

void K3D12::MMDModelManager::EraseMMDModel(std::string modelPath)
{
	_pool.Erase(modelPath);
}

void K3D12::MMDModelManager::Discard()
{
	_pool.DiscardMap();
}