////////////////////////////////////////////////////////////////////////////////
//	File	:	"IBaseInterface.h"
//
//	Author	:	John Albright (JA)
//
//	Purpose	:	Define a set of functions that EVERY object that inherits from
//				this class must implement.
/////////////////////////////////////////////////////////////////////////////////

#pragma once

class IBaseInterface
{
public:
	//	Blue print functions our object hierarchy will implement
	virtual ~IBaseInterface(void) = 0 {}

	virtual void Update(float fElapsedTime) = 0;
	virtual void Render(void) = 0;

	//	Reference counting functions:
	virtual void AddRef(void) = 0;
	virtual void Release(void) = 0;

};