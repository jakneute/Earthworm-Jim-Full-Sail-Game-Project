//////////////////////////////////////////////////////////////////////////////
//	Function	:	"CBulletPaper.cpp"
//
//	Author		:	John Albright (JA)
//
//	Purpose		:	Sets up the bullet fired by the businessman enemy
//////////////////////////////////////////////////////////////////////////////
#include "BulletPaper.h"
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

CBulletPaper::CBulletPaper()
{
	CSGD_EventSystem *m_pES = CSGD_EventSystem::GetInstance();
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
}

CBulletPaper::~CBulletPaper()
{

	CSGD_EventSystem *m_pES = CSGD_EventSystem::GetInstance();
	m_pES->UnregisterClient(this);

}

void CBulletPaper::Update(float fElapsedTime)
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

	CBase *player;
	player = CPlayerCharacter::GetInstance();

	RECT cross;

	if (IntersectRect(&cross,&rCollisionRect,&player->GetCollisionRect()))
		pMS->SendMsg(new CCollisionMessage(player,this));

	if (GetPosY() > 580 || GetPosY() < -100 || GetPosX() > 740 || GetPosX() < -100)
		pMS->SendMsg(new CDestroyMessage(this));

}

void CBulletPaper::Render(void)
{
	static RECT r = {615,32,624,42};
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	pTM->Draw(GetImageID(),GetPosX(),GetPosY(),2.0f,2.0f,&r,GetWidth() / 2, GetHeight() /2,GetRotation(),D3DCOLOR_XRGB(255,255,255));
}

void CBulletPaper::HandleEvent(CEvent *pEvent)
{	


}