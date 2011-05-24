//	MessageTypes.h : Contains all of my message types

#pragma once

enum eMessageTypes
{
	MSG_COLLISION = 0,	//	This message gets sent when objects collide.
	MSG_DESTROY,
	MSG_FIRE_RATE_CHEAT,
	MSG_BULLET_SIZE_CHEAT,
	MSG_SPEED_CHEAT,
	MSG_GOD_CHEAT,

	// insert additional messages BEFORE MSG_MAX	
	MSG_MAX				//	My total number of messages.
};