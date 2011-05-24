//////////////////////////////////////////////////////////////////////////
//	File	:	"SGD_Math.h"
//
//	Author	:	David Brown (DB)
//
//	Purpose	:	To do lots of math for us.
///////////////////////////////////////////////////////////////////////////
#pragma once

const float SGD_PI = 3.141592653589732f;

//	Define point	-	used for positions
struct tPoint2D
{
	float fX;
	float fY;
};

//	Define vector	-	used for movement or calculations
struct tVector2D
{
	float fX;
	float fY;
};

//	Find the length of a vector (magnitude)
float Vector2DLength(tVector2D vec);

// Find the dot product
float DotProduct(tVector2D v1, tVector2D v2);

//	Normalize a vector
tVector2D Vector2DNormalize(tVector2D vec);

//	Rotate a vector
tVector2D Vector2DRotate(tVector2D vec, float fRadians);

//	Find the angle between 2 vectors
float AngleBetweenVectors(tVector2D v1, tVector2D v2);