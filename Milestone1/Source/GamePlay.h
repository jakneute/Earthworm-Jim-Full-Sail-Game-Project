#pragma once
#include "AbstractState.h"
#include "CBase.h"
#include "CBitmapFont.h"
#include <vector>

class CGamePlay : public AbstractState
{
	int m_nImageID[3];
	int m_nBGID;
	int m_nBGMusicID;
	bool bIsPaused;
	bool bEnabledCheats[4];

	CBase *m_pPlayerCharacter;
	CBase *m_pBullet;
	std::vector<CBase*> enemies;

	CBitmapFont m_Font;
public:
	CGamePlay(void);
	~CGamePlay(void);

	static CGamePlay* GetInstance(void);

	void Enter(void);					// This method initializes the game entities

	void Update(float fElapsedTime);
	void Render(void);

	void Exit(void);					// This method does any necessary clean-up

	bool GetPaused(void) { return bIsPaused; }

	CBase* GetEnemy(int i) { return enemies[i]; }

	int GetNumEnemies(void) { return enemies.size(); }

};
