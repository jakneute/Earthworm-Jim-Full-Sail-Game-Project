//////////////////////////////////////////////////////////////////////////////
//	Function	:	"CBulletHook.cpp"
//
//	Author		:	John Albright (JA)
//
//	Purpose		:	Sets up the bullet fired from the final boss
//////////////////////////////////////////////////////////////////////////////
#include "BulletHook.h"
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
#include "EnemyPsycrow.h"
#include "CGame.h"
#include "Options.h"

CBulletHook::CBulletHook()
{
	CSGD_EventSystem *pES = CSGD_EventSystem::GetInstance();
	pES->RegisterClient("scroll left",this);
	pES->RegisterClient("scroll right", this);

	m_fLifeTime = 5.0f;

}

CBulletHook::~CBulletHook()
{
	CSGD_EventSystem *m_pES = CSGD_EventSystem::GetInstance();
	m_pES->UnregisterClient(this);
}

void CBulletHook::Update(float fElapsedTime)
{
	CSGD_ObjectManager *pOM = CSGD_ObjectManager::GetInstance();
	CSGD_MessageSystem *pMS = CSGD_MessageSystem::GetInstance();
	CGamePlay *pGP = CGamePlay::GetInstance();
	CPlayerCharacter *pPC = CPlayerCharacter::GetInstance();


	if (GetScoreValue() == 1.0f)
	{
		m_fLifeTime -= fElapsedTime;
		SetPosX(GetPosX() + GetVelX() * fElapsedTime);
		SetPosY(GetPosY() + GetVelY() * fElapsedTime);
	}
	else if (GetScoreValue() == 2.0f)
		m_fLifeTime -= fElapsedTime * 0.25f;
	else  if (GetScoreValue() == 3.0f)
	{
		m_fLifeTime -= fElapsedTime;
		SetPosX(GetPosX() + GetVelX() * fElapsedTime);
		SetPosY(GetPosY() + GetVelY() * fElapsedTime);
		if (GetPosX() < pPC->GetPosX())
			SetVelX(abs(GetVelX()));
		else if (GetPosX() >= pPC->GetPosX())
			SetVelX(-abs(GetVelX()));
		if (GetPosY() < pPC->GetPosY())
			SetVelY(abs(GetVelY()));
		else if (GetPosY() >= pPC->GetPosY())
			SetVelY(-abs(GetVelY()));
	}

	rCollisionRect.top = GetPosY();
	rCollisionRect.left = GetPosX();
	rCollisionRect.bottom = GetPosY() + GetHeight();
	rCollisionRect.right = GetPosX() + GetWidth();

	CBase *player;
	player = CPlayerCharacter::GetInstance();

	RECT cross;

	if (IntersectRect(&cross,&rCollisionRect,&player->GetCollisionRect()))
	{
		pMS->SendMsg(new CCollisionMessage(player,this));
		pMS->SendMsg(new CDestroyMessage(this));
	}

	if (m_fLifeTime <= 0)
		pMS->SendMsg(new CDestroyMessage(this));

}

void CBulletHook::Render(void)
{
	static RECT r = {525,728, 553, 769};
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	pTM->Draw(GetImageID(),GetCollisionRect().left,GetCollisionRect().top + 32,1.5f,1.5f,&r,GetWidth() / 2, GetHeight() /2,GetRotation(),D3DCOLOR_XRGB(255,255,255));
}

void CBulletHook::HandleEvent(CEvent *pEvent)
{	
	if (pEvent->GetEventID() == "scroll right")
		SetPosX(GetPosX() - (5 * CGame::GetInstance()->GetSpeed()));
	else if (pEvent->GetEventID() == "scroll left")
		SetPosX(GetPosX() + (5 * CGame::GetInstance()->GetSpeed()));

}