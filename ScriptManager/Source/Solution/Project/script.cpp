#include "script.h"
#include "list.hpp"

namespace
{
	// Declare/Initialise global variables here.
	int sampleHour = 0;
	float currentTimescale = 1.0f;
	int testInt = 5;
	int selectedScripts;
	int sScriptStackSize;
	std::string sScriptName = "DEFAULT";
	rage::atArray<rage::scrThread*>* m_ThreadMap{};
	std::vector<const char*> ScriptStates = { "~t2~Idle", "~t6~Running", "~e~Killed", "Unknown3", "Unknown4" };

	// Booleans for loops go here:
	bool playerInvincibility = false, playerInvisible = false;

	struct GlobalEdit
	{
		int index;

		void SetIndex(int ind) {
			index = ind;
		}

		int GetIndex() const {
			return this->index;
		}

		void Add(int offset) {
			this->index += offset;
		}

		void Add(int offset, int size) {
			Add(1 + offset * size);
		}

		uint64_t* Get() {
			return getGlobalPtr(this->index);
		}

		void Reset()
		{
			this->index = 0;
		}

		GlobalEdit() : index(0) {}
	};

	struct LocalEdit
	{
		int index;
		rage::scrThread* thread;

		void SetIndex(int ind) {
			index = ind;
		}

		void SetThread(rage::scrThread* thread) {
			this->thread = thread;
		}

		int GetIndex() const {
			return this->index;
		}

		void Add(int offset) {
			this->index += offset;
		}

		void Add(int offset, int size) {
			Add(1 + offset * size);
		}

		uint64_t* Get()
		{
			if (thread)
				return reinterpret_cast<uint64_t*>(thread->m_stack + (index * sizeof(uintptr_t)));

			static uint64_t tmp = 0;
			return &tmp;
		}

		void Reset()
		{
			this->index = 0;
			this->thread = nullptr;
		}

		LocalEdit() : index(0), thread(nullptr) {}
	};

	GlobalEdit globalEdit;
	LocalEdit localEdit;
	uint32_t threadId;
}

rage::scrThread* GetThread(uint32_t hash)
{
	for (const auto& thread : *m_ThreadMap)
	{
		if (thread && thread->m_scriptHash == hash)
		{
			return thread;
		}
	}
	return nullptr;
}

void init()
{
	auto sc = scanner(nullptr);
	m_ThreadMap = sc.scan("45 33 F6 85 C9 BD").Sub(4).Rip().Sub(8).As<decltype(m_ThreadMap)>();
}

namespace sub {
	// Define submenus here.

	void MainMenu()
	{
		AddTitle("MAIN MENU");
		AddOption("Scripts", null, nullFunc, SUB::SCRIPTLIST);
		AddOption("Global Editor", null, nullFunc, SUB::GLOBALEDITOR);
		AddOption("Local Editor", null, nullFunc, SUB::LOCALEDITOR);
		AddOption("Settings", null, nullFunc, SUB::SETTINGS);

		if (menuOpened)
		{
			Game::Print::PrintStringBottomCentre("Script Manager by kepmehz");
			menuOpened = false;
		}
	}

	void GlobalEditor()
	{
		AddTitle("GLOBAL EDITOR");
		AddOption(fmt::format("Index: {}", globalEdit.GetIndex()), null, [] 
			{
				globalEdit.SetIndex(NumKeyboard());
			});
		AddOption("Add", null, []
			{
				globalEdit.Add(NumKeyboard());
			});
		AddOption("Add Size", null, [] 
			{
				int offset = NumKeyboard();
				scriptWait(200);
				int size = NumKeyboard();
				globalEdit.Add(offset, size);
			});
		AddOption(fmt::format("State: {}", *globalEdit.Get()), null, [] 
			{
				*globalEdit.Get() = NumKeyboard();
			});
		AddOption("Reset", null, []
			{
				globalEdit.Reset();
			});
	}

	void LocalEditor()
	{
		AddTitle("LOCAL EDITOR");
		AddOption(fmt::format("Thread ID: {}", threadId), null, [] 
			{
				threadId = NumKeyboard();
				if (rage::scrThread* thread = GetThread(SCRIPTS::_GET_HASH_OF_THREAD(threadId))) 
					localEdit.SetThread(thread);
			});
		AddOption(fmt::format("Index: {}", localEdit.GetIndex()), null, []
			{
				localEdit.SetIndex(NumKeyboard());
			});
		AddOption("Add", null, []
			{
				localEdit.Add(NumKeyboard());
			});
		AddOption("Add Size", null, []
			{
				int offset = NumKeyboard();
				scriptWait(200);
				int size = NumKeyboard();
				localEdit.Add(offset, size);
			});
		AddOption(fmt::format("State: {}", *localEdit.Get()), null, []
			{
				*localEdit.Get() = NumKeyboard();
			});
		AddOption("Reset", null, []
			{
				localEdit.Reset();
				threadId = 0;
			});
	}

	void Scripts()
	{
		AddTitle("SCRIPTS");
		AddOption(fmt::format("Total Scripts {}", ScriptList.size()));
		AddOption("Start Script", null, nullFunc, SUB::START_SCRIPT);
		for (int i = 0; i < ScriptList.size(); i++)
		{
			ScriptEntry script = ScriptList.at(i);

			if (rage::scrThread* thread = GetThread(script.hash))
			{	
				AddOption
				(
					fmt::format("[{}~s~] {}", ScriptStates.at(thread->m_context.m_threadState), script.name), null, [=]
					{
						selectedScripts = i;
					}, SUB::SELECTED_SCRIPT
				);
			}
		}
	}

	void StartScript()
	{
		AddTitle("START SCRIPTS");
		AddOption(fmt::format("Name: {}", sScriptName), null, [] 
			{
				sScriptName = TextKeyboard();
			});
		AddOption(fmt::format("Stack Size: {}", sScriptStackSize), null, []
			{
				sScriptStackSize = NumKeyboard();
			});
		AddOption("Start", null, []
			{
				while (!SCRIPTS::HAS_SCRIPT_LOADED(sScriptName.c_str()))
				{
					SCRIPTS::REQUEST_SCRIPT(sScriptName.c_str());
					scriptWait(0);
				}
				SCRIPTS::START_NEW_SCRIPT(sScriptName.c_str(), sScriptStackSize);
				Game::Print::PrintStringBottomCentre(fmt::format("Started Script: {} with Size: {}", sScriptName, sScriptStackSize).c_str());
			});
		AddOption("Reset", null, [] 
			{
				sScriptName = "DEFAULT";
				sScriptStackSize = 0;
			});
	}

	void SelectedScript()
	{
		ScriptEntry script = ScriptList.at(selectedScripts);
		if (rage::scrThread* thread = GetThread(script.hash))
		{
			AddTitle(script.name);
			AddOption(fmt::format("State: {}", ScriptStates.at(thread->m_context.m_threadState)));
			AddOption(fmt::format("Thread Id: {}", thread->m_context.m_threadId));
			AddOption(fmt::format("Stack Size: {}", thread->m_context.m_stackSize));
			AddOption("~r~Kill", null, [=]
			{
				thread->m_context.m_threadState = rage::killed;
				menu::SetSub_previous();
			});
		}
	}

	void Settings()
	{
		AddTitle("MENU SETTINGS");
		AddOption("Menu Colors", null, nullFunc, SUB::SETTINGS_COLORS);
		AddNumber("Menu X", menuPos.x, 0.0f, 0.8f, 0.01f);
		AddNumber("Menu Y", menuPos.y, 0.0f, 0.9f, 0.01f);
	}

	void AddsettingscolOption(char* text, RGBA& feature)
	{
		AddOption(text, null, nullFunc, SUB::SETTINGS_COLORS2);
		if (menu::printingOp == menu::currentOp) settings_rgba = &feature;
	}

	void SettingsColours()
	{
		AddTitle("COLORS");
		AddsettingscolOption("Title Box", titlebox);
	}

	void SettingsColours2()
	{
		AddTitle("TITLE BOX");
		AddNumber("Red", settings_rgba->R, 0, 255);
		AddNumber("Green", settings_rgba->G, 0, 255);
		AddNumber("Blue", settings_rgba->B, 0, 255);
		AddNumber("Opacity", settings_rgba->A, 0, 255);
	}
}


void menu::loops()
{ 
	/*	Make calls to functions that you want looped over here, e.g. ambient lights, whale guns, explosions, checks, flying deers, etc.
		Can also be used for (bool) options that are to be executed from many parts of the script. */
	if (playerInvincibility)
	{
		if (!PLAYER::GET_PLAYER_INVINCIBLE(PLAYER::PLAYER_ID()))
			PLAYER::SET_PLAYER_INVINCIBLE(PLAYER::PLAYER_ID(), playerInvincibility);
	}
}

void menu::submenu_switch()
{ // Make calls to submenus over here.

	switch (currentSub)
	{
	case SUB::MAINMENU:					sub::MainMenu(); break;
	case SUB::SETTINGS:					sub::Settings(); break;
	case SUB::SETTINGS_COLORS:			sub::SettingsColours(); break;
	case SUB::SETTINGS_COLORS2:			sub::SettingsColours2(); break;
	case SUB::GLOBALEDITOR:				sub::GlobalEditor(); break;
	case SUB::LOCALEDITOR:				sub::LocalEditor(); break;
	case SUB::SCRIPTLIST:				sub::Scripts(); break;
	case SUB::START_SCRIPT:				sub::StartScript(); break;
	case SUB::SELECTED_SCRIPT:			sub::SelectedScript(); break;
	}
}

void main()
{

	srand(static_cast<unsigned int>(GetTickCount64()));
	init();
	for(;;)
	{	
		menu::base();
		menu::loops();
		menu::sub_handler();
		scriptWait(0);
	}
}


