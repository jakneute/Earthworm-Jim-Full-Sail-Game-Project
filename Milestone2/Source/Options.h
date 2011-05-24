/////////////////////////////////////////////////////////////////////////
//	File	:	"Options.h"
//
//	Author	:	John Albright (JA)
//
//	Purpose	:	Defines how the options screen is presented
/////////////////////////////////////////////////////////////////////////

#pragma once
#include "AbstractState.h"

class COptions : public AbstractState
{
	int m_nImageID[2];
	int m_nCursorPosition;
	int m_nMusicID[2];
	int m_nSFXVolume;
	int m_nMusicVolume;
	int m_nSoundID[2];

	int m_nDifficulty;

	CBitmapFont	m_Font;

	RECT rBarRect;

	//	Default constructor
	COptions(void);
	//	Copy constructor.
	COptions(const COptions& ref) { }
	//	Assignment Operator.
	COptions& operator=(const COptions& ref) { }

	//	Destructor
	~COptions(void);

public:

	static COptions* GetInstance(void);

	void Enter(void);					// This method initializes the game entities

	void Update(float fElapsedTime);
	void Render(void);

	void Exit(void);					// This method does any necessary clean-up

	// Accessors
	int GetMusicVolume(void) { return m_nMusicVolume; }
	int GetSFXVolume(void) { return m_nSFXVolume; }
	int GetDifficulty(void) { return m_nDifficulty; }


};