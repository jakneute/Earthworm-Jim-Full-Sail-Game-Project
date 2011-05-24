/////////////////////////////////////////////////////////////////////////
//	File	:	"CBase.h"
//
//	Author	:	David Brown (DB)
//
//	Purpose	:	The base object for our class hierarchy and implements
//				our base interface.
/////////////////////////////////////////////////////////////////////////

#pragma once
#include <windows.h>			//	for RECT
#include "IBaseInterface.h"
#include "SGD_Math.h"

class CBase: public IBaseInterface
{
private:
	unsigned int	m_unRefCount;	//	how many people have pointers to me.

	int			m_nImageID;
	int			m_nHP;

	tPoint2D	m_tPos;
	
	tVector2D	m_tVel;

	int			m_nWidth;
	int			m_nHeight;
	float		m_fRotation;


public:
	CBase(void);
	virtual ~CBase(void);

	/////////////////////////////////////////////////////////////////////
	//	Function	:	Accessors
	//
	//	Purpose		:	To access the specified type.
	/////////////////////////////////////////////////////////////////////
	inline int	GetImageID(void)		{ return m_nImageID; }
	inline float	GetPosX(void)			{ return m_tPos.fX;	}
	inline float	GetPosY(void)			{ return m_tPos.fY;	}
	inline float	GetVelX(void)			{ return m_tVel.fX;	}
	inline float	GetVelY(void)			{ return m_tVel.fY;	}
	inline int		GetWidth(void)			{ return m_nWidth; }
	inline int		GetHeight(void)			{ return m_nHeight; }
	inline float	GetRotation(void)		{ return m_fRotation; }
	inline unsigned int GetHP(void) { return m_nHP; }

	virtual RECT	GetCollisionRect(void);
	virtual void	SetCollisionRect(RECT);

	/////////////////////////////////////////////////////////////////////
	//	Function	:	Mutators
	//
	//	Purpose		:	To modify the specified type.
	/////////////////////////////////////////////////////////////////////
	inline void SetImageID(int nImageID)	{ m_nImageID = nImageID; }
	inline void SetPosX(float fPosX)			{ m_tPos.fX = fPosX;	}
	inline void SetPosY(float fPosY)			{ m_tPos.fY = fPosY;	}
	inline void SetVelX(float fVelX)			{ m_tVel.fX = fVelX;	}
	inline void SetVelY(float fVelY)			{ m_tVel.fY = fVelY;	}
	inline void	SetWidth(int nWidth)			{ m_nWidth = nWidth;	}
	inline void	SetHeight(int nHeight)			{ m_nHeight = nHeight;	}
	inline void SetRotation(float rot)			{ m_fRotation = rot;	}
	inline void SetHP(unsigned int nHP) { m_nHP = nHP; }


	virtual void Update(float fElapsedTime);
	void Render(void);

	void AddRef(void)	{	m_unRefCount++;	}
	void Release(void)
	{
		m_unRefCount--;

		// delete self if no more references
		if (m_unRefCount == 0)
			delete this;
	}
};