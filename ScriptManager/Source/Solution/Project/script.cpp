#include "script.h"

namespace
{
	// Declare/Initialise global variables here.
	int sampleHour = 0;
	float currentTimescale = 1.0f;
	int testInt = 5;

	// Booleans for loops go here:
	bool playerInvincibility = false, playerInvisible = false;
}

namespace sub {
	// Define submenus here.

	void MainMenu()
	{
		AddTitle("MAIN MENU");
		AddOption("Sample Self", null, nullFunc, SUB::SAMPLE);
		AddOption("Sample Teleport", null, nullFunc, SUB::SAMPLETELEPORT);
		AddOption("Sample World", null, nullFunc, SUB::SAMPLEWORLD);
		AddOption("Settings", null, nullFunc, SUB::SETTINGS);

		if (menuOpened)
		{
			Game::Print::PrintStringBottomCentre("Menyea Baes for RDR2"); // Your opening message goes here.
			menuOpened = false;
		}
	}

	void SampleSelf()
	{
		AddTitle("SAMPLE");
		AddToggle("Invincibility", playerInvincibility, [] { PLAYER::SET_PLAYER_INVINCIBLE(PLAYER::PLAYER_ID(), playerInvincibility); });
		AddToggle("Invisibility", playerInvisible, [] { ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), !playerInvisible); });
		AddNumber("Options", testInt, 0, 255);
		AddBreak("Options");
		for (int i = 0; i <= testInt; i++)
		{
			AddOption("Option " + std::to_string(i));
		}
	}

	void SampleTeleport()
	{
		AddTitle("TELEPORT");
		AddTele("Valentine", -213.152496f, 691.802979f, 112.37100f);
		AddTele("Annesburg", 2898.593994f, 1239.85253f, 44.073299f);
		AddTele("Saint Denis", 2336.584961f, -1106.2358f, 44.737598f);
		AddTele("Blackwater", -798.338379f, -1238.9395f, 43.537899f);
	}

	void SampleWorld()
	{
		sampleHour = CLOCK::GET_CLOCK_HOURS();
		AddTitle("WORLD");
		AddNumber("Hour", sampleHour, 0, 24, 1, [] { CLOCK::SET_CLOCK_TIME(sampleHour, 0, 0); });
		AddNumber("Time Scale", currentTimescale, 0.0f, 1.0f, 0.1f, [] { MISC::SET_TIME_SCALE(currentTimescale); });
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
	case SUB::SAMPLE:					sub::SampleSelf(); break;
	case SUB::SAMPLEWORLD:				sub::SampleWorld(); break;
	case SUB::SAMPLETELEPORT:			sub::SampleTeleport(); break;
	}
}

rage::atArray<rage::scrThread*>* m_ThreadMap{};

rage::scrThread* GetThread(uint32_t hash)
{
	for (const auto& thread : *m_ThreadMap)
	{
		if (thread && thread->m_scriptHash == hash && thread->m_context.m_threadState == rage::running)
		{
			return thread;
		}
	}
	return nullptr;
}

void allocateDebugConsole()
{
	AllocConsole();
	SetConsoleTitleA("Debug");
	FILE* pCout;
	freopen_s(&pCout, "CONOUT$", "w", stdout);
	freopen_s(&pCout, "CONOUT$", "w", stderr);
	std::cout.clear();
	std::clog.clear();
	std::cerr.clear();
}

void init()
{
	allocateDebugConsole();
	auto sc = new scanner(nullptr);
	m_ThreadMap = sc->scan("45 33 F6 85 C9 BD").Sub(4).Rip().Sub(8).As<decltype(m_ThreadMap)>();
	if (m_ThreadMap)
		std::cout << "Found thread map" << std::endl;
}

void main()
{

	srand(static_cast<unsigned int>(GetTickCount64()));
	init();
	for(;;)
	{
		if (auto testThread = GetThread(MISC::GET_HASH_KEY("startup")))
		{
			std::cout << "startup hash" << testThread->m_scriptHash <<std::endl;
		}
		
		menu::base();
		menu::loops();
		menu::sub_handler();
		scriptWait(0);
	}
}


