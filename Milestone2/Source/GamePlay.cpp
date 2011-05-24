/////////////////////////////////////////////////////////////////////////
//	File	:	"GamePlay.cpp"
//
//	Author	:	John Albright (JA)
//
//	Purpose	:	Defines any code specific towards you in gameplay mode
/////////////////////////////////////////////////////////////////////////

#include "BulletFire.h"
#include "BulletHook.h"
#include "BulletPaper.h"
#include "CBullet.h"
#include "CDestroyMessage.h"
#include "CGame.h"
#include "CPlayerCharacter.h"
#include "CSGD_Direct3D.h"
#include "CSGD_DirectSound.h"
#include "CSGD_EventSystem.h"
#include "CSGD_MessageSystem.h"
#include "CSGD_ObjectFactory.h"
#include "CSGD_ObjectManager.h"
#include "CSGD_TextureManager.h"
#include "Enums.h"
#include "EnemyBooger.h"
#include "EnemyBusiness.h"
#include "EnemyDemon.h"
#include "EnemyMeat.h"
#include "EnemyPsycrow.h"
#include "GamePlay.h"
#include "LevelTransition.h"
#include "MainMenu.h"
#include "Options.h"
#include "Powerup1up.h"
#include "PowerupHealth.h"
#include "PowerupRapidFire.h"
#include "PowerupThreeWay.h"
#include "TallyMenu.h"

CGamePlay* CGamePlay::GetInstance(void)
{
	static CGamePlay instance; // lazy instantiation
	return &instance;
}


CGamePlay::CGamePlay()
{
	m_nImageID[0] = -1;
	m_nImageID[1] = -1;
	m_nImageID[2] = -1;
	m_nImageID[3] = -1;
	m_nImageID[4] = -1;

	CBase *m_pPlayerCharacter = NULL;
	CBase *m_pBullet = NULL;
	m_nBGMusicID = -1;
}

CGamePlay::~CGamePlay()
{
}

void CGamePlay::Enter()
{
	COptions *op = COptions::GetInstance();
	CSGD_ObjectFactory<string, CBase> *pOF		= CSGD_ObjectFactory<string, CBase>::GetInstance();
	CSGD_ObjectManager *pOM						= CSGD_ObjectManager::GetInstance();
	CSGD_TextureManager *pTM					= CSGD_TextureManager::GetInstance();
	CPlayerCharacter *pPC						= CPlayerCharacter::GetInstance();
	CSGD_EventSystem *pEM						= CSGD_EventSystem::GetInstance();
	CSGD_DirectSound *pDS						= CSGD_DirectSound::GetInstance();
	CSGD_WaveManager* pWM						= CSGD_WaveManager::GetInstance();


	m_nBridgeOffset = 0;
	m_nSpaceOffset = 0;
	m_nEnemyCounter = 0;
	m_nTotalSpawns = 0;

	stateTimer = 6.0f;
	m_fSpawnPowerupTimer = 15.0f;

	//	Register classes with ObjectFactory
	pEM->RegisterClient("scroll right", this);
	pEM->RegisterClient("scroll left", this);
	pEM->RegisterClient("spawn enemies",this);
	pEM->RegisterClient("spawn powerups",this);

	pOF->RegisterClassType<CPlayerCharacter>("CPlayerCharacter");
	pOF->RegisterClassType<CBullet>("CBullet");
	pOF->RegisterClassType<CEnemyDemon>("CEnemyDemon");
	pOF->RegisterClassType<CEnemyBusiness>("CEnemyBusiness");
	pOF->RegisterClassType<CBulletPaper>("CBulletPaper");
	pOF->RegisterClassType<CEnemyMeat>("CEnemyMeat");
	pOF->RegisterClassType<CBulletFire>("CBulletFire");
	pOF->RegisterClassType<CEnemyBooger>("CEnemyBooger");
	pOF->RegisterClassType<CPowerup1up>("CPowerup1up");
	pOF->RegisterClassType<CPowerupHealth>("CPowerupHealth");
	pOF->RegisterClassType<CPowerupRapidFire>("CPowerupRapidFire");
	pOF->RegisterClassType<CPowerupThreeWay>("CPowerupThreeWay");
	pOF->RegisterClassType<CEnemyPsycrow>("CEnemyPsycrow");
	pOF->RegisterClassType<CBulletHook>("CBulletHook");

	m_nImageID[0] = pTM->LoadTexture("Resource\\JoA_EarthwormJim.bmp", D3DCOLOR_XRGB(80,192,64));
	m_nImageID[1] = pTM->LoadTexture("Resource\\JoA_jim_level2enemies.png",D3DCOLOR_XRGB(0,64,128));
	m_nImageID[2] = pTM->LoadTexture("Resource\\JoA_ewj2-flaminyawn.png",D3DCOLOR_XRGB(0,64,128));
	m_nImageID[3] = pTM->LoadTexture("Resource\\JoA_earthwormjim-items.png",D3DCOLOR_XRGB(0,128,255));
	m_nImageID[4] = pTM->LoadTexture("Resource\\JoA_ewj_forpete.png",D3DCOLOR_XRGB(0,64,128));
	m_nBGID		  = pTM->LoadTexture("Resource\\JoA_jim_psycro.png", D3DCOLOR_XRGB(0,64,128));

	pOM->AddObject(CPlayerCharacter::GetInstance());

	m_fFinishedTimer = 5.0f;

	m_Font.SetFontImageID( CSGD_TextureManager::GetInstance()->LoadTexture("resource/JoA_023 - small.bmp", D3DCOLOR_XRGB(0, 0, 0)) );
	m_Font.SetCharWidth( 16 );
	m_Font.SetCharHeight( 16 );
	m_Font.SetNumCols( 10 );
	m_Font.SetStartChar( ' ' );

	m_Font2.SetFontImageID( CSGD_TextureManager::GetInstance()->LoadTexture("resource/JoA_023 - large.bmp", D3DCOLOR_XRGB(0, 0, 0)) );
	m_Font2.SetCharWidth( 32 );
	m_Font2.SetCharHeight( 32 );
	m_Font2.SetNumCols( 10 );
	m_Font2.SetStartChar( ' ' );

	pPC->SetBulletScale(1.0f);
	pPC->SetRateOfFire(7);
	pPC->SetHP(100);
	pPC->SetPosX( 100.0f );
	pPC->SetImageID(m_nImageID[0]);
	pPC->SetThreeWay(false);
	pPC->SetGodMode(false);
	pPC->SetVelX(0.00000004f);
	pPC->SetVelY(0.00000004f);
	pPC->SetInvulnerable(false);
	pPC->SetRapidFireTimer(0.0f);
	pPC->SetThreeWayTimer(0.0f);
	if (GetLevel() == 1)
	{
		pPC->SetPosY( 202.0f );
		SetRemainingEnemies(10);
		m_nBGMusicID = pWM->LoadWave("Resource/sounds/JoA_Song3.wav");
		pWM->SetVolume(m_nBGMusicID,op->GetMusicVolume() - 25);
	}
	else if (GetLevel() == 2)
	{
		pPC->SetPosY(375.0f);
		SetRemainingEnemies(10);
		m_nBGMusicID = pWM->LoadWave("Resource/sounds/JoA_Song4.wav");
		pWM->SetVolume(m_nBGMusicID,op->GetMusicVolume() - 25);
	}
	else if (GetLevel() == 3)
	{
		pPC->SetPosY(250.0f);
		SetRemainingEnemies(1);
		m_nBridgeOffsetY = 300;
		m_nBridgeVelocityY = 25 * op->GetDifficulty();
		pEM->SendEvent("spawn enemies", NULL);
		m_nBGMusicID = pWM->LoadWave("Resource/sounds/JoA_Song2.wav");
		pWM->SetVolume(m_nBGMusicID,op->GetMusicVolume() - 25);
	}

	pWM->Play(m_nBGMusicID,1);
}

void CGamePlay::Update(float fElapsedTime)
{
	CSGD_MessageSystem *pMS = CSGD_MessageSystem::GetInstance();
	CSGD_ObjectManager *pOM = CSGD_ObjectManager::GetInstance();
	CPlayerCharacter *pPC = CPlayerCharacter::GetInstance();
	CGame *theGame = CGame::GetInstance();
	CSGD_DirectInput *pDI = CSGD_DirectInput::GetInstance();
	CSGD_EventSystem *pES = CSGD_EventSystem::GetInstance();

	stateTimer -= fElapsedTime;

	for (int i = 0; i < CHEAT_MAX; ++i)
		CheckForCheats(i);

	if (pDI->CheckBufferedKeys() != 0)
		m_cPreviousKeys.push(pDI->CheckBufferedKeys());

	if (stateTimer <= 0)
	{
		if (m_nEnemyCounter < 3 && m_nRemainingEnemies > 0 && GetLevel() <= 2)
			pES->SendEvent("spawn enemies", &fElapsedTime);
		stateTimer = 6;
	}

	if (pDI->GetBufferedKey(DIK_P))
		theGame->SetPaused(!theGame->GetPaused());

	if (GetLevel() == 3)
	{
		if (CGame::GetInstance()->GetPaused() == false)
			m_nBridgeOffsetY += m_nBridgeVelocityY * fElapsedTime * CGame::GetInstance()->GetSpeed();
		if (m_nBridgeOffsetY > 450)
		{
			m_nBridgeOffsetY = 449;
			m_nBridgeVelocityY *= -1;
		}
		if (m_nBridgeOffsetY < 64)
		{
			m_nBridgeOffsetY = 65;
			m_nBridgeVelocityY *= -1;
		}
		CPlayerCharacter::GetInstance()->SetPosY(m_nBridgeOffsetY - 64);
		m_fSpawnPowerupTimer -= fElapsedTime;
		if (m_fSpawnPowerupTimer <= 0)
		{
			CSGD_EventSystem::GetInstance()->SendEvent("spawn powerups",NULL);
			m_fSpawnPowerupTimer = 15.0f;
		}
	}

	vector<CBase*>::iterator iter = enemies.begin();
	int i = 0;

	while(iter != enemies.end())
	{
		//	see if we've found the pointer we are looking for
		int i = 0;
		if ((*iter)->GetHP() <= 0 && GetLevel() <= 3)
		{
			int value = enemies[i]->GetScoreValue();
			enemies.erase(iter);
			SetEnemyCounter(GetEnemyCounter() - 1);
			SetRemainingEnemies(GetRemainingEnemies() - 1);
			theGame->SetScore(theGame->GetScore() + value);
			++i;
			break;
		}
		else
			iter++;
	}

	if (pPC->GetHP() <= 0)
	{
		CGame::GetInstance()->SetLives(CGame::GetInstance()->GetLives() - 1);
		CGame::GetInstance()->ChangeState( CLevelTransition::GetInstance());
	}

	else if (m_nRemainingEnemies <= 0 && m_nEnemyCounter <= 0 && enemies.empty())
	{
		m_fFinishedTimer -= fElapsedTime;

		if (m_fFinishedTimer <= 0)
		{
			SetLevel(GetLevel() + 1);
			CTallyMenu::GetInstance()->SetHP(pPC->GetHP());
			CGame::GetInstance()->ChangeState( CTallyMenu::GetInstance());
		}
	}

	if (pDI->GetBufferedKey(DIK_ESCAPE))
		CGame::GetInstance()->ChangeState( CMainMenu::GetInstance() );	

}

void CGamePlay::Render()
{
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	CPlayerCharacter *pPC = CPlayerCharacter::GetInstance();
	CSGD_Direct3D *p3D = CSGD_Direct3D::GetInstance();

	static RECT rHealthRect = {50,115,89,131 };
	static RECT rMugshotRect = {14,107,35,135 };

	RECT rForegroundRect;
	RECT rBackgroundRect;

	if (GetLevel() == 1)
	{

		rForegroundRect.left = 0;
		rForegroundRect.top = 390;
		rForegroundRect.right = 745;
		rForegroundRect.bottom = 447;
		rBackgroundRect.left = 0;
		rBackgroundRect.top = 450;
		rBackgroundRect.right = 380;
		rBackgroundRect.bottom = 671;
		pTM->Draw(m_nBGID,0 + m_nSpaceOffset,0,2.5f,3.0f,&rBackgroundRect,0,0,0,D3DCOLOR_XRGB(255,255,255));
		pTM->Draw(m_nBGID,0 + m_nBridgeOffset,256,1,1,&rForegroundRect,0,0,0,D3DCOLOR_XRGB(255,255,255));
	}
	else if (GetLevel() == 2)
	{
		rForegroundRect.left = 41;
		rForegroundRect.top = 351;
		rForegroundRect.right = 392;
		rForegroundRect.bottom = 590;
		rBackgroundRect.left = 3;
		rBackgroundRect.top = 594;
		rBackgroundRect.right = 387;
		rBackgroundRect.bottom = 815;
		pTM->Draw(m_nImageID[2],0 + m_nSpaceOffset,0,2.5f,2.5f,&rBackgroundRect,0,0,0,D3DCOLOR_XRGB(255,255,255));
		pTM->Draw(m_nImageID[2],0 + m_nBridgeOffset,100,2.25f,1.75f,&rForegroundRect,0,0,0,D3DCOLOR_XRGB(255,255,255));
	}
	else if (GetLevel() == 3)
	{
		rForegroundRect.left = 536;
		rForegroundRect.top = 380;
		rForegroundRect.right = 894;
		rForegroundRect.bottom = 445;
		rBackgroundRect.left = 1418;
		rBackgroundRect.top = 4;
		rBackgroundRect.right = 1929;
		rBackgroundRect.bottom = 259;
		pTM->Draw(m_nImageID[4],0 + m_nSpaceOffset,0,2.5f,2.5f,&rBackgroundRect,0,0,0,D3DCOLOR_XRGB(255,255,255));
		pTM->Draw(m_nImageID[4],0 + m_nBridgeOffset,m_nBridgeOffsetY,2.25f,1.75f,&rForegroundRect,0,0,0,D3DCOLOR_XRGB(255,255,255));
	}

	pTM->Draw(m_nImageID[3],10,0,2.5f * CPlayerCharacter::GetInstance()->GetHP()/100,2.0f,&rHealthRect,0,0,0,D3DCOLOR_XRGB(255,255,255));
	pTM->Draw(m_nImageID[3],560,0,1.5f,1.5f,&rMugshotRect,0,0,0,D3DCOLOR_XRGB(255,255,255));

	m_Font.DrawString("x",592,0);
	char buffer[128];
	sprintf_s(buffer, _countof(buffer), "%d", CGame::GetInstance()->GetLives());
	m_Font2.DrawString(buffer,608,0);
	sprintf_s(buffer, _countof(buffer),"%d",CGame::GetInstance()->GetScore());
	m_Font2.DrawString(buffer,275,0);
	m_Font.DrawString("Enemies:",475,464);
	sprintf_s(buffer, _countof(buffer),"%d",GetRemainingEnemies());
	m_Font.DrawString(buffer,600,464);
	sprintf_s(buffer,_countof(buffer),"%d",pPC->GetHP());
	m_Font.DrawString(buffer,0,30);

	if (CGame::GetInstance()->GetPaused() == true)
		m_Font.DrawString("Paused",300,200);

}

void CGamePlay::Exit()
{
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	CSGD_ObjectManager *pOM = CSGD_ObjectManager::GetInstance();
	CSGD_ObjectFactory<string, CBase> *pOF = CSGD_ObjectFactory<string, CBase>::GetInstance();
	CSGD_WaveManager* pWM = CSGD_WaveManager::GetInstance();
	CPlayerCharacter *pPC = CPlayerCharacter::GetInstance();
	CSGD_EventSystem *m_pES = CSGD_EventSystem::GetInstance();

	m_pES->UnregisterClient(this);

	CGame::GetInstance()->SetPaused(false);

	pOM->RemoveAllObjects();
	pOF->ShutdownObjectFactory();

	enemies.clear();

	pTM->ReleaseTexture(m_nImageID[0]);
	pTM->ReleaseTexture(m_nImageID[1]);
	pTM->ReleaseTexture(m_nImageID[2]);
	pTM->ReleaseTexture(m_nImageID[3]);
	pTM->ReleaseTexture(m_nImageID[4]);
	pTM->ReleaseTexture(m_nBGID);

	pWM->UnloadWave(m_nBGMusicID);
}

void CGamePlay::HandleEvent(CEvent *pEvent)
{
	CSGD_ObjectFactory<string, CBase> *pOF		= CSGD_ObjectFactory<string, CBase>::GetInstance();
	CSGD_ObjectManager *pOM						= CSGD_ObjectManager::GetInstance();
 	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();

	if (pEvent->GetEventID() == "spawn powerups")
	{
		CBase *tempPowerup;
		int nRandomPowerup = rand() % 100 + 1;

		if (nRandomPowerup <= 30)
			tempPowerup = pOF->CreateObject("CPowerupHealth");
		else if (nRandomPowerup > 30 && nRandomPowerup <= 60)
			tempPowerup = pOF->CreateObject("CPowerupRapidFire");
		else if (nRandomPowerup > 60 && nRandomPowerup <= 90)
			tempPowerup = pOF->CreateObject("CPowerupThreeWay");
		else if (nRandomPowerup > 90)
			tempPowerup = pOF->CreateObject("CPowerup1up");

		tempPowerup ->SetImageID(m_nImageID[3]);
		tempPowerup->SetPosX(CPlayerCharacter::GetInstance()->GetPosX() + 128);
		if (tempPowerup->GetPosX() > 640)
			tempPowerup->SetPosX(640);
		tempPowerup->SetPosY(CPlayerCharacter::GetInstance()->GetPosY() + 32);
		pOM->AddObject(tempPowerup );
		tempPowerup ->Release();
	}

	else if (pEvent->GetEventID() == "scroll right")
	{
		if (GetBridgeOffset() > -100)
		{
			SetBridgeOffset(GetBridgeOffset() - (5 * CGame::GetInstance()->GetSpeed()));
			SetSpaceOffset(GetSpaceOffset() - (15 * CGame::GetInstance()->GetSpeed()));
			for (int i = 0; i < enemies.size(); ++i)
			{
				enemies[i]->SetPosX(enemies[i]->GetPosX() - (5 * CGame::GetInstance()->GetSpeed()));
			}
		}
	}

	else if (pEvent->GetEventID() == "scroll left")
	{
		if (GetBridgeOffset() < 0)
		{
			SetBridgeOffset(GetBridgeOffset() + (5 * CGame::GetInstance()->GetSpeed()));
			SetSpaceOffset(GetSpaceOffset() + (15 * CGame::GetInstance()->GetSpeed()));
			for (int i = 0; i < enemies.size(); ++i)
			{
				enemies[i]->SetPosX(enemies[i]->GetPosX() + (5 * CGame::GetInstance()->GetSpeed()));
			}
		}
	}

	else if (pEvent->GetEventID() == "spawn enemies")
	{
		CBase *tempEnemy;
		if (GetLevel() == 1 && m_nRemainingEnemies > 0 && m_nTotalSpawns < 10)
		{
			m_nEnemyCounter += 1;
			m_nTotalSpawns += 1;

			if (m_nTotalSpawns % 2 == 1)
			{
				tempEnemy = pOF->CreateObject("CEnemyDemon");
				tempEnemy ->SetImageID(m_nImageID[1]);
				pOM->AddObject(tempEnemy );
				tempEnemy ->Release();
				enemies.push_back(tempEnemy);
			}
			else
			{
				tempEnemy = pOF->CreateObject("CEnemyBusiness");
				tempEnemy ->SetImageID(m_nImageID[1]);
				pOM->AddObject(tempEnemy );
				tempEnemy ->Release();
				enemies.push_back(tempEnemy);
			}
	   }
	if (GetLevel() == 2 && m_nRemainingEnemies > 0 && m_nTotalSpawns < 10)
	{
		m_nEnemyCounter += 1;
		m_nTotalSpawns += 1;

		if (m_nTotalSpawns % 2 == 1)
		{
			tempEnemy = pOF->CreateObject("CEnemyMeat");
			tempEnemy ->SetImageID(m_nImageID[2]);
			pOM->AddObject(tempEnemy );
			tempEnemy ->Release();
			enemies.push_back(tempEnemy);
		}
		else
		{
			tempEnemy = pOF->CreateObject("CEnemyBooger");
			tempEnemy ->SetImageID(m_nImageID[2]);
			pOM->AddObject(tempEnemy );
			tempEnemy ->Release();
			enemies.push_back(tempEnemy);
		}
	}
	if (GetLevel() == 3)
	{
		tempEnemy = pOF->CreateObject("CEnemyPsycrow");
		tempEnemy->SetImageID(m_nBGID);
		pOM->AddObject(tempEnemy);
		tempEnemy->Release();
		enemies.push_back(tempEnemy);
	}
	}
}

void CGamePlay::CheckForCheats(int check)
{
	CSGD_ObjectFactory<string, CBase> *pOF		= CSGD_ObjectFactory<string, CBase>::GetInstance();
	CSGD_ObjectManager *pOM						= CSGD_ObjectManager::GetInstance();
 	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();

	if (m_cPreviousKeys.size() > 8)
			m_cPreviousKeys.pop();

	else if (check == CHEAT_WARP_LEVEL1)
	{
		if (m_cPreviousKeys.size() == 8)
		{
			char cTempKey = m_cPreviousKeys.front();
			if (tolower(cTempKey) == 'r')
			{
				m_cPreviousKeys.pop();
				cTempKey = m_cPreviousKeys.front();
				if (tolower(cTempKey) == 'e')
				{
					m_cPreviousKeys.pop();
					cTempKey = m_cPreviousKeys.front();
					if (tolower(cTempKey) == 's')
					{
						m_cPreviousKeys.pop();
						cTempKey = m_cPreviousKeys.front();
						if (tolower(cTempKey) == 't')
						{
							m_cPreviousKeys.pop();
							cTempKey = m_cPreviousKeys.front();
							if (tolower(cTempKey) == 'a')
							{
								m_cPreviousKeys.pop();
								cTempKey = m_cPreviousKeys.front();
								if (tolower(cTempKey) == 'r')
								{
									m_cPreviousKeys.pop();
									cTempKey = m_cPreviousKeys.front();
									if (tolower(cTempKey) == 't')
									{
										m_cPreviousKeys.pop();
										cTempKey = m_cPreviousKeys.front();
										if (cTempKey == '!')
										{
											m_cPreviousKeys.pop();
											CSGD_MessageSystem::GetInstance()->SendMsg(new CBaseMessage(MSG_WARP1));
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	if (check == CHEAT_WARP_LEVEL2)
	{
		if (m_cPreviousKeys.size() == 8)
		{
			char cTempKey = m_cPreviousKeys.front();
			if (tolower(cTempKey) == 'c')
			{
				m_cPreviousKeys.pop();
				cTempKey = m_cPreviousKeys.front();
				if (tolower(cTempKey) == 'h')
				{
					m_cPreviousKeys.pop();
					cTempKey = m_cPreviousKeys.front();
					if (tolower(cTempKey) == 'i')
					{
						m_cPreviousKeys.pop();
						cTempKey = m_cPreviousKeys.front();
						if (tolower(cTempKey) == 'c')
						{
							m_cPreviousKeys.pop();
							cTempKey = m_cPreviousKeys.front();
							if (tolower(cTempKey) == 'n')
							{
								m_cPreviousKeys.pop();
								cTempKey = m_cPreviousKeys.front();
								if (tolower(cTempKey) == 's')
								{
									m_cPreviousKeys.pop();
									cTempKey = m_cPreviousKeys.front();
									if (tolower(cTempKey) == 't')
									{
										m_cPreviousKeys.pop();
										cTempKey = m_cPreviousKeys.front();
										if (tolower(cTempKey) == 'u')
										{
											m_cPreviousKeys.pop();
											CSGD_MessageSystem::GetInstance()->SendMsg(new CBaseMessage(MSG_WARP2));
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (check == CHEAT_WARP_LEVEL3)
	{
		if (m_cPreviousKeys.size() == 8)
		{
			char cTempKey = m_cPreviousKeys.front();
			if (tolower(cTempKey) == 'd')
			{
				m_cPreviousKeys.pop();
				cTempKey = m_cPreviousKeys.front();
				if (tolower(cTempKey) == 'u')
				{
					m_cPreviousKeys.pop();
					cTempKey = m_cPreviousKeys.front();
					if (tolower(cTempKey) == 'm')
					{
						m_cPreviousKeys.pop();
						cTempKey = m_cPreviousKeys.front();
						if (tolower(cTempKey) == 'b')
						{
							m_cPreviousKeys.pop();
							cTempKey = m_cPreviousKeys.front();
							if (tolower(cTempKey) == 'b')
							{
								m_cPreviousKeys.pop();
								cTempKey = m_cPreviousKeys.front();
								if (tolower(cTempKey) == 'i')
								{
									m_cPreviousKeys.pop();
									cTempKey = m_cPreviousKeys.front();
									if (tolower(cTempKey) == 'r')
									{
										m_cPreviousKeys.pop();
										cTempKey = m_cPreviousKeys.front();
										if (tolower(cTempKey) == 'd')
										{
											m_cPreviousKeys.pop();
											CSGD_MessageSystem::GetInstance()->SendMsg(new CBaseMessage(MSG_WARP3));
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (check == CHEAT_GOD)
	{
		if (m_cPreviousKeys.size() == 8)
		{
			char cTempKey = m_cPreviousKeys.front();
			if (tolower(cTempKey) == 'i')
			{
				m_cPreviousKeys.pop();
				cTempKey = m_cPreviousKeys.front();
				if (tolower(cTempKey) == 'a')
				{
					m_cPreviousKeys.pop();
					cTempKey = m_cPreviousKeys.front();
					if (tolower(cTempKey) == 'm')
					{
						m_cPreviousKeys.pop();
						cTempKey = m_cPreviousKeys.front();
						if (tolower(cTempKey) == 'a')
						{
							m_cPreviousKeys.pop();
							cTempKey = m_cPreviousKeys.front();
							if (tolower(cTempKey) == 'd')
							{
								m_cPreviousKeys.pop();
								cTempKey = m_cPreviousKeys.front();
								if (tolower(cTempKey) == 'o')
								{
									m_cPreviousKeys.pop();
									cTempKey = m_cPreviousKeys.front();
									if (tolower(cTempKey) == 'r')
									{
										m_cPreviousKeys.pop();
										cTempKey = m_cPreviousKeys.front();
										if (tolower(cTempKey) == 'k')
										{
											m_cPreviousKeys.pop();
											CSGD_MessageSystem::GetInstance()->SendMsg(new CBaseMessage(MSG_GOD_CHEAT));
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (check == CHEAT_SPEED_SLOW)
	{
		if (m_cPreviousKeys.size() == 8)
		{
			char cTempKey = m_cPreviousKeys.front();
			if (tolower(cTempKey) == 'm')
			{
				m_cPreviousKeys.pop();
				cTempKey = m_cPreviousKeys.front();
				if (tolower(cTempKey) == 'o')
				{
					m_cPreviousKeys.pop();
					cTempKey = m_cPreviousKeys.front();
					if (tolower(cTempKey) == 'l')
					{
						m_cPreviousKeys.pop();
						cTempKey = m_cPreviousKeys.front();
						if (tolower(cTempKey) == 'a')
						{
							m_cPreviousKeys.pop();
							cTempKey = m_cPreviousKeys.front();
							if (tolower(cTempKey) == 's')
							{
								m_cPreviousKeys.pop();
								cTempKey = m_cPreviousKeys.front();
								if (tolower(cTempKey) == 's')
								{
									m_cPreviousKeys.pop();
									cTempKey = m_cPreviousKeys.front();
									if (tolower(cTempKey) == 'e')
									{
										m_cPreviousKeys.pop();
										cTempKey = m_cPreviousKeys.front();
										if (tolower(cTempKey) == 's')
										{
											m_cPreviousKeys.pop();
											CSGD_MessageSystem::GetInstance()->SendMsg(new CBaseMessage(MSG_SPEED_SLOW));
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (check == CHEAT_SPEED_FAST)
	{
		if (m_cPreviousKeys.size() == 8)
		{
			char cTempKey = m_cPreviousKeys.front();
			if (tolower(cTempKey) == 't')
			{
				m_cPreviousKeys.pop();
				cTempKey = m_cPreviousKeys.front();
				if (tolower(cTempKey) == 'u')
				{
					m_cPreviousKeys.pop();
					cTempKey = m_cPreviousKeys.front();
					if (tolower(cTempKey) == 'r')
					{
						m_cPreviousKeys.pop();
						cTempKey = m_cPreviousKeys.front();
						if (tolower(cTempKey) == 'b')
						{
							m_cPreviousKeys.pop();
							cTempKey = m_cPreviousKeys.front();
							if (tolower(cTempKey) == 'o')
							{
								m_cPreviousKeys.pop();
								cTempKey = m_cPreviousKeys.front();
								if (tolower(cTempKey) == 'm')
								{
									m_cPreviousKeys.pop();
									cTempKey = m_cPreviousKeys.front();
									if (tolower(cTempKey) == 'a')
									{
										m_cPreviousKeys.pop();
										cTempKey = m_cPreviousKeys.front();
										if (tolower(cTempKey) == 'n')
										{
											m_cPreviousKeys.pop();
											CSGD_MessageSystem::GetInstance()->SendMsg(new CBaseMessage(MSG_SPEED_FAST));
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}