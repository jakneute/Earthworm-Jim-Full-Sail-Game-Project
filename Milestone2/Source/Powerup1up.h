//////////////////////////////////////////////////////////////////////////////
//	Function	:	"Powerup1up.h"
//
//	Author		:	John Albright (JA)
//
//	Purpose		:	Sets up the 1up powerup
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "CBase.h"
#include "IListener.h"

class CPowerup1up : public CBase, public IListener
{
private:
	RECT rPowerupRect;
	RECT rCollisionRect;

	int m_nImageID;

public:
	CPowerup1up(void);
	~CPowerup1up(void);

	void Update(float fElapsedTime);
	void Render(void);

	void HandleEvent(CEvent* pEvent);

};