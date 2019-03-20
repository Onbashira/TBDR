#pragma once
#include <map>
#include <memory>
#include <string>
#include "VMD.h"
#include "VMDLoader.h"


namespace K3D12 {
	class MotionAssetPool
	{
		friend class MotionManager;
	private:
		std::map<std::string, std::shared_ptr<MotionData>> _motionMap;
	public:
	private:
	public:
		void EraseMotionData(std::string motionName);
		void RegistMotionData(std::string motionName, std::shared_ptr<MotionData> motion);
		std::weak_ptr<MotionData> GetMotionData(std::string motionName);
		bool IsLoadedData(std::string motionName);
		void DiscardMotion();
		MotionAssetPool();
		~MotionAssetPool();
	};
}