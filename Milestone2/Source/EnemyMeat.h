//////////////////////////////////////////////////////////////////////////////
//	Function	:	"EnemyMeat.h"
//
//	Author		:	John Albright (JA)
//
//	Purpose		:	Sets up the steak monster
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "CBase.h"
#include "IListener.h"
#include "CBaseMessage.h"

class CEnemyMeat : public CBase, public IListener
{
private:
	RECT rEnemyRect;
	RECT rCollisionRect;

	bool bIsFiring;
	bool bIsFired;
	CBase *pBullet;

	float m_fFireReady;
	float m_fHitTimer;
	float m_fFiringTimer;
	float m_fSineFloat;

	int m_nDeltaX;
	int m_nDeltaY;

	int m_nOffset;
	int m_nSoundID;

	static void MessageProc(CBaseMessage* pMsg);


public:
	CEnemyMeat(void);
	~CEnemyMeat(void);

	void Update(float fElapsedTime);
	void Render(void);

	void HandleEvent(CEvent* pEvent);

	RECT GetCollisionRect(void) { return rCollisionRect; }
	void SetCollisionRect(RECT rCollision) { rCollisionRect = rCollision; }

};