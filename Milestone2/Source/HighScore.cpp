/////////////////////////////////////////////////////////////////////////
//	File	:	"HighScore.cpp"
//
//	Author	:	John Albright (JA)
//
//	Purpose	:	Defines how the high score table is presented
/////////////////////////////////////////////////////////////////////////

#include "MainMenu.h"
#include "CSGD_Direct3D.h"
#include "CSGD_DirectInput.h"
#include "CSGD_TextureManager.h"
#include "CGame.h"
#include "CSGD_ObjectManager.h"
#include "CSGD_ObjectFactory.h"
#include "HighScore.h"

CHighScore* CHighScore::GetInstance(void)
{
	static CHighScore instance; // lazy instantiation
	return &instance;
}


CHighScore::CHighScore()
{
	m_nImageID[0] = -1;
	m_nImageID[1] = -1;

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

CHighScore::~CHighScore()
{
}

void CHighScore::Enter()
{
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();

	m_nCursorPosition = 450;
	
	m_nImageID[0] = pTM->LoadTexture("Resource\\JoA_earthwormjim-items.png", D3DCOLOR_XRGB(0,128,255));
	m_nImageID[1] = pTM->LoadTexture("Resource\\JoA_OptionsBar.bmp",D3DCOLOR_XRGB(255,0,255));

	fs.open("Highscore.dat");

	char buffer[10][128];
	char buffer2[10][128];

	for(int i = 0; i < 10; ++i)
	{
		fs.getline(buffer[i],128,'\n');
		scores[i].cName = buffer[i];
		strcpy(scores[i].cName,buffer[i]);
		fs.getline(buffer2[i],128,'\n');
		scores[i].nScore = atoi(buffer2[i]);
	}
	fs.clear();
	fs.close();

}

void CHighScore::Update(float fElapsedTime)
{
	CSGD_DirectInput *pDI = CSGD_DirectInput::GetInstance();

	if (pDI->GetBufferedKey(DIK_RETURN))
		CGame::GetInstance()->ChangeState( CMainMenu::GetInstance() );
}

void CHighScore::Render()
{
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();

	static RECT rCursorRect = {103,116,131,132};
	static RECT rBarRect = { 0,0,200,64 };

	pTM->Draw(m_nImageID[1],0,0,3.5f,8.0f,&rBarRect,0,0,0,D3DCOLOR_XRGB(255,255,255));

	m_Font.DrawString("High Scores", 150, 0);

	for (int i = 0; i < 10; ++i)
	{
		char buffer[128];
		sprintf_s(buffer, _countof(buffer), "%d", scores[i].nScore);
		m_Font.DrawString(buffer,250,i*40 + 40);
	}
	m_Font.DrawString("Back",500,450);

	pTM->Draw(m_nImageID[0],450,m_nCursorPosition,1.5f,1.5f,&rCursorRect,0,0,0,D3DCOLOR_XRGB(255,255,255));
}

void CHighScore::Exit()
{
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();
	pTM->ReleaseTexture(m_nImageID[0]);
	pTM->ReleaseTexture(m_nImageID[1]);
}