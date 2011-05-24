/////////////////////////////////////////////////////////////////////////
//	File	:	"FinishedState.h"
//
//	Author	:	John Albright (JA)
//
//	Purpose	:	Defines how the win condition screen is presented
/////////////////////////////////////////////////////////////////////////

#pragma once
#include "AbstractState.h"
#include "CBitmapFont.h"

class CFinishedState : public AbstractState
{
	int m_nImageID[2];
	int m_nSoundID;

	CBitmapFont	m_Font;
	CBitmapFont m_Font2;

	//	Default constructor
	CFinishedState(void);
	//	Copy constructor.
	CFinishedState(const CFinishedState& ref) { }
	//	Assignment Operator.
	CFinishedState& operator=(const CFinishedState& ref) { }

	//	Destructor
	~CFinishedState(void);
public:

	static CFinishedState* GetInstance(void);

	void Enter(void);					// This method initializes the game entities

	void Update(float fElapsedTime);
	void Render(void);

	void Exit(void);					// This method does any necessary clean-up

};
