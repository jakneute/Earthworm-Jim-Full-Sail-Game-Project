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

	CBitmapFont	m_Font;

	RECT rBarRect;

public:
	COptions(void);
	~COptions(void);

	static COptions* GetInstance(void);

	void Enter(void);					// This method initializes the game entities

	void Update(float fElapsedTime);
	void Render(void);

	void Exit(void);					// This method does any necessary clean-up

	// Accessors
	int GetMusicVolume(void) { return m_nMusicVolume; }
	int GetSFXVolume(void) { return m_nSFXVolume; }

};