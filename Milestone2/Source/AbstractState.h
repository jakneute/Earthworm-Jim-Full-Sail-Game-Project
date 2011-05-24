/////////////////////////////////////////////////////////////////////////
//	File	:	"AbstractState.h"
//
//	Author	:	John Albright (JA)
//
//	Purpose	:	Defines the interface for all states
/////////////////////////////////////////////////////////////////////////

#pragma once

class AbstractState
{
public:

	virtual void Enter(void) = 0;					// This method initializes the game entities

	virtual void Update(float fElapsedTime) = 0;
	virtual void Render(void) = 0;

	virtual void Exit(void) = 0;					// This method does any necessary clean-up


};