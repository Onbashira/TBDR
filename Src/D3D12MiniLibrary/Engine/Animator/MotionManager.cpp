#include <memory>
#include "MotionAssetPool.h"
#include "VMDLoader.h"
#include "MotionManager.h"


K3D12::MotionManager::~MotionManager()
{
	Discard();
}


std::weak_ptr<K3D12::MotionData> K3D12::MotionManager::LoadMotion(std::string motion)
{
	if (!GetInstance()._motionPool.IsLoadedData(motion)) {
		GetInstance()._motionPool.RegistMotionData(motion, GetInstance()._motionLoader.LoadMotion(motion));
		return GetInstance()._motionPool.GetMotionData(motion);
	}
	else {
		return GetInstance()._motionPool.GetMotionData(motion);
	}
}

std::weak_ptr<K3D12::MotionData> K3D12::MotionManager::GetMotionData(std::string motion)
{
	return GetInstance()._motionPool.GetMotionData(motion);
}

K3D12::MotionManager & K3D12::MotionManager::GetInstance()
{
	static MotionManager instance;
	return instance;
}

void K3D12::MotionManager::Discard()
{
	GetInstance()._motionPool.DiscardMotion();
}

