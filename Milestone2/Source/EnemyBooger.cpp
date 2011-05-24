//////////////////////////////////////////////////////////////////////////////
//	Function	:	"EnemyBooger.cpp"
//
//	Author		:	John Albright (JA)
//
//	Purpose		:	Sets up the booger monster
//////////////////////////////////////////////////////////////////////////////
#include "EnemyBooger.h"
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

CEnemyBooger::CEnemyBooger()
{
	CSGD_EventSystem *pES = CSGD_EventSystem::GetInstance();
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	COptions *op = COptions::GetInstance();

	SetHP(15 * op->GetDifficulty());
	SetVelX(-100.0f * op->GetDifficulty());
	SetVelY(0.0f);
	SetXFlip(-1);
	SetHeight(20);
	SetWidth(20);
	SetPosX(1000);
	SetPosY(430);
	SetPower(7 * op->GetDifficulty());
	SetType(ACTOR_ENEMY);
	SetScoreValue(400 * op->GetDifficulty());
	SetHit(false);

	m_fAttackTimer = 1.0f;
	m_fHurtTimer = 0.0f;

	bIsColliding = false;
	bIsAttacking = false;
	bIsHurt = false;

	m_nXOffset = 0;
	m_nYOffset = 0;
	m_nColor = 255;

	rEnemyRect.top = 248;
	rEnemyRect.bottom = 281;
	rEnemyRect.left = 0;
	rEnemyRect.right = 19;

	m_nSoundID = CSGD_WaveManager::GetInstance()->LoadWave("Resource/sounds/JoA_Chomp.wav");
	CSGD_WaveManager::GetInstance()->SetVolume(m_nSoundID,COptions::GetInstance()->GetSFXVolume() - 60);

	SetHit(false);

}

CEnemyBooger::~CEnemyBooger()
{
	CSGD_EventSystem *pES = CSGD_EventSystem::GetInstance();
	CSGD_WaveManager *pWM = CSGD_WaveManager::GetInstance();
	pES->UnregisterClient(this);
	pWM->UnloadWave(m_nSoundID);
}

void CEnemyBooger::Update(float fElapsedTime)
{
	COptions *op = COptions::GetInstance();
	CSGD_ObjectManager *pOM = CSGD_ObjectManager::GetInstance();
	CSGD_MessageSystem *pMS = CSGD_MessageSystem::GetInstance();
	CSGD_EventSystem *pES= CSGD_EventSystem::GetInstance();
	CPlayerCharacter *pPC = CPlayerCharacter::GetInstance();
	CGame *theGame = CGame::GetInstance();

	DWORD timer = GetTickCount();

	rCollisionRect.left = GetPosX();
	rCollisionRect.right = GetPosX() + GetWidth();
	rCollisionRect.top = GetPosY();
	rCollisionRect.bottom = GetPosY() + GetHeight();
	SetCollisionRect(rCollisionRect);

	if (GetHit())
		m_fHurtTimer = 0.75f;

	if (m_fHurtTimer <= 0)
	{
		m_nColor = 255;
		SetPower(10 * op->GetDifficulty());
	}
	else
	{
		m_nColor = 0;
		SetPower(0);
	}


	if (abs(GetPosX() - pPC->GetPosX()) < 64 && m_fHurtTimer <= 0)
		bIsColliding = true;
	else
		bIsColliding = false;

	if (!bIsColliding)
	{
		SetHeight(20);
		SetPosY(430);
		if (timer % 300 <= 100)
		{
			rEnemyRect.left = 0;
			rEnemyRect.right = 18;
		}
		else if (timer % 300 > 100 && timer % 300 <= 200)
		{
			rEnemyRect.left = 21;
			rEnemyRect.right = 36;
		}
		else if (timer % 300 > 200)
		{
			rEnemyRect.left = 40;
			rEnemyRect.right = 54;
		}

	}
	else if (bIsColliding)
	{
		SetHeight(128);
		bIsAttacking = true;
	}

	if (bIsAttacking && m_fHurtTimer <= 0)
	{
		m_nYOffset = -70;
		rEnemyRect.top = 202;
		rEnemyRect.bottom = 281;
		m_fAttackTimer -= fElapsedTime;

		if (m_fAttackTimer > 0.9)
		{
			rEnemyRect.left = 58;
			rEnemyRect.right = 120;
		}
		else if (m_fAttackTimer <= 0.9 && m_fAttackTimer > 0.8)
		{
			rEnemyRect.left = 124;
			rEnemyRect.right = 210;
		}
		else if (m_fAttackTimer <= 0.8 && m_fAttackTimer > 0.7)
		{
			rEnemyRect.left = 213;
			rEnemyRect.right = 266;
		}
		else if (m_fAttackTimer <= 0.7 && m_fAttackTimer > 0.6)
		{
			rEnemyRect.left = 268;
			rEnemyRect.right = 329;
			CSGD_WaveManager::GetInstance()->Play(m_nSoundID,0);
		}
		else if (m_fAttackTimer <= 0.6 && m_fAttackTimer > 0.5)
		{
			rEnemyRect.left = 332;
			rEnemyRect.right = 385;
		}
		else if (m_fAttackTimer <= 0.5 && m_fAttackTimer > 0.4)
		{
			rEnemyRect.left = 388;
			rEnemyRect.right = 451;
		}
		else if (m_fAttackTimer <= 0.4 && m_fAttackTimer > 0.3)
		{
			rEnemyRect.left = 454;
			rEnemyRect.right = 531;
		}
		else if (m_fAttackTimer <= 0.3 && m_fAttackTimer > 0.2)
		{
			rEnemyRect.left = 535;
			rEnemyRect.right = 628;
		}
		else if (m_fAttackTimer <= 0.2 && m_fAttackTimer > 0.1)
		{
			rEnemyRect.left = 631;
			rEnemyRect.right = 720;
		}
		else if (m_fAttackTimer <= 0.1)
		{
			rEnemyRect.left = 723;
			rEnemyRect.right = 823;
		}

		if (m_fAttackTimer < 0)
		{
			m_fAttackTimer = 1.0f;
			bIsAttacking = false;
		}

	}
	else if (!bIsAttacking)
	{
		m_nYOffset = 0;
		SetVelX(GetXFlip() * 100);
		rEnemyRect.top = 248;
		rEnemyRect.bottom = 281;
	}

	SetPosX(GetPosX() + GetVelX() * fElapsedTime);
	SetPosY(GetPosY() + GetVelY() * fElapsedTime);

	if (GetPosX() < 10)
	{
		SetVelX(abs(GetVelX() * 1));
		SetXFlip(abs(GetXFlip() * 1));
	}
	else if (GetPosX() > 650)
	{
		SetVelX(-abs(GetVelX() * 1));
		SetXFlip(-abs(GetXFlip() * 1));
	}

	if (GetHP() <= 0)
	{
		CSGD_EventSystem::GetInstance()->SendEvent("spawn powerups", NULL);
		pMS->SendMsg(new CDestroyMessage(this));
	}

	m_fHurtTimer -= fElapsedTime;

	SetHit(false);

}

void CEnemyBooger::Render(void)
{
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	CSGD_Direct3D *p3D = CSGD_Direct3D::GetInstance();
	CSGD_MessageSystem *pMS = CSGD_MessageSystem::GetInstance();

	RECT r = GetCollisionRect();

	if(GetHP() > 0)
	{
		if (GetXFlip() == 1)
			pTM->Draw(GetImageID(),GetCollisionRect().left + m_nXOffset, GetCollisionRect().top + m_nYOffset, GetXFlip() * 1.5f,1.5f,&rEnemyRect,GetWidth() / 2, GetHeight() /2,0.0f,D3DCOLOR_XRGB(255,m_nColor,m_nColor));
		else if (GetXFlip() == -1)
			pTM->Draw(GetImageID(),GetCollisionRect().right - m_nXOffset, GetCollisionRect().top + m_nYOffset, GetXFlip() * 1.5f,1.5f,&rEnemyRect,GetWidth() / 2, GetHeight() /2,0.0f,D3DCOLOR_XRGB(255,m_nColor,m_nColor));
	}

}

void CEnemyBooger::HandleEvent(CEvent *pEvent)
{
}