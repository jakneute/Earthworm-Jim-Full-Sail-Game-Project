//	CBaseMessage.h : Contains the Game Message class for all of my messages.

#pragma once

typedef unsigned int MESSAGEID;

class CBaseMessage
{
private:
	MESSAGEID	m_Msg;		//	The Message Type.

public:
	CBaseMessage(MESSAGEID msg) { m_Msg = msg; }
	virtual ~CBaseMessage() {}

	//	Accessors.
	inline MESSAGEID	GetMessageID(void)				{ return m_Msg;		}

};