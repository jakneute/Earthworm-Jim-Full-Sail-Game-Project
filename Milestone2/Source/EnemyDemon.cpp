//////////////////////////////////////////////////////////////////////////////
//	Function	:	"EnemyDemon.cpp"
//
//	Author		:	John Albright (JA)
//
//	Purpose		:	Sets up the demon enemy
//////////////////////////////////////////////////////////////////////////////
#include "EnemyDemon.h"
#include "CSGD_TextureManager.h"
#include "CPlayerCharacter.h"
#include "CBullet.h"
#include "Enums.h"
#include "CSGD_MessageSystem.h"
#include "CSGD_ObjectManager.h"
#include "CDestroyMessage.h"
#include "CGame.h"
#include "CSGD_EventSystem.h"
#include "Options.h"
#include "CSGD_WaveManager.h"
#include "Options.h"

CEnemyDemon::CEnemyDemon()
{
	COptions *op = COptions::GetInstance();
	CSGD_EventSystem *pES = CSGD_EventSystem::GetInstance();

	SetHP(25 * op->GetDifficulty());
	SetVelX(100.0f * op->GetDifficulty());
	SetVelY(100.0f * op->GetDifficulty());
	SetXFlip(-1);
	SetHeight(64);
	SetWidth(64);
	SetPosX(500);
	SetPosY(500);
	SetType(ACTOR_ENEMY);
	SetPower(5 * op->GetDifficulty());
	SetScoreValue(100 * op->GetDifficulty());
	SetHit(false);

	rEnemyRect.top = 65;
	rEnemyRect.bottom = 123;
	rEnemyRect.left = 0;
	rEnemyRect.right = 60;

	m_nSoundID = CSGD_WaveManager::GetInstance()->LoadWave("Resource/sounds/JoA_Bark.wav");
	CSGD_WaveManager::GetInstance()->SetVolume(m_nSoundID,COptions::GetInstance()->GetSFXVolume()-40);


}

CEnemyDemon::~CEnemyDemon()
{
	CSGD_WaveManager *pWM = CSGD_WaveManager::GetInstance();
	pWM->UnloadWave(m_nSoundID);
}

void CEnemyDemon::Update(float fElapsedTime)
{

	CSGD_ObjectManager *pOM = CSGD_ObjectManager::GetInstance();
	CSGD_MessageSystem *pMS = CSGD_MessageSystem::GetInstance();
	CGame *theGame = CGame::GetInstance();
	
	SetPosX(GetPosX() + GetVelX() * fElapsedTime);
	SetPosY(GetPosY() + GetVelY() * fElapsedTime);

	rCollisionRect.left = GetPosX();
	rCollisionRect.right = GetPosX() + GetWidth();
	rCollisionRect.top = GetPosY();
	rCollisionRect.bottom = GetPosY() + GetWidth();
	SetCollisionRect(rCollisionRect);

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


	DWORD timer = GetTickCount();

	if (abs(GetPosX() - CPlayerCharacter::GetInstance()->GetPosX()) < 128 && abs(GetPosY() - CPlayerCharacter::GetInstance()->GetPosY()) < 128)
		bIsClose = true;
	else
		bIsClose = false;

	if (!bIsClose)
	{
		if (timer % 525 <= 75)
		{
			rEnemyRect.left = 0;
			rEnemyRect.right = 48;
		}
		else if (timer % 525 > 75 && timer % 525 <= 150)
		{
			rEnemyRect.left = 51;
			rEnemyRect.right = 90;
		}
		else if (timer % 525 > 150 && timer % 525 <= 225)
		{
			rEnemyRect.left = 93;
			rEnemyRect.right = 138;
		}
		else if (timer % 525 > 225 && timer % 525 <= 300)
		{
			rEnemyRect.left = 142;
			rEnemyRect.right = 185;
		}
		else if (timer % 525 > 300 && timer % 525 <= 375)
		{
			rEnemyRect.left = 188;
			rEnemyRect.right = 234;
		}
		else if (timer % 525 > 375 && timer % 525 <= 450)
		{
			rEnemyRect.left = 237;
			rEnemyRect.right = 282;
		}
		else if (timer % 525 > 450)
		{
			rEnemyRect.left = 286;
			rEnemyRect.right = 332;
		}
	}
	else if (bIsClose)
	{
		if (timer % 600 <= 75)
		{
			rEnemyRect.left = 335;
			rEnemyRect.right = 372;
		}
		else if (timer % 600 > 75 && timer % 600 <= 150)
		{
			rEnemyRect.left = 375;
			rEnemyRect.right = 407;
		}
		else if (timer % 600 > 150 && timer % 600 <= 225)
		{
			rEnemyRect.left = 410;
			rEnemyRect.right = 445;
		}
		else if (timer % 600 > 225 && timer % 600 <= 300)
		{
			rEnemyRect.left = 448;
			rEnemyRect.right = 481;
		}
		else if (timer % 600 > 300 && timer % 600 <= 375)
		{
			rEnemyRect.left = 484;
			rEnemyRect.right = 515;
		}
		else if (timer % 600 > 375 && timer % 600 <= 450)
		{
			rEnemyRect.left = 518;
			rEnemyRect.right = 554;
			CSGD_WaveManager::GetInstance()->Play(m_nSoundID,0);

		}
		else if (timer % 600 > 450 && timer % 600 <= 525)
		{
			rEnemyRect.left = 558;
			rEnemyRect.right = 592;
		}
		else if (timer % 600 > 525)
		{
			rEnemyRect.left = 595;
			rEnemyRect.right = 636;
		}
	}
	SetHit(false);

	if (GetHP() <= 0)
	{
		CSGD_EventSystem::GetInstance()->SendEvent("spawn powerups", NULL);
		pMS->SendMsg(new CDestroyMessage(this));
	}

}

void CEnemyDemon::Render(void)
{
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();

	if(GetHP() > 0)
	{
		if (GetXFlip() == 1)
			pTM->Draw(GetImageID(),GetCollisionRect().left, GetCollisionRect().top, GetXFlip() * 1.5f,1.5f,&rEnemyRect,GetWidth() / 2, GetHeight() /2,0.0f,D3DCOLOR_XRGB(255,255,255));
		else if (GetXFlip() == -1)
			pTM->Draw(GetImageID(),GetCollisionRect().right, GetCollisionRect().top, GetXFlip() * 1.5f,1.5f,&rEnemyRect,GetWidth() / 2, GetHeight() /2,0.0f,D3DCOLOR_XRGB(255,255,255));
	//	pTM->Draw(GetImageID(),GetPosX() - GetWidth()/4, GetPosY() - GetHeight()/4, GetXFlip() * 1.5f,1.5f,&rEnemyRect,GetWidth() / 2, GetHeight() /2,0.0f,D3DCOLOR_XRGB(255,255,255));
	}
}

void CEnemyDemon::HandleEvent(CEvent *pEvent)
{
}