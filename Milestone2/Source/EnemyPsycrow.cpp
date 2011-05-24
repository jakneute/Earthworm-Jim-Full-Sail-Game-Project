//////////////////////////////////////////////////////////////////////////////
//	Function	:	"EnemyMeat.cpp"
//
//	Author		:	John Albright (JA)
//
//	Purpose		:	Sets up final boss
//////////////////////////////////////////////////////////////////////////////
#include "EnemyPsycrow.h"
#include "CSGD_TextureManager.h"
#include "BulletHook.h"
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
#include "GamePlay.h"

CEnemyPsycrow::CEnemyPsycrow()
{
	CSGD_EventSystem *pES = CSGD_EventSystem::GetInstance();
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	COptions *op = COptions::GetInstance();

	pES->RegisterClient("psycrow fire", this);
	pES->RegisterClient("state transition 1", this);
	pES->RegisterClient("state transition 2", this);
	pES->RegisterClient("state transition 3", this);
	pES->RegisterClient("state transition 4", this);

	m_nState = PSYCROW_STATE1;
	m_nSoundID = CSGD_WaveManager::GetInstance()->LoadWave("Resource/sounds/JoA_Squawk.wav");
	CSGD_WaveManager::GetInstance()->SetVolume(m_nSoundID, op->GetSFXVolume() - 40);
	m_fFireReady = 5.0f;
	m_fHitTimer = 0.4f;
	m_nOffset = 0;
	bIsFired = false;
	m_fFiringTimer = 0.5f;
	m_fSineFloat = 3.14f;
	m_nColor = 255;
	m_nScale = 1.5f;

	SetHP(250 * op->GetDifficulty());
	SetVelX(75 * op->GetDifficulty());
	SetVelY(0.0f);
	SetXFlip(-1);
	SetHeight(84);
	SetWidth(70);
	SetPosX(320);
	SetPosY(240);
	SetPower(0);
	SetType(ACTOR_ENEMY);
	SetScoreValue(1000 * op->GetDifficulty());
	SetHit(false);
	pBullet = NULL;

	bIsFiring = false;

}

CEnemyPsycrow::~CEnemyPsycrow()
{
	CSGD_EventSystem *pES = CSGD_EventSystem::GetInstance();
	pES->UnregisterClient(this);
	CSGD_WaveManager *pWM = CSGD_WaveManager::GetInstance();
	pWM->UnloadWave(m_nSoundID);
}

void CEnemyPsycrow::Update(float fElapsedTime)
{
	COptions *op = COptions::GetInstance();
	CSGD_ObjectManager *pOM = CSGD_ObjectManager::GetInstance();
	CSGD_MessageSystem *pMS = CSGD_MessageSystem::GetInstance();
	CSGD_EventSystem *pES= CSGD_EventSystem::GetInstance();
	CGame *theGame = CGame::GetInstance();

	if (m_nState % 2 == 0)
	{
		SetVelY(sin(m_fSineFloat) * 50 * op->GetDifficulty() * (m_nState + 0.5));
		rEnemyRect.top = 507;
		rEnemyRect.bottom = 561;
		rEnemyRect.left = 584;
		rEnemyRect.right = 650;

		if (!bIsFiring)
			m_fFireReady -= fElapsedTime;
		
		if (m_fFireReady <= 0)
			bIsFiring = true;

		if (bIsFiring)
		{
			m_fFiringTimer -= fElapsedTime;
			if (m_fFiringTimer <= 0.5f && m_fFiringTimer > 0.4f)
			{
				rEnemyRect.top = 507;
				rEnemyRect.bottom = 561;
				rEnemyRect.left = 494;
				rEnemyRect.right = 566;
				if (!bIsFired)
				{
					pES->SendEvent("psycrow fire", NULL);
					bIsFired = true;

				}
			}
			else if (m_fFiringTimer <= 0.4f && m_fFiringTimer > 0.3f)
			{
				rEnemyRect.top = 507;
				rEnemyRect.bottom = 567;
				rEnemyRect.left = 790;
				rEnemyRect.right = 856;
			}
			else if (m_fFiringTimer <= 0.3f && m_fFiringTimer > 0.2f)
			{
				rEnemyRect.top = 511;
				rEnemyRect.bottom = 564;
				rEnemyRect.left = 873;
				rEnemyRect.right = 935;
			}
			else if (m_fFiringTimer <= 0.2f && m_fFiringTimer > 0.1f)
			{
				rEnemyRect.top = 581;
				rEnemyRect.bottom = 641;
				rEnemyRect.left = 502;
				rEnemyRect.right = 573;
			}
			else if (m_fFiringTimer <= 0.1f)
			{
				rEnemyRect.top = 579;
				rEnemyRect.bottom = 639;
				rEnemyRect.left = 601;
				rEnemyRect.right = 673;
				bIsFiring = false;
				bIsFired = false;
				m_fFireReady = 5.0f;
				m_fFiringTimer = 0.5f;
			}

		}
	}
	else if (m_nState % 2 == 1)
	{
		rEnemyRect.top = 666;
		rEnemyRect.bottom = 726;
		DWORD timer = GetTickCount();
		if (timer % 300 <= 100)
		{
			rEnemyRect.left = 494;
			rEnemyRect.right = 533;
		}
		else if (timer % 300 > 100 && timer % 300 <= 200)
		{
			rEnemyRect.left = 542;
			rEnemyRect.right = 581;
		}
		else if (timer % 300 > 200)
		{
			rEnemyRect.left = 591;
			rEnemyRect.right = 630;
		}
	}

	if (GetHP() < op->GetDifficulty() * 200 && m_nState == PSYCROW_STATE1)
	{
		rEnemyRect.top = 674;
		rEnemyRect.bottom = 726;
		m_fHitTimer -= fElapsedTime;
		if (m_fHitTimer <= 0.4f && m_fHitTimer > 0.3f)
		{
			CSGD_WaveManager::GetInstance()->Play(m_nSoundID,0);
			rEnemyRect.left = 649;
			rEnemyRect.right = 708;
		}
		else if (m_fHitTimer <= 0.3f && m_fHitTimer > 0.2f)
		{
			rEnemyRect.left = 719;
			rEnemyRect.right = 772;
		}
		else if (m_fHitTimer <= 0.2f && m_fHitTimer > 0.1f)
		{
			rEnemyRect.left = 787;
			rEnemyRect.right = 844;
		}
		else if (m_fHitTimer <= 0.1f && m_fHitTimer > 0.0f)
		{
			rEnemyRect.left = 861;
			rEnemyRect.right = 912;
		}

		if (m_fHitTimer < 0)
			pES->SendEvent("state transition 1");
	}
	if (GetHP() < op->GetDifficulty() * 150 && m_nState == PSYCROW_STATE2)
	{
		rEnemyRect.top = 674;
		rEnemyRect.bottom = 726;
		m_fHitTimer -= fElapsedTime;
		if (m_fHitTimer <= 0.4f && m_fHitTimer > 0.3f)
		{
			CSGD_WaveManager::GetInstance()->Play(m_nSoundID,0);
			rEnemyRect.left = 649;
			rEnemyRect.right = 708;
		}
		else if (m_fHitTimer <= 0.3f && m_fHitTimer > 0.2f)
		{
			rEnemyRect.left = 719;
			rEnemyRect.right = 772;
		}
		else if (m_fHitTimer <= 0.2f && m_fHitTimer > 0.1f)
		{
			rEnemyRect.left = 787;
			rEnemyRect.right = 844;
		}
		else if (m_fHitTimer <= 0.1f && m_fHitTimer > 0.0f)
		{
			rEnemyRect.left = 861;
			rEnemyRect.right = 912;
		}

		if (m_fHitTimer < 0)
			pES->SendEvent("state transition 2");
	}
	if (GetHP() < op->GetDifficulty() * 100 && m_nState == PSYCROW_STATE3)
	{
		rEnemyRect.top = 674;
		rEnemyRect.bottom = 726;
		m_fHitTimer -= fElapsedTime;
		if (m_fHitTimer <= 0.4f && m_fHitTimer > 0.3f)
		{
			CSGD_WaveManager::GetInstance()->Play(m_nSoundID,0);
			rEnemyRect.left = 649;
			rEnemyRect.right = 708;
		}
		else if (m_fHitTimer <= 0.3f && m_fHitTimer > 0.2f)
		{
			rEnemyRect.left = 719;
			rEnemyRect.right = 772;
		}
		else if (m_fHitTimer <= 0.2f && m_fHitTimer > 0.1f)
		{
			rEnemyRect.left = 787;
			rEnemyRect.right = 844;
		}
		else if (m_fHitTimer <= 0.1f && m_fHitTimer > 0.0f)
		{
			rEnemyRect.left = 861;
			rEnemyRect.right = 912;
		}

		if (m_fHitTimer < 0)
			pES->SendEvent("state transition 3");
	}
	if (GetHP() < op->GetDifficulty() * 50 && m_nState == PSYCROW_STATE4)
	{
		rEnemyRect.top = 674;
		rEnemyRect.bottom = 726;
		m_fHitTimer -= fElapsedTime;
		if (m_fHitTimer <= 0.4f && m_fHitTimer > 0.3f)
		{
			CSGD_WaveManager::GetInstance()->Play(m_nSoundID,0);
			rEnemyRect.left = 649;
			rEnemyRect.right = 708;
		}
		else if (m_fHitTimer <= 0.3f && m_fHitTimer > 0.2f)
		{
			rEnemyRect.left = 719;
			rEnemyRect.right = 772;
		}
		else if (m_fHitTimer <= 0.2f && m_fHitTimer > 0.1f)
		{
			rEnemyRect.left = 787;
			rEnemyRect.right = 844;
		}
		else if (m_fHitTimer <= 0.1f && m_fHitTimer > 0.0f)
		{
			rEnemyRect.left = 861;
			rEnemyRect.right = 912;
		}

		if (m_fHitTimer < 0)
			pES->SendEvent("state transition 4");
	}
	if (GetHP() <= 0 && m_nState == PSYCROW_STATE5)
	{
		pMS->SendMsg(new CDestroyMessage(this));
	}

	if (CPlayerCharacter::GetInstance()->GetPosX() < GetPosX())
		SetXFlip(-1);
	else if (CPlayerCharacter::GetInstance()->GetPosX() >= GetPosX())
		SetXFlip(1);

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

	if (GetPosX() < -32)
	{
		SetVelX(-1 * GetVelX());
		SetPosX(-31);
	}
	else if (GetPosX() > 600)
	{
		SetVelX(-1 * GetVelX());
		SetPosX(599);
	}
	if (GetPosY() < 0)
	{
		SetVelY(-1 * GetVelY());
		SetPosY(1);
	}
	else if (GetPosY() > 400)
	{
		SetVelY(-abs(GetVelY()));
		SetPosY(399);
	}

	DWORD timer = GetTickCount();

}

void CEnemyPsycrow::Render(void)
{
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	CSGD_Direct3D *p3D = CSGD_Direct3D::GetInstance();
	CSGD_MessageSystem *pMS = CSGD_MessageSystem::GetInstance();

	RECT r = GetCollisionRect();

	if(GetHP() > 0)
	{
		if (GetXFlip() == 1)
			pTM->Draw(GetImageID(),GetCollisionRect().left + m_nOffset, GetCollisionRect().top, GetXFlip() * m_nScale,m_nScale,&rEnemyRect,GetWidth() / 2, GetHeight() /2,0.0f,D3DCOLOR_XRGB(255,m_nColor,m_nColor));
		else if (GetXFlip() == -1)
			pTM->Draw(GetImageID(),GetCollisionRect().right - m_nOffset, GetCollisionRect().top, GetXFlip() * m_nScale,m_nScale,&rEnemyRect,GetWidth() / 2, GetHeight() /2,0.0f,D3DCOLOR_XRGB(255,m_nColor,m_nColor));
	}
	this;

}

void CEnemyPsycrow::HandleEvent(CEvent *pEvent)
{
	CSGD_ObjectFactory<string, CBase> *pOF		= CSGD_ObjectFactory<string, CBase>::GetInstance();
	CSGD_ObjectManager *pOM						= CSGD_ObjectManager::GetInstance();
 	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	COptions *op = COptions::GetInstance();

	if (pEvent->GetEventID() == "psycrow fire")
	{
		pBullet = pOF->CreateObject("CBulletHook");

		pBullet->SetImageID(GetImageID());

		if (GetState() == PSYCROW_STATE1)
		{
			if (CPlayerCharacter::GetInstance()->GetPosX() > GetPosX())
				pBullet->SetVelX(op->GetDifficulty() * 150);
			else
				pBullet->SetVelX(op->GetDifficulty() * -150);
			if (CPlayerCharacter::GetInstance()->GetPosY() > GetPosY())
				pBullet->SetVelY(op->GetDifficulty() * 150);
			else
				pBullet->SetVelY(op->GetDifficulty() * -150);
			pBullet->SetScoreValue(1);
		}
		else if (GetState() == PSYCROW_STATE3)
		{
			if (CPlayerCharacter::GetInstance()->GetPosX() > GetPosX())
				pBullet->SetVelX(0);
			else
				pBullet->SetVelX(0);
			if (CPlayerCharacter::GetInstance()->GetPosY() > GetPosY())
				pBullet->SetVelY(0);
			else
				pBullet->SetVelY(0);
			pBullet->SetScoreValue(2);
		}
		else if (GetState() == PSYCROW_STATE5)
		{
			if (CPlayerCharacter::GetInstance()->GetPosX() > GetPosX())
				pBullet->SetVelX(op->GetDifficulty() * 50);
			else
				pBullet->SetVelX(op->GetDifficulty() * -50);
			if (CPlayerCharacter::GetInstance()->GetPosY() > GetPosY())
				pBullet->SetVelY(op->GetDifficulty() * 50);
			else
				pBullet->SetVelY(op->GetDifficulty() * -50);
			pBullet->SetScoreValue(3);
		}


		pBullet->SetPosX(GetPosX() + (GetXFlip() * 64));
		pBullet->SetPosY(GetPosY() + 48);
		pBullet->SetType(ACTOR_BULLET);

		pBullet->SetHeight(12);
		pBullet->SetWidth(28);
		pBullet->SetPower(5 * op->GetDifficulty());

		pOM->AddObject(pBullet);
	}

	else if (pEvent->GetEventID() == "state transition 1")
	{
		m_nState = PSYCROW_STATE2;
		m_nColor = 200;
		m_fHitTimer = 0.4f;
		SetVelX(100 * op->GetDifficulty());
		SetVelY(150 * op->GetDifficulty());
		SetPower(5 * COptions::GetInstance()->GetDifficulty());
		CGamePlay::GetInstance()->SetBridgeVelocity(COptions::GetInstance()->GetDifficulty() * 50);
		if (CPlayerCharacter::GetInstance()->GetPosX() < GetPosX())
			SetXFlip(-1);
		else if (CPlayerCharacter::GetInstance()->GetPosX() >= GetPosX())
			SetXFlip(1);
	}
	else if (pEvent->GetEventID() == "state transition 2")
	{
		m_nState = PSYCROW_STATE3;
		m_nColor = 150;
		m_fHitTimer = 0.4f;
		SetPower(0);
		SetVelX(75 * op->GetDifficulty() * m_nState);
		CGamePlay::GetInstance()->SetBridgeVelocity(COptions::GetInstance()->GetDifficulty() * 75);
		if (CPlayerCharacter::GetInstance()->GetPosX() < GetPosX())
			SetXFlip(-1);
		else if (CPlayerCharacter::GetInstance()->GetPosX() >= GetPosX())
			SetXFlip(1);
	}
	else if (pEvent->GetEventID() == "state transition 3")
	{
		m_nState = PSYCROW_STATE4;
		m_nColor = 100;
		m_fHitTimer = 0.4f;
		SetVelX(250 * op->GetDifficulty());
		SetVelY(150 * op->GetDifficulty());
		SetPower(5 * COptions::GetInstance()->GetDifficulty());
		CGamePlay::GetInstance()->SetBridgeVelocity(COptions::GetInstance()->GetDifficulty() * 100);
		if (CPlayerCharacter::GetInstance()->GetPosX() < GetPosX())
			SetXFlip(-1);
		else if (CPlayerCharacter::GetInstance()->GetPosX() >= GetPosX())
			SetXFlip(1);
	}
	else if (pEvent->GetEventID() == "state transition 4")
	{
		m_nState = PSYCROW_STATE5;
		m_nColor = 50;
		m_fHitTimer = 0.4f;
		SetPower(0);
		SetVelX(75 * op->GetDifficulty() * m_nState);
		CGamePlay::GetInstance()->SetBridgeVelocity(COptions::GetInstance()->GetDifficulty() * 125);
		if (CPlayerCharacter::GetInstance()->GetPosX() < GetPosX())
			SetXFlip(-1);
		else if (CPlayerCharacter::GetInstance()->GetPosX() >= GetPosX())
			SetXFlip(1);
	}

}