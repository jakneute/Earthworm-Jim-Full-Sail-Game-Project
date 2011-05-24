///////////////////////////////////////////////////////////////////////////
//	File	:	"CSGD_ObjectManager.cpp"
//
//	Author	:	David Brown (DB)
//
//	Purpose	:	To manage all of the objects in my game.
///////////////////////////////////////////////////////////////////////////

#include "CSGD_ObjectManager.h"
#include "CBase.h"

CSGD_ObjectManager::CSGD_ObjectManager(void)
{

}

CSGD_ObjectManager::~CSGD_ObjectManager(void)
{

}

void CSGD_ObjectManager::AddObject(CBase* pObject)
{
	// check if the object pointer is valid
	if (!pObject)
		return;

	m_vObjectList.push_back(pObject);
	pObject->AddRef();	//	tell the object we are pointing to it
}

void CSGD_ObjectManager::RemoveObject(CBase* pObject)
{
	//	check if the object pointer is valid
	if (!pObject)
		return;

	vector<CBase*>::iterator iter = m_vObjectList.begin();

	while(iter != m_vObjectList.end())
	{
		//	see if we've found the pointer we are looking for
		if ((*iter) == pObject)
		{
			//	releases the object
			(*iter)->Release();
			//	removes it from the list
			m_vObjectList.erase(iter);
			break;
		}
		else
			iter++;
	}
}

void CSGD_ObjectManager::RemoveAllObjects(void)
{
	// release all objects
	for (unsigned int i=0; i < m_vObjectList.size(); i++)
		m_vObjectList[i]->Release();

	//	empty out the list
	m_vObjectList.clear();
}

void CSGD_ObjectManager::UpdateObjects(float fElapsedTime)
{
	vector<CBase*>::iterator iter = m_vObjectList.begin();

	while(iter != m_vObjectList.end())
	{
		(*iter)->Update(fElapsedTime);
		iter++;
	}
}

void CSGD_ObjectManager::RenderObjects(void)
{
	for (unsigned int i = 0; i < m_vObjectList.size(); i++)
		m_vObjectList[i]->Render();
}

CSGD_ObjectManager* CSGD_ObjectManager::GetInstance(void)
{
	static CSGD_ObjectManager instance;	//	Lazy instantiation
	return &instance;
}