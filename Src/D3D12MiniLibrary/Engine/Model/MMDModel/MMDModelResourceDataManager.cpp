#include <memory>
#include "MMDModelResourceDataManager.h"
#include "MMDModelResourceData.h"


K3D12::MMDModelResourceDataManager::MMDModelResourceDataManager()
{
}


K3D12::MMDModelResourceDataManager::~MMDModelResourceDataManager()
{
	Discard();
}


std::weak_ptr<K3D12::MMDModelResourceData> K3D12::MMDModelResourceDataManager::GetConvertedModelData(std::string path)
{
	return _pool.Get(path);
}

void K3D12::MMDModelResourceDataManager::SetConvertedModelData(std::string key, std::shared_ptr<MMDModelResourceData> modelData)
{
	_pool.Set(key, modelData);
}

void K3D12::MMDModelResourceDataManager::EraseConvertedModelData(std::string key)
{
	_pool.Erase(key);
}

bool K3D12::MMDModelResourceDataManager::IsLoadedModel(std::string path)
{
	return _pool.IsLooded(path);
}

void K3D12::MMDModelResourceDataManager::Discard()
{
	_pool.DiscardMap();
}




