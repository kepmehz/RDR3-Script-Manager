#pragma once

#include <windows.h>

#define IMPORT __declspec(dllimport)

IMPORT int createTexture(const char *texFileName);

IMPORT void drawTexture(int id, int index, int level, int time,
	float sizeX, float sizeY, float centerX, float centerY,
	float posX, float posY, float rotation, float screenHeightScaleFactor,
	float r, float g, float b, float a);

typedef void(*PresentCallback)(void *);

IMPORT void presentCallbackRegister(PresentCallback cb);

IMPORT void presentCallbackUnregister(PresentCallback cb);

typedef void(*KeyboardHandler)(DWORD, WORD, BYTE, BOOL, BOOL, BOOL, BOOL);

IMPORT void keyboardHandlerRegister(KeyboardHandler handler);

IMPORT void keyboardHandlerUnregister(KeyboardHandler handler);

IMPORT void scriptWait(DWORD time);
IMPORT void scriptRegister(HMODULE module, void(*LP_SCRIPT_MAIN)());
IMPORT void scriptRegisterAdditionalThread(HMODULE module, void(*LP_SCRIPT_MAIN)());
IMPORT void scriptUnregister(HMODULE module);
IMPORT void scriptUnregister(void(*LP_SCRIPT_MAIN)()); // deprecated

IMPORT void nativeInit(UINT64 hash);
IMPORT void nativePush64(UINT64 val);
IMPORT PUINT64 nativeCall();

static void WAIT(DWORD time) { scriptWait(time); }
static void TERMINATE() { WAIT(MAXDWORD); }

IMPORT UINT64 *getGlobalPtr(int globalId);

IMPORT int worldGetAllVehicles(int *arr, int arrSize);
IMPORT int worldGetAllPeds(int *arr, int arrSize);
IMPORT int worldGetAllObjects(int *arr, int arrSize);
IMPORT int worldGetAllPickups(int *arr, int arrSize);

IMPORT BYTE *getScriptHandleBaseAddress(int handle);

enum eGameVersion : int
{
	v1_0_335_2_STEAM,
	v1_0_335_2_NOSTEAM,

	v1_0_350_1_STEAM,
	v1_0_350_2_NOSTEAM,

	v1_0_372_2_STEAM,
	v1_0_372_2_NOSTEAM,

	v1_0_393_2_STEAM,
	v1_0_393_2_NOSTEAM,

	v1_0_393_4_STEAM,
	v1_0_393_4_NOSTEAM,

	v1_0_463_1_STEAM,
	v1_0_463_1_NOSTEAM,

	v1_0_505_2_STEAM,
	v1_0_505_2_NOSTEAM,

	v1_0_573_1_STEAM,
	v1_0_573_1_NOSTEAM,

	v1_0_617_1_STEAM,
	v1_0_617_1_NOSTEAM,

	v1_0_678_1_STEAM,
	v1_0_678_1_NOSTEAM,

	v1_0_757_2_STEAM,
	v1_0_757_2_NOSTEAM,

	v1_0_757_3_STEAM,
	v1_0_757_4_NOSTEAM,

	v1_0_791_2_STEAM,
	v1_0_791_2_NOSTEAM,

	v1_0_877_1_STEAM,
	v1_0_877_1_NOSTEAM,

	v1_0_944_2_STEAM,
	v1_0_944_2_NOSTEAM,

	v1_0_1011_1_STEAM,
	v1_0_1011_1_NOSTEAM,

	v1_0_1032_1_STEAM,
	v1_0_1032_1_NOSTEAM,

	v1_0_1103_2_STEAM,
	v1_0_1103_2_NOSTEAM,

	v1_0_1180_2_STEAM,
	v1_0_1180_2_NOSTEAM,

	v1_0_1290_1_STEAM,
	v1_0_1290_1_NOSTEAM,

	VER_SIZE,
	VER_UNK = -1
};

IMPORT eGameVersion getGameVersion();
