#pragma once

#ifndef __ENUM_H__

enum eBlockType
{
	NONE,
	IMMORTALWALL,
	MOVEOBJECT,
	ICEBLOCK,
	NormalWall
};
enum eColDir
{
	ColDirNONE,
	ColUp,
	ColDown,
	ColRight,
	ColLeft	
};

enum eBehavior
{
	BehaviorIdle,
	BehaviorWalk,
	BehaviorAttack,
	BehaviorPull,
	BehaviorPush,
	BehaviorIceSlide
};

enum eArrow
{
	ArrowUP,
	ArrowDOWN,
	ArrowRIGHT,
	ArrowLEFT
};
enum OBJECT_TYPE { TERRAIN,TERRAINOBJECT ,PLAYER, MONSTER, EFFECT, UI, END };

enum StageNumber
{
	Stage01,Stage02,Stage03
};

#define __ENUM_H__
#endif