//////////////////////////////////////////////////////////////////////////////
//	Function	:	"CEvent.h"
//
//	Author		:	John Albright (JA)
//
//	Purpose		:	Defines a CEvent data type
//////////////////////////////////////////////////////////////////////////////
#pragma once

#include <string>
using std::string;

typedef string EVENTID;			//	could be an int or enum, just used strings to change it up

class CEvent
{
private:
	EVENTID			m_EventID;			//	The even id's type.
	void*			m_pParam;			//	Store any variable type.  (but only one so we are limited).

public:
	//CEvent()	{	m_pParam = NULL;	m_EventID = "NO ID Set"; }
	CEvent(EVENTID eventID, void* pParam=NULL)
	{
		m_EventID	= eventID;
		m_pParam	= pParam;
	}

	~CEvent()	{}

	//void SetEventParams(EVENTID eventID, void* pParam)
	//{
	//	m_EventID = eventID;
	//	m_pParam = pParam;
	//}

	//	Accessors.
	inline EVENTID GetEventID(void)			{	return m_EventID;		}
	inline void*	GetParam(void)			{	return m_pParam;	}

};