//////////////////////////////////////////////////////////////////////////////
//	Function	:	"CBullet.h"
//
//	Author		:	John Albright (JA)
//
//	Purpose		:	Sets up the bullet fired by the player
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "CBase.h"
#include "IListener.h"
#include "CBaseMessage.h"

class CBullet : public CBase, public IListener
{
private:
	RECT rCollisionRect;
	float m_nRotation;
	int m_nImageID;

public:
	CBullet(void);
	~CBullet(void);

	void Update(float fElapsedTime);
	void Render(void);

	void HandleEvent(CEvent* pEvent);

	RECT GetCollisionRect(void) { return rCollisionRect; }
	void SetCollisionRect(RECT col) { rCollisionRect = col; }

};