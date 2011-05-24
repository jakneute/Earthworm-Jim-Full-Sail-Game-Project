//////////////////////////////////////////////////////////////////////////////
//	Function	:	"PowerupHealth.h"
//
//	Author		:	John Albright (JA)
//
//	Purpose		:	Sets up the health powerup
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "CBase.h"
#include "IListener.h"
#include "CBaseMessage.h"

class CPowerupHealth : public CBase, public IListener
{
private:
	RECT rCollisionRect;
	RECT rPowerupRect;

	float m_nRotation;
	int m_nImageID;

public:
	CPowerupHealth(void);
	~CPowerupHealth(void);

	void Update(float fElapsedTime);
	void Render(void);

	void HandleEvent(CEvent* pEvent);

};