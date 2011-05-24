//	CCollisionMessage.h : This Message gets sent whenever a collision occurs between two objects.

#pragma once

#include "CBaseMessage.h"
#include "MessageTypes.h"

class CBase;	//	forward declaration of CBase (since we only have pointers declared to it in the header)

class CDestroyMessage : public CBaseMessage
{
private:
	//	These member variables are specific to this message.
	CBase*	m_pObjectToDestroy;			//	Pointer to first object.

public:

	CDestroyMessage(CBase* pItem) : CBaseMessage( MSG_DESTROY ) // passing the message type to the base class.
																				  // we can hard code it since we know it will never change.
	{
		m_pObjectToDestroy = pItem;
	}

	~CDestroyMessage() {}

	//	Accessors.
	inline CBase* GetItem(void)			{ return m_pObjectToDestroy; }
};