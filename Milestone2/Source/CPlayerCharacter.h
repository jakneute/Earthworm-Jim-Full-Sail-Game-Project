//////////////////////////////////////////////////////////////////////////////
//	Function	:	"CPlayerCharacter.h"
//
//	Author		:	John Albright (JA)
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
	bool bInvulnerable;

	int m_nXFlip;
	int m_nColors;
	int m_nOffSet;
	int m_nImageID[3];
	int m_nRateOfFire;
	float m_nBulletScale;
	float m_nInvulnerableTimer;
	float m_fRapidFireTimer;
	float m_fThreeWayTimer;

	bool bFireThreeWay;

	CBase *pBullet;

	RECT rPlayerRect;
	RECT rShootingRect[3];
	RECT rCollisionRect;

public:
	CPlayerCharacter(void);
	~CPlayerCharacter(void);

	void Update(float fElapsedTime);
	void Render(void);

	void HandleEvent(CEvent* pEvent);

	static CPlayerCharacter* GetInstance(void);

	RECT GetCollisionRect(void) { return rCollisionRect; }
	void SetCollisionRect(RECT rCollision) { rCollisionRect = rCollision; }

	int GetRateOfFire(void) { return m_nRateOfFire; }
	void SetRateOfFire(int nRate) { m_nRateOfFire = nRate; }

	float GetBulletScale(void) { return m_nBulletScale; }
	void SetBulletScale(float nScale) { m_nBulletScale = nScale; }

	bool GetGodMode(void) { return bGodMode; }
	void SetGodMode(bool god) { bGodMode = god; }

	bool GetInvulnerable(void) { return bInvulnerable; }
	void SetInvulnerable(bool invulnerable) { bInvulnerable = invulnerable; }

	bool GetThreeWay(void) { return bFireThreeWay; }
	void SetThreeWay(bool threeway) { bFireThreeWay = threeway; }

	float GetThreeWayTimer(void) { return m_fThreeWayTimer; }
	void SetThreeWayTimer(float timer) { m_fThreeWayTimer = timer; }

	float GetRapidFireTimer(void) { return m_fRapidFireTimer; }
	void SetRapidFireTimer(float timer) { m_fRapidFireTimer = timer; }

};