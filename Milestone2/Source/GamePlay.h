/////////////////////////////////////////////////////////////////////////
//	File	:	"GamePlay.h"
//
//	Author	:	John Albright (JA)
//
//	Purpose	:	Defines any code specific towards you in gameplay mode
/////////////////////////////////////////////////////////////////////////

#pragma once
#include "AbstractState.h"
#include "CBase.h"
#include "CBitmapFont.h"
#include "IListener.h"
#include <vector>
#include <queue>

class CGamePlay : public AbstractState, public IListener
{

	//	Default constructor
	CGamePlay(void);
	//	Copy constructor.
	CGamePlay(const CGamePlay& ref) { }
	//	Assignment Operator.
	CGamePlay& operator=(const CGamePlay& ref) { }
	//	Destructor
	~CGamePlay(void);

	int m_nImageID[5];
	int m_nBGID;
	int m_nBGMusicID;

	float stateTimer;

	float m_nBridgeOffset;
	float m_nSpaceOffset;
	int m_nLevel;
	int m_nEnemyCounter;
	int m_nRemainingEnemies;
	int m_nTotalSpawns;
	float m_nBridgeOffsetY;
	float m_nBridgeVelocityY;

	float m_fFinishedTimer;
	float m_fSpawnPowerupTimer;

	CBase *m_pPlayerCharacter;
	CBase *m_pBullet;
	std::vector<CBase*> enemies;
	std::queue<char> m_cPreviousKeys;

	CBitmapFont m_Font;
	CBitmapFont m_Font2;
public:
	static CGamePlay* GetInstance(void);

	void Enter(void);					// This method initializes the game entities

	void Update(float fElapsedTime);
	void Render(void);

	void Exit(void);					// This method does any necessary clean-up

	CBase* GetEnemy(int i) { return enemies[i]; }

	int GetNumEnemies(void) { return enemies.size(); }

	int GetBridgeOffset(void) { return m_nBridgeOffset; }
	void SetBridgeOffset(int offset) { m_nBridgeOffset = offset; }

	float GetSpaceOffset(void) { return m_nSpaceOffset; }
	void SetSpaceOffset(float offset) { m_nSpaceOffset = offset; }

	int GetLevel(void) { return m_nLevel; }
	void SetLevel(int level) { m_nLevel = level; }

	int GetEnemyCounter(void) { return m_nEnemyCounter; }
	void SetEnemyCounter(int counter) { m_nEnemyCounter = counter; }

	int GetRemainingEnemies(void) { return m_nRemainingEnemies; }
	void SetRemainingEnemies(int remaining) { m_nRemainingEnemies = remaining; }

	float GetBridgeVelocity(void) { return m_nBridgeVelocityY; }
	void SetBridgeVelocity(float velocity) { m_nBridgeVelocityY = velocity; }

	void CheckForCheats(int cheatCheck);

	void HandleEvent(CEvent* pEvent);

};
