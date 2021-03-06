#include "MainMenu.h"
#include "Options.h"
#include "CSGD_WaveManager.h"
#include "CSGD_TextureManager.h"
#include "CSGD_DirectInput.h"
#include "CGame.h"

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

	m_Font.SetFontImageID( CSGD_TextureManager::GetInstance()->LoadTexture("resource/023 - large.bmp", D3DCOLOR_XRGB(0, 0, 0)) );
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
	m_nCursorPosition = 100;

	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	CSGD_WaveManager *pWM = CSGD_WaveManager::GetInstance();
	
	m_nImageID[0] = pTM->LoadTexture("Resource\\earthwormjim-items.png", D3DCOLOR_XRGB(0,128,255));
	m_nImageID[1] = pTM->LoadTexture("Resource\\OptionsBar.bmp", D3DCOLOR_XRGB(255,0,255));

	m_nSoundID[0] = pWM->LoadWave("Resource\\SGD_sfx1.wav");
	m_nSoundID[1] = pWM->LoadWave("Resource\\SGD_MarioSong.wav");

}

void COptions::Update(float fElapsedTime)
{
	CSGD_DirectInput *pDI = CSGD_DirectInput::GetInstance();
	CSGD_WaveManager *pWM = CSGD_WaveManager::GetInstance();

	pWM->SetVolume(m_nSoundID[0], m_nSFXVolume);
	pWM->SetVolume(m_nSoundID[1], m_nMusicVolume);

	if (pDI->GetBufferedKey(DIK_DOWN))
	{
		if (m_nCursorPosition == 370)
			m_nCursorPosition = 100;
		else
			m_nCursorPosition += 135;
	}

	if (pDI->GetBufferedKey(DIK_UP))
	{
		if (m_nCursorPosition == 100)
			m_nCursorPosition = 370;
		else
			m_nCursorPosition -= 135;
	}

	if (m_nCursorPosition == 100 && !pWM->IsWavePlaying(m_nSoundID[1]))
	{
		if (pWM->IsWavePlaying(m_nSoundID[0]))
			pWM->Stop(m_nSoundID[0]);
		pWM->Reset(m_nSoundID[1]);
		pWM->Play(m_nSoundID[1],1);

	}

	if (m_nCursorPosition == 235 && !pWM->IsWavePlaying(m_nSoundID[0]))
	{
		if (pWM->IsWavePlaying(m_nSoundID[1]))
			pWM->Stop(m_nSoundID[1]);
		pWM->Reset(m_nSoundID[0]);
		pWM->Play(m_nSoundID[0],1);

	}

	if (m_nCursorPosition == 370)
	{
		if (pWM->IsWavePlaying(m_nSoundID[0]))
			pWM->Stop(m_nSoundID[0]);
		if (pWM->IsWavePlaying(m_nSoundID[1]))
			pWM->Stop(m_nSoundID[1]);
	}

	if (pDI->GetBufferedKey(DIK_LEFT) && m_nCursorPosition == 100)
	{
		if (m_nMusicVolume != 0)
			m_nMusicVolume -= 10;
	}

	if (pDI->GetBufferedKey(DIK_RIGHT) && m_nCursorPosition == 100)
	{
		if (m_nMusicVolume != 100)
			m_nMusicVolume += 10;
	}

	if (pDI->GetBufferedKey(DIK_LEFT) && m_nCursorPosition == 235)
	{
		if (m_nSFXVolume != 0)
			m_nSFXVolume -= 10;
	}

	if (pDI->GetBufferedKey(DIK_RIGHT) && m_nCursorPosition == 235)
	{
		if (m_nSFXVolume != 100)
			m_nSFXVolume += 10;
	}

	if (pDI->GetBufferedKey(DIK_RETURN) && m_nCursorPosition == 370 || pDI->GetBufferedKey(DIK_ESCAPE))
	{
		CGame::GetInstance()->ChangeState( CMainMenu::GetInstance() );
	}
}

void COptions::Render()
{
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();

	m_Font.DrawString("Options", 200, 0);
	m_Font.DrawString("Music Volume", 125, 100);
	m_Font.DrawString("SFX Volume", 125, 235);
	m_Font.DrawString("Back",125,370);

	static RECT rCursorRect = {138,120,172,140};

	rBarRect.right = m_nMusicVolume * 2;

	pTM->Draw(m_nImageID[0],65,m_nCursorPosition,1.5f,1.5f,&rCursorRect,0,0,0,D3DCOLOR_XRGB(255,255,255));	
	pTM->Draw(m_nImageID[1], 125,150,2.0f,1,&rBarRect,0,0,0,D3DCOLOR_XRGB(255,0,0));

	rBarRect.right = m_nSFXVolume * 2;
	pTM->Draw(m_nImageID[1], 125,285,2.0f,1,&rBarRect,0,0,0,D3DCOLOR_XRGB(255,0,0));


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



}