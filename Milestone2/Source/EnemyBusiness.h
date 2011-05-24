//////////////////////////////////////////////////////////////////////////////
//	Function	:	"EnemyBusiness.h"
//
//	Author		:	John Albright (JA)
//
//	Purpose		:	Sets up the businessman enemy
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "CBase.h"
#include "IListener.h"
#include "CBaseMessage.h"

class CEnemyBusiness : public CBase, public IListener
{
private:
	RECT rEnemyRect;
	RECT rCollisionRect;

	bool bIsFiring;
	CBase *pBullet;

	float m_fFireReady;
	float m_fHitTimer;
	float m_fFiringTimer;

	int m_nOffset;
	int m_nImageID;
	int m_nSoundID;

	static void MessageProc(CBaseMessage* pMsg);


public:
	CEnemyBusiness(void);
	~CEnemyBusiness(void);

	void Update(float fElapsedTime);
	void Render(void);

	void HandleEvent(CEvent* pEvent);

	RECT GetCollisionRect(void) { return rCollisionRect; }
	void SetCollisionRect(RECT rCollision) { rCollisionRect = rCollision; }

};