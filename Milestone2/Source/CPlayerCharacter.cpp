//////////////////////////////////////////////////////////////////////////////
//	Function	:	"CPlayerCharacter.cpp"
//
//	Author		:	John Albright (JA)
//
//	Purpose		:	Sets up the player character
//////////////////////////////////////////////////////////////////////////////
#include "CPlayerCharacter.h"
#include "CSGD_TextureManager.h"
#include "CSGD_Direct3D.h"
#include "CSGD_DirectInput.h"
#include "CSGD_EventSystem.h"
#include "CSGD_MessageSystem.h"
#include "CSGD_ObjectManager.h"
#include "CSGD_ObjectFactory.h"
#include "CBullet.h"
#include "GamePlay.h"
#include "Enums.h"
#include "CGame.h"
#include "MainMenu.h"
#include "CCollisionMessage.h"
#include "LevelTransition.h"

CPlayerCharacter* CPlayerCharacter::GetInstance()
{
	static CPlayerCharacter instance;
	return &instance;
}

CPlayerCharacter::CPlayerCharacter()
{
	CSGD_ObjectFactory<string, CBase> *pOF		= CSGD_ObjectFactory<string, CBase>::GetInstance();
	CSGD_EventSystem *m_pES = CSGD_EventSystem::GetInstance();
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	CSGD_ObjectManager *pOM = CSGD_ObjectManager::GetInstance();

	m_nXFlip = 1;
	m_nOffSet = 0;

	m_nInvulnerableTimer = 2.0f;

	SetVelX(0.00000004f);
	SetVelY(0.00000004f);

	SetWidth(64);
	SetHeight(64);

	m_nColors = 255;

	bIsMoving = false;
	bIsShooting = false;

	SetType(ACTOR_PLAYER);

	m_nImageID[1] = pTM->LoadTexture("Resource\\SGD_laser.bmp", D3DCOLOR_XRGB(0,0,0));

	m_pES->RegisterClient("move left", this);
	m_pES->RegisterClient("move right", this);

	m_pES->RegisterClient("shoot up",this);
	m_pES->RegisterClient("shoot down",this);
	m_pES->RegisterClient("shoot left",this);
	m_pES->RegisterClient("shoot right",this);
	m_pES->RegisterClient("shoot up-left",this);
	m_pES->RegisterClient("shoot up-right",this);
	m_pES->RegisterClient("shoot down-left",this);
	m_pES->RegisterClient("shoot down-right",this);

	m_pES->RegisterClient("scroll right", this);
	m_pES->RegisterClient("scroll left",this);

}

CPlayerCharacter::~CPlayerCharacter()
{
	CSGD_EventSystem *m_pES = CSGD_EventSystem::GetInstance();
	m_pES->UnregisterClient(this);

}

void CPlayerCharacter::Update(float fElapsedTime)
{
	CGamePlay *pGP = CGamePlay::GetInstance();
	CSGD_MessageSystem *pMS = CSGD_MessageSystem::GetInstance();

	rCollisionRect.left = GetPosX();
	rCollisionRect.right = GetPosX() + GetWidth();
	rCollisionRect.top = GetPosY();
	rCollisionRect.bottom = GetPosY() + GetWidth();
	SetCollisionRect(rCollisionRect);

	m_fRapidFireTimer -= fElapsedTime;
	m_fThreeWayTimer -= fElapsedTime;

	if (m_fRapidFireTimer <= 0)
	{
		m_nRateOfFire = 7;
		m_fRapidFireTimer = 0;
	}
	else
		m_nRateOfFire = 30;

	if (m_fThreeWayTimer <= 0)
	{
		bFireThreeWay = false;
		m_fThreeWayTimer = 0;
	}
	else
		bFireThreeWay = true;

	if (bInvulnerable)
	{
		m_nColors = 0;
		m_nInvulnerableTimer -= fElapsedTime;
		if (m_nInvulnerableTimer <= 0)
		{
			SetInvulnerable(false);
			m_nInvulnerableTimer = 2;
		}
	}
	else
		m_nColors = 255;

	CBase *enemy;
	for (int i = 0; i < pGP->GetNumEnemies(); ++i)
	{
		enemy = pGP->GetEnemy(i);
		RECT r = enemy->GetCollisionRect();

		RECT cross;

		if (IntersectRect(&cross,&rCollisionRect,&r))
			pMS->SendMsg(new CCollisionMessage(enemy,this));
	}

	CSGD_DirectInput *m_pDI = CSGD_DirectInput::GetInstance();
	CSGD_EventSystem *m_pES = CSGD_EventSystem::GetInstance();

	// movement
	if (m_pDI->GetKey(DIK_LEFT) && !m_pDI->GetKey(DIK_S) && !m_pDI->GetKey(DIK_RIGHT))
		m_pES->SendEvent("move left", &fElapsedTime);
	if (m_pDI->GetKey(DIK_RIGHT) && !m_pDI->GetKey(DIK_S) && !m_pDI->GetKey(DIK_LEFT))
		m_pES->SendEvent("move right", &fElapsedTime);

	// shooting
	if (m_pDI->GetKey(DIK_S) && !m_pDI->GetKey(DIK_LEFT) && !m_pDI->GetKey(DIK_RIGHT) && !m_pDI->GetKey(DIK_UP) && !m_pDI->GetKey(DIK_DOWN) && m_nXFlip == -1)
		m_pES->SendEvent("shoot left", &fElapsedTime);

	if (m_pDI->GetKey(DIK_S) && !m_pDI->GetKey(DIK_LEFT) && !m_pDI->GetKey(DIK_RIGHT) && !m_pDI->GetKey(DIK_UP) && !m_pDI->GetKey(DIK_DOWN) && m_nXFlip == 1)
		m_pES->SendEvent("shoot right", &fElapsedTime);

	if (m_pDI->GetKey(DIK_S) && m_pDI->GetKey(DIK_LEFT) && !m_pDI->GetKey(DIK_RIGHT) && !m_pDI->GetKey(DIK_UP) && !m_pDI->GetKey(DIK_DOWN))
		m_pES->SendEvent("shoot left", &fElapsedTime);

	if (m_pDI->GetKey(DIK_S) && m_pDI->GetKey(DIK_RIGHT) && !m_pDI->GetKey(DIK_LEFT) && !m_pDI->GetKey(DIK_UP) && !m_pDI->GetKey(DIK_DOWN))
		m_pES->SendEvent("shoot right", &fElapsedTime);

	if (m_pDI->GetKey(DIK_S) && m_pDI->GetKey(DIK_UP) && !m_pDI->GetKey(DIK_LEFT) && !m_pDI->GetKey(DIK_RIGHT) && !m_pDI->GetKey(DIK_DOWN))
		m_pES->SendEvent("shoot up", &fElapsedTime);

	if (m_pDI->GetKey(DIK_S) && m_pDI->GetKey(DIK_DOWN) && !m_pDI->GetKey(DIK_LEFT) && !m_pDI->GetKey(DIK_RIGHT) && !m_pDI->GetKey(DIK_UP))
		m_pES->SendEvent("shoot down", &fElapsedTime);

	if (m_pDI->GetKey(DIK_S) && m_pDI->GetKey(DIK_DOWN) && m_pDI->GetKey(DIK_RIGHT) && !m_pDI->GetKey(DIK_LEFT) && !m_pDI->GetKey(DIK_UP))
		m_pES->SendEvent("shoot down-right", &fElapsedTime);

	if (m_pDI->GetKey(DIK_S) && m_pDI->GetKey(DIK_DOWN) && m_pDI->GetKey(DIK_LEFT) && !m_pDI->GetKey(DIK_RIGHT) && !m_pDI->GetKey(DIK_UP))
		m_pES->SendEvent("shoot down-left", &fElapsedTime);

	if (m_pDI->GetKey(DIK_S) && m_pDI->GetKey(DIK_UP) && m_pDI->GetKey(DIK_LEFT) && !m_pDI->GetKey(DIK_RIGHT) && !m_pDI->GetKey(DIK_DOWN))
		m_pES->SendEvent("shoot up-left", &fElapsedTime);

	if (m_pDI->GetKey(DIK_S) && m_pDI->GetKey(DIK_UP) && m_pDI->GetKey(DIK_RIGHT) && !m_pDI->GetKey(DIK_LEFT) && !m_pDI->GetKey(DIK_DOWN))
		m_pES->SendEvent("shoot up-right", &fElapsedTime);

	if (GetPosX() >= 401)
	{
		float pos = GetPosX();
		if ((CGamePlay::GetInstance()->GetBridgeOffset() > -100 || CGamePlay::GetInstance()->GetBridgeOffset() <= -100 && GetPosX() >= 640))
			m_pES->SendEvent("scroll right", &pos);
	}
	if (GetPosX() < 240)
	{
		float pos = GetPosX();
		if ((CGamePlay::GetInstance()->GetBridgeOffset() >= 0 && GetPosX() <=0) || CGamePlay::GetInstance()->GetBridgeOffset() < 0 && GetPosX() <= 240)
			m_pES->SendEvent("scroll left", &pos);
	}

	bIsMoving = false;
	bIsShooting = false;
}

void CPlayerCharacter::Render(void)
{
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();

	if (!CGame::GetInstance()->GetPaused())
	{
		if (bIsShooting)
		{
			DWORD timer = GetTickCount();
			if (timer % 225 <= 75)
				rPlayerRect = rShootingRect[0];
			if (timer % 225 > 75 && timer % 225 <= 150)
				rPlayerRect = rShootingRect[1];
			if (timer % 225 > 150)
				rPlayerRect = rShootingRect[2];
		}

		//moving animation
		else if (bIsMoving)
		{
			DWORD timer = GetTickCount();

			if (timer % 1700 <= 100)
			{
				rPlayerRect.bottom = 166;
				rPlayerRect.top = 85;
				rPlayerRect.left = 0;
				rPlayerRect.right = 76;
			}
			if (timer % 1700 > 100 && timer % 1700 <= 200)
			{
				rPlayerRect.bottom = 166;
				rPlayerRect.top = 90;
				rPlayerRect.left = 77;
				rPlayerRect.right = 153;
			}
			if (timer % 1700 > 200 && timer % 1700 <= 300)
			{
				rPlayerRect.bottom = 166;
				rPlayerRect.top = 90;
				rPlayerRect.left = 154;
				rPlayerRect.right = 230;
			}
			if (timer % 1700 > 300 && timer % 1700 <= 400)
			{
				rPlayerRect.bottom = 166;
				rPlayerRect.top = 90;
				rPlayerRect.left = 231;
				rPlayerRect.right = 307;
			}
			if (timer % 1700 > 400 && timer % 1700 <= 500)
			{
				rPlayerRect.bottom = 166;
				rPlayerRect.top = 90;
				rPlayerRect.left = 308;
				rPlayerRect.right = 384;
			}
			if (timer % 1700 > 500 && timer % 1700 <= 600)
			{
				rPlayerRect.bottom = 166;
				rPlayerRect.top = 90;
				rPlayerRect.left = 385;
				rPlayerRect.right = 461;
			}
			if (timer % 1700 > 600 && timer % 1700 <= 700)
			{
				rPlayerRect.bottom = 166;
				rPlayerRect.top = 90;
				rPlayerRect.left = 462;
				rPlayerRect.right = 549;
			}
			if (timer % 1700 > 700 && timer % 1700 <= 800)
			{
				rPlayerRect.bottom = 166;
				rPlayerRect.top = 90;
				rPlayerRect.left = 550;
				rPlayerRect.right = 615;
			}
			if (timer % 1700 > 800 && timer % 1700 <= 900)
			{
				rPlayerRect.bottom = 166;
				rPlayerRect.top = 90;
				rPlayerRect.left = 621;
				rPlayerRect.right = 692;
			}
			if (timer % 1700 > 900 && timer % 1700 <= 1000)
			{
				rPlayerRect.bottom = 166;
				rPlayerRect.top = 90;
				rPlayerRect.left = 693;
				rPlayerRect.right = 764;
			}
			if (timer % 1700 > 1000 && timer % 1700 <= 1100)
			{
				rPlayerRect.bottom = 166;
				rPlayerRect.top = 90;
				rPlayerRect.left = 765;
				rPlayerRect.right = 841;
			}
			if (timer % 1700 > 1100 && timer % 1700 <= 1200)
			{
				rPlayerRect.bottom = 166;
				rPlayerRect.top = 90;
				rPlayerRect.left = 842;
				rPlayerRect.right = 918;
			}
			if (timer % 1700 > 1200 && timer % 1700 <= 1300)
			{
				rPlayerRect.bottom = 257;
				rPlayerRect.top = 181;
				rPlayerRect.left = 19;
				rPlayerRect.right = 95;
			}
			if (timer % 1700 > 1300 && timer % 1700 <= 1400)
			{
				rPlayerRect.bottom = 257;
				rPlayerRect.top = 181;
				rPlayerRect.left = 96;
				rPlayerRect.right = 172;
			}
			if (timer % 1700 > 1400 && timer % 1700 <= 1500)
			{
				rPlayerRect.bottom = 257;
				rPlayerRect.top = 181;
				rPlayerRect.left = 173;
				rPlayerRect.right = 252;
			}
			if (timer % 1700 > 1500 && timer % 1700 <= 1600)
			{
				rPlayerRect.bottom = 257;
				rPlayerRect.top = 181;
				rPlayerRect.left = 253;
				rPlayerRect.right = 329;
			}
			if (timer % 1700 > 1600)
			{
				rPlayerRect.bottom = 257;
				rPlayerRect.top = 181;
				rPlayerRect.left = 330;
				rPlayerRect.right = 406;
			}

		}

		//idle animation
		else if (!bIsMoving && !bIsShooting)
		{
			DWORD timer = GetTickCount();

			m_nOffSet = 0;
			

			rPlayerRect.top = 0;
			rPlayerRect.bottom = 75;
			if (timer % 750 <= 125)
			{
				rPlayerRect.left = 15;
				rPlayerRect.right = 72;
			}

			if (timer % 750 > 125 && timer % 750 <= 250)
			{
				rPlayerRect.left = 88;
				rPlayerRect.right = 145;

			}

			if (timer % 750 > 250 && timer % 750 <= 375)
			{
				rPlayerRect.left = 161;
				rPlayerRect.right = 218;
			}

			if (timer % 750 > 375 && timer % 750 <= 500)
			{
				rPlayerRect.left = 234;
				rPlayerRect.right = 291;
			}

			if (timer % 750 > 500 && timer % 750 <= 625)
			{
				rPlayerRect.left = 307;
				rPlayerRect.right = 364;
			}

			if (timer % 750 > 625)
			{
				rPlayerRect.left = 380;
				rPlayerRect.right = 437;
			}
		}
	}
	if (m_nXFlip == 1)
		pTM->Draw(GetImageID(),GetCollisionRect().left,GetCollisionRect().top-m_nOffSet ,m_nXFlip,1,&rPlayerRect,0,0,0,D3DCOLOR_XRGB(255,m_nColors,m_nColors));
	else if (m_nXFlip == -1)
		pTM->Draw(GetImageID(),GetCollisionRect().right,GetCollisionRect().top-m_nOffSet ,m_nXFlip,1,&rPlayerRect,0,0,0,D3DCOLOR_XRGB(255,m_nColors,m_nColors));


}

void CPlayerCharacter::HandleEvent(CEvent *pEvent)
{
 	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	CSGD_ObjectManager *pOM = CSGD_ObjectManager::GetInstance();
	CSGD_EventSystem *pEM = CSGD_EventSystem::GetInstance();
	CSGD_ObjectFactory<string, CBase> *pOF		= CSGD_ObjectFactory<string, CBase>::GetInstance();

	if (pEvent->GetEventID() == "scroll right")
	{
		if (CGamePlay::GetInstance()->GetBridgeOffset() > -100)
			SetPosX(400);
		else if (CGamePlay::GetInstance()->GetBridgeOffset() <= -100 && GetPosX() >= 640)
			SetPosX(639);
	}

	else if (pEvent->GetEventID() == "scroll left")
	{
		if (CGamePlay::GetInstance()->GetBridgeOffset() >= 0 && GetPosX() <=0)
			SetPosX(1);
		else if (CGamePlay::GetInstance()->GetBridgeOffset() < 0 && GetPosX() <= 240)
			SetPosX(241);
	}

	else if (pEvent->GetEventID() == "move left")
	{
		m_nOffSet = 0;
		this;

		SetPosX(GetPosX() + GetVelX() * *((float*)pEvent->GetParam()) * CGame::GetInstance()->GetSpeed());
		bIsMoving = true;
		m_nXFlip = -abs(m_nXFlip);
	}
	else if (pEvent->GetEventID() == "move right")
	{
		m_nOffSet = 0;

		SetPosX(GetPosX() - GetVelX() * *((float*)pEvent->GetParam()) * CGame::GetInstance()->GetSpeed());
		bIsMoving = true;
		m_nXFlip = abs(m_nXFlip);
	}
	else if (pEvent->GetEventID() == "shoot left")
	{
		m_nOffSet = -16;

		m_nXFlip = -abs(m_nXFlip);
		bIsMoving = false;
		bIsShooting = true;

		rShootingRect[0].left = 10;
		rShootingRect[0].top = 1155;
		rShootingRect[0].right = 110;
		rShootingRect[0].bottom = 1215;

		rShootingRect[1].left = 125;
		rShootingRect[1].top = 1155;
		rShootingRect[1].right = 200;
		rShootingRect[1].bottom = 1215;

		rShootingRect[2].left = 215;
		rShootingRect[2].top = 1155;
		rShootingRect[2].right = 330;
		rShootingRect[2].bottom = 1215;

		if (GetTickCount() % 100 < GetRateOfFire())
		{
			pBullet = pOF->CreateObject("CBullet");
			pBullet->SetImageID(m_nImageID[1]);

			pBullet->SetVelX(-500);
			pBullet->SetVelY(0);
			pBullet->SetPosX(GetPosX() - 32);
			pBullet->SetPosY(GetPosY() + 32);

			pBullet->SetHeight(16);
			pBullet->SetWidth(32);
			pBullet->SetRotation(1.5708f);
			if (m_nRateOfFire == 7.0f)
				pBullet->SetPower(5);
			else
				pBullet->SetPower(2);

			if (GetThreeWay())
			{
				pEM->SendEvent("shoot up-left", pEvent->GetParam());
				pEM->SendEvent("shoot down-left", pEvent->GetParam());
			}

			pOM->AddObject(pBullet);

		}
	}
	else if (pEvent->GetEventID() == "shoot right")
	{
		m_nOffSet = -16;

		m_nXFlip = abs(m_nXFlip);
		bIsMoving = false;
		bIsShooting = true;

		rShootingRect[0].left = 10;
		rShootingRect[0].top = 1155;
		rShootingRect[0].right = 110;
		rShootingRect[0].bottom = 1215;

		rShootingRect[1].left = 125;
		rShootingRect[1].top = 1155;
		rShootingRect[1].right = 200;
		rShootingRect[1].bottom = 1215;

		rShootingRect[2].left = 215;
		rShootingRect[2].top = 1155;
		rShootingRect[2].right = 330;
		rShootingRect[2].bottom = 1215;

		if (GetTickCount() % 100 < GetRateOfFire())
		{

			pBullet = pOF->CreateObject("CBullet");
			pBullet->SetImageID(m_nImageID[1]);

			pBullet->SetVelX(500);
			pBullet->SetVelY(0);
			pBullet->SetPosX(GetPosX() + 80);
			pBullet->SetPosY(GetPosY() + 32);

			pBullet->SetHeight(16 * m_nBulletScale);
			pBullet->SetWidth(32 * m_nBulletScale);
			pBullet->SetRotation(1.5708f);
			if (m_nRateOfFire == 7.0f)
				pBullet->SetPower(5);
			else
				pBullet->SetPower(2);

			if (GetThreeWay())
			{
				pEM->SendEvent("shoot up-right", pEvent->GetParam());
				pEM->SendEvent("shoot down-right", pEvent->GetParam());
			}

			pOM->AddObject(pBullet);
		}

	}
	else if (pEvent->GetEventID() == "shoot up")
	{

		m_nOffSet = 40;

		bIsMoving = false;
		bIsShooting = true;

		rShootingRect[0].left = 10;
		rShootingRect[0].top = 1230;
		rShootingRect[0].right = 85;
		rShootingRect[0].bottom = 1350;

		rShootingRect[1].left = 100;
		rShootingRect[1].top = 1230;
		rShootingRect[1].right = 175;
		rShootingRect[1].bottom = 1350;

		rShootingRect[2].left = 190;
		rShootingRect[2].top = 1230;
		rShootingRect[2].right = 265;
		rShootingRect[2].bottom = 1350;

		if (GetTickCount() % 100 < GetRateOfFire())
		{

			pBullet = pOF->CreateObject("CBullet");
			pBullet->SetImageID(m_nImageID[1]);

			pBullet->SetVelX(0);
			pBullet->SetVelY(-500);
			pBullet->SetPosX(GetPosX() + 24);
			pBullet->SetPosY(GetPosY() - 32);

			pBullet->SetHeight(32 * m_nBulletScale);
			pBullet->SetWidth(16 * m_nBulletScale);
			pBullet->SetRotation(0);
			if (m_nRateOfFire == 7.0f)
				pBullet->SetPower(5);
			else
				pBullet->SetPower(2);

			if (GetThreeWay())
			{
				pEM->SendEvent("shoot up-left", pEvent->GetParam());
				pEM->SendEvent("shoot up-right", pEvent->GetParam());
			}

			pOM->AddObject(pBullet);
		}
	}
	else if (pEvent->GetEventID() == "shoot down")
	{
		m_nOffSet = -24;

		bIsMoving = false;
		bIsShooting = true;

		rShootingRect[0].left = 10;
		rShootingRect[0].right = 75;
		rShootingRect[0].top = 1400;
		rShootingRect[0].bottom = 1500;

		rShootingRect[1].left = 90;
		rShootingRect[1].right = 155;
		rShootingRect[1].top = 1400;
		rShootingRect[1].bottom = 1500;

		rShootingRect[2].left = 170;
		rShootingRect[2].right = 235;
		rShootingRect[2].top = 1400;
		rShootingRect[2].bottom = 1500;

		if (GetTickCount() % 100 < GetRateOfFire())
		{

			pBullet = pOF->CreateObject("CBullet");
			pBullet->SetImageID(m_nImageID[1]);

			pBullet->SetVelX(0);
			pBullet->SetVelY(500);
			pBullet->SetPosX(GetPosX() + 24);
			pBullet->SetPosY(GetPosY() + 80);

			pBullet->SetHeight(32 * m_nBulletScale);
			pBullet->SetWidth(16 * m_nBulletScale);
			pBullet->SetRotation(0);
			if (m_nRateOfFire == 7.0f)
				pBullet->SetPower(5);
			else
				pBullet->SetPower(2);

			if (GetThreeWay())
			{
				pEM->SendEvent("shoot down-left", pEvent->GetParam());
				pEM->SendEvent("shoot down-right", pEvent->GetParam());
			}

			pOM->AddObject(pBullet);
		}

	}
	else if (pEvent->GetEventID() == "shoot down-right")
	{
		m_nOffSet = -24;

		bIsMoving = false;
		bIsShooting = true;
		m_nXFlip = abs(m_nXFlip);

		rShootingRect[0].left = 10;
		rShootingRect[0].right = 105;
		rShootingRect[0].top = 1510;
		rShootingRect[0].bottom = 1590;

		rShootingRect[1].left = 120;
		rShootingRect[1].right = 200;
		rShootingRect[1].top = 1510;
		rShootingRect[1].bottom = 1590;

		rShootingRect[2].left = 205;
		rShootingRect[2].right = 305;
		rShootingRect[2].top = 1510;
		rShootingRect[2].bottom = 1590;

		if (GetTickCount() % 100 < GetRateOfFire())
		{

			pBullet = pOF->CreateObject("CBullet");
			pBullet->SetImageID(m_nImageID[1]);

			pBullet->SetVelX(500);
			pBullet->SetVelY(500);
			pBullet->SetPosX(GetPosX() + 75);
			pBullet->SetPosY(GetPosY() + 64);

			pBullet->SetHeight(32 * m_nBulletScale);
			pBullet->SetWidth(32 * m_nBulletScale);
			pBullet->SetRotation(-0.7854f);
			if (m_nRateOfFire == 7.0f)
				pBullet->SetPower(5);
			else
				pBullet->SetPower(2);

			pOM->AddObject(pBullet);
		}

	}
	else if (pEvent->GetEventID() == "shoot down-left")
	{
		m_nOffSet = -24;

		bIsMoving = false;
		bIsShooting = true;
		m_nXFlip = -abs(m_nXFlip);

		rShootingRect[0].left = 10;
		rShootingRect[0].right = 105;
		rShootingRect[0].top = 1510;
		rShootingRect[0].bottom = 1590;

		rShootingRect[1].left = 120;
		rShootingRect[1].right = 200;
		rShootingRect[1].top = 1510;
		rShootingRect[1].bottom = 1590;

		rShootingRect[2].left = 205;
		rShootingRect[2].right = 305;
		rShootingRect[2].top = 1510;
		rShootingRect[2].bottom = 1590;

		if (GetTickCount() % 100 < GetRateOfFire())
		{

			pBullet = pOF->CreateObject("CBullet");
			pBullet->SetImageID(m_nImageID[1]);

			pBullet->SetVelX(-500);
			pBullet->SetVelY(500);
			pBullet->SetPosX(GetPosX() - 32);
			pBullet->SetPosY(GetPosY() + 80);

			pBullet->SetHeight(32 * m_nBulletScale);
			pBullet->SetWidth(32 * m_nBulletScale);
			pBullet->SetRotation(0.7854f);
			if (m_nRateOfFire == 7.0f)
				pBullet->SetPower(5);
			else
				pBullet->SetPower(2);

			pOM->AddObject(pBullet);
		}
	}
	else if (pEvent->GetEventID() == "shoot up-right")
	{
		m_nOffSet = 16;

		bIsMoving = false;
		bIsShooting = true;
		m_nXFlip = abs(m_nXFlip);

		rShootingRect[0].left = 10;
		rShootingRect[0].right = 95;
		rShootingRect[0].top = 1610;
		rShootingRect[0].bottom = 1705;

		rShootingRect[1].left = 115;
		rShootingRect[1].right = 200;
		rShootingRect[1].top = 1610;
		rShootingRect[1].bottom = 1705;

		rShootingRect[2].left = 200;
		rShootingRect[2].right = 285;
		rShootingRect[2].top = 1605;
		rShootingRect[2].bottom = 1705;

		if (GetTickCount() % 100 < GetRateOfFire())
		{
			pBullet = pOF->CreateObject("CBullet");
			pBullet->SetImageID(m_nImageID[1]);

			pBullet->SetVelX(500);
			pBullet->SetVelY(-500);
			pBullet->SetPosX(GetPosX() + 64);
			pBullet->SetPosY(GetPosY() - 16);

			pBullet->SetHeight(32 * m_nBulletScale);
			pBullet->SetWidth(32 * m_nBulletScale);
			pBullet->SetRotation(0.7854f);
			if (m_nRateOfFire == 7.0f)
				pBullet->SetPower(5);
			else
				pBullet->SetPower(2);

			pOM->AddObject(pBullet);
		}
	}
	else if (pEvent->GetEventID() == "shoot up-left")
	{
		m_nOffSet = 16;

		bIsMoving = false;
		bIsShooting = true;
		m_nXFlip = -abs(m_nXFlip);

		rShootingRect[0].left = 10;
		rShootingRect[0].right = 95;
		rShootingRect[0].top = 1610;
		rShootingRect[0].bottom = 1705;

		rShootingRect[1].left = 115;
		rShootingRect[1].right = 200;
		rShootingRect[1].top = 1610;
		rShootingRect[1].bottom = 1705;

		rShootingRect[2].left = 200;
		rShootingRect[2].right = 285;
		rShootingRect[2].top = 1605;
		rShootingRect[2].bottom = 1705;

		if (GetTickCount() % 100 < GetRateOfFire())
		{
			pBullet = pOF->CreateObject("CBullet");
			pBullet->SetImageID(m_nImageID[1]);

			pBullet->SetVelX(-500);
			pBullet->SetVelY(-500);
			pBullet->SetPosX(GetPosX() - 32);
			pBullet->SetPosY(GetPosY() - 32);

			pBullet->SetHeight(32 * m_nBulletScale);
			pBullet->SetWidth(32 * m_nBulletScale);
			pBullet->SetRotation(-0.7854f);
			if (m_nRateOfFire == 7.0f)
				pBullet->SetPower(5);
			else
				pBullet->SetPower(2);

			pOM->AddObject(pBullet);
		}
	}
}