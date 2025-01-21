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
// *			Ram_Allocate										 	   *
// *************************************************************************
void* CL64_Maths::Ram_Allocate(Ogre::int32 size)
{
	void* p;

	//do
	//{
		p = malloc(size);
	//} while ((p == NULL) && (geRam_DoCriticalCallback()));

	return p;
}

//static char* ram_verify_block
//(
//	void* ptr
//)
//{
//	char* p = ptr;
//	uint32 size;
//
//	if (p == NULL)
//	{
//		assert(0 && "freeing NULL");
//		return NULL;
//	}
//
//	// make p point to the beginning of the block
//	p -= HEADER_SIZE;
//
//	// get size from block
//	size = *((uint32*)p);
//
//	// check stamp at front
//	if (memcmp(p + SizeSize, MemStamp, MemStampSize) != 0)
//	{
//		assert(0 && "ram_verify_block:  Memory block corrupted at front");
//		return NULL;
//	}
//
//	// and at back
//	if (memcmp(p + HEADER_SIZE + size, MemStamp, MemStampSize) != 0)
//	{
//		assert(0 && "ram_verify_block:  Memory block corrupted at tail");
//		return NULL;
//	}
//
//	return p;
//}

// *************************************************************************
// *				Ram_Free										 	   *
// *************************************************************************
void CL64_Maths::Ram_Free(void* ptr)
{
	//char* p;
	//Ogre::uint32 size;

	//// make sure it's a valid block...
	//p = ram_verify_block(ptr);
	//if (p == NULL)
	//{
	//	return;
	//}

	//// gotta get the size before you free it
	//size = *((Ogre::uint32*)p);

	//// fill it with trash...
	//memset(p, FreeFillerByte, size + EXTRA_SIZE);

	//// free the memory
	//free(p);

	//// update allocations
	//geRam_NumberOfAllocations--;
	//assert((geRam_NumberOfAllocations >= 0) && "free()d more ram than you allocated!");

	//geRam_CurrentlyUsed -= size;
	//assert((geRam_CurrentlyUsed >= 0) && "free()d more ram than you allocated!");
}

// *************************************************************************
// *							Vector3_Set								   *
// *************************************************************************
void CL64_Maths::Vector3_Set(T_Vec3* V, float X, float Y, float Z)
{
	V->x = X;
	V->y = Y;
	V->z = Z;
}

// *************************************************************************
// *								Vector3_Add						 	   *
// *************************************************************************
void CL64_Maths::Vector3_Add(const T_Vec3* V1, const T_Vec3* V2, T_Vec3* V1PlusV2)
{
	V1PlusV2->x = V1->x + V2->x;
	V1PlusV2->y = V1->y + V2->y;
	V1PlusV2->z = V1->z + V2->z;
}

// *************************************************************************
// *						Vector3_Subtract							   *
// *************************************************************************
void CL64_Maths::Vector3_Subtract(const T_Vec3* V1, const T_Vec3* V2, T_Vec3* V1MinusV2)
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
// *							Vector3_Scale						 	   *
// *************************************************************************
void CL64_Maths::Vector3_Scale(const T_Vec3* VSrc, float Scale, T_Vec3* VDst)
{
	VDst->x = VSrc->x * Scale;
	VDst->y = VSrc->y * Scale;
	VDst->z = VSrc->z * Scale;
}

// *************************************************************************
// *							Vector3_Normalize					 	   *
// *************************************************************************
float CL64_Maths::Vector3_Normalize(T_Vec3* V1)
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
// *						Vector3_DotProduct						 	   *
// *************************************************************************
float CL64_Maths::Vector3_DotProduct(const T_Vec3* V1, const T_Vec3* V2)
{
	return(V1->x * V2->x + V1->y * V2->y + V1->z * V2->z);
}

// *************************************************************************
// *							Vector3_Copy						 	   *
// *************************************************************************
void CL64_Maths::Vector3_Copy(const T_Vec3* VSrc, T_Vec3* VDst)
{
	*VDst = *VSrc;
}

// *************************************************************************
// *							Vector3_Clear						 	   *
// *************************************************************************
void CL64_Maths::Vector3_Clear(T_Vec3* V)
{
	V->x = 0.0f;
	V->y = 0.0f;
	V->z = 0.0f;
}

// *************************************************************************
// *						Vector3_CrossProduct					 	   *
// *************************************************************************
void CL64_Maths::Vector3_CrossProduct(const T_Vec3* V1, const T_Vec3* V2, T_Vec3* VResult)
{
	T_Vec3 Result;

	Result.x = V1->y * V2->z - V1->z * V2->y;
	Result.y = V1->z * V2->x - V1->x * V2->z;
	Result.z = V1->x * V2->y - V1->y * V2->x;

	*VResult = Result;
}

// *************************************************************************
// *							Vector3_Compare							   *
// *************************************************************************
signed int CL64_Maths::Vector3_Compare(const T_Vec3* V1, const T_Vec3* V2, float Tolerance)
{

	if (fabs(V2->x - V1->x) > Tolerance)
		return false;
	if (fabs(V2->y - V1->y) > Tolerance)
		return false;
	if (fabs(V2->z - V1->z) > Tolerance)
		return false;

	return true;
}

// *************************************************************************
// *			Vector3_Compare:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Maths::Vector3_Inverse(T_Vec3* V)
{
	V->x = -V->x;
	V->y = -V->y;
	V->z = -V->z;
}

// *************************************************************************
// *			Vector3_AddScaled:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Maths::Vector3_AddScaled(const T_Vec3* V1, const T_Vec3* V2, float Scale, T_Vec3* V1PlusV2Scaled)
{
	V1PlusV2Scaled->x = V1->x + V2->x * Scale;
	V1PlusV2Scaled->y = V1->y + V2->y * Scale;
	V1PlusV2Scaled->z = V1->z + V2->z * Scale;
}

// *************************************************************************
// *				Vector3_MA:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Maths::Vector3_MA(T_Vec3* V1, float Scale, const T_Vec3* V2, T_Vec3* V1PlusV2Scaled)
{
	assert(geVec3d_IsValid(V1) != GE_FALSE);
	assert(geVec3d_IsValid(V2) != GE_FALSE);
	assert(V1PlusV2Scaled != NULL);

	V1PlusV2Scaled->x = V1->x + V2->x * Scale;
	V1PlusV2Scaled->y = V1->y + V2->y * Scale;
	V1PlusV2Scaled->z = V1->z + V2->z * Scale;
}

// *************************************************************************
// *			XForm3d_SetIdentity:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Maths::XForm3d_SetIdentity(Matrix3d* M)
{
	assert(M != NULL);

	M->AX = M->BY = M->CZ = 1.0f;
	M->AY = M->AZ = M->BX = M->BZ = M->CX = M->CY = 0.0f;
	M->Translation.x = M->Translation.y = M->Translation.z = 0.0f;
}

// *************************************************************************
// *			XForm3d_RotateX:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Maths::XForm3d_RotateX(Matrix3d* M, float RadianAngle)
{
	Matrix3d R;
	
	//geXForm3d_Assert(geXForm3d_IsOrthogonal(M) == GE_TRUE);

	XForm3d_SetXRotation(&R, RadianAngle);
	XForm3d_Multiply(&R, M, M);
	//XForm3d_Assert(geXForm3d_IsOrthogonal(M) == GE_TRUE);
}

// *************************************************************************
// *			XForm3d_RotateX:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Maths::XForm3d_SetXRotation(Matrix3d* M, float RadianAngle)
{
	float Cos, Sin;
	assert(M != NULL);
	assert(RadianAngle * RadianAngle >= 0.0f);

	Cos = (float)cos(RadianAngle);
	Sin = (float)sin(RadianAngle);
	M->BY = Cos;
	M->BZ = -Sin;
	M->CY = Sin;
	M->CZ = Cos;
	M->AX = 1.0f;
	M->AY = M->AZ = M->BX = M->CX = 0.0f;
	M->Translation.x = M->Translation.y = M->Translation.z = 0.0f;
}

// *************************************************************************
// *		XForm3d_SetYRotation:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Maths::XForm3d_SetYRotation(Matrix3d* M, float RadianAngle)
{
	geFloat Cos, Sin;
	assert(M != NULL);
	assert(RadianAngle * RadianAngle >= 0.0f);

	Cos = (geFloat)cos(RadianAngle);
	Sin = (geFloat)sin(RadianAngle);

	M->AX = Cos;
	M->AZ = Sin;
	M->CX = -Sin;
	M->CZ = Cos;
	M->BY = 1.0f;
	M->AY = M->BX = M->BZ = M->CY = 0.0f;
	M->Translation.x = M->Translation.y = M->Translation.z = 0.0f;
}

// *************************************************************************
// *		XForm3d_SetZRotation:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Maths::XForm3d_SetZRotation(Matrix3d* M, float RadianAngle)
{
	geFloat Cos, Sin;
	assert(M != NULL);
	assert(RadianAngle * RadianAngle >= 0.0f);

	Cos = (geFloat)cos(RadianAngle);
	Sin = (geFloat)sin(RadianAngle);

	M->AX = Cos;
	M->AY = -Sin;
	M->BX = Sin;
	M->BY = Cos;
	M->CZ = 1.0f;
	M->AZ = M->BZ = M->CX = M->CY = 0.0f;
	M->Translation.x = M->Translation.y = M->Translation.z = 0.0f;
}

// *************************************************************************
// *		geXForm3d_Rotate:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Maths::XForm3d_Rotate(const Matrix3d* M, const T_Vec3* V, T_Vec3* Result)
{
	T_Vec3 VL;

	VL = *V;

	Result->x = (VL.x * M->AX) + (VL.y * M->AY) + (VL.z * M->AZ);
	Result->y = (VL.x * M->BX) + (VL.y * M->BY) + (VL.z * M->BZ);
	Result->z = (VL.x * M->CX) + (VL.y * M->CY) + (VL.z * M->CZ);
}

// *************************************************************************
// *						geXForm3d_Transform							   *
// *************************************************************************
void CL64_Maths::XForm3d_Transform(const Matrix3d* M, const T_Vec3* V, T_Vec3* Result)
{
	T_Vec3 VL;

	VL = *V;

	Result->x = (VL.x * M->AX) + (VL.y * M->AY) + (VL.z * M->AZ) + M->Translation.x;
	Result->y = (VL.x * M->BX) + (VL.y * M->BY) + (VL.z * M->BZ) + M->Translation.y;
	Result->z = (VL.x * M->CX) + (VL.y * M->CY) + (VL.z * M->CZ) + M->Translation.z;
}

// *************************************************************************
// *							XForm3d_Multiply						   *
// *************************************************************************
void CL64_Maths::XForm3d_Multiply(const Matrix3d* M1,const Matrix3d* M2, Matrix3d* MProduct)
{
	Matrix3d M1L;
	Matrix3d M2L;

	//XForm3d_Assert(geXForm3d_IsOrthogonal(M1) == GE_TRUE);
	//XForm3d_Assert(geXForm3d_IsOrthogonal(M2) == GE_TRUE);

	M1L = *M1;
	M2L = *M2;

	MProduct->AX = M1L.AX * M2L.AX;
	MProduct->AX += M1L.AY * M2L.BX;
	MProduct->AX += M1L.AZ * M2L.CX;

	MProduct->AY = M1L.AX * M2L.AY;
	MProduct->AY += M1L.AY * M2L.BY;
	MProduct->AY += M1L.AZ * M2L.CY;

	MProduct->AZ = M1L.AX * M2L.AZ;
	MProduct->AZ += M1L.AY * M2L.BZ;
	MProduct->AZ += M1L.AZ * M2L.CZ;

	MProduct->BX = M1L.BX * M2L.AX;
	MProduct->BX += M1L.BY * M2L.BX;
	MProduct->BX += M1L.BZ * M2L.CX;

	MProduct->BY = M1L.BX * M2L.AY;
	MProduct->BY += M1L.BY * M2L.BY;
	MProduct->BY += M1L.BZ * M2L.CY;

	MProduct->BZ = M1L.BX * M2L.AZ;
	MProduct->BZ += M1L.BY * M2L.BZ;
	MProduct->BZ += M1L.BZ * M2L.CZ;

	MProduct->CX = M1L.CX * M2L.AX;
	MProduct->CX += M1L.CY * M2L.BX;
	MProduct->CX += M1L.CZ * M2L.CX;

	MProduct->CY = M1L.CX * M2L.AY;
	MProduct->CY += M1L.CY * M2L.BY;
	MProduct->CY += M1L.CZ * M2L.CY;

	MProduct->CZ = M1L.CX * M2L.AZ;
	MProduct->CZ += M1L.CY * M2L.BZ;
	MProduct->CZ += M1L.CZ * M2L.CZ;

	MProduct->Translation.x = M1L.AX * M2L.Translation.x;
	MProduct->Translation.x += M1L.AY * M2L.Translation.y;
	MProduct->Translation.x += M1L.AZ * M2L.Translation.z;
	MProduct->Translation.x += M1L.Translation.x;

	MProduct->Translation.y = M1L.BX * M2L.Translation.x;
	MProduct->Translation.y += M1L.BY * M2L.Translation.y;
	MProduct->Translation.y += M1L.BZ * M2L.Translation.z;
	MProduct->Translation.y += M1L.Translation.y;

	MProduct->Translation.z = M1L.CX * M2L.Translation.x;
	MProduct->Translation.z += M1L.CY * M2L.Translation.y;
	MProduct->Translation.z += M1L.CZ * M2L.Translation.z;
	MProduct->Translation.z += M1L.Translation.z;

	//geXForm3d_Assert(geXForm3d_IsOrthogonal(MProduct) == GE_TRUE);
}

// *************************************************************************
// *					geXForm3d_SetEulerAngles						   *
// *************************************************************************
void CL64_Maths::XForm3d_SetEulerAngles(Matrix3d* M, const T_Vec3* Angles)
{
	Matrix3d XM, YM, ZM;

	XForm3d_SetXRotation(&XM, Angles->x);
	XForm3d_SetYRotation(&YM, Angles->y);
	XForm3d_SetZRotation(&ZM, Angles->z);

	XForm3d_Multiply(&XM, &YM, M);
	XForm3d_Multiply(M, &ZM, M);

}

// *************************************************************************
// *	Quaternion_SetFromAxisAngle:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Maths::Quaternion_SetFromAxisAngle(Ogre::Quaternion* Q, const T_Vec3* Axis, float Theta)
{
	float sinTheta;
	
	Theta = Theta * (float)0.5f;
	Q->w = (float)cos(Theta);
	sinTheta = (float)sin(Theta);
	Q->x = sinTheta * Axis->x;
	Q->y = sinTheta * Axis->y;
	Q->z = sinTheta * Axis->z;

	//Quaternion_Assert(geQuaternion_IsUnit(Q) == GE_TRUE);
}

// *************************************************************************
// *		Quaternion_ToMatrix:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Maths::Quaternion_ToMatrix(const Ogre::Quaternion* Q, Matrix3d* M)
{
	float X2, Y2, Z2;		//2*QX, 2*QY, 2*QZ
	float XX2, YY2, ZZ2;	//2*QX*QX, 2*QY*QY, 2*QZ*QZ
	float XY2, XZ2, XW2;	//2*QX*QY, 2*QX*QZ, 2*QX*QW
	float YZ2, YW2, ZW2;	// ...

	//geQuaternion_Assert(geQuaternion_IsUnit(Q) == GE_TRUE);


	X2 = 2.0f * Q->x;
	XX2 = X2 * Q->x;
	XY2 = X2 * Q->y;
	XZ2 = X2 * Q->z;
	XW2 = X2 * Q->w;

	Y2 = 2.0f * Q->y;
	YY2 = Y2 * Q->y;
	YZ2 = Y2 * Q->z;
	YW2 = Y2 * Q->w;

	Z2 = 2.0f * Q->z;
	ZZ2 = Z2 * Q->z;
	ZW2 = Z2 * Q->w;

	M->AX = 1.0f - YY2 - ZZ2;
	M->AY = XY2 - ZW2;
	M->AZ = XZ2 + YW2;

	M->BX = XY2 + ZW2;
	M->BY = 1.0f - XX2 - ZZ2;
	M->BZ = YZ2 - XW2;

	M->CX = XZ2 - YW2;
	M->CY = YZ2 + XW2;
	M->CZ = 1.0f - XX2 - YY2;

	M->Translation.x = M->Translation.y = M->Translation.z = 0.0f;

}

