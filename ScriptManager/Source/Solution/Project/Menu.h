#pragma once

extern Vector2 menuPos;
extern float OptionY;

extern RGBA* settings_rgba;
extern RGBA titlebox;
extern RGBA BG;
extern RGBA optiontext;
extern RGBA selectedtext;
extern RGBA optionbreaks;
extern RGBA selectionhi;

extern bool menuOpened;

class menu
{
public:

	static int currentSub;
	static int currentOp;
	static int currentOpwBreaks;
	static int totalOp;
	static int printingOp;
	static int breakCount;
	static int totalBreaks;
	static int breakScroll;
	static int currentSubArIndex;
	static int currentSubAr[100];
	static int currentOpAr[100];
	static int setSubDelayed;
	static int liveTimer;

	static void loops();
	static void sub_handler();
	static void submenu_switch();

	static void DisableControls();
	static void base();
	static void background();
	static void optionhi();
	static bool isBinds();
	static void while_closed();
	static void while_opened();
	static void Up();
	static void Down();
	static void Bottom();
	static void Top();
	static void SetSub_previous();
	static void SetSub_new(int sub_index);
	static void SetSub_closed();
};

extern bool null;
void nullFunc();

extern float NumberKeyboard();
extern int NumKeyboard();
extern const char* TextKeyboard();

void AddTitle(const std::string& title);
void AddOption(const std::string& option, bool& option_code_bool = null, std::function<void()> func = [] {}, int submenu_index = -1, bool show_arrow = 0);
void AddToggle(const std::string& option, bool& loop_variable, std::function<void()> func = [] {});
void AddBreak(const std::string& text);
void AddNumber(const std::string& option, int& value, int min, int max, int step = 1, std::function<void()> func = [] {});
void AddNumber(const std::string& option, float& value, float min, float max, float step = 0.1f, std::function<void()> func = [] {});
void AddTele(const std::string& option, float X, float Y, float Z);
