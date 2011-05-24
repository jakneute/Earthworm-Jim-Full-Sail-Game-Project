/////////////////////////////////////////////////////////////////////////
//	File	:	"TallyMenu.h"
//
//	Author	:	John Albright (JA)
//
//	Purpose	:	Defines how the tally screen is presented
/////////////////////////////////////////////////////////////////////////

#pragma once
#include "AbstractState.h"
#include "CBitmapFont.h"

class CTallyMenu : public AbstractState
{
	int m_nImageID;

	float m_fStateTimer;

	int m_nMusicID;
	int m_nHP;

	int m_nBonusLives;
	int m_nBonusHealth;
	int m_nNewScore;

	CBitmapFont	m_Font;

	//	Default constructor
	CTallyMenu(void);
	//	Copy constructor.
	CTallyMenu(const CTallyMenu& ref) { }
	//	Assignment Operator.
	CTallyMenu& operator=(const CTallyMenu& ref) { }

	//	Destructor
	~CTallyMenu(void);

public:

	void Enter(void);					// This method initializes the game entities

	void Update(float fElapsedTime);
	void Render(void);

	static CTallyMenu* GetInstance(void);

	void Exit(void);					// This method does any necessary clean-up

	int GetHP() { return m_nHP; }
	void SetHP(int hp) { m_nHP = hp; }
};
