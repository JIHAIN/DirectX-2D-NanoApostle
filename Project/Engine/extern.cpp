#include "pch.h"
#include "global.h"

TransInfo g_Trans = {};
GlobalData g_Data = {};
Mouseinfo g_MouseInfo = {};

const char* ASSET_TYPE_STRING[] =
{
	"MESH",
	"MESH_DATA",
	"TEXTURE",
	"SOUND",
	"PREFAB",
	"FLIPBOOK",
	"SPRITE",
	"MATERIAL",
	"GRAPHIC_SHADER",
	"COMPUTE_SHADER",
};