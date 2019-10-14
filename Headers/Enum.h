#pragma once

#ifndef __ENUM_H__

enum eBlockType
{
	NONE,
	IMMORTALWALL,
	MOVEOBJECT,
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
	BehaviorAttack
};

enum eArrow
{
	ArrowUP,
	ArrowDOWN,
	ArrowRIGHT,
	ArrowLEFT
};
enum OBJECT_TYPE { TERRAIN, PLAYER, MONSTER, EFFECT, UI, END };

#define __ENUM_H__
#endif