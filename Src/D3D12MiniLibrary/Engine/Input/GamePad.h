#pragma once
#include <Windows.h>
#include <functional>
#include <Xinput.h>
#pragma comment(lib,"xinput.lib")
#include "../Util/Math.h"
//Xinputは同時に四つまで接続できる。Xinputはすべて個別IDが必要。
//dwUserIndex の値 0 は左上の象限に対応し、そこから時計回りに数値が大きくなります。


namespace K3D12 {
	enum class GAME_PAD {

		DPAD_UP = 0x0001,
		DPAD_DOWN = 0x0002,
		DPAD_LEFT = 0x0004,
		DPAD_RIGHT = 0x0008,
		START = 0x0010,
		BACK = 0x0020,
		LEFT_THUMB = 0x0040,
		RIGHT_THUMB = 0x0080,
		LEFT_SHOULDER = 0x0100,
		RIGHT_SHOULDER = 0x0200,
		A = 0x1000,
		B = 0x2000,
		X = 0x4000,
		Y = 0x8000,
		RT = 0x0400,
		LT = 0x0800,
		R_STICK = 0x1001,
		L_STICK = 0x1002,
	};

	class GamePad
	{
	private:
		int					_gamePadID;

		unsigned short		_righrStickDeadline;
		unsigned short		_leftStickDeadline;

		short				_rightStickFixValueX;
		short				_rightStickFixValueY;
		short				_leftStickFixValueX;
		short				_leftStickFixValueY;

		unsigned char		_rightTriggerDeadline;
		unsigned char		_leftTriggerDeadline;

		unsigned char		_rightTriggerFixValue;
		unsigned char		_leftTriggerFixValue;

		unsigned int		_rightVibrationTime;
		unsigned int		_leftVibrationTime;

		std::function<void()> _rightVibrationController;
		std::function<void()> _leftVibrationController;

		XINPUT_VIBRATION	_vibration;
		XINPUT_STATE		_nowState;
		XINPUT_STATE		_oldState;

	public:

	private:
		void VibrationRight();
		void VibrationStopRight();
		void VibrationLeft();
		void VibrationStopLeft();

		unsigned short GetPadKeyCode(GAME_PAD button);
		std::string GetPadKeyName(GAME_PAD button);
	public:
		void Update();
		void SetControllerID(int id);
		void SetUp(int id);
		void ButtonDebug();
		void AnalogDebug();
		bool IsTriggerDownButton(GAME_PAD keycode);
		bool IsTriggerUpButton(GAME_PAD keycode);
		bool IsDownButton(GAME_PAD keycode);

		void SetRightStickDeadline(unsigned short deadline);
		void SetLeftStickDeadline(unsigned short deadline);
		void SetRightTriggerDeadline(unsigned char deadline);
		void SetLeftTriggerDeadline(unsigned char deadline);
		void VibrationRightMotor(unsigned short frequency = 0, unsigned int frame = 0);
		void VibrationLeftMotor(unsigned short frequency = 0, unsigned int frame = 0);

		Vector2 GetSaturatedRightStick();
		Vector2 GetSaturatedLeftStick();

		float  GetSaturatedRT();
		float  GetSaturatedLT();

		GamePad();
		~GamePad();
		GamePad(const GamePad& other);
		GamePad(GamePad&& other);
		GamePad& operator=(const  GamePad& value);
		GamePad& operator=(GamePad&& value);
	};
}