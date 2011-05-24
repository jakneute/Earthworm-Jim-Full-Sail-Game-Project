//////////////////////////////////////////////////////////////////////////////
//	Function	:	"Powerup1up.cpp"
//
//	Author		:	John Albright (JA)
//
//	Purpose		:	Sets up the health powerup
//////////////////////////////////////////////////////////////////////////////
#include "PowerupHealth.h"
#include "Enums.h"
#include "CSGD_TextureManager.h"
#include "CPlayerCharacter.h"
#include "CSGD_MessageSystem.h"
#include "CCollisionMessage.h"
#include "CDestroyMessage.h"
#include "CSGD_EventSystem.h"
#include "CGame.h"

CPowerupHealth::CPowerupHealth()
{
	CSGD_EventSystem *pES = CSGD_EventSystem::GetInstance();
	
	pES->RegisterClient("scroll left", this);
	pES->RegisterClient("scroll right",this);

	SetType(POWERUP_HEALTH);
	rPowerupRect.top = 40;
	rPowerupRect.bottom = 54;
	rPowerupRect.left = 8;
	rPowerupRect.right = 23;

	SetWidth(30);
	SetHeight(30);
	SetScoreValue(100);
}

CPowerupHealth::~CPowerupHealth()
{
	CSGD_EventSystem *m_pES = CSGD_EventSystem::GetInstance();
	m_pES->UnregisterClient(this);
}

void CPowerupHealth::Update(float fElapsedTime)
{
	CPlayerCharacter *pPC = CPlayerCharacter::GetInstance();
	CSGD_MessageSystem *pMS = CSGD_MessageSystem::GetInstance();

	DWORD timer = GetTickCount();

	rCollisionRect.top = GetPosY();
	rCollisionRect.left = GetPosX();
	rCollisionRect.bottom = GetPosY() + GetHeight();
	rCollisionRect.right = GetPosX() + GetWidth();

	if (timer % 600 <= 100)
	{
		rPowerupRect.left = 8;
		rPowerupRect.right = 23;
	}
	else if (timer % 600 > 100 && timer % 600 <= 200)
	{
		rPowerupRect.left = 28;
		rPowerupRect.right = 43;
	}
	else if (timer % 600 > 200 && timer % 600 <= 300)
	{
		rPowerupRect.left = 52;
		rPowerupRect.right = 67;
	}
	else if (timer % 600 > 300 && timer % 600 <= 400)
	{
		rPowerupRect.left = 74;
		rPowerupRect.right = 89;
	}
	else if (timer % 600 > 400 && timer % 600 <= 500)
	{
		rPowerupRect.left = 97;
		rPowerupRect.right = 112;
	}
	else if (timer % 600 > 500)
	{
		rPowerupRect.left = 117;
		rPowerupRect.right = 132;
	}


	RECT rCollide;

	if (IntersectRect(&rCollide,&GetCollisionRect(),&pPC->GetCollisionRect()))
	{
		pMS->SendMsg(new CCollisionMessage(CPlayerCharacter::GetInstance(),this));
		pMS->SendMsg(new CDestroyMessage(this));
	}

}

void CPowerupHealth::Render(void)
{
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	pTM->Draw(GetImageID(),GetCollisionRect().left,GetCollisionRect().top,1.5f,1.5f,&rPowerupRect,GetWidth() / 2, GetHeight() /2,0,D3DCOLOR_XRGB(255,255,255));
}

void CPowerupHealth::HandleEvent(CEvent *pEvent)
{
	if (pEvent->GetEventID() == "scroll right")
		SetPosX(GetPosX() - (5 * CGame::GetInstance()->GetSpeed()));
	else if (pEvent->GetEventID() == "scroll left")
		SetPosX(GetPosX() + (5 * CGame::GetInstance()->GetSpeed()));
}