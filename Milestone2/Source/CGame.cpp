/////////////////////////////////////////////////////////////////
//	File	:	"CGame.cpp"
//
//	Author	:	John Albright (DB)
//
//	Purpose	:	Contains all game specific code.
/////////////////////////////////////////////////////////////////
#include "CGame.h"
#include "CPlayerCharacter.h"	//	only include header where it is being used (to avoid circular includes)
#include "CBullet.h"
#include "Enums.h"	//	access to enum of message types.
#include "Gameplay.h"
#include "MainMenu.h"
#include "CCollisionMessage.h"
#include "CDestroyMessage.h"
#include "LevelTransition.h"
#include "Options.h"
#include <ctime>

CGame* CGame::GetInstance(void)
{
	static CGame instance;	//	Lazy instantiation

	return &instance;
}

CGame::CGame(void)
{
	m_pD3D	=	NULL;
	m_pTM	=	NULL;
	m_pDS	=	NULL;
	m_pWM	=	NULL;
	m_pDI	=	NULL;
	m_pOF	=	NULL;

	m_pMS	=	NULL;
	m_pES	=	NULL;

	m_bIsPaused = false;

	m_nImageID = -1;
	m_nSoundID[0] = -1;
	m_nSoundID[1] = -1;
	m_nSoundID[2] = -1;

	m_pPlayerCharacter = NULL;

	m_nFrameCounter = 0;
	m_nFPS = 0;
	m_dwFrameTimer = GetTickCount();

	m_fElapsedTime = 0.0f;
	m_dwPreviousTime = GetTickCount();

	m_nFrameCounter = 0;
	m_nFPS = 0;
	m_dwFrameTimer = GetTickCount();
}

CGame::~CGame(void)
{

}

//	Initialization
void CGame::GameInit(HWND hWnd, HINSTANCE hInstance, int nScreenWidth, int nScreenHeight, bool bIsWindowed)
{
	//	Get pointers to singletons
	m_pD3D	= CSGD_Direct3D::GetInstance();
	m_pTM	= CSGD_TextureManager::GetInstance();
	m_pDS	= CSGD_DirectSound::GetInstance();
	m_pWM	= CSGD_WaveManager::GetInstance();
	m_pDI	= CSGD_DirectInput::GetInstance();
	m_pOF	= CSGD_ObjectFactory<string, CBase>::GetInstance();
	m_OM	= CSGD_ObjectManager::GetInstance();

	//	Change these to singletons:
	m_pMS	= CSGD_MessageSystem::GetInstance();
	m_pES	= CSGD_EventSystem::GetInstance();

	//	Initialize singletons:
	srand(unsigned int(time(0)));	//randomize!

	//	Init D3D
	m_pD3D->InitDirect3D(hWnd, nScreenWidth, nScreenHeight, bIsWindowed, true);
	//	Init TextureManager
	m_pTM->InitTextureManager(m_pD3D->GetDirect3DDevice(), m_pD3D->GetSprite());
	//	Init DirectSound
	m_pDS->InitDirectSound(hWnd);
	//	Init WaveManager
	m_pWM->InitWaveManager(hWnd, m_pDS->GetDSObject());
	//	Init DirectInput
	m_pDI->InitDevices(hWnd, hInstance);

	//	Init MessageSystem
	m_pMS->InitMessageSystem(MessageProc);

	m_Font.SetFontImageID( CSGD_TextureManager::GetInstance()->LoadTexture("resource/JoA_023 - large.bmp", D3DCOLOR_XRGB(0, 0, 0)) );
	m_Font.SetCharWidth( 32 );
	m_Font.SetCharHeight( 32 );
	m_Font.SetNumCols( 10 );
	m_Font.SetStartChar( ' ' );

	m_Font2.SetFontImageID( CSGD_TextureManager::GetInstance()->LoadTexture("resource/JoA_023 - small.bmp", D3DCOLOR_XRGB(0, 0, 0)) );
	m_Font2.SetCharWidth( 16 );
	m_Font2.SetCharHeight( 16 );
	m_Font2.SetNumCols( 10 );
	m_Font2.SetStartChar( ' ' );

	m_nSoundID[0] = m_pWM->LoadWave("resource/sounds/JoA_Bosssnatchprincess.wav");
	m_nSoundID[1] = m_pWM->LoadWave("resource/sounds/JoA_Jimshocked.wav");
	m_nSoundID[2] = m_pWM->LoadWave("resource/sounds/JoA_Cheater.wav");

	m_pD3D->ChangeDisplayParam(640,480,false);

	ChangeState(CMainMenu::GetInstance() );

}

//	Clean up (Shutdown)
void CGame::GameShutdown(void)
{
	if (m_pCurState)
		m_pCurState->Exit();

	m_pES->UnregisterClient(this);

	//// clean up any remaining messages.
	m_pMS->ShutdownMessageSystem();
	//SAFE_DELETE(m_pMS);

	m_pOF->ShutdownObjectFactory();
	m_OM->RemoveAllObjects();

	//	Shutdown in the OPPOSITE order you initialized them in!
	m_pDI->ShutdownDirectInput();
	m_pWM->ShutdownWaveManager();
	m_pDS->ShutdownDirectSound();
	m_pTM->ShutdownTextureManager();
	m_pD3D->ShutdownDirect3D();

}

//	Execution
bool CGame::GameMain(void)
{
	//	Calculate the elapsed time
	DWORD dwCurrentTime = GetTickCount();

	m_fElapsedTime = (float)(dwCurrentTime - m_dwPreviousTime) / 1000.0f;

	m_dwPreviousTime = dwCurrentTime;

	//	Frame Based movement:
	//	pos += vel;

	//	Time based movement:
	//	pos += vel * time;

	//	3 steps to any game:
	//	1.	Input
	//	2.	Update
	//	3.	Draw

	//	1.	Input
	m_pDI->ReadDevices();

	if (m_pDI->GetBufferedKey(DIK_RETURN) && (m_pDI->GetKey(DIK_LMENU) || m_pDI->GetKey(DIK_RMENU)))
	{
		static bool bWindowed = true;
		bWindowed = !bWindowed;
		m_pD3D->ChangeDisplayParam(640, 480, bWindowed);
	}

	//	2.	Update
	if (m_pCurState)
		m_pCurState->Update(m_fElapsedTime);

	if (!GetPaused())
	{
		m_pWM->Update();

		m_OM->UpdateObjects(m_fElapsedTime * GetSpeed());
		
		//	Process Events and Messages
		m_pES->ProcessEvents();
		m_pMS->ProcessMessages();

	}

			//	3.	Draw
	m_pD3D->Clear(0, 0, 128);
	m_pD3D->DeviceBegin();
	m_pD3D->SpriteBegin();

	if (m_pCurState)
		m_pCurState->Render();

	m_OM->RenderObjects();

	m_pD3D->SpriteEnd();
	m_pD3D->DeviceEnd();

	//char buffer[128] = {0};
	//sprintf_s(buffer, _countof(buffer), "FPS: %d", m_nFPS);
	//m_pD3D->DrawTextA(buffer, 10, 10, 255, 255, 255);

	m_pD3D->Present();

	//	increment the frame count
	m_nFrameCounter++;

	//	see if 1 second has passed
	if (GetTickCount() - m_dwFrameTimer > 1000)
	{
		//	remember the FPS
		m_nFPS = m_nFrameCounter;
		//	reset the frame count
		m_nFrameCounter = 0;

		//	reset the timer
		m_dwFrameTimer = GetTickCount();
	}

	//	Success.
	return true;
}

void CGame::MessageProc(CBaseMessage* pMsg)
{
	CSGD_ObjectManager *m_OM = CSGD_ObjectManager::GetInstance();
	CSGD_WaveManager *m_WM = CSGD_WaveManager::GetInstance();

	switch ( pMsg->GetMessageID() )
	{
		case MSG_COLLISION:
		{
			CCollisionMessage * pCollMsg = (CCollisionMessage*)pMsg;
			CBase* pCol1 = pCollMsg->GetCollider1();
			CBase* pCol2 = pCollMsg->GetCollider2();

			if (pCol1->GetType() == ACTOR_ENEMY && pCol2->GetType() == ACTOR_BULLET)
			{
				pCol1->SetHP(pCol1->GetHP() - pCol2->GetPower());
				pCol1->SetHit(true);
				m_OM->RemoveObject(pCol2);
				m_WM->SetVolume(CGame::GetInstance()->GetSound(0),COptions::GetInstance()->GetSFXVolume() - 40);
				m_WM->Play(CGame::GetInstance()->GetSound(0),0);
			}
			else if (pCol1->GetType() == ACTOR_ENEMY && pCol2->GetType() == ACTOR_PLAYER && !CPlayerCharacter::GetInstance()->GetGodMode())
			{
				if (CPlayerCharacter::GetInstance()->GetInvulnerable() == false && pCol1->GetPower() > 0)
				{
					pCol2->SetHP(pCol2->GetHP() - pCol1->GetPower());
					CPlayerCharacter::GetInstance()->SetInvulnerable(true);
					m_WM->SetVolume(CGame::GetInstance()->GetSound(1),COptions::GetInstance()->GetSFXVolume());
					m_WM->Play(CGame::GetInstance()->GetSound(1),0);
				}
			}
			else if (pCol2->GetType() == ACTOR_BULLET && pCol1->GetType() == ACTOR_PLAYER && !CPlayerCharacter::GetInstance()->GetGodMode())
			{
				if (CPlayerCharacter::GetInstance()->GetInvulnerable() == false && pCol2->GetPower() > 0)
				{
					pCol1->SetHP(pCol1->GetHP() - pCol2->GetPower());
					CPlayerCharacter::GetInstance()->SetInvulnerable(true);
					m_WM->SetVolume(CGame::GetInstance()->GetSound(1),COptions::GetInstance()->GetSFXVolume());
					m_WM->Play(CGame::GetInstance()->GetSound(1),0);
				}
			}
			else if (pCol1->GetType() == ACTOR_PLAYER && pCol2->GetType() == POWERUP_1UP && CGame::GetInstance()->GetLives() < 9)
			{
				CGame::GetInstance()->SetLives(CGame::GetInstance()->GetLives() + 1);
				m_WM->SetVolume(CGame::GetInstance()->GetSound(2),COptions::GetInstance()->GetSFXVolume());
				m_WM->Play(CGame::GetInstance()->GetSound(2),0);
				CGame::GetInstance()->SetScore(CGame::GetInstance()->GetScore() + pCol2->GetScoreValue());
			}
			else if (pCol1->GetType() == ACTOR_PLAYER && pCol2->GetType() == POWERUP_HEALTH && CPlayerCharacter::GetInstance()->GetHP() < 100)
			{
				CPlayerCharacter::GetInstance()->SetHP(CPlayerCharacter::GetInstance()->GetHP() + 20);
				if (CPlayerCharacter::GetInstance()->GetHP() > 100)
					CPlayerCharacter::GetInstance()->SetHP(100);
				m_WM->SetVolume(CGame::GetInstance()->GetSound(2),COptions::GetInstance()->GetSFXVolume());
				m_WM->Play(CGame::GetInstance()->GetSound(2),0);
				CGame::GetInstance()->SetScore(CGame::GetInstance()->GetScore() + pCol2->GetScoreValue());
			}
			else if (pCol1->GetType() == ACTOR_PLAYER && pCol2->GetType() == POWERUP_RAPIDFIRE)
			{
				CPlayerCharacter::GetInstance()->SetRapidFireTimer(10.0f);
				m_WM->SetVolume(CGame::GetInstance()->GetSound(2),COptions::GetInstance()->GetSFXVolume());
				m_WM->Play(CGame::GetInstance()->GetSound(2),0);
				CGame::GetInstance()->SetScore(CGame::GetInstance()->GetScore() + pCol2->GetScoreValue());
			}
			else if (pCol1->GetType() == ACTOR_PLAYER && pCol2->GetType() == POWERUP_THREEWAY)
			{
				CPlayerCharacter::GetInstance()->SetThreeWayTimer(10.0f);
				m_WM->SetVolume(CGame::GetInstance()->GetSound(2),COptions::GetInstance()->GetSFXVolume());
				m_WM->Play(CGame::GetInstance()->GetSound(2),0);
				CGame::GetInstance()->SetScore(CGame::GetInstance()->GetScore() + pCol2->GetScoreValue());
			}

		}
		break;
		case MSG_DESTROY:
		{
			CDestroyMessage * pDestMsg = (CDestroyMessage*)pMsg;
			CBase* pItem = pDestMsg->GetItem();
			m_OM->RemoveObject(pItem);
		}
		break;
		case MSG_GOD_CHEAT:
		{
			CPlayerCharacter *player = CPlayerCharacter::GetInstance();
			player->SetGodMode(!player->GetGodMode());
			player->SetHP(100);
			m_WM->SetVolume(CGame::GetInstance()->GetSound(2),COptions::GetInstance()->GetSFXVolume());
			m_WM->Play(CGame::GetInstance()->GetSound(2),0);
		}	
		break;
		case MSG_WARP1:
		{
			if (CGamePlay::GetInstance()->GetLevel() != 1)
			{
				CGamePlay::GetInstance()->SetLevel(1);
				CGame::GetInstance()->ChangeState( CLevelTransition::GetInstance());
			}
		}
		break;
		case MSG_WARP2:
		{
			if (CGamePlay::GetInstance()->GetLevel() != 2)
			{
				CGamePlay::GetInstance()->SetLevel(2);
				CGame::GetInstance()->ChangeState( CLevelTransition::GetInstance());
			}
		}
		break;
		case MSG_WARP3:
		{
			if (CGamePlay::GetInstance()->GetLevel() != 3)
			{
				CGamePlay::GetInstance()->SetLevel(3);
				CGame::GetInstance()->ChangeState( CLevelTransition::GetInstance());
			}
		}
		break;
		case MSG_SPEED_FAST:
		{
			CGame::GetInstance()->SetSpeed(3.0f);
			m_WM->SetVolume(CGame::GetInstance()->GetSound(2),COptions::GetInstance()->GetSFXVolume());
			m_WM->Play(CGame::GetInstance()->GetSound(2),0);
		}
		break;
		case MSG_SPEED_SLOW:
		{
			CGame::GetInstance()->SetSpeed(0.25f);
			m_WM->SetVolume(CGame::GetInstance()->GetSound(2),COptions::GetInstance()->GetSFXVolume());
			m_WM->Play(CGame::GetInstance()->GetSound(2),0);
		}
		break;
	}
}

void CGame::HandleEvent(CEvent* pEvent)
{
}

void CGame::ChangeState(AbstractState *state)
{
	// call Exit on the old state (if there was one)
	if (m_pCurState)
		m_pCurState->Exit();

	//	set current state to the new state
	m_pCurState = state;

	// call Enter on the new state (if there is one now)
	if (m_pCurState)
		m_pCurState->Enter();
}

