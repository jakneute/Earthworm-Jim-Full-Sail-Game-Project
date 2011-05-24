//////////////////////////////////////////////////////////////////////////////
//	Function	:	"PowerupRapidFire.cpp"
//
//	Author		:	John Albright (JA)
//
//	Purpose		:	Sets up the rapid-fire powerup
//////////////////////////////////////////////////////////////////////////////
#include "PowerupRapidFire.h"
#include "Enums.h"
#include "CSGD_TextureManager.h"
#include "CPlayerCharacter.h"
#include "CSGD_MessageSystem.h"
#include "CCollisionMessage.h"
#include "CDestroyMessage.h"
#include "CSGD_EventSystem.h"
#include "CGame.h"

CPowerupRapidFire::CPowerupRapidFire()
{
	CSGD_EventSystem *pES = CSGD_EventSystem::GetInstance();
	
	pES->RegisterClient("scroll left", this);
	pES->RegisterClient("scroll right",this);

	SetType(POWERUP_RAPIDFIRE);
	rPowerupRect.top = 8;
	rPowerupRect.bottom = 30;
	rPowerupRect.left = 8;
	rPowerupRect.right = 30;

	SetWidth(30);
	SetHeight(30);
	SetScoreValue(100);
}

CPowerupRapidFire::~CPowerupRapidFire()
{
	CSGD_EventSystem *m_pES = CSGD_EventSystem::GetInstance();
	m_pES->UnregisterClient(this);
}

void CPowerupRapidFire::Update(float fElapsedTime)
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
		rPowerupRect.right = 30;
	}
	else if (timer % 600 > 100 && timer % 600 <= 200)
	{
		rPowerupRect.left = 36;
		rPowerupRect.right = 58;
	}
	else if (timer % 600 > 200 && timer % 600 <= 300)
	{
		rPowerupRect.left = 66;
		rPowerupRect.right = 88;
	}
	else if (timer % 600 > 300 && timer % 600 <= 400)
	{
		rPowerupRect.left = 98;
		rPowerupRect.right = 120;
	}
	else if (timer % 600 > 400 && timer % 600 <= 500)
	{
		rPowerupRect.left = 128;
		rPowerupRect.right = 150;
	}
	else if (timer % 600 > 500)
	{
		rPowerupRect.left = 156;
		rPowerupRect.right = 178;
	}


	RECT rCollide;

	if (IntersectRect(&rCollide,&GetCollisionRect(),&pPC->GetCollisionRect()))
	{
		pMS->SendMsg(new CCollisionMessage(CPlayerCharacter::GetInstance(),this));
		pMS->SendMsg(new CDestroyMessage(this));
	}

}

void CPowerupRapidFire::Render(void)
{
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	pTM->Draw(GetImageID(),GetCollisionRect().left,GetCollisionRect().top,1.5f,1.5f,&rPowerupRect,GetWidth() / 2, GetHeight() /2,0,D3DCOLOR_XRGB(255,255,255));
}

void CPowerupRapidFire::HandleEvent(CEvent *pEvent)
{
	if (pEvent->GetEventID() == "scroll right")
		SetPosX(GetPosX() - (5 * CGame::GetInstance()->GetSpeed()));
	else if (pEvent->GetEventID() == "scroll left")
		SetPosX(GetPosX() + (5 * CGame::GetInstance()->GetSpeed()));
}