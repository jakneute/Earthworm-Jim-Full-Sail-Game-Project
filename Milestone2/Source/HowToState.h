/////////////////////////////////////////////////////////////////////////
//	File	:	"HowToState.h"
//
//	Author	:	John Albright (JA)
//
//	Purpose	:	Defines how the help screen is presented
/////////////////////////////////////////////////////////////////////////

#pragma once
#include "AbstractState.h"

class CHowToState : public AbstractState
{
	int m_nImageID[2];

	CBitmapFont m_Font;

	//	Default constructor
	CHowToState(void);
	//	Copy constructor.
	CHowToState(const CHowToState& ref) { }
	//	Assignment Operator.
	CHowToState& operator=(const CHowToState& ref) { }

	//	Destructor
	~CHowToState(void);

public:

	void Enter(void);					// This method initializes the game entities

	void Update(float fElapsedTime);
	void Render(void);

	static CHowToState* GetInstance(void);


	void Exit(void);					// This method does any necessary clean-up

};