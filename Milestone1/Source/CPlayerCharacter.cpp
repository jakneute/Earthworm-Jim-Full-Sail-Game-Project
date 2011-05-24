//////////////////////////////////////////////////////////////////////////////
//	Function	:	"CDerived.cpp"
//
//	Author		:	David Brown (DB)
//
//	Purpose		:	To define an object that will bounce around the screen.
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

	SetHP(100);
	SetPosX( 100.0f );
	SetPosY( 350.0f );
	SetRateOfFire(7);
	SetBulletScale(1.0f);
	SetGodMode(false);

	SetVelX(0.00000004f);
	SetVelY(0.00000004f);

	bIsMoving = false;
	bIsShooting = false;

	m_nImageID[0] = pTM->LoadTexture("Resource\\EarthwormJim.bmp", D3DCOLOR_XRGB(80,192,64));
	m_nImageID[1] = pTM->LoadTexture("Resource\\SGD_laser.bmp", D3DCOLOR_XRGB(0,0,0));


	SetImageID(m_nImageID[0]);

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


}

CPlayerCharacter::~CPlayerCharacter()
{
	CSGD_EventSystem *m_pES = CSGD_EventSystem::GetInstance();
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	CSGD_ObjectManager *pOM = CSGD_ObjectManager::GetInstance();

	m_pES->UnregisterClient(this);

}

void CPlayerCharacter::Update(float fElapsedTime)
{
	CSGD_DirectInput *m_pDI = CSGD_DirectInput::GetInstance();
	CSGD_EventSystem *m_pES = CSGD_EventSystem::GetInstance();

	// movement
	if (m_pDI->GetKey(DIK_LEFT) && !m_pDI->GetKey(DIK_LCONTROL) && !m_pDI->GetKey(DIK_RIGHT))
		m_pES->SendEvent("move left", &fElapsedTime);
	if (m_pDI->GetKey(DIK_RIGHT) && !m_pDI->GetKey(DIK_LCONTROL) && !m_pDI->GetKey(DIK_LEFT))
		m_pES->SendEvent("move right", &fElapsedTime);

	// shooting
	if (m_pDI->GetKey(DIK_LCONTROL) && !m_pDI->GetKey(DIK_LEFT) && !m_pDI->GetKey(DIK_RIGHT) && !m_pDI->GetKey(DIK_UP) && !m_pDI->GetKey(DIK_DOWN) && m_nXFlip == -1)
		m_pES->SendEvent("shoot left", &fElapsedTime);

	if (m_pDI->GetKey(DIK_LCONTROL) && !m_pDI->GetKey(DIK_LEFT) && !m_pDI->GetKey(DIK_RIGHT) && !m_pDI->GetKey(DIK_UP) && !m_pDI->GetKey(DIK_DOWN) && m_nXFlip == 1)
		m_pES->SendEvent("shoot right", &fElapsedTime);

	if (m_pDI->GetKey(DIK_LCONTROL) && m_pDI->GetKey(DIK_LEFT) && !m_pDI->GetKey(DIK_RIGHT) && !m_pDI->GetKey(DIK_UP) && !m_pDI->GetKey(DIK_DOWN))
		m_pES->SendEvent("shoot left", &fElapsedTime);

	if (m_pDI->GetKey(DIK_LCONTROL) && m_pDI->GetKey(DIK_RIGHT) && !m_pDI->GetKey(DIK_LEFT) && !m_pDI->GetKey(DIK_UP) && !m_pDI->GetKey(DIK_DOWN))
		m_pES->SendEvent("shoot right", &fElapsedTime);

	if (m_pDI->GetKey(DIK_LCONTROL) && m_pDI->GetKey(DIK_UP) && !m_pDI->GetKey(DIK_LEFT) && !m_pDI->GetKey(DIK_RIGHT) && !m_pDI->GetKey(DIK_DOWN))
		m_pES->SendEvent("shoot up", &fElapsedTime);

	if (m_pDI->GetKey(DIK_LCONTROL) && m_pDI->GetKey(DIK_DOWN) && !m_pDI->GetKey(DIK_LEFT) && !m_pDI->GetKey(DIK_RIGHT) && !m_pDI->GetKey(DIK_UP))
		m_pES->SendEvent("shoot down", &fElapsedTime);

	if (m_pDI->GetKey(DIK_LCONTROL) && m_pDI->GetKey(DIK_DOWN) && m_pDI->GetKey(DIK_RIGHT) && !m_pDI->GetKey(DIK_LEFT) && !m_pDI->GetKey(DIK_UP))
		m_pES->SendEvent("shoot down-right", &fElapsedTime);

	if (m_pDI->GetKey(DIK_LCONTROL) && m_pDI->GetKey(DIK_DOWN) && m_pDI->GetKey(DIK_LEFT) && !m_pDI->GetKey(DIK_RIGHT) && !m_pDI->GetKey(DIK_UP))
		m_pES->SendEvent("shoot down-left", &fElapsedTime);

	if (m_pDI->GetKey(DIK_LCONTROL) && m_pDI->GetKey(DIK_UP) && m_pDI->GetKey(DIK_RIGHT) && !m_pDI->GetKey(DIK_LEFT) && !m_pDI->GetKey(DIK_DOWN))
		m_pES->SendEvent("shoot up-right", &fElapsedTime);

	if (m_pDI->GetKey(DIK_LCONTROL) && m_pDI->GetKey(DIK_UP) && m_pDI->GetKey(DIK_LEFT) && !m_pDI->GetKey(DIK_RIGHT) && !m_pDI->GetKey(DIK_DOWN))
		m_pES->SendEvent("shoot up-left", &fElapsedTime);


	bIsMoving = false;
	bIsShooting = false;
}

void CPlayerCharacter::Render(void)
{

	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();

	//shooting animation
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
		pTM->Draw(GetImageID(),GetPosX()-(36*m_nXFlip),GetPosY()-m_nOffSet ,m_nXFlip,1,&rPlayerRect,0,0,0,D3DCOLOR_XRGB(255,255,255));


}

void CPlayerCharacter::HandleEvent(CEvent *pEvent)
{
 	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	CSGD_ObjectManager *pOM = CSGD_ObjectManager::GetInstance();
	CSGD_ObjectFactory<string, CBase> *pOF		= CSGD_ObjectFactory<string, CBase>::GetInstance();

	if (pEvent->GetEventID() == "move left")
	{
		m_nOffSet = 0;
		this;

		SetPosX(GetPosX() + GetVelX() * *((float*)pEvent->GetParam()));
		bIsMoving = true;
		m_nXFlip = -abs(m_nXFlip);
	}
	else if (pEvent->GetEventID() == "move right")
	{
		m_nOffSet = 0;

		SetPosX(GetPosX() - GetVelX() * *((float*)pEvent->GetParam()));
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
			pBullet->SetPosX(GetPosX() - 16);
			pBullet->SetPosY(GetPosY() + 32);

			pBullet->SetHeight(16);
			pBullet->SetWidth(32);
			pBullet->SetRotation(1.5708f);

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
			pBullet->SetPosX(GetPosX() + 32);
			pBullet->SetPosY(GetPosY() + 32);

			pBullet->SetHeight(16 * m_nBulletScale);
			pBullet->SetWidth(32 * m_nBulletScale);
			pBullet->SetRotation(1.5708f);

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
			pBullet->SetPosX(GetPosX() - 8);
			pBullet->SetPosY(GetPosY() - 16);

			pBullet->SetHeight(32 * m_nBulletScale);
			pBullet->SetWidth(16 * m_nBulletScale);
			pBullet->SetRotation(0);

			pOM->AddObject(pBullet);
		}
	}
	else if (pEvent->GetEventID() == "shoot down")
	{
		m_nOffSet = -32;

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
			pBullet->SetPosX(GetPosX() - 8);
			pBullet->SetPosY(GetPosY() + 64);

			pBullet->SetHeight(32 * m_nBulletScale);
			pBullet->SetWidth(16 * m_nBulletScale);
			pBullet->SetRotation(0);

			pOM->AddObject(pBullet);
		}

	}
	else if (pEvent->GetEventID() == "shoot down-right")
	{
		m_nOffSet = -32;

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
			pBullet->SetPosX(GetPosX() + 16);
			pBullet->SetPosY(GetPosY() + 48);

			pBullet->SetHeight(24 * m_nBulletScale);
			pBullet->SetWidth(48 * m_nBulletScale);
			pBullet->SetRotation(-0.7854f);

			pOM->AddObject(pBullet);
		}

	}
	else if (pEvent->GetEventID() == "shoot down-left")
	{
		m_nOffSet = -32;

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
			pBullet->SetPosY(GetPosY() + 64);

			pBullet->SetHeight(24 * m_nBulletScale);
			pBullet->SetWidth(48 * m_nBulletScale);
			pBullet->SetRotation(0.7854f);

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
			pBullet->SetPosX(GetPosX() + 32);
			pBullet->SetPosY(GetPosY() - 16);

			pBullet->SetHeight(24 * m_nBulletScale);
			pBullet->SetWidth(48 * m_nBulletScale);
			pBullet->SetRotation(0.7854f);

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
			pBullet->SetPosY(GetPosY());

			pBullet->SetHeight(24 * m_nBulletScale);
			pBullet->SetWidth(48 * m_nBulletScale);
			pBullet->SetRotation(-0.7854f);

			pOM->AddObject(pBullet);
		}
	}
}