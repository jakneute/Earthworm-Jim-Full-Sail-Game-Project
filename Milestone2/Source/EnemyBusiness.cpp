//////////////////////////////////////////////////////////////////////////////
//	Function	:	"EnemyBusiness.cpp"
//
//	Author		:	John Albright (JA)
//
//	Purpose		:	Sets up the businessman enemy
//////////////////////////////////////////////////////////////////////////////
#include "EnemyBusiness.h"
#include "CSGD_TextureManager.h"
#include "CPlayerCharacter.h"
#include "CBullet.h"
#include "CSGD_Direct3D.h"
#include "Enums.h"
#include "CSGD_MessageSystem.h"
#include "CSGD_ObjectManager.h"
#include "CDestroyMessage.h"
#include "CSGD_EventSystem.h"
#include "CGame.h"
#include "CSGD_ObjectFactory.h"
#include "Options.h"
#include "CSGD_WaveManager.h"

CEnemyBusiness::CEnemyBusiness()
{
	CSGD_EventSystem *pES = CSGD_EventSystem::GetInstance();
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	COptions *op = COptions::GetInstance();
	CSGD_WaveManager *pWM = CSGD_WaveManager::GetInstance();

	pES->RegisterClient("businessman fire", this);

	SetHP(50 *op->GetDifficulty());
	SetVelX(-50.0f * op->GetDifficulty());
	SetVelY(0.0f);
	SetXFlip(-1);
	SetHeight(63);
	SetWidth(86);
	SetPosX(1000);
	SetPosY(220);
	SetPower(0);
	SetType(ACTOR_ENEMY);
	SetScoreValue(200 * op->GetDifficulty());
	SetHit(false);
	pBullet = NULL;

	bIsFiring = false;

	m_fFireReady = 5.0f;
	m_fHitTimer = 1.0f;
	m_nOffset = 0;
	m_fFiringTimer = 1.0f;

	rEnemyRect.top = 18;
	rEnemyRect.bottom = 61;
	rEnemyRect.left = 2;
	rEnemyRect.right = 54;

	m_nSoundID = pWM->LoadWave("Resource/sounds/JoA_Scream.wav");
	pWM->SetVolume(m_nSoundID,op->GetSFXVolume() - 25);
	

}

CEnemyBusiness::~CEnemyBusiness()
{
	CSGD_EventSystem *pES = CSGD_EventSystem::GetInstance();
	pES->UnregisterClient(this);
	//CSGD_WaveManager *pWM = CSGD_WaveManager::GetInstance();
	//pWM->UnloadWave(m_nSoundID);
}

void CEnemyBusiness::Update(float fElapsedTime)
{

	CSGD_ObjectManager *pOM = CSGD_ObjectManager::GetInstance();
	CSGD_MessageSystem *pMS = CSGD_MessageSystem::GetInstance();
	CSGD_EventSystem *pES= CSGD_EventSystem::GetInstance();
	CGame *theGame = CGame::GetInstance();

	SetPosX(GetPosX() + GetVelX() * fElapsedTime);
	SetPosY(GetPosY() + GetVelY() * fElapsedTime);

	rCollisionRect.left = GetPosX();
	rCollisionRect.right = GetPosX() + GetWidth();
	rCollisionRect.top = GetPosY();
	rCollisionRect.bottom = GetPosY() + GetHeight();
	SetCollisionRect(rCollisionRect);

	DWORD timer = GetTickCount();

	if (GetHit())
	{
		m_nOffset = 32;
		m_fHitTimer = 1.0f;
		SetVelX(0);
		rEnemyRect.left = 442;
		rEnemyRect.right = 463;
	}

	else if (!GetHit() && m_fHitTimer <= 0)
	{
		m_nOffset = 0;
		m_fFireReady -= fElapsedTime;
		SetVelX(GetXFlip() * -100);
		if (timer % 700 <= 100)
		{
			rEnemyRect.left = 2;
			rEnemyRect.right = 54;
		}
		else if (timer % 700 > 100 && timer % 700 <= 200)
		{
			rEnemyRect.left = 58;
			rEnemyRect.right = 115;
		}
		else if (timer % 700 > 200 && timer % 700 <= 300)
		{
			rEnemyRect.left = 119;
			rEnemyRect.right = 163;
		}
		else if (timer % 700 > 300 && timer % 700 <= 400)
		{
			rEnemyRect.left = 700;
			rEnemyRect.right = 753;
		}
		else if (timer % 700 > 400 && timer % 700 <= 500)
		{
			rEnemyRect.left = 756;
			rEnemyRect.right = 795;
		}
		else if (timer % 700 > 500 && timer % 700 <= 600)
		{
			rEnemyRect.left = 798;
			rEnemyRect.right = 850;
		}
		else if (timer % 700 > 600)
		{
			rEnemyRect.left = 853;
			rEnemyRect.right = 899;
		}
	}

	if (m_fFireReady <=0 && !GetHit() && GetHP() > 0)
		pES->SendEvent("businessman fire",&fElapsedTime);

	if (bIsFiring)
	{
		rEnemyRect.left = 329;
		rEnemyRect.right = 397;
		m_fFiringTimer -= fElapsedTime;
		SetVelX(0);
	}

	if (m_fFiringTimer <= 0)
	{
		bIsFiring = false;
		m_fFiringTimer = 1.0f;
	}

	if (GetPosX() < -32)
	{
		SetVelX(abs(GetVelX() * -1));
		SetXFlip(-abs(GetXFlip() * -1));
	}
	else if (GetPosX() > 640)
	{
		SetVelX(-abs(GetVelX() * -1));
		SetXFlip(abs(GetXFlip() * -1));
	}
	else if (GetPosY() > 480)
		SetVelY(-abs(GetVelY() * -1));
	else if (GetPosY() < -32)
		SetVelY(abs(GetVelY() * -1));

	m_fHitTimer -= fElapsedTime;

	SetHit(false);

	if (GetHP() <= 0)
	{
		CSGD_WaveManager::GetInstance()->Play(m_nSoundID,0);

		CSGD_EventSystem::GetInstance()->SendEvent("spawn powerups", NULL);
		pMS->SendMsg(new CDestroyMessage(this));

	}

}

void CEnemyBusiness::Render(void)
{
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	CSGD_Direct3D *p3D = CSGD_Direct3D::GetInstance();
	CSGD_MessageSystem *pMS = CSGD_MessageSystem::GetInstance();

	RECT r = GetCollisionRect();

	if(GetHP() > 0)
	{
		if (GetXFlip() == 1)
			pTM->Draw(GetImageID(),GetCollisionRect().left + m_nOffset, GetCollisionRect().top, GetXFlip() * 1.5f,1.5f,&rEnemyRect,GetWidth() / 2, GetHeight() /2,0.0f,D3DCOLOR_XRGB(255,255,255));
		else if (GetXFlip() == -1)
			pTM->Draw(GetImageID(),GetCollisionRect().right - m_nOffset, GetCollisionRect().top, GetXFlip() * 1.5f,1.5f,&rEnemyRect,GetWidth() / 2, GetHeight() /2,0.0f,D3DCOLOR_XRGB(255,255,255));
	}

}

void CEnemyBusiness::HandleEvent(CEvent *pEvent)
{
	CSGD_ObjectFactory<string, CBase> *pOF		= CSGD_ObjectFactory<string, CBase>::GetInstance();
	CSGD_ObjectManager *pOM						= CSGD_ObjectManager::GetInstance();
 	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	COptions *op = COptions::GetInstance();

	if (pEvent->GetEventID() == "businessman fire")
	{
		m_fFireReady = 5.0f;
		bIsFiring = true;

		pBullet = pOF->CreateObject("CBulletPaper");
		pBullet->SetImageID(GetImageID());

		if (GetXFlip() == -1)
			pBullet->SetVelX(500);
		else
			pBullet->SetVelX(-500);

		pBullet->SetVelY(0);
		pBullet->SetPosX(GetPosX() - (GetXFlip() * 48));
		pBullet->SetPosY(GetPosY());
		pBullet->SetType(ACTOR_BULLET);

		pBullet->SetHeight(20);
		pBullet->SetWidth(20);
		pBullet->SetPower(5 * op->GetDifficulty());

		pOM->AddObject(pBullet);
	}

}