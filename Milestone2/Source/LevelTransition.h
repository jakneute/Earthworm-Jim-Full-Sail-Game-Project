/////////////////////////////////////////////////////////////////////////
//	File	:	"LevelTransition.h"
//
//	Author	:	John Albright (JA)
//
//	Purpose	:	Defines how the screen shown between levels is presented
/////////////////////////////////////////////////////////////////////////

#pragma once
#include "AbstractState.h"
#include "CBitmapFont.h"

class CLevelTransition : public AbstractState
{
	int m_nImageID;

	float m_fStateTimer;

	int m_nMusicID;

	CBitmapFont	m_Font;

	//	Default constructor
	CLevelTransition(void);
	//	Copy constructor.
	CLevelTransition(const CLevelTransition& ref) { }
	//	Assignment Operator.
	CLevelTransition& operator=(const CLevelTransition& ref) { }

	//	Destructor
	~CLevelTransition(void);

public:

	static CLevelTransition* GetInstance(void);

	void Enter(void);					// This method initializes the game entities

	void Update(float fElapsedTime);
	void Render(void);

	void Exit(void);					// This method does any necessary clean-up
};
