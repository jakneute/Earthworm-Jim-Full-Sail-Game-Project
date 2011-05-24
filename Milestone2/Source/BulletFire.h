//////////////////////////////////////////////////////////////////////////////
//	Function	:	"CBulletFire.h"
//
//	Author		:	John Albright (JA)
//
//	Purpose		:	Sets up the bullet fired from the steak enemy
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "CBase.h"
#include "IListener.h"
#include "CBaseMessage.h"

class CBulletFire : public CBase, public IListener
{
private:
	RECT rCollisionRect;

public:
	CBulletFire(void);
	~CBulletFire(void);

	void Update(float fElapsedTime);
	void Render(void);

	void HandleEvent(CEvent* pEvent);

	RECT GetCollisionRect(void) { return rCollisionRect; }
	void SetCollisionRect(RECT col) { rCollisionRect = col; }

};