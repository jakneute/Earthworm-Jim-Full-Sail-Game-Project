#pragma once
#include "AbstractState.h"
#include "CBitmapFont.h"

class CMainMenu : public AbstractState
{
	int m_nImageID;
	int m_nCursorPosition;

	CBitmapFont	m_Font;
public:
	CMainMenu(void);
	~CMainMenu(void);

	static CMainMenu* GetInstance(void);

	void Enter(void);					// This method initializes the game entities

	void Update(float fElapsedTime);
	void Render(void);

	void Exit(void);					// This method does any necessary clean-up
};
