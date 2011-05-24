//////////////////////////////////////////////////////////////////////////////
//	Function	:	"PowerupThreeWay.h"
//
//	Author		:	John Albright (JA)
//
//	Purpose		:	Sets up three-way powerup
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "CBase.h"
#include "IListener.h"

class CPowerupThreeWay : public CBase, public IListener
{
private:
	RECT rPowerupRect;
	RECT rCollisionRect;

	int m_nImageID;

public:
	CPowerupThreeWay(void);
	~CPowerupThreeWay(void);

	void Update(float fElapsedTime);
	void Render(void);

	void HandleEvent(CEvent* pEvent);

};