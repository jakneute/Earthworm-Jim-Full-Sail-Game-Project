//////////////////////////////////////////////////////////////////////////////
//	Function	:	"CBulletHook.h"
//
//	Author		:	John Albright (JA)
//
//	Purpose		:	Sets up the boss projectile
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "CBase.h"
#include "IListener.h"
#include "CBaseMessage.h"

class CBulletHook : public CBase, public IListener
{
private:
	RECT rCollisionRect;
	float m_fLifeTime;

public:
	CBulletHook(void);
	~CBulletHook(void);

	void Update(float fElapsedTime);
	void Render(void);

	void HandleEvent(CEvent* pEvent);

	RECT GetCollisionRect(void) { return rCollisionRect; }
	void SetCollisionRect(RECT col) { rCollisionRect = col; }

};