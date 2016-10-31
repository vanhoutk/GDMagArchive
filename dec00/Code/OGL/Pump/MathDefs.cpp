///////////////////////////////////////////////////////////////////////////////
//
// MathDefs.c : implementation file
//
// Purpose:	Implementation of Math Routines
//
// Created:
//		JL  2/18/98
// Revisions:
//		JL 9/7/00	Changes to add volume preservation technique
//
///////////////////////////////////////////////////////////////////////////////
//
//	Copyright 2000 Jeff Lander, All Rights Reserved.
//  For educational purposes only.
//  Please do not republish in electronic or print form without permission
//  Thanks - jeffl@darwin3d.com
//
///////////////////////////////////////////////////////////////////////////////

#include <math.h>
#include "mathdefs.h"

///////////////////////////////////////////////////////////////////////////////
// Function:	IdentityMatrix
// Purpose:		Creates and Identity 4x4 Matrix in OpenGL Format
// Arguments:	Matrix
// Notes:		This routing is tweaked to handle OpenGLs column-major format
//				This is one obvious place for optimization perhaps asm code
///////////////////////////////////////////////////////////////////////////////
void IdentityMatrix(tMatrix *mat) 
{
///// Local Variables /////////////////////////////////////////////////////////
	int loop;
///////////////////////////////////////////////////////////////////////////////
	for (loop = 0; loop < 16; loop++)
		mat->m[loop] = 0.0f;
	mat->m[0] = 
	mat->m[5] = 
	mat->m[10] = 
	mat->m[15] = 
		1.0f;
}
//// IdentityMatrix ///////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// tVector Structure
//////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Function:	Constructor
// Purpose:		Initializes a tVector
///////////////////////////////////////////////////////////////////////////////
tVector::tVector()
{
	x = y = z = 0.0f;
}

///////////////////////////////////////////////////////////////////////////////
// Function:	Set
// Purpose:		Sets the current vector to float list
// Arguments:	xyz floats 
///////////////////////////////////////////////////////////////////////////////
void tVector::Set( float fX, float fY, float fZ )
{
	x = fX;
	y = fY;
	z = fZ;
}

///////////////////////////////////////////////////////////////////////////////
// Function:	equals operator
// Purpose:		Sets the current vector to equal tVector
// Arguments:	tVector 
///////////////////////////////////////////////////////////////////////////////
tVector tVector::operator=( tVector arg )
{
	x = arg.x;
	y = arg.y;
	z = arg.z;
	
	return *this;
}

///////////////////////////////////////////////////////////////////////////////
// Function:	equals operator
// Purpose:		Sets the current vector to equal list of floats
// Arguments:	float * 
///////////////////////////////////////////////////////////////////////////////
tVector tVector::operator=( float *arg )
{
	x = arg[0];
	y = arg[1];
	z = arg[2];
	
	return *this;
}

///////////////////////////////////////////////////////////////////////////////
// Function:	equals operator
// Purpose:		Sets the current vector to equal a float
// Arguments:	float * 
///////////////////////////////////////////////////////////////////////////////
tVector tVector::operator=( float arg )
{
	x = arg;
	y = arg;
	z = arg;
	
	return *this;
}
///////////////////////////////////////////////////////////////////////////////
// Function:	addition operator
// Purpose:		Adds a tVector to the current vector
// Arguments:	tVector 
///////////////////////////////////////////////////////////////////////////////
tVector tVector::operator+( tVector arg )
{
	tVector v;

	v.x = x + arg.x;
	v.y = y + arg.y;
	v.z = z + arg.z;

	return v;
}

///////////////////////////////////////////////////////////////////////////////
// Function:	addition operator
// Purpose:		Adds a tVector to the current vector
// Arguments:	tVector 
///////////////////////////////////////////////////////////////////////////////
tVector tVector::operator+=( tVector arg )
{
	x += arg.x;
	y += arg.y;
	z += arg.z;

	return *this;
}

///////////////////////////////////////////////////////////////////////////////
// Function:	subtraction operator
// Purpose:		Subtracts a tVector from the current vector
// Arguments:	tVector 
///////////////////////////////////////////////////////////////////////////////
tVector tVector::operator-( tVector arg )
{
	tVector v;

	v.x = x - arg.x;
	v.y = y - arg.y;
	v.z = z - arg.z;

	return v;
}

///////////////////////////////////////////////////////////////////////////////
// Function:	subtraction operator
// Purpose:		Subtracts a tVector from the current vector
// Arguments:	tVector 
///////////////////////////////////////////////////////////////////////////////
tVector tVector::operator-=( tVector v )
{
	x -= v.x;
	y -= v.y;
	z -= v.z;

	return *this;
}

///////////////////////////////////////////////////////////////////////////////
// Function:	multiplication operator
// Purpose:		Multiply a tVector by float
// Arguments:	float
///////////////////////////////////////////////////////////////////////////////
tVector tVector::operator*( float arg )
{
	tVector v;

	v.x = x * arg;
	v.y = y * arg;
	v.z = z * arg;

	return v;
}

///////////////////////////////////////////////////////////////////////////////
// Function:	division operator
// Purpose:		Divides a tVector by float
// Arguments:	float
///////////////////////////////////////////////////////////////////////////////
tVector tVector::operator/( float arg )
{
	tVector v;

	v.x = x / arg;
	v.y = y / arg;
	v.z = z / arg;

	return v;
}

///////////////////////////////////////////////////////////////////////////////
// Function:	multiplication operator
// Purpose:		Multiply a tVector by float
// Arguments:	float
///////////////////////////////////////////////////////////////////////////////
tVector tVector::operator*=( float arg )
{
	tVector v;

	x *= arg;
	y *= arg;
	z *= arg;

	return *this;
}

///////////////////////////////////////////////////////////////////////////////
// Function:	division operator
// Purpose:		Divides a tVector by float
// Arguments:	float
///////////////////////////////////////////////////////////////////////////////
tVector tVector::operator/=( float arg )
{
	tVector v;

	x /= arg;
	y /= arg;
	z /= arg;

	return *this;
}
///////////////////////////////////////////////////////////////////////////////
// Function:	VectorSquaredDistance
// Purpose:		Returns the squared distance between two vector
// Arguments:	tVector *
///////////////////////////////////////////////////////////////////////////////
float tVector::VectorSquaredDistance(tVector *v) 
{
	return(	((x - v->x) * (x - v->x)) + 
			((y - v->y) * (y - v->y)) + 	
			((z - v->z) * (z - v->z)) ); 	
}

///////////////////////////////////////////////////////////////////////////////
// Function:	VectorDistance
// Purpose:		Returns the distance between two vector
// Arguments:	tVector *
///////////////////////////////////////////////////////////////////////////////
float tVector::VectorDistance(tVector *v) 
{
	return((float)sqrt(VectorSquaredDistance(v)));
}


///////////////////////////////////////////////////////////////////////////////
// Function:	SquaredLength
// Purpose:		Returns the squared length of the vector
// Arguments:	none
///////////////////////////////////////////////////////////////////////////////
float tVector::SquaredLength() 
{
	return((x * x) + (y * y) + (z * z));
}

///////////////////////////////////////////////////////////////////////////////
// Function:	Length
// Purpose:		Returns the length of the vector
// Arguments:	none
///////////////////////////////////////////////////////////////////////////////
float tVector::Length() 
{
	return((float)sqrt(SquaredLength()));
}

///////////////////////////////////////////////////////////////////////////////
// Function:	NormalizeVector
// Purpose:		Normalizes the class vector
// Arguments:	none
///////////////////////////////////////////////////////////////////////////////
void tVector::NormalizeVector() 
{
	float len = Length();
    if (len != 0.0) 
	{ 
		x /= len;  
		y /= len; 
		z /= len; 
	}
}

///////////////////////////////////////////////////////////////////////////////
// Function:	Lerp
// Purpose:		Interpolates between vector and another by a factor
// Arguments:	Vector to lerp to and factor
///////////////////////////////////////////////////////////////////////////////
void tVector::Lerp(tVector *v1, float factor) 
{
	x = x * (1.0 - factor) + v1->x * factor; 
	y = y * (1.0 - factor) + v1->y * factor; 
	z = z * (1.0 - factor) + v1->z * factor; 
}

///////////////////////////////////////////////////////////////////////////////
// Function:	CrossProduct
// Purpose:		Sets the vector to equal the cross product of two vectors
// Arguments:	tVector *
///////////////////////////////////////////////////////////////////////////////
void tVector::CrossProduct(tVector *v1, tVector *v2)
{
	x = (v1->y * v2->z) - (v1->z * v2->y);
	y = (v1->z * v2->x) - (v1->x * v2->z);
	z = (v1->x * v2->y) - (v1->y * v2->x);
}

///////////////////////////////////////////////////////////////////////////////
// Function:	DotProduct
// Purpose:		Return the dot product of two vectors
// Arguments:	tVector *
///////////////////////////////////////////////////////////////////////////////
float tVector::DotProduct(tVector *v)
{
	return ((x * v->x) + (y * v->y) + (z * v->z));
}

///////////////////////////////////////////////////////////////////////////////
// Function:	DotProduct
// Purpose:		Return the dot product of two vectors
// Arguments:	vector elements
///////////////////////////////////////////////////////////////////////////////
float tVector::DotProduct(float vx, float vy, float vz)
{
	return ((x * vx) + (y * vy) + (z * vz));
}

///////////////////////////////////////////////////////////////////////////////
// Function:	MultVectorByMatrix
// Purpose:		Multiplies a vector by a 4x4 Matrix in OpenGL Format
// Arguments:	Matrix
// Notes:		This routing is tweaked to handle OpenGLs column-major format
//				This is one obvious place for optimization perhaps asm code
///////////////////////////////////////////////////////////////////////////////
void tVector::MultVectorByMatrix(float *mat)
{
	tVector result;
	result.x = (mat[0] * x) +
			   (mat[4] * y) +	
			   (mat[8] * z) +
			   mat[12];
	result.y = (mat[1] * x) +
			   (mat[5] * y) +	
			   (mat[9] * z) +
			   mat[13];
	result.z = (mat[2] * x) +
			   (mat[6] * y) +	
			   (mat[10] * z) +
			   mat[14];

	x = result.x;
	y = result.y;
	z = result.z;
}
//// MultVectorByMatrix //////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Function:	MultVectorByRotMatrix
// Purpose:		Multiplies a vector by a rotation only 4x4 Matrix in OpenGL Format
// Arguments:	Matrix
// Notes:		This routing is tweaked to handle OpenGLs column-major format
//				This is one obvious place for optimization perhaps asm code
///////////////////////////////////////////////////////////////////////////////
void tVector::MultVectorByRotMatrix(float *mat)
{
	tVector result;
	result.x = (mat[0] * x) +
			   (mat[4] * y) +	
			   (mat[8] * z);
	result.y = (mat[1] * x) +
			   (mat[5] * y) +	
			   (mat[9] * z);
	result.z = (mat[2] * x) +
			   (mat[6] * y) +	
			   (mat[10] * z);

	x = result.x;
	y = result.y;
	z = result.z;
}
//// MultVectorByRotMatrix //////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// End of tVector Structure
//////////////////////////////////////////////////////////////////////

