/////////////////////////////////////////////////////////////////////////
//	File	:	"TallyMenu.cpp"
//
//	Author	:	John Albright (JA)
//
//	Purpose	:	Defines how the tally screen is presented
/////////////////////////////////////////////////////////////////////////

#include "CSGD_DirectInput.h"
#include "CSGD_TextureManager.h"
#include "GamePlay.h"
#include "CSGD_WaveManager.h"
#include "CPlayerCharacter.h"
#include "LevelTransition.h"
#include "TallyMenu.h"
#include "CGame.h"
#include "Options.h"
#include "MainMenu.h"
#include "FinishedState.h"

CTallyMenu* CTallyMenu::GetInstance(void)
{
	static CTallyMenu instance; // lazy instantiation
	return &instance;
}


CTallyMenu::CTallyMenu()
{
	m_nImageID = -1;
	m_nMusicID = -1;

	m_Font.SetFontImageID( CSGD_TextureManager::GetInstance()->LoadTexture("resource/JoA_023 - small.bmp", D3DCOLOR_XRGB(0, 0, 0)) );
	m_Font.SetCharWidth( 16 );
	m_Font.SetCharHeight( 16 );
	m_Font.SetNumCols( 10 );
	m_Font.SetStartChar( ' ' );
}

CTallyMenu::~CTallyMenu()
{
}

void CTallyMenu::Enter()
{
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	CSGD_WaveManager *pWM = CSGD_WaveManager::GetInstance();
	COptions *op = COptions::GetInstance();
	CGame *theGame = CGame::GetInstance();

	m_fStateTimer = 15.0f;

	m_nBonusHealth = GetHP() * 5 * op->GetDifficulty();
	m_nBonusLives = theGame->GetLives() * theGame->GetLives() * 25 * op->GetDifficulty();
	m_nNewScore = theGame->GetScore() + m_nBonusHealth + m_nBonusLives;
	
	m_nImageID = pTM->LoadTexture("Resource\\JoA_TallyScreen.png");

	m_nMusicID = pWM->LoadWave("Resource/sounds/JoA_Song5.wav");
	pWM->SetVolume(m_nMusicID,op->GetMusicVolume() - 25);

	pWM->Play(m_nMusicID,0);

}

void CTallyMenu::Update(float fElapsedTime)
{
	CSGD_DirectInput *pDI = CSGD_DirectInput::GetInstance();
	m_fStateTimer -= fElapsedTime;

	if (pDI->GetBufferedKey(DIK_RETURN) || m_fStateTimer <= 0)
	{
		if (CGamePlay::GetInstance()->GetLevel() < 4)
			CGame::GetInstance()->ChangeState(CLevelTransition::GetInstance() );
		else
			CGame::GetInstance()->ChangeState(CFinishedState::GetInstance() );
	}
}

void CTallyMenu::Render()
{
	CGame *theGame = CGame::GetInstance();
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();

	pTM->Draw(m_nImageID,0,0,2.5f,1.87f,NULL,0,0,0,D3DCOLOR_XRGB(255,255,255));

	m_Font.DrawString("Bonus Points", 315,235);
	m_Font.DrawString("Lives Left",230,275);
	m_Font.DrawString("Health Left",230,307);
	m_Font.DrawString("Current Score",185,371);
	m_Font.DrawString("New Score",185,403);

	char buffer[128];

	sprintf_s(buffer, _countof(buffer), "%d", m_nBonusHealth);
	m_Font.DrawString(buffer,475,307);

	sprintf_s(buffer, _countof(buffer), "%d", m_nBonusLives);
	m_Font.DrawString(buffer,475,275);

	sprintf_s(buffer, _countof(buffer), "%d", theGame->GetScore());
	m_Font.DrawString(buffer,475,371);

	sprintf_s(buffer, _countof(buffer), "%d", m_nNewScore);
	m_Font.DrawString(buffer,475,403);

}
void CTallyMenu::Exit()
{
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	CGame *theGame = CGame::GetInstance();
	pTM->ReleaseTexture(m_nImageID);

	CSGD_WaveManager::GetInstance()->UnloadWave(m_nMusicID);
	theGame->SetScore(theGame->GetScore() + m_nNewScore);

}