
#pragma once

#include <list>
#include <map>
using std::multimap;
using std::pair;
using std::list;

#include "CEvent.h"
#include "IListener.h"

class CSGD_EventSystem
{
	private:
		//	Our Database, this will contain clients that can "listen" for events.
		multimap<EVENTID, IListener*>	m_ClientDatabase;
	
		//	Events waiting to be processed.
		list<CEvent>		m_CurrentEvents;

		//	Utility function - private because it will only ever be used by this class.
		//
		//	Find the event in the database and then calls each of the client's 
		//	EventHandler functions.
		void DispatchEvent(CEvent* pEvent);

	public:
		CSGD_EventSystem() {}
		~CSGD_EventSystem() {}

		static CSGD_EventSystem* GetInstance();

		//	This adds a client to the database.  This will make new "buckets" if necessary and then
		//	add the client to that given bucket.
		void RegisterClient(EVENTID eventID, IListener* pClient);

		//	Removes the client from the database
		void UnregisterClient(IListener* pClient);

		//	Sends an event to be processed later on.
		void SendEvent(EVENTID eventID, void* pData = NULL);

		//	Processes all events in our event list.
		void ProcessEvents(void);
};