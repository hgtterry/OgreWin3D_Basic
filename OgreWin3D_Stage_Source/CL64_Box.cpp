#include "pch.h"
#include "CL64_App.h"
#include "CL64_Box.h"

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

CL64_Box::CL64_Box(void)
{
}

CL64_Box::~CL64_Box(void)
{
}

void CL64_Box::Box3d_Clear(Box3d* b)
{
	Box3d_Set(b, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
}

void CL64_Box::Box3d_SetBogusBounds(Box3d* b)
{
	App->CL_Maths->Vector3_Set(&b->Min, FLT_MAX, FLT_MAX, FLT_MAX);
	App->CL_Maths->Vector3_Set(&b->Max, -FLT_MAX, -FLT_MAX, -FLT_MAX);
}

void CL64_Box::Box3d_Set(Box3d* b,float x1, float y1, float z1, float x2, float y2, float z2)
{
	App->CL_Maths->Vector3_Set(&b->Min,min(x1, x2),min(y1, y2),min(z1, z2));
	App->CL_Maths->Vector3_Set(&b->Max,max(x1, x2),max(y1, y2),max(z1, z2));
}

void CL64_Box::Box3d_SetSize(Box3d* b, float sx, float sy, float sz)
{
	Box3d_Set(b, -sx / 2, -sy / 2, -sz / 2, sx / 2, sy / 2, sz / 2);
}

void CL64_Box::Box3d_AddPoint(Box3d* b, float px, float py, float pz)
{
	if (px < b->Min.x) b->Min.x = px;
	if (px > b->Max.x) b->Max.x = px;
	if (py < b->Min.y) b->Min.y = py;
	if (py > b->Max.y) b->Max.y = py;
	if (pz < b->Min.z) b->Min.z = pz;
	if (pz > b->Max.z) b->Max.z = pz;
}

static bool Box3d_Intersects(const Box3d* b1,const Box3d* b2)
{

	if ((b1->Min.x > b2->Max.x) || (b1->Max.x < b2->Min.x)) return false;
	if ((b1->Min.y > b2->Max.y) || (b1->Max.y < b2->Min.y)) return false;
	if ((b1->Min.z > b2->Max.z) || (b1->Max.z < b2->Min.z)) return false;

	return true;
}

bool CL64_Box::Box3d_Intersection(const Box3d* b1,const Box3d* b2,Box3d* bResult)
{
	bool rslt;

	rslt = Box3d_Intersects(b1, b2);
	if (rslt && (bResult != NULL))
	{
		Box3d_Set
		(
			bResult,
			max(b1->Min.x, b2->Min.x),
			max(b1->Min.y, b2->Min.y),
			max(b1->Min.z, b2->Min.z),
			min(b1->Max.x, b2->Max.x),
			min(b1->Max.y, b2->Max.y),
			min(b1->Max.z, b2->Max.z)
		);
	}
	return rslt;
}

void CL64_Box::Box3d_Union(const Box3d* b1,const Box3d* b2,Box3d* bResult)
{
	Box3d_Set
	(
		bResult,
		min(b1->Min.x, b2->Min.x),
		min(b1->Min.y, b2->Min.y),
		min(b1->Min.z, b2->Min.z),
		max(b1->Max.x, b2->Max.x),
		max(b1->Max.y, b2->Max.y),
		max(b1->Max.z, b2->Max.z)
	);
}

signed int CL64_Box::Box3d_ContainsPoint(const Box3d* b, float px, float py, float pz)
{

	return ((px >= b->Min.x) && (px <= b->Max.x) &&
		(py >= b->Min.y) && (py <= b->Max.y) &&
		(pz >= b->Min.z) && (pz <= b->Max.z));
}

const Ogre::Vector3* CL64_Box::Box3d_GetMin(const Box3d* b)
{
	return &b->Min;
}

const Ogre::Vector3* CL64_Box::Box3d_GetMax(const Box3d* b)
{
	return &b->Max;
}

void CL64_Box::Box3d_GetCenter(const Box3d* b, Ogre::Vector3* pCenter)
{
	App->CL_Maths->Vector3_Set
	(
		pCenter,
		(b->Min.x + b->Max.x) / 2,
		(b->Min.y + b->Max.y) / 2,
		(b->Min.z + b->Max.z) / 2
	);
}

float CL64_Box::Box3d_GetWidth(const Box3d* b)
{
	return (b->Max.x - b->Min.x + 1);
}

float CL64_Box::Box3d_GetHeight(const Box3d* b)
{
	return (b->Max.y - b->Min.y + 1);
}

float CL64_Box::Box3d_GetDepth(const Box3d* b)
{
	return (b->Max.z - b->Min.z + 1);
}


void CL64_Box::Box3d_GetSize(const Box3d* b, Ogre::Vector3* pSize)
{
	App->CL_Maths->Vector3_Set
	(
		pSize,
		(b->Max.x - b->Min.x + 1),
		(b->Max.y - b->Min.y + 1),
		(b->Max.z - b->Min.z + 1)
	);
}

void CL64_Box::Box3d_Scale(Box3d* b, float Scale)
{
	App->CL_Maths->Vector3_Scale(&b->Min, Scale, &b->Min);
	App->CL_Maths->Vector3_Scale(&b->Max, Scale, &b->Max);
}

void CL64_Box::Box3d_Move(Box3d* b, float dx, float dy, float dz)
{
	Ogre::Vector3 VecDelta;

	App->CL_Maths->Vector3_Set(&VecDelta, dx, dy, dz);
	App->CL_Maths->Vector3_Add(&b->Min, &VecDelta, &b->Min);
	App->CL_Maths->Vector3_Add(&b->Max, &VecDelta, &b->Max);
}

void CL64_Box::Box3d_Inflate(Box3d* b, float dx, float dy, float dz)
{
	Ogre::Vector3 VecDelta;

	App->CL_Maths->Vector3_Set(&VecDelta, dx, dy, dz);
	App->CL_Maths->Vector3_Subtract(&b->Min, &VecDelta, &b->Min);
	App->CL_Maths->Vector3_Add(&b->Max, &VecDelta, &b->Max);
}
