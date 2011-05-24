//////////////////////////////////////////////////////////////////////////////
//	Function	:	"CDerived.cpp"
//
//	Author		:	David Brown (DB)
//
//	Purpose		:	To define an object that will bounce around the screen.
//////////////////////////////////////////////////////////////////////////////
#include "Enemy.h"
#include "CSGD_TextureManager.h"
#include "CBullet.h"
#include "MessageTypes.h"
#include "CSGD_MessageSystem.h"
#include "CSGD_ObjectManager.h"
#include "CDestroyMessage.h"

CEnemy::CEnemy()
{
	SetHP(10);
	
}

CEnemy::~CEnemy()
{
}

void CEnemy::Update(float fElapsedTime)
{
	CSGD_ObjectManager *pOM = CSGD_ObjectManager::GetInstance();
	CSGD_MessageSystem *pMS = CSGD_MessageSystem::GetInstance();

	rEnemyRect.left = GetPosX();
	rEnemyRect.right = GetPosX() + GetWidth();
	rEnemyRect.top = GetPosY();
	rEnemyRect.bottom = GetPosY() + GetHeight();
	
	SetCollisionRect(rEnemyRect);
		

}

void CEnemy::Render(void)
{
	CSGD_TextureManager *pTM = CSGD_TextureManager::GetInstance();

	if(GetHP() > 0)
		pTM->Draw(GetImageID(),GetPosX(),GetPosY(),1,1,NULL,GetWidth() / 2, GetHeight() /2,3.14f,D3DCOLOR_XRGB(255,255,255));
}

void CEnemy::HandleEvent(CEvent *pEvent)
{
}