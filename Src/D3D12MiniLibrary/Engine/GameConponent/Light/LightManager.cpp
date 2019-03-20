#include <memory>
#include <map>
#include "LightManager.h"
#include "../../Resource/UnorderdAccessView.h"

K3D12::LightManager::LightManager()
{

}

K3D12::LightManager::LightManager(unsigned int numLight) :
	_maxLightNum(numLight),_currentLightNum(0)
{
}

HRESULT K3D12::LightManager::CommitStructuredBuffer()

{
	if (_instance != nullptr) {
		return _instance->_lightStructuredBuffer.Create(sizeof(LightInfo), _instance->_maxLightNum,&_instance->_lightIntermidiate[0]);
	}
	return E_FAIL;
}

void K3D12::LightManager::Create(unsigned int numLight)
{
	if (_instance == nullptr) {
		_instance = new LightManager(numLight);
	}
}

void K3D12::LightManager::Destroy()
{
	if (_instance != nullptr) {
		delete _instance;
	}
}

void K3D12::LightManager::ResetLightPool()
{
	_instance->_lightPool.clear();
}

void K3D12::LightManager::ResetMaxLight(unsigned int numLight)
{
	_instance->_maxLightNum = numLight;
}

void K3D12::LightManager::ResetManager()
{
	ResetLightPool();
	ResetMaxLight(0);
}

HRESULT K3D12::LightManager::AddLight(K3D12::LightInfo & lightInfo)
{
	
	if (_instance->_maxLightNum <= _instance->_currentLightNum) {
		return E_FAIL;
	}
	if (_instance->_lightPool.find(_instance->_currentLightNum) == _instance->_lightPool.end()) {
		_instance->_lightIntermidiate.push_back(lightInfo);
		_instance->_lightPool[_instance->_currentLightNum] = std::make_shared<Light>();
		++_instance->_currentLightNum;
		return S_OK;
	}
	return E_FAIL;
}

HRESULT K3D12::LightManager::EraseLight(unsigned int lightIndex)
{
	if (_instance->_maxLightNum <= lightIndex) {
		return E_FAIL;
	}
	if (_instance->_lightPool.find(lightIndex) != _instance->_lightPool.end()) {
		_instance->_lightIntermidiate.erase(_instance->_lightIntermidiate.begin() + lightIndex);
		_instance->_lightPool.erase(lightIndex);
		return S_OK;
	}
	return E_FAIL;
}

K3D12::LightInfo K3D12::LightManager::GetLightInfo(unsigned int lightIndex)
{
	if (_instance->_maxLightNum <= lightIndex) {
		return LightInfo();
	}
	if (_instance->_lightPool.find(lightIndex) != _instance->_lightPool.end()) {
		return _instance->_lightPool.at(lightIndex)->GetInfo();
	}
	return LightInfo();
}

K3D12::UnorderedAccessValue * K3D12::LightManager::GetLightUAV()
{
	return &_instance->_lightStructuredBuffer;
}


K3D12::LightManager::~LightManager()
{
	_lightStructuredBuffer.Discard();
	ResetManager();
}
