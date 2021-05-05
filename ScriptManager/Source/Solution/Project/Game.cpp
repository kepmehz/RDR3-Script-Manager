#include "script.h"

namespace Game
{
	namespace Print
	{
		void setupdraw()
		{
			HUD::SET_TEXT_CENTRE(0);
			HUD::_SET_TEXT_COLOR(255, 255, 255, 255);
			HUD::SET_TEXT_SCALE(0.3f, 0.3f);
		}

		void drawtitlestring(std::string string, float X, float Y)
		{
			HUD::SET_TEXT_DROPSHADOW(1, 0, 0, 0, 0);
			std::string Template = "<FONT FACE='$title1' COLOR='#%04X'>" + string + "</FONT>";
			const char* InsertedTemplate = MISC::VAR_STRING(10, "LITERAL_STRING", _strdup(Template.c_str()));
			HUD::_DISPLAY_TEXT(MISC::VAR_STRING(42, "COLOR_STRING", 0, InsertedTemplate), X, Y);
		}

		void drawstring(std::string string, float X, float Y)
		{
			std::string Template = "<FONT FACE='$body' COLOR='#%04X'>" + string + "</FONT>";
			const char* InsertedTemplate = MISC::VAR_STRING(10, "LITERAL_STRING", _strdup(Template.c_str()));
			HUD::_DISPLAY_TEXT(MISC::VAR_STRING(42, "COLOR_STRING", 0, InsertedTemplate), X, Y);
		}

		void drawinteger(int value, float X, float Y)
		{
			drawstring(std::to_string(value), X, Y);
		}

		void drawfloat(float value, float X, float Y)
		{
			char buf[20];
			std::snprintf(buf, sizeof(buf), "%0.2f", value);
			drawstring(static_cast<std::string>(buf), X, Y);
		}

		void PrintStringBottomCentre(const char* string)
		{
			const char* literalString = MISC::VAR_STRING(10, "LITERAL_STRING", string);
			UILOG::_UILOG_SET_CACHED_OBJECTIVE(literalString);
			UILOG::_UILOG_PRINT_CACHED_OBJECTIVE();
			UILOG::_UILOG_CLEAR_CACHED_OBJECTIVE();
		}
	}

	namespace Sound
	{
		void PlaySoundFrontend(char* sound_dict, char* sound_name)
		{
			AUDIO::PLAY_SOUND_FRONTEND(sound_name, sound_dict, 0, 0);
		}

		void PlaySoundFrontend_default(char* sound_name)
		{
			AUDIO::PLAY_SOUND_FRONTEND(sound_name, "HUD_PLAYER_MENU", 0, 0);
		}
	}

	namespace Misc
	{
		uint32_t joaat(const char* str)
		{
			uint32_t hash = 0;

			while (*str)
			{
				hash += ::tolower(*(str++));
				hash += (hash << 10);
				hash ^= (hash >> 6);
			}

			hash += (hash << 3);
			hash ^= (hash >> 11);
			hash += (hash << 15);

			return hash;
		}
	}
}