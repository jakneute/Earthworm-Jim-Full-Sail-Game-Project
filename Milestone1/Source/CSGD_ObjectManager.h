///////////////////////////////////////////////////////////////////////////
//	File	:	"CSGD_ObjectManager.h"
//
//	Author	:	David Brown (DB)
//
//	Purpose	:	To manage all of the objects in my game.
///////////////////////////////////////////////////////////////////////////

#pragma once
#include <vector>
using std::vector;

class CBase;

class CSGD_ObjectManager
{
private:
	vector<CBase*>	m_vObjectList;
public:
	CSGD_ObjectManager(void);
	~CSGD_ObjectManager(void);

	static CSGD_ObjectManager* GetInstance(void);
	CSGD_ObjectManager(const CSGD_ObjectManager& ref);
	//	Assignment Operator.
	CSGD_ObjectManager& operator=(const CSGD_ObjectManager& ref);

	void AddObject(CBase* pObject);
	void RemoveObject(CBase* pObject);
	
	void RemoveAllObjects(void);

	void UpdateObjects(float fElapsedTime);
	void RenderObjects(void);
};