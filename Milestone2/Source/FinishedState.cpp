/////////////////////////////////////////////////////////////////////////
//	File	:	"FinishedState.cpp"
//
//	Author	:	John Albright (JA)
//
//	Purpose	:	Defines how the win condition screen is presented
/////////////////////////////////////////////////////////////////////////

#include "FinishedState.h"
#include "CSGD_TextureManager.h"
#include "CSGD_DirectInput.h"
#include "Enums.h"
#include "HighScore.h"
#include "Options.h"
#include "CGame.h"
#include "CSGD_WaveManager.h"

CFinishedState* CFinishedState::GetInstance(void)
{
	static CFinishedState instance; // lazy instantiation
	return &instance;
}


CFinishedState::CFinishedState()
{
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
}

CFinishedState::~CFinishedState()
{
}

void CFinishedState::Enter()
{
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	CSGD_WaveManager *pWM = CSGD_WaveManager::GetInstance();
	
	m_nImageID[0] = pTM->LoadTexture("Resource\\JoA_OptionsBar.bmp", D3DCOLOR_XRGB(255,0,255));
	m_nImageID[1] = pTM->LoadTexture("Resource\\JoA_earthwormjim-items.png", D3DCOLOR_XRGB(0,128,255));
	m_nSoundID = pWM->LoadWave("Resource/sounds/JoA_Song1.wav");
	pWM->SetVolume(m_nSoundID,COptions::GetInstance()->GetMusicVolume() - 25);
	pWM->Play(m_nSoundID,0);

}

void CFinishedState::Update(float fElapsedTime)
{
	CSGD_DirectInput *pDI = CSGD_DirectInput::GetInstance();
	if (pDI->GetBufferedKey(DIK_RETURN))
		CGame::GetInstance()->ChangeState(CHighScore::GetInstance());
}

void CFinishedState::Render()
{
	COptions *op = COptions::GetInstance();
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	static RECT rCursorRect = {103,116,131,132};
	static RECT rBarRect = { 0,0,200,64 };
	pTM->Draw(m_nImageID[0],0,0,3.5f,8.0f,&rBarRect,0,0,0,D3DCOLOR_XRGB(255,255,255));
	pTM->Draw(m_nImageID[1],460,450,1.5f,1.5f,&rCursorRect,0,0,0,D3DCOLOR_XRGB(255,255,255));
	m_Font.DrawString("Congratulations!",75,0);
	m_Font.DrawString("Back",500,450);

	if (op->GetDifficulty() == DIFFICULTY_MEDIUM)
	{
		m_Font2.DrawString("You beat the game on the medium",32,64);
		m_Font2.DrawString("difficulty! Don't you feel special?",32,96); 
		m_Font2.DrawString("You're skills are average. For some,",32,128);
		m_Font2.DrawString("that's good enough, but are you going",32,160); 
		m_Font2.DrawString("to settle just for good enough? Play",32,192); 
		m_Font2.DrawString("the game on hard. I dare you.",32,224);
	}
	else if (op->GetDifficulty() == DIFFICULTY_EASY)
	{
		m_Font2.DrawString(	"Wow! You managed to complete the",32,64); 
		m_Font2.DrawString("game on easy? Good for you! Why don't",32,96);
		m_Font2.DrawString("you try stepping up the difficulty a",32,128);
		m_Font2.DrawString("notch? I wouldn't kill you to try.",32,160); 
		m_Font2.DrawString("Or maybe it will.",32,192);  
	}
	else if (op->GetDifficulty() == DIFFICULTY_HARD)
	{
		m_Font2.DrawString("Amazing! Put the game down. You",32,64); 
		m_Font2.DrawString("mastered it! You beat the game on",32,96); 
		m_Font2.DrawString("hard! You're the best. If you ever",32,128); 
		m_Font2.DrawString("find yourself failing at life, just",32,160); 
		m_Font2.DrawString("say to yourself 'I'm the best!'",32,192);
		m_Font2.DrawString("P.S. you better not have cheated.",32,256);
	}
}

void CFinishedState::Exit()
{
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	CSGD_WaveManager *pWM = CSGD_WaveManager::GetInstance();

	pTM->ReleaseTexture(m_nImageID[0]);
	pTM->ReleaseTexture(m_nImageID[1]);

	pWM->UnloadWave(m_nSoundID);
}