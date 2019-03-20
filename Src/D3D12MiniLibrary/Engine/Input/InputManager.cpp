#include "InputManager.h"
#include "../System/D3D12System.h"


K3D12::InputManager::InputManager() :
	_mousePos({})
{

}


K3D12::InputManager::~InputManager()
{
}


void K3D12::InputManager::InputUpdate()
{
	CopyMemory(_preKey,_key,sizeof(_preKey));
	if (!GetKeyboardState(&_key[0])) {
		MessageBox(nullptr, L"ƒL[î•ñ‚Ìæ“¾‚É¸”s", L"ERROR", MB_OK);
		return ;
	}
	POINT pos;
	GetCursorPos(&pos);
	const HWND hwnd = K3D12::D3D12System::GetWindow().GetWindowHandle();
	ScreenToClient(hwnd, &pos);
#ifdef _DEBUG
	//std::cout <<pos.x << std::endl;
	//std::cout <<pos.y << std::endl;
#endif // _DEBUG
	_mousePos.x = float(pos.x);
	_mousePos.x = float(pos.y);

	_gamePadManager.Update();

}

bool K3D12::InputManager::IsTriggerDown(VIRTUAL_KEY_STATE code)
{
	if (!((_preKey[code] & (0x80)) == 0x80) && ((_key[code] & (0x80)) == 0x80)) {
		return true;
	}
	return false;
}

bool K3D12::InputManager::IsTriggerUp(VIRTUAL_KEY_STATE code)
{
	if (((_preKey[code] & (0x80)) == 0x80) && !((_key[code] & (0x80)) == 0x80)) {
		return true;
	}
	return false;
}

bool K3D12::InputManager::IsToggle(VIRTUAL_KEY_STATE code)
{
	if ((_key[code] & (0x01f)) == (0x01)) {
		return true;
	};
	return false;
}

Vector2 K3D12::InputManager::GetMousePos()
{
	return _mousePos;
}


bool K3D12::InputManager::IsDown(K3D12::VIRTUAL_KEY_STATE code)
{

	if (_key[code] & 0x80) {
		return true;
	}
	return false;
}

void K3D12::InputManager::Discard()
{

}

void K3D12::InputManager::GamePadInitialize(int cotrollerNum)
{
	_gamePadManager.Initialize(cotrollerNum);
}

K3D12::GamePadManager & K3D12::InputManager::GetGamePadManager()
{
	return _gamePadManager;
}

std::shared_ptr<K3D12::GamePad> K3D12::InputManager::GetGamePad(int controllerID)
{
	return _gamePadManager.GetPad(controllerID);
}
