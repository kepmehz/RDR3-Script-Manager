#pragma once

#include <string>

namespace Game
{
	namespace Print
	{
		void setupdraw();
		void drawstring(std::string string, float X, float Y);
		void drawtitlestring(std::string string, float X, float Y);
		void drawinteger(int value, float X, float Y);
		void drawfloat(float value, float X, float Y);

		void PrintStringBottomCentre(const char* string);
	}

	namespace Sound
	{
		void PlaySoundFrontend(char* sound_dict, char* sound_name);
		void PlaySoundFrontend_default(char* sound_name);
	}

	namespace Misc
	{
		extern uint32_t joaat(const char* str);
	}
}