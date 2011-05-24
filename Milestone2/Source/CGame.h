/////////////////////////////////////////////////////////////////
//	File	:	"CGame.h"
//
//	Author	:	John Albright (JA)
//
//	Purpose	:	Contains all game specific code.
/////////////////////////////////////////////////////////////////

#pragma once // MSVC specific, tells the compiler to make sure to only include this once

/*	This is cross compiler
#ifndef _CGAME_H_
#define _CGAME_H_


#endif

The only thing is you have to do this for EVERY header
*/

#include "CSGD_Direct3D.h"
#include "CSGD_TextureManager.h"
#include "CSGD_DirectSound.h"
#include "CSGD_WaveManager.h"
#include "CSGD_DirectInput.h"
#include "CSGD_ObjectManager.h"
#include "CSGD_ObjectFactory.h"
#include "AbstractState.h"
#include "CBitmapFont.h"

#include "CSGD_MessageSystem.h"
#include "CSGD_EventSystem.h"

#include "CBase.h"

#include "SGD_Math.h"

#include <string>
using std::string;


// Macros:
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)	if (p) { p->Release(); p = NULL; }
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)	if (p) { delete p; p = NULL; }
#endif

class CGame: public IListener
{
private:
	
	CSGD_Direct3D*			m_pD3D;			//	D3D singleton.
	CSGD_TextureManager*	m_pTM;			//	TextureManager singleton.
	CSGD_DirectSound*		m_pDS;			//	DirectSound singleton.
	CSGD_WaveManager*		m_pWM;			//	WaveManager singleton.
	CSGD_DirectInput*		m_pDI;			//	DirectInput singleton.

	CSGD_ObjectFactory<string, CBase>*		m_pOF;			//	ObjectFactory singleton.
	CSGD_ObjectManager						*m_OM;			//	Object Manager

	CSGD_MessageSystem*						m_pMS;
	CSGD_EventSystem*						m_pES;

	CBitmapFont m_Font;
	CBitmapFont m_Font2;


	int	m_nImageID;
	int	m_nSoundID[3];

	int m_nNumLives;
	int m_nScore;
	float m_nSpeed;

	bool m_bIsPaused;

	AbstractState*			m_pCurState;

	CBase*				m_pPlayerCharacter;			//	pointer to my player character

	//	For Frame Counting
	int					m_nFrameCounter;	//	counts the number of frames we're getting
	int					m_nFPS;				//	displays the number of frames counted from the previous frame
	DWORD				m_dwFrameTimer;		//	time stamp of the start of the frame
	
	//	For calculating elapsed time
	DWORD				m_dwPreviousTime;	//	Time stamp of the last frame
	float				m_fElapsedTime;		//	Time elapsed (in seconds) since the previous frame.

	//	This define a PROPER singleton.

	//	Trilogy of EVIL!

	//	Default constructor
	CGame(void);
	//	Copy constructor.
	CGame(const CGame& ref);
	//	Assignment Operator.
	CGame& operator=(const CGame& ref);

	//	Destructor
	~CGame(void);

	static void MessageProc(CBaseMessage* pMsg);

public:
	static CGame* GetInstance(void);

	//	Initialization
	void GameInit(HWND hWnd, HINSTANCE hInstance, int nScreenWidth, int nScreenHeight, bool bIsWindowed);

	//	Execution
	bool GameMain(void);
	
	//	Clean up (Shutdown)
	void GameShutdown(void);

	//	Handles events that the Game has registered for
	void HandleEvent(CEvent* pEvent);

	void ChangeState(AbstractState* state);

	bool GetPaused(void) { return m_bIsPaused; }
	void SetPaused(bool bPaused) { m_bIsPaused = bPaused; }

	int GetLives(void) { return m_nNumLives; }
	void SetLives(int lives) { m_nNumLives = lives; }

	CBase* GetPlayerCharacter(void) { return m_pPlayerCharacter; }
	void SetPlayerCharacter(CBase* player) { m_pPlayerCharacter = player; }

	int GetScore(void) { return m_nScore; }
	void SetScore(int score) { m_nScore = score; }

	float GetSpeed(void) { return m_nSpeed; }
	void SetSpeed(float speed) { m_nSpeed = speed; }

	int GetSound(int i) { return m_nSoundID[i]; }
};