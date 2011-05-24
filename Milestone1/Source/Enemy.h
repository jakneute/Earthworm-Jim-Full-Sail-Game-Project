//////////////////////////////////////////////////////////////////////////////
//	Function	:	"CDerived.h"
//
//	Author		:	David Brown (DB)
//
//	Purpose		:	Sets up the player character
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "CBase.h"
#include "IListener.h"
#include "CBaseMessage.h"

class CEnemy : public CBase, public IListener
{
private:
	RECT rEnemyRect;

public:
	CEnemy(void);
	~CEnemy(void);

	void Update(float fElapsedTime);
	void Render(void);

	void HandleEvent(CEvent* pEvent);

	static CEnemy* GetInstance(void);

	RECT GetCollisionRect(void) { return rEnemyRect; }
	void SetCollisionRect(RECT rCollision) { rEnemyRect = rCollision; }
};