//////////////////////////////////////////////////////////////////////////////
//	Function	:	"EnemyBooger.h"
//
//	Author		:	John Albright (JA)
//
//	Purpose		:	Sets up the booger monster
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "CBase.h"
#include "IListener.h"
#include "CBaseMessage.h"

class CEnemyBooger : public CBase, public IListener
{
private:
	RECT rEnemyRect;
	RECT rCollisionRect;

	float m_fAttackTimer;
	float m_fHurtTimer;

	int m_nXOffset;
	int m_nYOffset;
	int m_nColor;

	bool bIsColliding;
	bool bIsAttacking;
	bool bIsHurt;

	int m_nSoundID;

	static void MessageProc(CBaseMessage* pMsg);


public:
	CEnemyBooger(void);
	~CEnemyBooger(void);

	void Update(float fElapsedTime);
	void Render(void);

	void HandleEvent(CEvent* pEvent);

	RECT GetCollisionRect(void) { return rCollisionRect; }
	void SetCollisionRect(RECT rCollision) { rCollisionRect = rCollision; }

};