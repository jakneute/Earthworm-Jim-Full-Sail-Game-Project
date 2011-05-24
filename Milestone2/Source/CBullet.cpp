//////////////////////////////////////////////////////////////////////////////
//	Function	:	"CBullet.cpp"
//
//	Author		:	John Albright (JA)
//
//	Purpose		:	Sets up the bullet fired by the player
//////////////////////////////////////////////////////////////////////////////
#include "CBullet.h"
#include "CBase.h"
#include "CCollisionMessage.h"
#include "CPlayerCharacter.h"
#include "CSGD_TextureManager.h"
#include "CSGD_Direct3D.h"
#include "CSGD_EventSystem.h"
#include "CSGD_ObjectManager.h"
#include "CSGD_ObjectFactory.h"
#include "CSGD_MessageSystem.h"
#include "Enums.h"
#include "GamePlay.h"
#include "CDestroyMessage.h"
#include "BulletPaper.h"

CBullet::CBullet()
{
	CSGD_EventSystem *m_pES = CSGD_EventSystem::GetInstance();
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	
	m_pES->RegisterClient("scroll left", this);
	m_pES->RegisterClient("scroll right",this);
	m_pES->RegisterClient("businessman fire",this);


	SetType(ACTOR_BULLET);

}

CBullet::~CBullet()
{

	CSGD_EventSystem *m_pES = CSGD_EventSystem::GetInstance();
	m_pES->UnregisterClient(this);

}

void CBullet::Update(float fElapsedTime)
{
	CSGD_ObjectManager *pOM = CSGD_ObjectManager::GetInstance();
	CSGD_MessageSystem *pMS = CSGD_MessageSystem::GetInstance();
	CGamePlay *pGP = CGamePlay::GetInstance();

	SetPosX(GetPosX() + GetVelX() * fElapsedTime);
	SetPosY(GetPosY() + GetVelY() * fElapsedTime);

	rCollisionRect.top = GetPosY();
	rCollisionRect.left = GetPosX();
	rCollisionRect.bottom = GetPosY() + GetHeight();
	rCollisionRect.right = GetPosX() + GetWidth();

	CBase *enemy;

	for (int i = 0; i < pGP->GetNumEnemies(); ++i)
	{
		enemy = pGP->GetEnemy(i);
		RECT r = enemy->GetCollisionRect();

		RECT cross;

		if (IntersectRect(&cross,&rCollisionRect,&r))
			pMS->SendMsg(new CCollisionMessage(enemy,this));
	}

	if (GetPosY() > 580 || GetPosY() < -100 || GetPosX() > 740 || GetPosX() < -100)
		pMS->SendMsg(new CDestroyMessage(this));

}

void CBullet::Render(void)
{
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	CPlayerCharacter *pPC = CPlayerCharacter::GetInstance();
	float scale = pPC->GetBulletScale();
	pTM->Draw(GetImageID(),GetCollisionRect().left,GetCollisionRect().top,scale,scale,NULL,GetWidth() / 2, GetHeight() /2,GetRotation(),D3DCOLOR_XRGB(255,255,255));
}

void CBullet::HandleEvent(CEvent *pEvent)
{
}