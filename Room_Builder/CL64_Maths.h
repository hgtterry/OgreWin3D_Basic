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

#pragma once

typedef struct
{
	float AX, AY, AZ;			// e[0][0],e[0][1],e[0][2]
	float BX, BY, BZ;			// e[1][0],e[1][1],e[1][2]
	float CX, CY, CZ;			// e[2][0],e[2][1],e[2][2]
	Ogre::Vector3 Translation;  // e[0][3],e[1][3],e[2][3]
	//	  0,0,0,1				// e[3][0],e[3][1],e[3][2]
} Matrix3d;

class CL64_Maths
{
public:
	CL64_Maths();
	~CL64_Maths();

	void Vector3_Set(Ogre::Vector3* V, float X, float Y, float Z);
	void Vector3_Add(const Ogre::Vector3* V1, const Ogre::Vector3* V2, Ogre::Vector3* V1PlusV2);
	void Vector3_Subtract(const Ogre::Vector3* V1, const Ogre::Vector3* V2, Ogre::Vector3* V1MinusV2);
	void Vector3_Scale(const Ogre::Vector3* VSrc, float Scale, Ogre::Vector3* VDst);
	float Vector3_Normalize(Ogre::Vector3* V1);
	float Vector3_DotProduct(const Ogre::Vector3* V1, const Ogre::Vector3* V2);
	void Vector3_Copy(const Ogre::Vector3* VSrc, Ogre::Vector3* VDst);
	void Vector3_Clear(Ogre::Vector3* V);
	void Vector3_CrossProduct(const Ogre::Vector3* V1, const Ogre::Vector3* V2, Ogre::Vector3* VResult);
	signed int Vector3_Compare(const Ogre::Vector3* V1, const Ogre::Vector3* V2, float Tolerance);
	void Vector3_Inverse(Ogre::Vector3* V);
	void Vector3_AddScaled(const Ogre::Vector3* V1, const Ogre::Vector3* V2, float Scale, Ogre::Vector3* V1PlusV2Scaled);
	void Vector3_MA(Ogre::Vector3* V1, float Scale, const Ogre::Vector3* V2, Ogre::Vector3* V1PlusV2Scaled);


	void XForm3d_SetIdentity(Matrix3d* M);
	void XForm3d_RotateX(Matrix3d* M, float RadianAngle);
	void XForm3d_SetXRotation(Matrix3d* M, float RadianAngle);
	void XForm3d_Multiply(const Matrix3d* M1, const Matrix3d* M2, Matrix3d* MProduct);
	void XForm3d_SetYRotation(Matrix3d* M, float RadianAngle);
	void XForm3d_SetZRotation(Matrix3d* M, float RadianAngle);


	void Quaternion_SetFromAxisAngle(Ogre::Quaternion* Q, const Ogre::Vector3* Axis, float Theta);
	void Quaternion_ToMatrix(const Ogre::Quaternion* Q, Matrix3d* M);
	void geXForm3d_Rotate(const Matrix3d* M, const Ogre::Vector3* V, Ogre::Vector3* Result);


	void* Ram_Allocate(Ogre::int32 size);
	void Ram_Free(void* ptr);

};

