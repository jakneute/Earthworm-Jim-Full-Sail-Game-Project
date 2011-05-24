/////////////////////////////////////////////////////////////////////////
//	File	:	"CBitmapFont.cpp"
//
//	Author	:	John Albright (JA)
//
//	Purpose	:	Sets up the bitmap fonts used
/////////////////////////////////////////////////////////////////////////

#include "CBitmapFont.h"
#include "CSGD_TextureManager.h"

CBitmapFont::CBitmapFont(void)
{
	SetFontImageID(-1);
	SetNumCols(0);
	SetCharWidth(0);
	SetCharHeight(0);
}

RECT CBitmapFont::CalculateRect(int id)
{
	RECT rCell;
	rCell.left	= (id % GetNumCols()) * GetCharWidth();
	rCell.top	= (id / GetNumCols()) * GetCharHeight();

	rCell.right		= rCell.left + GetCharWidth();
	rCell.bottom	= rCell.top + GetCharHeight();

	return rCell;
}

void CBitmapFont::DrawString(const char* szText, int nPosX, int nPosY)
{
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();

	//	iterate through the string 1 character at a time
	int length = (int)strlen(szText);

	for (int i=0; i < length; i++)
	{
		//	get ascii value of character
		char ch = szText[i];

		//	make sure character is uppercase
		ch = toupper(ch);

		//	calculate the id on the bitmap using the start char
		int id = ch - GetStartChar();

		//	Make a rect based on an ID
		RECT rLetter = CalculateRect(id);

		//	Draw it to the screen
		pTM->Draw(GetFontImageID(), nPosX + (i*GetCharWidth()), nPosY, 1.0f, 1.0f, &rLetter);
	}
}