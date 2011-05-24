//////////////////////////////////////////////////////////////////////////////
//	Function	:	"EnemyPsycrow.h"
//
//	Author		:	John Albright (JA)
//
//	Purpose		:	Sets up the final boss
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "CBase.h"
#include "IListener.h"
#include "CBaseMessage.h"

class CEnemyPsycrow : public CBase, public IListener
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
	int m_nState;
	int m_nOffset;
	int m_nColor;

	int m_nSoundID;
	float m_nScale;

	static void MessageProc(CBaseMessage* pMsg);


public:
	CEnemyPsycrow(void);
	~CEnemyPsycrow(void);

	void Update(float fElapsedTime);
	void Render(void);

	void HandleEvent(CEvent* pEvent);

	RECT GetCollisionRect(void) { return rCollisionRect; }
	void SetCollisionRect(RECT rCollision) { rCollisionRect = rCollision; }

	int GetState(void) { return m_nState; }

};