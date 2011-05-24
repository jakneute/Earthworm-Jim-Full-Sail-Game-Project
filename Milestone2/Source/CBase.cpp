/////////////////////////////////////////////////////////////////////////
//	File	:	"CBase.cpp"
//
//	Author	:	John Albright (JA)
//
//	Purpose	:	The base object for our class hierarchy and implements
//				our base interface.
/////////////////////////////////////////////////////////////////////////
#include "CBase.h"
#include "CSGD_TextureManager.h"
#include "CSGD_Direct3D.h"

CBase::CBase(void)
{
	SetImageID(-1);
	SetVelX(0.0f);
	SetVelY(0.0f);
	SetWidth(0);
	SetHeight(0);

	m_unRefCount = 1;
}

CBase::~CBase(void)
{

}

RECT CBase::GetCollisionRect(void)
{
	RECT rCollision;
	rCollision.left		= GetPosX();
	rCollision.top		= GetPosY();
	rCollision.right	= rCollision.left + GetWidth();
	rCollision.bottom	= rCollision.top + GetHeight();

	return rCollision;
}

void CBase::SetCollisionRect(RECT rCollision)
{
}

void CBase::Update(float fElapsedTime)
{
	SetPosX( GetPosX() + GetVelX() * fElapsedTime );
	SetPosY( GetPosY() + GetVelY() * fElapsedTime );
}

void CBase::Render(void)
{
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();

	pTM->Draw( GetImageID(), GetPosX() - m_nWidth/2, GetPosY() - m_nHeight/2, 
		1.0f, 1.0f, NULL, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(128, 255, 255, 255));

	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();

	tVector2D tVisualize;
	tVisualize.fX = GetVelX();
	tVisualize.fY = GetVelY();

	tVisualize = Vector2DNormalize(tVisualize);
	tVisualize.fX *= 32.0f;
	tVisualize.fY *= 32.0f;

	pD3D->DrawLine(GetPosX(), GetPosY(), GetPosX() + tVisualize.fX, GetPosY() + tVisualize.fY,
						255, 0, 0);
}