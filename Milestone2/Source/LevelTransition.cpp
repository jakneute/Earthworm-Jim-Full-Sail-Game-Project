/////////////////////////////////////////////////////////////////////////
//	File	:	"LevelTransition.cpp"
//
//	Author	:	John Albright (JA)
//
//	Purpose	:	Defines how the screen shown between levels is presented
/////////////////////////////////////////////////////////////////////////

#include "CSGD_DirectInput.h"
#include "CSGD_TextureManager.h"
#include "GamePlay.h"
#include "CSGD_WaveManager.h"
#include "CPlayerCharacter.h"
#include "LevelTransition.h"
#include "CGame.h"
#include "Options.h"
#include "HighScore.h"

CLevelTransition* CLevelTransition::GetInstance(void)
{
	static CLevelTransition instance; // lazy instantiation
	return &instance;
}


CLevelTransition::CLevelTransition()
{
	m_nImageID = -1;
	m_nMusicID = -1;

	m_Font.SetFontImageID( CSGD_TextureManager::GetInstance()->LoadTexture("resource/JoA_023 - large.bmp", D3DCOLOR_XRGB(0, 0, 0)) );
	m_Font.SetCharWidth( 32 );
	m_Font.SetCharHeight( 32 );
	m_Font.SetNumCols( 10 );
	m_Font.SetStartChar( ' ' );
}

CLevelTransition::~CLevelTransition()
{
}

void CLevelTransition::Enter()
{
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	CSGD_WaveManager *pWM = CSGD_WaveManager::GetInstance();
	COptions *op = COptions::GetInstance();

	m_fStateTimer = 5.0f;
	
	m_nImageID = pTM->LoadTexture("Resource\\JoA_Menu.png");

	m_nMusicID = pWM->LoadWave("Resource/sounds/JoA_Song3.wav");
	pWM->SetVolume(m_nMusicID,op->GetMusicVolume() - 25);

	pWM->Play(m_nMusicID,1);

}

void CLevelTransition::Update(float fElapsedTime)
{
	CSGD_DirectInput *pDI = CSGD_DirectInput::GetInstance();
	m_fStateTimer -= fElapsedTime;

	if (pDI->GetBufferedKey(DIK_RETURN) || m_fStateTimer <= 0)
	{
		if (CGame::GetInstance()->GetLives() >= 0)
			CGame::GetInstance()->ChangeState( CGamePlay::GetInstance() );
		else
			CGame::GetInstance()->ChangeState( CHighScore::GetInstance() );
	}
}

void CLevelTransition::Render()
{
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();

	pTM->Draw(m_nImageID,0,0,2.5f,2.0f,NULL,0,0,0,D3DCOLOR_XRGB(255,255,255));

	if (CGame::GetInstance()->GetLives() < 0)
		m_Font.DrawString("Game Over",200,0);
	else
	{
		char buffer[128];
		if (CGame::GetInstance()->GetLives() >= 0)
			sprintf_s(buffer, _countof(buffer), "Lives:%d", CGame::GetInstance()->GetLives());

		m_Font.DrawString(buffer, 400,0);

		if (CGamePlay::GetInstance()->GetLevel() == 1)
			m_Font.DrawString("Level One", 0, 0);
		else if (CGamePlay::GetInstance()->GetLevel() == 2)
			m_Font.DrawString("Level Two", 0, 0);
		else if (CGamePlay::GetInstance()->GetLevel() == 3)
			m_Font.DrawString("Level Three", 0, 0);
	}

}

void CLevelTransition::Exit()
{
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	pTM->ReleaseTexture(m_nImageID);

	CSGD_WaveManager::GetInstance()->UnloadWave(m_nMusicID);


}