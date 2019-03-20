#include <iostream>
#include <string>
#include "GamePad.h"
#include "../Util/Math.h"
#include "../Util/Utility.h"

constexpr unsigned char TRIGGER_DEAD_LINE_OFFSET = 128;
constexpr unsigned short STICK_DEAD_LINE_OFFSET = 500;

K3D12::GamePad::GamePad() :
	_gamePadID(-1), _righrStickDeadline(XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE + STICK_DEAD_LINE_OFFSET), _leftStickDeadline(XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE + STICK_DEAD_LINE_OFFSET),
	_rightTriggerDeadline(XINPUT_GAMEPAD_TRIGGER_THRESHOLD + TRIGGER_DEAD_LINE_OFFSET), _leftTriggerDeadline(XINPUT_GAMEPAD_TRIGGER_THRESHOLD + TRIGGER_DEAD_LINE_OFFSET), _vibration(XINPUT_VIBRATION()),
	_nowState(XINPUT_STATE()), _oldState(XINPUT_STATE()),_rightVibrationTime(0),_leftVibrationTime(0)
{
	_rightVibrationController = []() {};
	_leftVibrationController = []()	{};

	Update();

	this->_leftStickFixValueX = _nowState.Gamepad.sThumbLX;
	this->_leftStickFixValueY = _nowState.Gamepad.sThumbLY;

	this->_rightStickFixValueX = _nowState.Gamepad.sThumbRX;
	this->_rightStickFixValueY = _nowState.Gamepad.sThumbRY;


}

K3D12::GamePad::~GamePad()
{
	XINPUT_VIBRATION terminalVibration{};
	XInputSetState(this->_gamePadID, &terminalVibration);
	_rightVibrationController = []() {};
	_leftVibrationController = []() {};
}

K3D12::GamePad::GamePad(const GamePad & other)
{
	*this = other;
}

K3D12::GamePad::GamePad(GamePad && other)
{
	*this = std::move(other);
}

K3D12::GamePad & K3D12::GamePad::operator=(const GamePad & value)
{
	_gamePadID = value._gamePadID;
	_righrStickDeadline = value._righrStickDeadline;
	_leftStickDeadline = value._leftStickDeadline;
	_rightTriggerDeadline = value._rightTriggerDeadline;
	_leftTriggerDeadline = value._leftTriggerDeadline;
	_rightVibrationTime = value._rightVibrationTime;
	_leftVibrationTime = value._leftVibrationTime;
	_vibration = value._vibration;
	_nowState = value._nowState;
	_oldState = value._oldState;
	return *this;
}

K3D12::GamePad & K3D12::GamePad::operator=(GamePad && value)
{
	_gamePadID = value._gamePadID;
	_righrStickDeadline = value._righrStickDeadline;
	_leftStickDeadline = value._leftStickDeadline;
	_rightTriggerDeadline = value._rightTriggerDeadline;
	_leftTriggerDeadline = value._leftTriggerDeadline;
	_rightVibrationTime = value._rightVibrationTime;
	_leftVibrationTime = value._leftVibrationTime;
	_vibration = value._vibration;
	_nowState = value._nowState;
	_oldState = value._oldState;
	value._gamePadID = 0;
	value._righrStickDeadline = 0;
	value._leftStickDeadline = 0;
	value._rightTriggerDeadline = 0;
	value._leftTriggerDeadline = 0;
	value._rightVibrationTime = 0;
	value._leftVibrationTime = 0;
	value._vibration = XINPUT_VIBRATION();
	value._nowState = XINPUT_STATE();
	value._oldState = XINPUT_STATE();
	return *this;
}

void K3D12::GamePad::VibrationRight()
{

}

void K3D12::GamePad::VibrationStopRight()
{

}

void K3D12::GamePad::VibrationLeft()
{

}

void K3D12::GamePad::VibrationStopLeft()
{

}

unsigned short K3D12::GamePad::GetPadKeyCode(GAME_PAD button)
{

	switch (button)
	{
	case K3D12::GAME_PAD::DPAD_UP:
		return 0x00000001;
		break;
	case K3D12::GAME_PAD::DPAD_DOWN:
		return 0x00000002;
		break;
	case K3D12::GAME_PAD::DPAD_LEFT:
		return 0x00000004;
		break;
	case K3D12::GAME_PAD::DPAD_RIGHT:
		return 0x00000008;
		break;
	case K3D12::GAME_PAD::START:
		return 0x00000010;
		break;
	case K3D12::GAME_PAD::BACK:
		return 0x00000020;
		break;
	case K3D12::GAME_PAD::LEFT_THUMB:
		return 0x00000040;
		break;
	case K3D12::GAME_PAD::RIGHT_THUMB:
		return 0x00000080;
		break;
	case K3D12::GAME_PAD::LEFT_SHOULDER:
		return 0x0100;
		break;
	case K3D12::GAME_PAD::RIGHT_SHOULDER:
		return 0x0200;
		break;
	case K3D12::GAME_PAD::A:
		return 0x1000;
		break;
	case K3D12::GAME_PAD::B:
		return 	0x2000;
		break;
	case K3D12::GAME_PAD::X:
		return 0x4000;
		break;
	case K3D12::GAME_PAD::Y:
		return 0x8000;
		break;
	default:
		break;
	}
	return 0;
}

std::string K3D12::GamePad::GetPadKeyName(GAME_PAD button)
{
	switch (button)
	{
	case K3D12::GAME_PAD::DPAD_UP:
		return "UP";
		break;
	case K3D12::GAME_PAD::DPAD_DOWN:
		return "DOWN";
		break;
	case K3D12::GAME_PAD::DPAD_LEFT:
		return "LEFT";
		break;
	case K3D12::GAME_PAD::DPAD_RIGHT:
		return "RIGHT";
		break;
	case K3D12::GAME_PAD::START:
		return "START";
		break;
	case K3D12::GAME_PAD::BACK:
		return "BACK";
		break;
	case K3D12::GAME_PAD::LEFT_THUMB:
		return "LEFT_THUMB";
		break;
	case K3D12::GAME_PAD::RIGHT_THUMB:
		return "RIGHT_THUMB";
		break;
	case K3D12::GAME_PAD::LEFT_SHOULDER:
		return "LEFT_SHOULDER";
		break;
	case K3D12::GAME_PAD::RIGHT_SHOULDER:
		return "RIGHT_SHOULDER";
		break;
	case K3D12::GAME_PAD::A:
		return "A";
		break;
	case K3D12::GAME_PAD::B:
		return "B";
		break;
	case K3D12::GAME_PAD::X:
		return "X";
		break;
	case K3D12::GAME_PAD::Y:
		return "Y";
		break;
	default:
		break;
	}
	return 0;
}

void K3D12::GamePad::Update()
{
	_oldState = _nowState;
	XInputGetState(this->_gamePadID, &this->_nowState);
	_rightVibrationController();
	_leftVibrationController();
}

void K3D12::GamePad::SetControllerID(int id)
{
	this->_gamePadID = id;
}

void K3D12::GamePad::SetUp(int id)
{
	this->_gamePadID = XInputGetState(id, &this->_nowState);
}

void K3D12::GamePad::ButtonDebug()
{
	
	GAME_PAD::DPAD_UP;
	IsTriggerDownButton(GAME_PAD::DPAD_UP);
	IsTriggerUpButton(GAME_PAD::DPAD_UP);
	IsDownButton(GAME_PAD::DPAD_UP);
	GAME_PAD::DPAD_DOWN;
	IsTriggerDownButton(GAME_PAD::DPAD_DOWN);
	IsTriggerUpButton(GAME_PAD::DPAD_DOWN);
	IsDownButton(GAME_PAD::DPAD_DOWN);
	GAME_PAD::DPAD_LEFT;
	IsTriggerDownButton(GAME_PAD::DPAD_LEFT);
	IsTriggerUpButton(GAME_PAD::DPAD_LEFT);
	IsDownButton(GAME_PAD::DPAD_LEFT);
	GAME_PAD::DPAD_RIGHT;
	IsTriggerDownButton(GAME_PAD::DPAD_RIGHT);
	IsTriggerUpButton(GAME_PAD::DPAD_RIGHT);
	IsDownButton(GAME_PAD::DPAD_RIGHT);
	GAME_PAD::START;
	IsTriggerDownButton(GAME_PAD::START);
	IsTriggerUpButton(GAME_PAD::START);
	IsDownButton(GAME_PAD::START);
	GAME_PAD::BACK;
	IsTriggerDownButton(GAME_PAD::BACK);
	IsTriggerUpButton(GAME_PAD::BACK);
	IsDownButton(GAME_PAD::BACK);
	GAME_PAD::LEFT_THUMB;
	IsTriggerDownButton(GAME_PAD::LEFT_THUMB);
	IsTriggerUpButton(GAME_PAD::LEFT_THUMB);
	IsDownButton(GAME_PAD::LEFT_THUMB);
	GAME_PAD::RIGHT_THUMB;
	IsTriggerDownButton(GAME_PAD::RIGHT_THUMB);
	IsTriggerUpButton(GAME_PAD::RIGHT_THUMB);
	IsDownButton(GAME_PAD::RIGHT_THUMB);
	GAME_PAD::LEFT_SHOULDER;
	IsTriggerDownButton(GAME_PAD::LEFT_SHOULDER);
	IsTriggerUpButton(GAME_PAD::LEFT_SHOULDER);
	IsDownButton(GAME_PAD::LEFT_SHOULDER);
	GAME_PAD::RIGHT_SHOULDER;
	IsTriggerDownButton(GAME_PAD::RIGHT_SHOULDER);
	IsTriggerUpButton(GAME_PAD::RIGHT_SHOULDER);
	IsDownButton(GAME_PAD::RIGHT_SHOULDER);
	GAME_PAD::A;
	IsTriggerDownButton(GAME_PAD::A);
	IsTriggerUpButton(GAME_PAD::A);
	IsDownButton(GAME_PAD::A);
	GAME_PAD::B;
	IsTriggerDownButton(GAME_PAD::B);
	IsTriggerUpButton(GAME_PAD::B);
	IsDownButton(GAME_PAD::B);
	GAME_PAD::X;
	IsTriggerDownButton(GAME_PAD::X);
	IsTriggerUpButton(GAME_PAD::X);
	IsDownButton(GAME_PAD::X);
	GAME_PAD::Y;
	IsTriggerDownButton(GAME_PAD::Y);
	IsTriggerUpButton(GAME_PAD::Y);
	IsDownButton(GAME_PAD::Y);

}

void K3D12::GamePad::AnalogDebug()
{

}

bool K3D12::GamePad::IsTriggerDownButton(K3D12::GAME_PAD keycode)
{
	if (((_oldState.Gamepad.wButtons & GetPadKeyCode(keycode)) == 0) && ((_nowState.Gamepad.wButtons & GetPadKeyCode(keycode)) >= 1)) {
#ifdef _DEBUG
		std::cout << GetPadKeyName(keycode) << "IsTriggerDown" << std::endl;
#endif // _DEBUG
		return true;
	}
	return false;
}

bool K3D12::GamePad::IsTriggerUpButton(K3D12::GAME_PAD keycode)
{
	if (((_oldState.Gamepad.wButtons & GetPadKeyCode(keycode)) >= 1) && ((_nowState.Gamepad.wButtons & GetPadKeyCode(keycode)) == 0)) {
#ifdef _DEBUG
		std::cout << GetPadKeyName(keycode) << "IsTriggerUP" << std::endl;
#endif
		return true;
	}
	return false;
}

bool K3D12::GamePad::IsDownButton(K3D12::GAME_PAD keycode)
{
	if (_nowState.Gamepad.wButtons & GetPadKeyCode(keycode)) {
#ifdef _DEBUG
		std::cout << GetPadKeyName(keycode) << "Is Down" << std::endl;
#endif
		return true;
	}
	return false;
}

void K3D12::GamePad::SetRightStickDeadline(unsigned short deadline)
{
	_righrStickDeadline = deadline;
}

void K3D12::GamePad::SetLeftStickDeadline(unsigned short deadline)
{
	_leftStickDeadline = deadline;
}

void K3D12::GamePad::SetRightTriggerDeadline(unsigned char deadline)
{
	_rightTriggerDeadline = deadline;
}

void K3D12::GamePad::SetLeftTriggerDeadline(unsigned char deadline)
{
	_leftTriggerDeadline = deadline;
}

void K3D12::GamePad::VibrationRightMotor(unsigned short frequency, unsigned int  time)
{
	_vibration.wRightMotorSpeed = frequency;
	XInputSetState(this->_gamePadID, &_vibration);
	_rightVibrationController = [this,time]()->void {
		_rightVibrationTime += 1;
		if (time<= _rightVibrationTime) {
			_rightVibrationTime = 0;
			//ゼロにセット
			VibrationRightMotor();
			_rightVibrationController = []() {};
		};
	};
}

void K3D12::GamePad::VibrationLeftMotor(unsigned short frequency, unsigned int time)
{
	_vibration.wLeftMotorSpeed = frequency;
	XInputSetState(this->_gamePadID, &_vibration);
	_leftVibrationController = [this, time]()->void {
		_leftVibrationTime += 1;
		if (time <= _leftVibrationTime) {
			_leftVibrationTime = 0;
			//ゼロにセット
			VibrationLeftMotor();
			_leftVibrationController = []() {};
		};
	};

}

Vector2 K3D12::GamePad::GetSaturatedRightStick()
{
	float x = ::Clamp(static_cast<float>(_nowState.Gamepad.sThumbRX - _rightStickFixValueX)  / static_cast<float>(_righrStickDeadline - _rightStickFixValueX), -1.0f, 1.0f);
	float y = ::Clamp(static_cast<float>(_nowState.Gamepad.sThumbRY - _rightStickFixValueY) / static_cast<float>(_righrStickDeadline - _rightStickFixValueY),-1.0f,1.0f);
#ifdef _DEBUG
	std::cout << x << std::endl;
	std::cout << y << std::endl;
#endif // _DEBUG
	return Vector2(x, y);
}

Vector2 K3D12::GamePad::GetSaturatedLeftStick()
{
	float x = ::Clamp(static_cast<float>(_nowState.Gamepad.sThumbLX - _leftStickFixValueX) / static_cast<float>(_leftStickDeadline - _leftStickFixValueX), -1.0f, 1.0f);
	float y = ::Clamp(static_cast<float>(_nowState.Gamepad.sThumbLY - _leftStickFixValueY) / static_cast<float>(_leftStickDeadline - _leftStickFixValueY),	-1.0f,1.0f);
#ifdef _DEBUG
	std::cout << x << std::endl;
	std::cout << y << std::endl;
#endif // _DEBUG
	return Vector2(x, y);
}

float K3D12::GamePad::GetSaturatedRT()
{
	float st = ::Clamp(static_cast<float>(_nowState.Gamepad.bRightTrigger) / static_cast<float>(_rightTriggerDeadline), -1.0f, 1.0f);
#ifdef _DEBUG
	std::cout << st << std::endl;
#endif // _DEBUG
	return st;
}

float K3D12::GamePad::GetSaturatedLT()
{
	float st = ::Clamp(static_cast<float>(_nowState.Gamepad.bLeftTrigger) / static_cast<float>(_leftTriggerDeadline), -1.0f, 1.0f);
#ifdef _DEBUG
	std::cout << st << std::endl;
#endif // _DEBUG
	return st;
}


