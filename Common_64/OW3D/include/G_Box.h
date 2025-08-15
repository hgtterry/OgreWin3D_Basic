#pragma once

//struct tag_Box3d
//{
//	T_Vec3 Min;
//	T_Vec3 Max;
//};

typedef struct tag_Box3d Box3d;

class CX_Box
{
public:
	CX_Box(void);
	~CX_Box(void);

	void Box3d_Clear(Box3d* b);
	void Box3d_SetBogusBounds(Box3d* b);
	void Box3d_Set(Box3d* b, float x1, float y1, float z1, float x2, float y2, float z2);
	void Box3d_SetSize(Box3d* b, float sx, float sy, float sz);
	void Box3d_AddPoint(Box3d* b, float px, float py, float pz);
	bool Box3d_Intersection(const Box3d* b1, const Box3d* b2, Box3d* bResult);
	void Box3d_Union(const Box3d* b1, const Box3d* b2, Box3d* bResult);
	signed int Box3d_ContainsPoint(const Box3d* b, float px, float py, float pz);
	const T_Vec3* Box3d_GetMin(const Box3d* b);
	const T_Vec3* Box3d_GetMax(const Box3d* b);
	void Box3d_GetCenter(const Box3d* b, T_Vec3* pCenter);
	float Box3d_GetWidth(const Box3d* b);
	float Box3d_GetHeight(const Box3d* b);
	float Box3d_GetDepth(const Box3d* b);
	void Box3d_GetSize(const Box3d* b, T_Vec3* pSize);
	void Box3d_Scale(Box3d* b, float Scale);
	void Box3d_Move(Box3d* b, float dx, float dy, float dz);
	void Box3d_Inflate(Box3d* b, float dx, float dy, float dz);
};

