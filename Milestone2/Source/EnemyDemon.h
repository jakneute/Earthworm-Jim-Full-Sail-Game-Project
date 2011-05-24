//////////////////////////////////////////////////////////////////////////////
//	Function	:	"EnemyDemon.h"
//
//	Author		:	John Albright (JA)
//
//	Purpose		:	Sets up the demon enemy
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "CBase.h"
#include "IListener.h"
#include "CBaseMessage.h"

class CEnemyDemon : public CBase, public IListener
{
private:
	RECT rEnemyRect;
	RECT rCollisionRect;

	int m_nSoundID;

	bool bIsClose;

public:
	CEnemyDemon(void);
	~CEnemyDemon(void);

	void Update(float fElapsedTime);
	void Render(void);

	void HandleEvent(CEvent* pEvent);

	RECT GetCollisionRect(void) { return rCollisionRect; }
	void SetCollisionRect(RECT rCollision) { rCollisionRect = rCollision; }
};