#pragma once
#include <memory>
#include "MotionAssetPool.h"
#include "VMDLoader.h"

namespace K3D12 {

	class MotionManager
	{
	private:
		MotionAssetPool _motionPool;
		VMDLoader		_motionLoader;
	public:
	private:
		MotionManager() {};
		MotionManager(const K3D12::MotionManager&) {};
		MotionManager(K3D12::MotionManager&&) {};
		K3D12::MotionManager& operator= (const K3D12::MotionManager& other) { return *this; };
		K3D12::MotionManager& operator= (K3D12::MotionManager&& other) { return *this; };

	public:
		static std::weak_ptr<K3D12::MotionData> LoadMotion(std::string motion);
		static std::weak_ptr<K3D12::MotionData> GetMotionData(std::string motion);
		static MotionManager& GetInstance();
		static void Discard();
		~MotionManager();
	};

}