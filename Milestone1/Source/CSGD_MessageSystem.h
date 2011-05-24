//	CSGD_MessageSystem.h : The Message System for our application.  Handles sending the messages
//							to the specified proc function.

#pragma once

#include "CBaseMessage.h"
#include <queue.>
using std::queue;

typedef void (*MESSAGEPROC)(CBaseMessage*);

class CSGD_MessageSystem
{
private:
	queue<CBaseMessage*>			m_MsgQueue;				//	Stores my messages.
	MESSAGEPROC						m_pfnMsgProc;			//	Points to user defined function.

public:
	CSGD_MessageSystem() { m_pfnMsgProc = NULL;	}
	~CSGD_MessageSystem() {}

	//	How many messages waiting to be processed.
	inline int GetNumMessages(void) { return (int)m_MsgQueue.size(); }

	//	Setup the function pointer for our messages.
	void InitMessageSystem(MESSAGEPROC pfnMsgProc);

	//	Sends the message into the queue and awaits processing later on through
	//	The ProcessMessages() function.
	void SendMsg(CBaseMessage* pMsg);

	//	Processes all the messages that are waiting inside of the queue.
	//	Normally you only call this function once per frame.
	void ProcessMessages(void);

	//	Clears any messages that may be remaining and sets the function pointer to NULL.
	void ShutdownMessageSystem(void);

	static CSGD_MessageSystem* GetInstance();
};