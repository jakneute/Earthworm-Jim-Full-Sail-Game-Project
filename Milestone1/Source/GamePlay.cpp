#include "GamePlay.h"
#include "MainMenu.h"
#include "CSGD_TextureManager.h"
#include "CPlayerCharacter.h"
#include "CBullet.h"
#include "CGame.h"
#include "CSGD_Direct3D.h"
#include "CSGD_ObjectManager.h"
#include "CSGD_ObjectFactory.h"
#include "Options.h"
#include "Enemy.h"
#include "CSGD_MessageSystem.h"
#include "MessageTypes.h"

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

	bEnabledCheats[0] = false;
	bEnabledCheats[1] = false;
	bEnabledCheats[2] = false;
	bEnabledCheats[3] = false;

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

	//	Register classes with ObjectFactory
	bIsPaused = false;

	pOF->RegisterClassType<CPlayerCharacter>("CPlayerCharacter");
	pOF->RegisterClassType<CBullet>("CBullet");
	pOF->RegisterClassType<CEnemy>("CEnemy");

	m_nImageID[0] = pTM->LoadTexture("Resource\\EarthwormJim.bmp", D3DCOLOR_XRGB(80,192,64));
	m_nImageID[1] = pTM->LoadTexture("Resource\\SGD_Ship.bmp",D3DCOLOR_XRGB(0,0,0));
	m_nImageID[2] = pTM->LoadTexture("Resource\\OptionsBar.bmp",D3DCOLOR_XRGB(255,0,255));
	m_nBGID		  = pTM->LoadTexture("Resource\\jim_psycro.png", D3DCOLOR_XRGB(0,64,128));

	m_Font.SetFontImageID( CSGD_TextureManager::GetInstance()->LoadTexture("resource/023 - large.bmp", D3DCOLOR_XRGB(0, 0, 0)) );
	m_Font.SetCharWidth( 32 );
	m_Font.SetCharHeight( 32 );
	m_Font.SetNumCols( 10 );
	m_Font.SetStartChar( ' ' );

	pOM->AddObject(CPlayerCharacter::GetInstance());

	for (int i = 0; i < 8; ++i)
	{
		CBase *tempEnemy;
		tempEnemy = pOF->CreateObject("CEnemy");
		tempEnemy ->SetImageID(m_nImageID[1]);
		tempEnemy ->SetPosX(i * 64);
		tempEnemy ->SetPosY(0);
		tempEnemy ->SetHeight(64);
		tempEnemy ->SetWidth(64);
		pOM->AddObject(tempEnemy );
		tempEnemy ->Release();
		enemies.push_back(tempEnemy);
	}

	pPC->SetBulletScale(1.0f);
	pPC->SetRateOfFire(7);


	CSGD_WaveManager* pWM = CSGD_WaveManager::GetInstance();

	//	Load the music
	m_nBGMusicID = pWM->LoadWave("Resource/SGD_MarioSong.wav");
	pWM->SetVolume(m_nBGMusicID,op->GetMusicVolume());

	pWM->Play(m_nBGMusicID,1);
}

void CGamePlay::Update(float fElapsedTime)
{
	CSGD_MessageSystem *pMS = CSGD_MessageSystem::GetInstance();
	CSGD_ObjectManager *pOM = CSGD_ObjectManager::GetInstance();
	CPlayerCharacter *pPC = CPlayerCharacter::GetInstance();
	CGame *theGame = CGame::GetInstance();
	CSGD_DirectInput *pDI = CSGD_DirectInput::GetInstance();

	if (pDI->GetBufferedKey(DIK_P))
		bIsPaused = !bIsPaused;

	if (pDI->GetBufferedKey(DIK_F2))
		pMS->SendMsg(new CBaseMessage(MSG_FIRE_RATE_CHEAT));

	if (pDI->GetBufferedKey(DIK_F3))
		pMS->SendMsg(new CBaseMessage(MSG_BULLET_SIZE_CHEAT));

	if (pDI->GetBufferedKey(DIK_F4))
		pMS->SendMsg(new CBaseMessage(MSG_SPEED_CHEAT));

	if (pDI->GetBufferedKey(DIK_F5))
		pMS->SendMsg(new CBaseMessage(MSG_GOD_CHEAT));

	if (pDI->GetBufferedKey(DIK_ESCAPE))
		CGame::GetInstance()->ChangeState( CMainMenu::GetInstance() );	

	vector<CBase*>::iterator iter = enemies.begin();
	int i = 0;

	while(iter != enemies.end())
	{
		//	see if we've found the pointer we are looking for
		if ((*iter)->GetHP() <= 0)
		{
			enemies.erase(iter);
			break;
		}
		else
			iter++;
	}

	if (pPC->GetHP() <= 0)
		CGame::GetInstance()->ChangeState(CMainMenu::GetInstance() );

	//if (GetTickCount() % 1000 == 0 && !pPC->GetGodMode())
	//	pPC->SetHP(pPC->GetHP() - 10);

}

void CGamePlay::Render()
{
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	CPlayerCharacter *pPC = CPlayerCharacter::GetInstance();

	static RECT rBarRectGreen = {0,0,200,64};
	static RECT rBarRectRed = { 0, 0, 200, 64};
	static RECT rBridgeRect = {0,500,1000,680};
	static RECT rSpaceRect = {0,732,500,1009};
	rBarRectGreen.right = 2 * pPC->GetHP();

	pTM->Draw(m_nBGID,0,0,1.3f,1.75f,&rSpaceRect,0,0,0,D3DCOLOR_XRGB(255,255,255));
	pTM->Draw(m_nBGID,0,300,1,1,&rBridgeRect,0,0,0,D3DCOLOR_XRGB(255,255,255));
	pTM->Draw(m_nImageID[2],0,450,2.5f,0.25f,&rBarRectRed,0,0,0,D3DCOLOR_XRGB(255,0,0));
	pTM->Draw(m_nImageID[2],0,450,2.5f,0.25f,&rBarRectGreen,0,0,0,D3DCOLOR_XRGB(0,255,0));

	if (bIsPaused)
		m_Font.DrawString("Paused",200,200);

}

void CGamePlay::Exit()
{
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	CSGD_ObjectManager *pOM = CSGD_ObjectManager::GetInstance();
	CSGD_ObjectFactory<string, CBase> *pOF = CSGD_ObjectFactory<string, CBase>::GetInstance();
	CSGD_WaveManager* pWM = CSGD_WaveManager::GetInstance();
	CPlayerCharacter *pPC = CPlayerCharacter::GetInstance();

	bIsPaused = false;

	pOM->RemoveAllObjects();

	pPC->SetHP(100);
	pPC->SetPosX( 100.0f );
	pPC->SetPosY( 350.0f );
	pPC->SetRateOfFire(7);
	pPC->SetBulletScale(1.0f);
	pPC->SetGodMode(false);
	pPC->SetVelX(0.00000004f);
	pPC->SetVelY(0.00000004f);

	pTM->ReleaseTexture(m_nImageID[0]);
	pTM->ReleaseTexture(m_nImageID[1]);
	pTM->ReleaseTexture(m_nImageID[2]);
	pTM->ReleaseTexture(m_nBGID);

	enemies.clear();

	pWM->UnloadWave(m_nBGMusicID);
}