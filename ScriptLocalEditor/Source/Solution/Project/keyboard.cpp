/*
	THIS FILE IS A PART OF RDR 2 SCRIPT HOOK SDK
				http://dev-c.com
			(C) Alexander Blade 2019
*/
#include "script.h"

const int KEYS_SIZE = 255;
const int NOW_PERIOD = 100, MAX_DOWN = 5000; // ms

struct {
	ULONGLONG time;
	BOOL isWithAlt;
	BOOL wasDownBefore;
	BOOL isUpNow;
} keyStates[KEYS_SIZE];

void OnKeyboardMessage(DWORD key, WORD repeats, BYTE scanCode, BOOL isExtended, BOOL isWithAlt, BOOL wasDownBefore, BOOL isUpNow)
{
	if (key < KEYS_SIZE)
	{
		keyStates[key].time = GetTickCount64();
		keyStates[key].isWithAlt = isWithAlt;
		keyStates[key].wasDownBefore = wasDownBefore;
		keyStates[key].isUpNow = isUpNow;
	}
}

bool IsKeyDown(DWORD key)
{
	return (key < KEYS_SIZE) ? ((GetTickCount64() < keyStates[key].time + MAX_DOWN) && !keyStates[key].isUpNow) : false;
}

bool IsKeyJustUp(DWORD key, bool exclusive)
{
	bool b = (key < KEYS_SIZE) ? (GetTickCount64() < keyStates[key].time + NOW_PERIOD && keyStates[key].isUpNow) : false;
	if (b && exclusive)
		ResetKeyState(key);
	return b;
}

void ResetKeyState(DWORD key)
{
	if (key < KEYS_SIZE)
		std::memset(&keyStates[key], 0, sizeof(keyStates[0]));
}

namespace MenuPressTimer
{
	MenuPressTimer::Button currentButton = { MenuPressTimer::Button::None };
	ULONGLONG offsettedTime = 0;

	void Update()
	{
		if (currentButton == Button::None) {
			offsettedTime = GetTickCount64() + 630;
		}

		if (PAD::IS_DISABLED_CONTROL_PRESSED(2, INPUT_FRONTEND_RIGHT) || IsKeyDown(VirtualKey::Numpad6)) {
			currentButton = Button::Right;
		}
		else if (PAD::IS_DISABLED_CONTROL_PRESSED(2, INPUT_FRONTEND_LEFT) || IsKeyDown(VirtualKey::Numpad4)) {
			currentButton = Button::Left;
		}
		else if (PAD::IS_DISABLED_CONTROL_PRESSED(2, INPUT_FRONTEND_DOWN) || IsKeyDown(VirtualKey::Numpad2)) {
			currentButton = Button::Down;
		}
		else if (PAD::IS_DISABLED_CONTROL_PRESSED(2, INPUT_FRONTEND_UP) || IsKeyDown(VirtualKey::Numpad8)) {
			currentButton = Button::Up;
		}
		else {
			currentButton = Button::None;
			offsettedTime = 0;
		}
	}

	bool IsButtonHeld(const MenuPressTimer::Button& button) {
		ULONGLONG tickCount = GetTickCount64();
		return (currentButton == button && offsettedTime < tickCount);
	}

	bool IsButtonTapped(const MenuPressTimer::Button & button) {
		bool isTapped = true;
		switch (button)
		{
		case Button::Right: isTapped = PAD::IS_DISABLED_CONTROL_JUST_PRESSED(2, INPUT_FRONTEND_RIGHT) || IsKeyJustUp(VirtualKey::Numpad6); break;
		case Button::Left: isTapped = PAD::IS_DISABLED_CONTROL_JUST_PRESSED(2, INPUT_FRONTEND_LEFT) || IsKeyJustUp(VirtualKey::Numpad4); break;
		case Button::Down: isTapped = PAD::IS_DISABLED_CONTROL_JUST_PRESSED(2, INPUT_FRONTEND_DOWN) || IsKeyJustUp(VirtualKey::Numpad2); break;
		case Button::Up: isTapped = PAD::IS_DISABLED_CONTROL_JUST_PRESSED(2, INPUT_FRONTEND_UP) || IsKeyJustUp(VirtualKey::Numpad8); break;
		case Button::Back: isTapped = PAD::IS_DISABLED_CONTROL_JUST_PRESSED(2, INPUT_FRONTEND_RRIGHT) || IsKeyJustUp(VirtualKey::Numpad0); break;
		case Button::Accept: isTapped = PAD::IS_DISABLED_CONTROL_JUST_PRESSED(2, INPUT_FRONTEND_ACCEPT) || IsKeyJustUp(VirtualKey::Numpad5); break;
		}
		return isTapped;
	}

	bool IsButtonHeldOrTapped(const MenuPressTimer::Button & button) {
		return IsButtonHeld(button) || IsButtonTapped(button);
	}
}