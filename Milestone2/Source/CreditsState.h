/////////////////////////////////////////////////////////////////////////
//	File	:	"CreditsState.h"
//
//	Author	:	John Albright (JA)
//
//	Purpose	:	Defines how the credits screen is shown
/////////////////////////////////////////////////////////////////////////

#pragma once
#include "AbstractState.h"

class CCreditsState : public AbstractState
{
	int m_nImageID[2];

	CBitmapFont m_Font;

	//	Default constructor
	CCreditsState(void);
	//	Copy constructor.
	CCreditsState(const CCreditsState& ref) { }
	//	Assignment Operator.
	CCreditsState& operator=(const CCreditsState& ref) { }

	//	Destructor
	~CCreditsState(void);

public:
	//CCreditsState(void);
	//~CCreditsState(void);

	void Enter(void);					// This method initializes the game entities

	void Update(float fElapsedTime);
	void Render(void);

	static CCreditsState* GetInstance(void);


	void Exit(void);					// This method does any necessary clean-up

};