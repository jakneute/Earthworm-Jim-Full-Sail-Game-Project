//////////////////////////////////////////////////////////////////////////
//	File	:	"SGD_Math.cpp"
//
//	Author	:	David Brown (DB)
//
//	Purpose	:	To do lots of math for us.
///////////////////////////////////////////////////////////////////////////
#include "SGD_Math.h"
#include <math.h>	//	for sqrt()
#include <float.h>	//	for _isnan()

//	Find the length of a vector (magnitude)
float Vector2DLength(tVector2D vec)
{
	//	A^2 + B^2 = C^2
	// C = sqrt(A^2 + B^2)
	return sqrt( (vec.fX * vec.fX) + (vec.fY * vec.fY) );
}

// Find the dot product
float DotProduct(tVector2D v1, tVector2D v2)
{
	//	(a.x * b.x) + (a.y * b.y)
	return (v1.fX * v2.fX) + (v1.fY * v2.fY);
}

//	Normalize a vector
tVector2D Vector2DNormalize(tVector2D vec)
{
	tVector2D vNormalized;

	float fLength = Vector2DLength(vec);

	vNormalized.fX = vec.fX / fLength;
	vNormalized.fY = vec.fY / fLength;

	//	Return the normalized vector
	return vNormalized;
}

//	Rotate a vector
/*
	X, Y	=	[X]
				[Y]
				[1]

	[	cos&	sin&	0	]		[X]		[X1]
	[  -sin&	cos&	0	]	*	[Y]	=	[Y1]
	[	0		0		1	]		[1]		[1]
*/
tVector2D Vector2DRotate(tVector2D vec, float fRadians)
{
	//	Convert into the Cartesian coordinate system
	vec.fY *= -1.0f;

	tVector2D vRotated;
	vRotated.fX = (cos(fRadians) * vec.fX) + (sin(fRadians) * vec.fY);
	vRotated.fY = (-sin(fRadians) * vec.fX) + (cos(fRadians) * vec.fY);

	//	Convert back to our windows coordinate system
	vRotated.fY *= -1.0f;

	//	Return our rotated vector
	return vRotated;
}

//	Find the angle between 2 vectors
float AngleBetweenVectors(tVector2D v1, tVector2D v2)
{
	//	Dot product
	float fDot = DotProduct(v1, v2);
	//	Length of those vectors
	float fLength =  Vector2DLength(v1) * Vector2DLength(v2);

	//	Make sure we don't get a divide by zero error.
	if (fLength == 0.0f)	return 0.0f;

	//	Calculate the angle between the two vectors
	float fAngle = acos( fDot / fLength );

	//	Make sure the number is not indefinite.  Shows up as -1.#IND00000.  Not a number.
	if (_isnan(fAngle))
		return 0.0f;

	//	Return the angle.
	return fAngle;
}