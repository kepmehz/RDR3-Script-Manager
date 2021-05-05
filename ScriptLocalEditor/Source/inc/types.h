#pragma once

#include <windows.h>

typedef DWORD Void;
typedef DWORD Any;
typedef DWORD uint;
typedef DWORD Hash;
typedef int Entity;
typedef int Player;
typedef int FireId;
typedef int Ped;
typedef int Vehicle;
typedef int Cam;
typedef int CarGenerator;
typedef int Group;
typedef int Train;
typedef int Pickup;
typedef int Object;
typedef int Weapon;
typedef int Interior;
typedef int Blip;
typedef int Texture;
typedef int TextureDict;
typedef int CoverPoint;
typedef int Camera;
typedef int TaskSequence;
typedef int ColourIndex;
typedef int Instructional;
typedef int Sphere;
typedef int ScrHandle;
typedef int Scaleform;
typedef int AnimScene;
typedef int Prompt;
typedef int PopZone;
typedef int Volume;
typedef int PropSet;
typedef int ItemSet;
typedef int PersChar;

#define ALIGN8 __declspec(align(8))

#pragma pack(push, 1)
typedef struct
{
	ALIGN8 float x;
	ALIGN8 float y;
	ALIGN8 float z;
} Vector3;
#pragma pack(pop)

typedef struct 
{ 
	float x, y;
} Vector2;

typedef struct
{ 
	int R, G, B, A; 
} RGBA;