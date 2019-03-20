#include "GamePadManager.h"
#include "GamePad.h"
#include "../Util/Utility.h"

constexpr int MaxConectingCount = 4;

void K3D12::GamePadManager::AllSetUp()
{
	for (int i = 0; i < _maxConectionCount;i++) {
		this->_gamePads.push_back(std::move(std::make_shared<GamePad>()));
		this->_gamePads[i]->SetUp(i);
	}
}

void K3D12::GamePadManager::Update()
{
	for (size_t i = 0; i < _gamePads.size(); i++) {
		this->_gamePads[i]->Update();
	}
}

std::shared_ptr<K3D12::GamePad> K3D12::GamePadManager::GetPad(int padID)
{
	return _gamePads[::Clamp(padID, 0, MaxConectingCount-1)];
}

void K3D12::GamePadManager::Initialize(int maxConectionCount)
{
	this->_maxConectionCount = ::Clamp(maxConectionCount,0, MaxConectingCount);
	AllSetUp();
}

K3D12::GamePadManager::GamePadManager() : _maxConectionCount(0)
{
}

K3D12::GamePadManager::~GamePadManager()
{
	for (int i = 0; i < _maxConectionCount; i++) {
		this->_gamePads[i].reset();
	}
}
