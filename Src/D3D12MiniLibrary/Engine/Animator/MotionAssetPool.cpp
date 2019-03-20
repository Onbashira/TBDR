#include <string>
#include <map>
#include <algorithm>
#include <memory>
#include "VMD.h"
#include "MotionAssetPool.h"



K3D12::MotionAssetPool::MotionAssetPool()
{
}


K3D12::MotionAssetPool::~MotionAssetPool()
{
	DiscardMotion();
}


void K3D12::MotionAssetPool::EraseMotionData(std::string motionName)
{
	if (_motionMap.find(motionName) != _motionMap.end()) {
		_motionMap.erase(motionName);
	}
}

void K3D12::MotionAssetPool::RegistMotionData(std::string motionName, std::shared_ptr<MotionData> motion)
{
	if (_motionMap.find(motionName) == _motionMap.end()) {
		_motionMap[motionName] = std::move(motion);
	}
}

std::weak_ptr<K3D12::MotionData> K3D12::MotionAssetPool::GetMotionData(std::string motionName)
{
	if (_motionMap.find(motionName) != _motionMap.end()) {
		return 	_motionMap[motionName];
	}
	return std::weak_ptr<K3D12::MotionData>();
}

bool K3D12::MotionAssetPool::IsLoadedData(std::string motionName)
{
	if (_motionMap.find(motionName) != _motionMap.end()) {
		return 	true;
	}
	return false;
}

void K3D12::MotionAssetPool::DiscardMotion()
{
	_motionMap.clear();
}

