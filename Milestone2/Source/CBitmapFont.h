//////////////////////////////////////////////////////////////////////////////
//	Function	:	"CBitmapFont.h"
//
//	Author		:	John Albright (JA)
//
//	Purpose		:	Defines the bitmap font used
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <windows.h>	//	for RECT

class CBitmapFont
{
private:
	int			m_nFontImageID;
	int			m_nNumCols;
	int			m_nCharacterWidth;
	int			m_nCharacterHeight;

	char		m_cStartChar;		//	first char on the bitmap font sheet

	//	Calculate the RECT on the sheet based on an id.
	RECT CalculateRect(int id);

public:

	CBitmapFont(void);

	inline	int GetFontImageID(void)	{	return m_nFontImageID;	}
	inline	int	GetNumCols(void)		{	return m_nNumCols; }
	inline	int GetCharWidth(void)		{	return m_nCharacterWidth; }
	inline	int GetCharHeight(void)		{	return m_nCharacterHeight; }
	inline	char GetStartChar(void)		{	return m_cStartChar; }
	
	inline	void SetFontImageID(int nImageID)	{	m_nFontImageID = nImageID; }
	inline	void SetNumCols(int nNumCols)		{	m_nNumCols = nNumCols; }
	inline	void SetCharWidth(int nWidth)		{	m_nCharacterWidth = nWidth; }
	inline	void SetCharHeight(int nHeight)		{	m_nCharacterHeight = nHeight; }
	inline	void SetStartChar(char ch)			{	m_cStartChar = ch;	}

	void DrawString(const char* szText, int nPosX, int nPosY);
};