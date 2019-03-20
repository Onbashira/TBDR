#pragma once
#include <memory>


namespace K3D12{
	struct  MotionData;
	class VMDLoader
	{
	private:

	public:

	private:

	public:
		std::shared_ptr<K3D12::MotionData> LoadMotion(std::string path);
		VMDLoader();
		~VMDLoader();
	};
}