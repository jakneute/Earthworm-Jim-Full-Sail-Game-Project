/////////////////////////////////////////////////////////////////////////
//	File	:	"MainMenu.h"
//
//	Author	:	John Albright (JA)
//
//	Purpose	:	Defines how the main menu is presented
/////////////////////////////////////////////////////////////////////////

#pragma once
#include "AbstractState.h"
#include "CBitmapFont.h"

class CMainMenu : public AbstractState
{
	int m_nImageID[2];
	int m_nCursorPosition;

	CBitmapFont	m_Font;

	//	Default constructor
	CMainMenu(void);
	//	Copy constructor.
	CMainMenu(const CMainMenu& ref) { }
	//	Assignment Operator.
	CMainMenu& operator=(const CMainMenu& ref) { }

	//	Destructor
	~CMainMenu(void);
public:

	static CMainMenu* GetInstance(void);

	void Enter(void);					// This method initializes the game entities

	void Update(float fElapsedTime);
	void Render(void);

	void Exit(void);					// This method does any necessary clean-up
};
