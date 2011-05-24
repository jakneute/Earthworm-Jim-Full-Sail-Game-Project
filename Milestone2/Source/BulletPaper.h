//////////////////////////////////////////////////////////////////////////////
//	Function	:	"CBulletPaper.cpp"
//
//	Author		:	John Albright (JA)
//
//	Purpose		:	Sets up the bullet fired by the businessman enemy
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "CBase.h"
#include "IListener.h"
#include "CBaseMessage.h"

class CBulletPaper : public CBase, public IListener
{
private:
	RECT rCollisionRect;
	int m_nImageID;

public:
	CBulletPaper(void);
	~CBulletPaper(void);

	void Update(float fElapsedTime);
	void Render(void);

	void HandleEvent(CEvent* pEvent);

	RECT GetCollisionRect(void) { return rCollisionRect; }
	void SetCollisionRect(RECT col) { rCollisionRect = col; }

};