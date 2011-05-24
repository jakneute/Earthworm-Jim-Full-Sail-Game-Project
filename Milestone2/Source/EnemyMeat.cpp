//////////////////////////////////////////////////////////////////////////////
//	Function	:	"EnemyMeat.cpp"
//
//	Author		:	John Albright (JA)
//
//	Purpose		:	Sets up the steak monster
//////////////////////////////////////////////////////////////////////////////
#include "EnemyMeat.h"
#include "CSGD_TextureManager.h"
#include "BulletFire.h"
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

CEnemyMeat::CEnemyMeat()
{
	CSGD_EventSystem *pES = CSGD_EventSystem::GetInstance();
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	COptions *op = COptions::GetInstance();

	pES->RegisterClient("meat fire", this);
	pES->RegisterClient("check velocity",this);

	SetHP(75 * op->GetDifficulty());
	SetVelX(-50.0f * op->GetDifficulty());
	SetVelY(0.0f);
	SetXFlip(-1);
	SetHeight(84);
	SetWidth(36);
	SetPosX(1000);
	SetPosY(100);
	SetPower(0);
	SetType(ACTOR_ENEMY);
	SetScoreValue(300 * op->GetDifficulty());
	SetHit(false);
	pBullet = NULL;

	bIsFiring = false;
	bIsFired = false;

	m_fFireReady = 5.0f;
	m_fHitTimer = 1.0f;
	m_nOffset = 0;
	m_fFiringTimer = 1.0f;
	m_fSineFloat = 3.14f;

	rEnemyRect.top = 1;
	rEnemyRect.bottom = 57;
	rEnemyRect.left = 1;
	rEnemyRect.right = 26;

	m_nSoundID = CSGD_WaveManager::GetInstance()->LoadWave("Resource/sounds/JoA_Fire.wav");
	CSGD_WaveManager::GetInstance()->SetVolume(m_nSoundID,COptions::GetInstance()->GetSFXVolume() - 40);

}

CEnemyMeat::~CEnemyMeat()
{
	CSGD_EventSystem *pES = CSGD_EventSystem::GetInstance();
	CSGD_WaveManager *pWM = CSGD_WaveManager::GetInstance();
	pES->UnregisterClient(this);
	pWM->UnloadWave(m_nSoundID);
}

void CEnemyMeat::Update(float fElapsedTime)
{
	COptions *op = COptions::GetInstance();
	CSGD_ObjectManager *pOM = CSGD_ObjectManager::GetInstance();
	CSGD_MessageSystem *pMS = CSGD_MessageSystem::GetInstance();
	CSGD_EventSystem *pES= CSGD_EventSystem::GetInstance();
	CGame *theGame = CGame::GetInstance();

	if (GetXFlip() == -1)
		SetVelX(abs(GetVelX()));
	else
		SetVelX(-abs(GetVelX()));

	SetVelY(m_fSineFloat * 25 * op->GetDifficulty());

	SetPosX(GetPosX() + GetVelX() * fElapsedTime);
	SetPosY(GetPosY() + GetVelY() * fElapsedTime);

	if (m_fSineFloat <= -3.14f)
		m_fSineFloat = 3.14f;

	m_fSineFloat -= fElapsedTime;

	rCollisionRect.left = GetPosX();
	rCollisionRect.right = GetPosX() + GetWidth();
	rCollisionRect.top = GetPosY();
	rCollisionRect.bottom = GetPosY() + GetHeight();
	SetCollisionRect(rCollisionRect);

	DWORD timer = GetTickCount();

	if (!bIsFiring)
	{
		rEnemyRect.top = 1;
		rEnemyRect.bottom = 57;
		m_nOffset = 0;
		m_fFireReady -= fElapsedTime;
		SetVelX(GetXFlip() * -100);
		if (timer % 800 <= 100)
		{
			rEnemyRect.left = 1;
			rEnemyRect.right = 28;
		}
		else if (timer % 800 > 100 && timer % 800 <= 200)
		{
			rEnemyRect.left = 34;
			rEnemyRect.right = 66;
		}
		else if (timer % 800 > 200 && timer % 800 <= 300)
		{
			rEnemyRect.left = 72;
			rEnemyRect.right = 100;
		}
		else if (timer % 800 > 300 && timer % 800 <= 400)
		{
			rEnemyRect.left = 106;
			rEnemyRect.right = 135;
		}
		else if (timer % 800 > 400 && timer % 800 <= 500)
		{
			rEnemyRect.left = 141;
			rEnemyRect.right = 164;
		}
		else if (timer % 800 > 500 && timer % 800 <= 600)
		{
			rEnemyRect.left = 170;
			rEnemyRect.right = 190;
		}
		else if (timer % 800 > 600 && timer % 800 <= 700)
		{
			rEnemyRect.left = 196;
			rEnemyRect.right = 231;
		}
		else if (timer % 800 > 700)
		{
			rEnemyRect.left = 237;
			rEnemyRect.right = 267;
		}
	}

	if (m_fFireReady <=0 && GetHP() > 0)
	{
		m_fFireReady = 5.0f;
		bIsFiring = true;
	}
	if (bIsFiring && m_fFiringTimer > 0)
	{
		m_fFiringTimer * 1000;
		rEnemyRect.top = 88;
		rEnemyRect.bottom = 151;
		m_fFiringTimer -= fElapsedTime;
		SetVelX(0);
		SetVelY(0);
		
		if (m_fFiringTimer <= 1.0f && m_fFiringTimer > 0.875f)
		{
			rEnemyRect.left = 1;
			rEnemyRect.right = 31;
			pES->SendEvent("check velocity",&fElapsedTime);

		}
		else if (m_fFiringTimer <= 0.875f && m_fFiringTimer > 0.75f)
		{
			rEnemyRect.left = 36;
			rEnemyRect.right = 71;
		}
		else if (m_fFiringTimer <= 0.75f && m_fFiringTimer > 0.625f)
		{
			rEnemyRect.left = 76;
			rEnemyRect.right = 112;
		}
		else if (m_fFiringTimer <= 0.625f && m_fFiringTimer > 0.5f)
		{
			rEnemyRect.left = 118;
			rEnemyRect.right = 152;
		}
		else if (m_fFiringTimer <= 0.5f && m_fFiringTimer > 0.375f)
		{
			rEnemyRect.left = 158;
			rEnemyRect.right = 201;
		}
		else if (m_fFiringTimer <= 0.375f && m_fFiringTimer > 0.25f)
		{
			rEnemyRect.left = 206;
			rEnemyRect.right = 248;
			bIsFired = true;
		}
		else if (m_fFiringTimer <= 0.25f && m_fFiringTimer > 0.125f)
		{
			if (bIsFired)
				CSGD_WaveManager::GetInstance()->Play(m_nSoundID,0);
			rEnemyRect.left = 254;
			rEnemyRect.right = 297;
			pES->SendEvent("meat fire",&fElapsedTime);
			bIsFired = false;
		}
		else if (m_fFiringTimer <= 0.125f)
		{
			rEnemyRect.left = 303;
			rEnemyRect.right = 334;
		}
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

	if (GetHP() <= 0)
	{
		CSGD_EventSystem::GetInstance()->SendEvent("spawn powerups", NULL);
		pMS->SendMsg(new CDestroyMessage(this));
	}

}

void CEnemyMeat::Render(void)
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

void CEnemyMeat::HandleEvent(CEvent *pEvent)
{
	CSGD_ObjectFactory<string, CBase> *pOF		= CSGD_ObjectFactory<string, CBase>::GetInstance();
	CSGD_ObjectManager *pOM						= CSGD_ObjectManager::GetInstance();
 	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	COptions *op = COptions::GetInstance();

	if (pEvent->GetEventID() == "check velocity" )
	{
		if (CPlayerCharacter::GetInstance()->GetPosX() < GetPosX())
			SetXFlip(1);
		else
			SetXFlip(-1);

		m_nDeltaX = CPlayerCharacter::GetInstance()->GetPosX() - GetPosX();

		m_nDeltaY = CPlayerCharacter::GetInstance()->GetPosY() - GetPosY();

	}

	if (pEvent->GetEventID() == "meat fire")
	{
		pBullet = pOF->CreateObject("CBulletFire");

		pBullet->SetImageID(GetImageID());

		pBullet->SetVelX(m_nDeltaX * op->GetDifficulty());
		pBullet->SetVelY(m_nDeltaY * op->GetDifficulty());

		pBullet->SetPosX(GetPosX() - (GetXFlip() * 48));
		pBullet->SetPosY(GetPosY() + 32);
		pBullet->SetType(ACTOR_BULLET);

		pBullet->SetHeight(20);
		pBullet->SetWidth(20);
		pBullet->SetPower(5 * op->GetDifficulty());

		pOM->AddObject(pBullet);
	}

}