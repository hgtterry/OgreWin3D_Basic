#pragma once

typedef struct
{
	float x, y, z;
} T_Vec3;

typedef struct
{
	float AX, AY, AZ;			// e[0][0],e[0][1],e[0][2]
	float BX, BY, BZ;			// e[1][0],e[1][1],e[1][2]
	float CX, CY, CZ;			// e[2][0],e[2][1],e[2][2]
	T_Vec3 Translation;  // e[0][3],e[1][3],e[2][3]
	//	  0,0,0,1				// e[3][0],e[3][1],e[3][2]
} Matrix3d;

class CL64_Lib_Maths
{
public:
	CL64_Lib_Maths();
	~CL64_Lib_Maths();

	void Vector3_Set(T_Vec3* V, float X, float Y, float Z);
	void Vector3_Add(const T_Vec3* V1, const T_Vec3* V2, T_Vec3* V1PlusV2);
	void Vector3_Subtract(const T_Vec3* V1, const T_Vec3* V2, T_Vec3* V1MinusV2);
	void Vector3_Scale(const T_Vec3* VSrc, float Scale, T_Vec3* VDst);
	float Vector3_Normalize(T_Vec3* V1);
	float Vector3_DotProduct(const T_Vec3* V1, const T_Vec3* V2);
	void Vector3_Copy(const T_Vec3* VSrc, T_Vec3* VDst);
	void Vector3_Clear(T_Vec3* V);
	void Vector3_CrossProduct(const T_Vec3* V1, const T_Vec3* V2, T_Vec3* VResult);
	signed int Vector3_Compare(const T_Vec3* V1, const T_Vec3* V2, float Tolerance);
	void Vector3_Inverse(T_Vec3* V);
	void Vector3_AddScaled(const T_Vec3* V1, const T_Vec3* V2, float Scale, T_Vec3* V1PlusV2Scaled);
	void Vector3_MA(T_Vec3* V1, float Scale, const T_Vec3* V2, T_Vec3* V1PlusV2Scaled);

	int Ogre_Vector3_Compare(Ogre::Vector3* V1, Ogre::Vector3* V2, float Tolerance);
	int Ogre_Quaternion_Compare(Ogre::Quaternion* Q1, Ogre::Quaternion* Q2, float Tolerance);

	void XForm3d_SetIdentity(Matrix3d* M);
	void XForm3d_RotateX(Matrix3d* M, float RadianAngle);
	void XForm3d_SetXRotation(Matrix3d* M, float RadianAngle);
	void XForm3d_Multiply(const Matrix3d* M1, const Matrix3d* M2, Matrix3d* MProduct);
	void XForm3d_SetYRotation(Matrix3d* M, float RadianAngle);
	void XForm3d_SetZRotation(Matrix3d* M, float RadianAngle);
	void XForm3d_Transform(const Matrix3d* M, const T_Vec3* V, T_Vec3* Result);
	void XForm3d_GetEulerAngles(const Matrix3d* M, T_Vec3* Angles);
	void XForm3d_SetEulerAngles(Matrix3d* M, const T_Vec3* Angles);


	void Quaternion_SetFromAxisAngle(Ogre::Quaternion* Q, const T_Vec3* Axis, float Theta);
	void Quaternion_ToMatrix(const Ogre::Quaternion* Q, Matrix3d* M);
	void Quaternion_From_Matrix(const Matrix3d* M, Ogre::Quaternion* Q);

	void XForm3d_Rotate(const Matrix3d* M, const T_Vec3* V, T_Vec3* Result);


	void* Ram_Allocate(Ogre::int32 size);
	void Ram_Free(void* ptr);
};

