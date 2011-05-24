//////////////////////////////////////////////////////////////////////////////
//	Function	:	"PowerupThreeWay.cpp"
//
//	Author		:	John Albright (JA)
//
//	Purpose		:	Sets up the three-way powerup
//////////////////////////////////////////////////////////////////////////////
#include "PowerupThreeWay.h"
#include "Enums.h"
#include "CSGD_TextureManager.h"
#include "CPlayerCharacter.h"
#include "CSGD_MessageSystem.h"
#include "CCollisionMessage.h"
#include "CDestroyMessage.h"
#include "CSGD_EventSystem.h"
#include "CGame.h"

CPowerupThreeWay::CPowerupThreeWay()
{
	CSGD_EventSystem *pES = CSGD_EventSystem::GetInstance();
	
	pES->RegisterClient("scroll left", this);
	pES->RegisterClient("scroll right",this);

	SetType(POWERUP_THREEWAY);
	rPowerupRect.top = 354;
	rPowerupRect.bottom = 373;
	rPowerupRect.left = 69;
	rPowerupRect.right = 92;

	SetWidth(30);
	SetHeight(30);
	SetScoreValue(100);
}

CPowerupThreeWay::~CPowerupThreeWay()
{
	CSGD_EventSystem *m_pES = CSGD_EventSystem::GetInstance();
	m_pES->UnregisterClient(this);
}

void CPowerupThreeWay::Update(float fElapsedTime)
{
	CPlayerCharacter *pPC = CPlayerCharacter::GetInstance();
	CSGD_MessageSystem *pMS = CSGD_MessageSystem::GetInstance();

	DWORD timer = GetTickCount();

	rCollisionRect.top = GetPosY();
	rCollisionRect.left = GetPosX();
	rCollisionRect.bottom = GetPosY() + GetHeight();
	rCollisionRect.right = GetPosX() + GetWidth();

	if (timer % 400 <= 100)
	{
		rPowerupRect.left = 69;
		rPowerupRect.right = 92;
	}
	else if (timer % 400 > 100 && timer % 400 <= 200)
	{
		rPowerupRect.left = 99;
		rPowerupRect.right = 122;
	}
	else if (timer % 400 > 200 && timer % 400 <= 300)
	{
		rPowerupRect.left = 130;
		rPowerupRect.right = 153;
	}
	else if (timer % 400 > 300)
	{
		rPowerupRect.left = 162;
		rPowerupRect.right = 185;
	}

	RECT rCollide;

	if (IntersectRect(&rCollide,&GetCollisionRect(),&pPC->GetCollisionRect()))
	{
		pMS->SendMsg(new CCollisionMessage(CPlayerCharacter::GetInstance(),this));
		pMS->SendMsg(new CDestroyMessage(this));
	}

}

void CPowerupThreeWay::Render(void)
{
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	pTM->Draw(GetImageID(),GetCollisionRect().left,GetCollisionRect().top,1.5f,1.5f,&rPowerupRect,GetWidth() / 2, GetHeight() /2,0,D3DCOLOR_XRGB(255,255,255));
}

void CPowerupThreeWay::HandleEvent(CEvent *pEvent)
{
	if (pEvent->GetEventID() == "scroll right")
		SetPosX(GetPosX() - (5 * CGame::GetInstance()->GetSpeed()));
	else if (pEvent->GetEventID() == "scroll left")
		SetPosX(GetPosX() + (5 * CGame::GetInstance()->GetSpeed()));
}