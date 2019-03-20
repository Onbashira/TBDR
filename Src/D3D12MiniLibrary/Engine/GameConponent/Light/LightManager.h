#pragma once
#include <memory>
#include <map>
#include <vector>
#include <Windows.h>
#include "Light.h"
#include "../../Resource/UnorderdAccessView.h"

namespace K3D12 {
	class LightManager
	{
	private:
		inline static LightManager*						_instance = nullptr;
		std::map<unsigned int, std::shared_ptr<Light>>	_lightPool;
		std::vector<LightInfo>							_lightIntermidiate;
		unsigned int									_maxLightNum;
		unsigned int									_currentLightNum;
		UnorderedAccessValue							_lightStructuredBuffer;
	public:
		LightManager();
		LightManager(unsigned int numLight);
	private:
	public:
		static HRESULT		CommitStructuredBuffer();
		static void			Create(unsigned int numLight);
		static void			Destroy();
		static void			ResetLightPool();
		static void			ResetMaxLight(unsigned int numLight);
		static void			ResetManager();
		static HRESULT		AddLight(LightInfo& lightInfo);
		static HRESULT		EraseLight(unsigned int lightIndex);
		static K3D12::LightInfo	GetLightInfo(unsigned int lightIndex);
		static K3D12::UnorderedAccessValue* GetLightUAV();
		~LightManager();
	};
}