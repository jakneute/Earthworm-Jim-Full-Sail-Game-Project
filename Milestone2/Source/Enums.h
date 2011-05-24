//////////////////////////////////////////////////////////////////////////////
//	Function	:	"enums.h"
//
//	Author		:	John Albright (JA)
//
//	Purpose		:	Defines all enumerators
////////////////////////////////////////////////////////////////////////////////	

#pragma once

enum eMessageTypes
{
	MSG_COLLISION = 0,	//	This message gets sent when objects collide.
	MSG_DESTROY,
	MSG_GOD_CHEAT,
	MSG_WARP1,
	MSG_WARP2,
	MSG_WARP3,
	MSG_SPEED_SLOW,
	MSG_SPEED_NORMAL,
	MSG_SPEED_FAST,
	// insert additional messages BEFORE MSG_MAX	
	MSG_MAX				//	My total number of messages.
};

enum eActorTypes
{
	ACTOR_BULLET = 0,
	ACTOR_PLAYER,
	ACTOR_ENEMY,
	POWERUP_1UP,
	POWERUP_HEALTH,
	POWERUP_RAPIDFIRE,
	POWERUP_THREEWAY,
	MAX_STUFF
};

enum eCheatCodeTypes
{
	CHEAT_WARP_LEVEL1 = 0,
	CHEAT_WARP_LEVEL2,
	CHEAT_WARP_LEVEL3,
	CHEAT_GOD,
	CHEAT_SPEED_SLOW,
	CHEAT_SPEED_FAST,
	CHEAT_MAX
};

enum eDifficulty
{
	DIFFICULTY_EASY = 1,
	DIFFICULTY_MEDIUM,
	DIFFICULTY_HARD
};

enum ePsycrowStates
{
	PSYCROW_STATE1,
	PSYCROW_STATE2,
	PSYCROW_STATE3,
	PSYCROW_STATE4,
	PSYCROW_STATE5,
	PSYCROW_ESCAPE,
};