/////////////////////////////////////////////////////////////////////////
//	File	:	"HighScore.h"
//
//	Author	:	John Albright (JA)
//
//	Purpose	:	Defines how the high score table is presented
/////////////////////////////////////////////////////////////////////////

#pragma once
#include "AbstractState.h"
#include "CBitmapFont.h"
#include <fstream>
using namespace std;

struct sScore
{
	char* cName;
	int nScore;
};

class CHighScore : public AbstractState
{
	sScore scores[10];

	int m_nImageID[2];
	int m_nCursorPosition;

	fstream fs;

	CBitmapFont	m_Font;
	CBitmapFont m_Font2;

	//	Default constructor
	CHighScore(void);
	//	Copy constructor.
	CHighScore(const CHighScore& ref) { }
	//	Assignment Operator.
	CHighScore& operator=(const CHighScore& ref) { }

	//	Destructor
	~CHighScore(void);
public:

	static CHighScore* GetInstance(void);

	void Enter(void);					// This method initializes the game entities

	void Update(float fElapsedTime);
	void Render(void);

	void Exit(void);					// This method does any necessary clean-up

};
