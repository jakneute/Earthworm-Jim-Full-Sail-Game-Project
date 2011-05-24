//////////////////////////////////////////////////////////////////////////////
//	Function	:	"CDerived.cpp"
//
//	Author		:	David Brown (DB)
//
//	Purpose		:	To define an object that will bounce around the screen.
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
#include "MessageTypes.h"
#include "GamePlay.h"
#include "CDestroyMessage.h"

CBullet::CBullet()
{
	CSGD_EventSystem *m_pES = CSGD_EventSystem::GetInstance();
	
	m_pES->RegisterClient("shoot up",this);
	m_pES->RegisterClient("shoot down",this);
	m_pES->RegisterClient("shoot left",this);
	m_pES->RegisterClient("shoot right",this);
	m_pES->RegisterClient("shoot up-left",this);
	m_pES->RegisterClient("shoot up-right",this);
	m_pES->RegisterClient("shoot down-left",this);
	m_pES->RegisterClient("shoot down-right",this);

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

	if (GetPosY() > 480 || GetPosY() < 0 || GetPosX() > 640 || GetPosX() < 0)
		pMS->SendMsg(new CDestroyMessage(this));

}

void CBullet::Render(void)
{
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	CPlayerCharacter *pPC = CPlayerCharacter::GetInstance();
	float scale = pPC->GetBulletScale();
	pTM->Draw(GetImageID(),GetPosX(),GetPosY(),scale,scale,NULL,GetWidth() / 2, GetHeight() /2,GetRotation(),D3DCOLOR_XRGB(255,255,255));

}

void CBullet::HandleEvent(CEvent *pEvent)
{
}