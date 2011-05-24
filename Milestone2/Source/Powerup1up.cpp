//////////////////////////////////////////////////////////////////////////////
//	Function	:	"Powerup1up.cpp"
//
//	Author		:	John Albright (JA)
//
//	Purpose		:	Sets up the 1up powerup
//////////////////////////////////////////////////////////////////////////////
#include "Powerup1up.h"
#include "Enums.h"
#include "CSGD_TextureManager.h"
#include "CPlayerCharacter.h"
#include "CSGD_MessageSystem.h"
#include "CCollisionMessage.h"
#include "CDestroyMessage.h"
#include "CSGD_EventSystem.h"
#include "CGame.h"

CPowerup1up::CPowerup1up()
{
	CSGD_EventSystem *pES = CSGD_EventSystem::GetInstance();
	
	pES->RegisterClient("scroll left", this);
	pES->RegisterClient("scroll right",this);

	SetType(POWERUP_1UP);
	rPowerupRect.top = 107;
	rPowerupRect.bottom = 135;
	rPowerupRect.left = 12;
	rPowerupRect.right = 35;

	SetWidth(20);
	SetHeight(28);
	SetScoreValue(250);

}

CPowerup1up::~CPowerup1up()
{
	CSGD_EventSystem *m_pES = CSGD_EventSystem::GetInstance();
	m_pES->UnregisterClient(this);
}

void CPowerup1up::Update(float fElapsedTime)
{
	CPlayerCharacter *pPC = CPlayerCharacter::GetInstance();
	CSGD_MessageSystem *pMS = CSGD_MessageSystem::GetInstance();

	rCollisionRect.top = GetPosY();
	rCollisionRect.left = GetPosX();
	rCollisionRect.bottom = GetPosY() + GetHeight();
	rCollisionRect.right = GetPosX() + GetWidth();

	RECT rCollide;

	if (IntersectRect(&rCollide,&GetCollisionRect(),&pPC->GetCollisionRect()))
	{
		pMS->SendMsg(new CCollisionMessage(CPlayerCharacter::GetInstance(),this));
		pMS->SendMsg(new CDestroyMessage(this));
	}

}

void CPowerup1up::Render(void)
{
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	pTM->Draw(GetImageID(),GetCollisionRect().left,GetCollisionRect().top,1,1,&rPowerupRect,GetWidth() / 2, GetHeight() /2,0,D3DCOLOR_XRGB(255,255,255));
}

void CPowerup1up::HandleEvent(CEvent *pEvent)
{
	if (pEvent->GetEventID() == "scroll right")
		SetPosX(GetPosX() - (5 * CGame::GetInstance()->GetSpeed()));
	else if (pEvent->GetEventID() == "scroll left")
		SetPosX(GetPosX() + (5 * CGame::GetInstance()->GetSpeed()));
}