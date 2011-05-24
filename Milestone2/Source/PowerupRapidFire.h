//////////////////////////////////////////////////////////////////////////////
//	Function	:	"PowerupRapidFire.h"
//
//	Author		:	John Albright (JA)
//
//	Purpose		:	Sets up rapid-fire powerup
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "CBase.h"
#include "IListener.h"

class CPowerupRapidFire : public CBase, public IListener
{
private:
	RECT rPowerupRect;
	RECT rCollisionRect;

	int m_nImageID;

public:
	CPowerupRapidFire(void);
	~CPowerupRapidFire(void);

	void Update(float fElapsedTime);
	void Render(void);

	void HandleEvent(CEvent* pEvent);

};