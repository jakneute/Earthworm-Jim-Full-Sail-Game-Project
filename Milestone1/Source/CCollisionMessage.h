//	CCollisionMessage.h : This Message gets sent whenever a collision occurs between two objects.

#pragma once

#include "CBaseMessage.h"
#include "MessageTypes.h"

class CBase;	//	forward declaration of CBase (since we only have pointers declared to it in the header)

class CCollisionMessage : public CBaseMessage
{
private:
	//	These member variables are specific to this message.
	CBase*	m_pCollider1;			//	Pointer to first object.
	CBase*	m_pCollider2;			//	Pointer to second object.

public:

	CCollisionMessage(CBase* pCol1, CBase* pCol2) : CBaseMessage( MSG_COLLISION ) // passing the message type to the base class.
																				  // we can hard code it since we know it will never change.
	{
		m_pCollider1 = pCol1;
		m_pCollider2 = pCol2;
	}

	~CCollisionMessage() {}

	//	Accessors.
	inline CBase* GetCollider1(void)			{ return m_pCollider1; }
	inline CBase* GetCollider2(void)			{ return m_pCollider2; }
};