#include "CSGD_EventSystem.h"

//	Register Client.
void CSGD_EventSystem::RegisterClient(EVENTID eventID, IListener* pClient)
{
	//	Error check to make sure the client exists.
	if (!pClient)	return;

	//	Register (Build) the database of clients.
	m_ClientDatabase.insert( make_pair(eventID, pClient) );
}

//	Unregister Client
void CSGD_EventSystem::UnregisterClient(IListener *pClient)
{
	multimap<string, IListener*>::iterator mmIter = m_ClientDatabase.begin();

	while(mmIter !=m_ClientDatabase.end())
	{
		if((*mmIter).second == pClient)
		{
			mmIter = m_ClientDatabase.erase(mmIter);
			continue;
		}

		mmIter++;
	}
}

//	Dispatch Event.
void CSGD_EventSystem::DispatchEvent(CEvent *pEvent)
{
	//	Make an iterator that will iterate all of our clients that
	//	should be receiveing this event
	pair<multimap<EVENTID, IListener*>::iterator,
		 multimap<EVENTID, IListener*>::iterator> range;

	//	Find all of the clients taht are able to receive this event.
	range = m_ClientDatabase.equal_range(pEvent->GetEventID());

	//	Go through the linked list of clients that are able to receive this event.
	for(multimap<EVENTID, IListener*>::iterator mmIter = range.first;
					mmIter != range.second; mmIter++)
	{
		//	Pass this event to the client
		(*mmIter).second->HandleEvent(pEvent);
	}
}

void CSGD_EventSystem::SendEvent(EVENTID eventID, void* pData)
{
	//	Push my event into the list.
	CEvent newEvent(eventID, pData);
	//newEvent.SetEventParams(eventID, pData);

	m_CurrentEvents.push_back(newEvent);
}

void CSGD_EventSystem::ProcessEvents(void)
{
	//	Go through my list of events that are waiting to be processed.
	while(m_CurrentEvents.size())
	{
		DispatchEvent(&m_CurrentEvents.front());
		m_CurrentEvents.pop_front();
	}
}

CSGD_EventSystem* CSGD_EventSystem::GetInstance(void)
{
	static CSGD_EventSystem instance;	//	Lazy instantiation

	return &instance;
}
