/////////////////////////////////////////////////////////////////////////
//	File	:	"Options.cpp"
//
//	Author	:	John Albright (JA)
//
//	Purpose	:	Defines how the options screen is presented
/////////////////////////////////////////////////////////////////////////

#include "MainMenu.h"
#include "Options.h"
#include "CSGD_WaveManager.h"
#include "CSGD_TextureManager.h"
#include "CSGD_DirectInput.h"
#include "CGame.h"
#include "Enums.h"

COptions* COptions::GetInstance(void)
{
	static COptions instance; // lazy instantiation
	return &instance;
}


COptions::COptions()
{
	m_nImageID[0] = -1;
	m_nImageID[1] = -1;

	m_nSoundID[0] = -1;
	m_nSoundID[1] = -1;

	m_nMusicVolume = 100;
	m_nSFXVolume = 100;

	m_nDifficulty = DIFFICULTY_MEDIUM;

	m_Font.SetFontImageID( CSGD_TextureManager::GetInstance()->LoadTexture("resource/JoA_023 - large.bmp", D3DCOLOR_XRGB(0, 0, 0)) );
	m_Font.SetCharWidth( 32 );
	m_Font.SetCharHeight( 32 );
	m_Font.SetNumCols( 10 );
	m_Font.SetStartChar( ' ' );


	rBarRect.top = 0;
	rBarRect.bottom = 62;
	rBarRect.left = 0;
	rBarRect.right = 200;
}

COptions::~COptions()
{
}

void COptions::Enter()
{
	m_nCursorPosition = 75;

	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	CSGD_WaveManager *pWM = CSGD_WaveManager::GetInstance();
	
	m_nImageID[0] = pTM->LoadTexture("Resource\\JoA_earthwormjim-items.png", D3DCOLOR_XRGB(0,128,255));
	m_nImageID[1] = pTM->LoadTexture("Resource\\JoA_OptionsBar.bmp", D3DCOLOR_XRGB(255,0,255));

	m_nSoundID[0] = pWM->LoadWave("Resource/sounds/JoA_Bosssnatchprincess.wav");
	m_nSoundID[1] = pWM->LoadWave("Resource/sounds/JoA_Song3.wav");

}

void COptions::Update(float fElapsedTime)
{
	CSGD_DirectInput *pDI = CSGD_DirectInput::GetInstance();
	CSGD_WaveManager *pWM = CSGD_WaveManager::GetInstance();

	pWM->SetVolume(m_nSoundID[0], m_nSFXVolume);
	pWM->SetVolume(m_nSoundID[1], m_nMusicVolume);

	if (pDI->GetBufferedKey(DIK_DOWN))
	{
		if (m_nCursorPosition == 450)
			m_nCursorPosition = 75;
		else
			m_nCursorPosition += 125;
	}

	if (pDI->GetBufferedKey(DIK_UP))
	{
		if (m_nCursorPosition == 75)
			m_nCursorPosition = 450;
		else
			m_nCursorPosition -= 125;
	}

	if (m_nCursorPosition == 75 && !pWM->IsWavePlaying(m_nSoundID[1]))
	{
		if (pWM->IsWavePlaying(m_nSoundID[0]))
			pWM->Stop(m_nSoundID[0]);
		pWM->Reset(m_nSoundID[1]);
		pWM->SetVolume(m_nSoundID[1], m_nMusicVolume - 40);
		//pWM->Play(m_nSoundID[1],1);

	}

	if (m_nCursorPosition == 200 && !pWM->IsWavePlaying(m_nSoundID[0]))
	{
		if (pWM->IsWavePlaying(m_nSoundID[1]))
			pWM->Stop(m_nSoundID[1]);
		pWM->Reset(m_nSoundID[0]);
		pWM->SetVolume(m_nSoundID[0], GetSFXVolume() - 40);
		//pWM->Play(m_nSoundID[0],1);
	}

	if (m_nCursorPosition > 200)
	{
		if (pWM->IsWavePlaying(m_nSoundID[0]))
			pWM->Stop(m_nSoundID[0]);
		if (pWM->IsWavePlaying(m_nSoundID[1]))
			pWM->Stop(m_nSoundID[1]);
	}

	if (pDI->GetBufferedKey(DIK_LEFT) && m_nCursorPosition == 75)
	{
		if (m_nMusicVolume != 0)
			m_nMusicVolume -= 10;
	}

	if (pDI->GetBufferedKey(DIK_RIGHT) && m_nCursorPosition == 75)
	{
		if (m_nMusicVolume != 100)
			m_nMusicVolume += 10;
	}

	if (pDI->GetBufferedKey(DIK_LEFT) && m_nCursorPosition == 200)
	{
		if (m_nSFXVolume != 0)
			m_nSFXVolume -= 10;
	}

	if (pDI->GetBufferedKey(DIK_RIGHT) && m_nCursorPosition == 200)
	{
		if (m_nSFXVolume != 100)
			m_nSFXVolume += 10;
	}

	if (pDI->GetBufferedKey(DIK_LEFT) && m_nCursorPosition == 325)
	{
		if (m_nDifficulty == DIFFICULTY_EASY)
			m_nDifficulty = DIFFICULTY_HARD;
		else
			m_nDifficulty -= 1;
	}

	if (pDI->GetBufferedKey(DIK_RIGHT) && m_nCursorPosition == 325)
	{
		if (m_nDifficulty == DIFFICULTY_HARD)
			m_nDifficulty = DIFFICULTY_EASY;
		else
			m_nDifficulty += 1;
	}

	if (pDI->GetBufferedKey(DIK_RETURN) && m_nCursorPosition == 450 || pDI->GetBufferedKey(DIK_ESCAPE))
	{
		CGame::GetInstance()->ChangeState( CMainMenu::GetInstance() );
	}
}

void COptions::Render()
{
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();

	static RECT rBarRect = { 0,0,200,64 };

	rBarRect.right = 200;
	pTM->Draw(m_nImageID[1],0,0,3.5f,8.0f,&rBarRect,0,0,0,D3DCOLOR_XRGB(255,255,255));

	m_Font.DrawString("Options", 200, 0);
	m_Font.DrawString("Music Volume", 125, 75);
	m_Font.DrawString("SFX Volume", 125, 200);
	m_Font.DrawString("Difficulty",125,325);
	m_Font.DrawString("Back",125,450);

	static RECT rCursorRect = {103,116,131,132};

	rBarRect.right = m_nMusicVolume * 2;

	pTM->Draw(m_nImageID[0],75,m_nCursorPosition,1.5f,1.5f,&rCursorRect,0,0,0,D3DCOLOR_XRGB(255,255,255));	
	pTM->Draw(m_nImageID[1], 125,115,2.0f,1,&rBarRect,0,0,0,D3DCOLOR_XRGB(0,0,255));

	rBarRect.right = m_nSFXVolume * 2;
	pTM->Draw(m_nImageID[1], 125,240,2.0f,1,&rBarRect,0,0,0,D3DCOLOR_XRGB(0,0,255));

	if (m_nDifficulty == DIFFICULTY_MEDIUM)
		m_Font.DrawString("Medium",175,375);
	else if (m_nDifficulty == DIFFICULTY_EASY)
		m_Font.DrawString("Easy",175,375);
	else if (m_nDifficulty == DIFFICULTY_HARD)
		m_Font.DrawString("Hard",175,375);


}

void COptions::Exit()
{
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	CSGD_WaveManager *pWM = CSGD_WaveManager::GetInstance();

	pTM->ReleaseTexture(m_nImageID[0]);
	pTM->ReleaseTexture(m_nImageID[1]);

	if (pWM->IsWavePlaying(m_nSoundID[0]))
			pWM->Stop(m_nSoundID[0]);
	if (pWM->IsWavePlaying(m_nSoundID[1]))
			pWM->Stop(m_nSoundID[1]);

	pWM->UnloadWave(m_nSoundID[0]);
	pWM->UnloadWave(m_nSoundID[1]);


}