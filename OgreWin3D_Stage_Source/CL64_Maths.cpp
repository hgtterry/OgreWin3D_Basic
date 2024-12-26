/*
Copyright (c) 2024 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OgreWin3D_Stage

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "pch.h"
#include "CL64_App.h"
#include "CL64_Maths.h"

CL64_Maths::CL64_Maths()
{
}

CL64_Maths::~CL64_Maths()
{
}

// *************************************************************************
// *			Vector3_Set:- Terry and Hazel Flanigan 2024			 	   *
// *************************************************************************
void CL64_Maths::Vector3_Set(Ogre::Vector3* V, float X, float Y, float Z)
{
	V->x = X;
	V->y = Y;
	V->z = Z;
}

// *************************************************************************
// *			Vector3_Add:- Terry and Hazel Flanigan 2024			 	   *
// *************************************************************************
void CL64_Maths::Vector3_Add(const Ogre::Vector3* V1, const Ogre::Vector3* V2, Ogre::Vector3* V1PlusV2)
{
	V1PlusV2->x = V1->x + V2->x;
	V1PlusV2->y = V1->y + V2->y;
	V1PlusV2->z = V1->z + V2->z;
}

// *************************************************************************
// *		Vector3_Subtract:- Terry and Hazel Flanigan 2024		 	   *
// *************************************************************************
void CL64_Maths::Vector3_Subtract(const Ogre::Vector3* V1, const Ogre::Vector3* V2, Ogre::Vector3* V1MinusV2)
{

	if (V1MinusV2 == NULL)
	{
		return;
	}

	V1MinusV2->x = V1->x - V2->x;
	V1MinusV2->y = V1->y - V2->y;
	V1MinusV2->z = V1->z - V2->z;
}

// *************************************************************************
// *			Vector3_Scale:- Terry and Hazel Flanigan 2024		 	   *
// *************************************************************************
void CL64_Maths::Vector3_Scale(const Ogre::Vector3* VSrc, float Scale, Ogre::Vector3* VDst)
{
	VDst->x = VSrc->x * Scale;
	VDst->y = VSrc->y * Scale;
	VDst->z = VSrc->z * Scale;
}

// *************************************************************************
// *			Vector3_Normalize:- Terry and Hazel Flanigan 2024	 	   *
// *************************************************************************
float CL64_Maths::Vector3_Normalize(Ogre::Vector3* V1)
{
	float OneOverDist;
	float Dist;

	Dist = (float)sqrt(Vector3_DotProduct(V1, V1));

	if (Dist == 0.0)
		return 0.0f;
	OneOverDist = 1.0f / Dist;

	V1->x *= OneOverDist;
	V1->y *= OneOverDist;
	V1->z *= OneOverDist;

	return Dist;
}

// *************************************************************************
// *		Vector3_DotProduct:- Terry and Hazel Flanigan 2024		 	   *
// *************************************************************************
float CL64_Maths::Vector3_DotProduct(const Ogre::Vector3* V1, const Ogre::Vector3* V2)
{
	return(V1->x * V2->x + V1->y * V2->y + V1->z * V2->z);
}

// *************************************************************************
// *			Vector3_Copy:- Terry and Hazel Flanigan 2024		 	   *
// *************************************************************************
void CL64_Maths::Vector3_Copy(const Ogre::Vector3* VSrc, Ogre::Vector3* VDst)
{
	*VDst = *VSrc;
}

// *************************************************************************
// *			Vector3_Clear:- Terry and Hazel Flanigan 2024		 	   *
// *************************************************************************
void CL64_Maths::Vector3_Clear(Ogre::Vector3* V)
{
	V->x = 0.0f;
	V->y = 0.0f;
	V->z = 0.0f;
}
