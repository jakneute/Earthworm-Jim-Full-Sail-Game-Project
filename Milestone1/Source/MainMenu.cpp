#include "MainMenu.h"
#include "CSGD_Direct3D.h"
#include "CSGD_DirectInput.h"
#include "CSGD_TextureManager.h"
#include "CGame.h"
#include "GamePlay.h"
#include "Options.h"
#include "CSGD_ObjectManager.h"
#include "CSGD_ObjectFactory.h"

CMainMenu* CMainMenu::GetInstance(void)
{
	static CMainMenu instance; // lazy instantiation
	return &instance;
}


CMainMenu::CMainMenu()
{
	m_nImageID = -1;
	m_nCursorPosition = 175;

	m_Font.SetFontImageID( CSGD_TextureManager::GetInstance()->LoadTexture("resource/023 - large.bmp", D3DCOLOR_XRGB(0, 0, 0)) );
	m_Font.SetCharWidth( 32 );
	m_Font.SetCharHeight( 32 );
	m_Font.SetNumCols( 10 );
	m_Font.SetStartChar( ' ' );
}

CMainMenu::~CMainMenu()
{
}

void CMainMenu::Enter()
{
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	
	m_nImageID = pTM->LoadTexture("Resource\\earthwormjim-items.png", D3DCOLOR_XRGB(0,128,255));
}

void CMainMenu::Update(float fElapsedTime)
{
	CSGD_DirectInput *pDI = CSGD_DirectInput::GetInstance();

	if (pDI->GetBufferedKey(DIK_RETURN))
	{
		if (m_nCursorPosition == 175)
			CGame::GetInstance()->ChangeState( CGamePlay::GetInstance() );
		else if (m_nCursorPosition == 215)
			CGame::GetInstance()->ChangeState( COptions::GetInstance() );
		else
			PostQuitMessage(0);
	}

	if (pDI->GetBufferedKey(DIK_DOWN))
	{
		if (m_nCursorPosition == 375)
			m_nCursorPosition = 175;
		else
			m_nCursorPosition += 40;
	}

	if (pDI->GetBufferedKey(DIK_UP))
	{
		if (m_nCursorPosition == 175)
			m_nCursorPosition = 375;
		else
			m_nCursorPosition -= 40;
	}
}

void CMainMenu::Render()
{
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();

	static RECT rCursorRect = {138,120,172,140};

	m_Font.DrawString("Main Menu", 175, 0);
	m_Font.DrawString("Start Game",155,175);
	m_Font.DrawString("Options",155,215);
	m_Font.DrawString("How to Play",155,255);
	m_Font.DrawString("High Scores",155,295);
	m_Font.DrawString("Credits",155,335);
	m_Font.DrawString("Quit Game",155,375);


	pTM->Draw(m_nImageID,95,m_nCursorPosition,1.5f,1.5f,&rCursorRect,0,0,0,D3DCOLOR_XRGB(255,255,255));
}

void CMainMenu::Exit()
{
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	pTM->ReleaseTexture(m_nImageID);

}