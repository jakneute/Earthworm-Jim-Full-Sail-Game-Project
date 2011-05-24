/////////////////////////////////////////////////////////////////////////
//	File	:	"HowToState.cpp"
//
//	Author	:	John Albright (JA)
//
//	Purpose	:	Defines how the help screen is presented
/////////////////////////////////////////////////////////////////////////
#include "MainMenu.h"
#include "HowToState.h"
#include "CSGD_TextureManager.h"
#include "CSGD_DirectInput.h"
#include "CGame.h"

CHowToState* CHowToState::GetInstance(void)
{
	static CHowToState instance; // lazy instantiation
	return &instance;
}


CHowToState::CHowToState()
{
	m_Font.SetFontImageID( CSGD_TextureManager::GetInstance()->LoadTexture("resource/JoA_023 - large.bmp", D3DCOLOR_XRGB(0, 0, 0)) );
	m_Font.SetCharWidth( 32 );
	m_Font.SetCharHeight( 32 );
	m_Font.SetNumCols( 10 );
	m_Font.SetStartChar( ' ' );
}

CHowToState::~CHowToState()
{
}

void CHowToState::Enter()
{
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	
	m_nImageID[0] = pTM->LoadTexture("Resource\\JoA_HowTo.PNG");
	m_nImageID[1] = pTM->LoadTexture("Resource\\JoA_earthwormjim-items.png", D3DCOLOR_XRGB(0,128,255));

}

void CHowToState::Update(float fElapsedTime)
{
	CSGD_DirectInput *pDI = CSGD_DirectInput::GetInstance();
	if (pDI->GetBufferedKey(DIK_RETURN))
		CGame::GetInstance()->ChangeState(CMainMenu::GetInstance());
}

void CHowToState::Render()
{
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	static RECT rCursorRect = {103,116,131,132};
	pTM->Draw(m_nImageID[0],0,0,.65f,1.0f,NULL,0,0,0,D3DCOLOR_XRGB(255,255,255));
	m_Font.DrawString("Back",500,450);
	pTM->Draw(m_nImageID[1],460,450,1.5f,1.5f,&rCursorRect,0,0,0,D3DCOLOR_XRGB(255,255,255));

}

void CHowToState::Exit()
{
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();

	pTM->ReleaseTexture(m_nImageID[0]);
	pTM->ReleaseTexture(m_nImageID[1]);


}