
#pragma once

#include "CEvent.h"


/*	Game Project Hierarchy:
	IListener
	IBaseInterface
	CBase		->	IBaseInterface		(-20 points for having IListener being derived from here)
	CPlayer		->	CBase, IListener
	CEnemy		->	CBase, IListener
	CGameClass	->	IListener
*/

class IListener
{
public:
	IListener()	{}
	~IListener()	{}

	//	Blue print function - MUST be defined in the derived class.
	//	Mini-Proc function whenever this object receives an event.
	virtual void HandleEvent(CEvent* pEvent) = 0;
};