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

class CPlayerCharacter : public CBase, public IListener
{
private:
	bool bIsMoving;
	bool bIsShooting;
	bool bGodMode;

	int m_nXFlip;
	int m_nOffSet;
	int m_nImageID[2];
	int m_nRateOfFire;
	float m_nBulletScale;

	CBase *pBullet;

	RECT rPlayerRect;
	RECT rShootingRect[3];

public:
	CPlayerCharacter(void);
	~CPlayerCharacter(void);

	void Update(float fElapsedTime);
	void Render(void);

	void HandleEvent(CEvent* pEvent);

	static CPlayerCharacter* GetInstance(void);

	RECT GetCollisionRect(void) { return rPlayerRect; }
	void SetCollisionRect(RECT rCollision) { rPlayerRect = rCollision; }

	int GetRateOfFire(void) { return m_nRateOfFire; }
	void SetRateOfFire(int nRate) { m_nRateOfFire = nRate; }

	float GetBulletScale(void) { return m_nBulletScale; }
	void SetBulletScale(float nScale) { m_nBulletScale = nScale; }

	bool GetGodMode(void) { return bGodMode; }
	void SetGodMode(bool god) { bGodMode = god; }
};