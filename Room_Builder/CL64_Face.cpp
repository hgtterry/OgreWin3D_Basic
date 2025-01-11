/*
Copyright (c) 2024 Inflanite_HGT W.T.Flanigan H.C.Flanigan

Room Builder

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
#include "CL64_Face.h"

static	Ogre::Vector3 spf[256], spb[256];
const Ogre::Vector3	VecOrigin = { 0.0f, 0.0f, 0.0f };

#define	VCOMPARE_EPSILON			(0.00001f)
#define MAX_POINTS					64
#define	ON_EPSILON					(0.1f)
#define	VectorToSUB(a, b)			(*((((float *)(&a))) + (b)))
#define FACE_DEFAULT_LIGHT			300
#define FACE_DEFAULT_BIAS			(1.0f)
#define FACE_DEFAULT_TRANSLUCENCY	(255.0f)
#define FACE_DEFAULT_REFLECTIVITY	(1.0f)


enum FaceFlags
{
	FACE_MIRROR = (1 << 0),
	FACE_FULLBRIGHT = (1 << 1),
	FACE_SKY = (1 << 2),
	FACE_LIGHT = (1 << 3),
	FACE_SELECTED = (1 << 4),
	FACE_FIXEDHULL = (1 << 5),		//doesn't expand (rings)
	FACE_GOURAUD = (1 << 6),
	FACE_FLAT = (1 << 7),
	FACE_TEXTURELOCKED = (1 << 8),
	FACE_VISIBLE = (1 << 9),
	FACE_SHEET = (1 << 10),		//visible from both sides
	FACE_TRANSPARENT = (1 << 11)		//use transparency value for something
};

typedef struct TexInfoTag
{
	Ogre::Vector3 VecNormal;
	float xScale, yScale;
	int xShift, yShift;
	float	Rotate;			// texture rotation angle in degrees
	TexInfo_Vectors TVecs;
	int Dib;				// index into the wad
	char Name[16];
	signed int DirtyFlag;
	Ogre::Vector3 Pos;
	int txSize, tySize;		// texture size (not currently used)
	Matrix3d XfmFaceAngle;	// face rotation angle
} TexInfo;

typedef struct FaceTag
{
	int				NumPoints;
	int				Flags;
	GPlane			Face_Plane;
	int				LightIntensity;
	float			Reflectivity;
	float			Translucency;
	float			MipMapBias;
	float			LightXScale, LightYScale;
	TexInfo			Tex;
	Ogre::Vector3*	Points;

} Face;

enum SideFlags
{
	SIDE_FRONT = 0,
	SIDE_BACK = 1,
	SIDE_ON = 2
};

CL64_Face::CL64_Face(void)
{
}

CL64_Face::~CL64_Face(void)
{
}

// *************************************************************************
// *							Face_SetVisible							   *
// *************************************************************************
void CL64_Face::Face_SetVisible(Face* f, const signed int bState)
{
	f->Flags = (bState) ? (f->Flags | FACE_VISIBLE) : (f->Flags & ~FACE_VISIBLE);
}

// *************************************************************************
// *							Face_Create								   *
// *************************************************************************
Face* CL64_Face::Face_Create(int NumPnts, const Ogre::Vector3* pnts, int DibId)
{
	Face* f;

	assert(NumPnts > 0);
	// changed QD 11/03
	assert(NumPnts <= MAX_POINTS);
	// end change
	assert(pnts != NULL);

	f = (Face*)App->CL_Maths->Ram_Allocate(sizeof(Face));
	if (f)
	{
		memset(f, 0, sizeof(Face));

		f->NumPoints = NumPnts;
		f->LightIntensity = FACE_DEFAULT_LIGHT;
		f->MipMapBias = FACE_DEFAULT_BIAS;
		f->Translucency = FACE_DEFAULT_TRANSLUCENCY;
		f->Reflectivity = FACE_DEFAULT_REFLECTIVITY;
		f->LightXScale = 1.0f;
		f->LightYScale = 1.0f;

		Face_SetVisible(f, true);

		f->Points = (Ogre::Vector3*)App->CL_Maths->Ram_Allocate(sizeof(Ogre::Vector3) * NumPnts);
		if (f->Points)
		{
			memcpy(f->Points, pnts, sizeof(Ogre::Vector3) * NumPnts);

			if (Face_SetPlaneFromFace(f))
			{
				Face_InitTexInfo(&f->Tex, &f->Face_Plane.Normal);
				Face_SetTextureDibId(f, DibId);
				Face_SetTexturePos(f);
			}
			else
			{
				App->CL_Maths->Ram_Free(f->Points); //hgtterry Debug
				App->CL_Maths->Ram_Free(f);
				f = NULL;
			}
		}
		else
		{
			App->CL_Maths->Ram_Free(f);
			f = NULL;
		}
	}
	return	f;
}

// *************************************************************************
// *						Face_GetBounds								   *
// *************************************************************************
void CL64_Face::Face_GetBounds(const Face* f, Box3d* b)
{
	int i;

	assert(f != NULL);
	assert(b != NULL);

	App->CL_Box->Box3d_SetBogusBounds(b);
	for (i = 0; i < f->NumPoints; i++)
	{
		App->CL_Box->Box3d_AddPoint(b, f->Points[i].x, f->Points[i].y, f->Points[i].z);
	}
}

// *************************************************************************
// *						Face_SetTexturePos							   *
// *************************************************************************
void CL64_Face::Face_SetTexturePos(Face* f)
{
	//	Face_GetCenter (f, &f->Tex.Pos);
	App->CL_Maths->Vector3_Clear(&f->Tex.Pos);
	f->Tex.DirtyFlag = true;
}

// *************************************************************************
// *						Face_InitTexInfo							   *
// *************************************************************************
void CL64_Face::Face_SetTextureDibId(Face* f, const int Dib)
{
	assert(f != NULL);

	f->Tex.Dib = Dib;
}

// *************************************************************************
// *						Face_InitTexInfo							   *
// *************************************************************************
void CL64_Face::Face_InitTexInfo(TexInfo* t,Ogre::Vector3 const* pNormal)
{
	t->Name[0] = '\0';
	t->xScale = 1.0f;
	t->yScale = 1.0f;
	t->xShift = 0;
	t->yShift = 0;
	t->Rotate = 0.0f;
	t->Dib = 0;
	t->DirtyFlag = false;
	t->txSize = 0;
	t->tySize = 0;

	App->CL_Maths->Vector3_Clear(&t->Pos);
	Face_SetTexInfoPlane(t, pNormal);
	Face_InitFaceAngle(t, pNormal);
}

// *************************************************************************
// *						Face_InitFaceAngle							   *
// *************************************************************************
void CL64_Face::Face_InitFaceAngle(TexInfo* t,Ogre::Vector3 const* pNormal)
{
	Ogre::Vector3 VecDest;
	Ogre::Vector3 VecAxis;
	float cosv, Theta;
	Ogre::Vector3		PosNormal;

	PosNormal = *pNormal;

	if (fabs(pNormal->x) > fabs(pNormal->y))
	{
		if (fabs(pNormal->x) > fabs(pNormal->z))
		{
			if (pNormal->x > 0)
				App->CL_Maths->Vector3_Inverse(&PosNormal);
		}
		else
		{
			if (pNormal->z > 0)
				App->CL_Maths->Vector3_Inverse(&PosNormal);
		}

	}
	else
	{
		if (fabs(pNormal->y) > fabs(pNormal->z))
		{
			if (pNormal->y > 0)
				App->CL_Maths->Vector3_Inverse(&PosNormal);
		}
		else
		{
			if (pNormal->z > 0)
				App->CL_Maths->Vector3_Inverse(&PosNormal);
		}
	}

	// Create rotation matrix that will put this face into the X,Y plane.
	App->CL_Maths->Vector3_Set(&VecDest, 0.0f, 0.0f, 1.0f);
	App->CL_Maths->Vector3_CrossProduct(&VecDest, &PosNormal, &VecAxis);
	cosv = App->CL_Maths->Vector3_DotProduct(&VecDest, &PosNormal);
	if (cosv > 1.0f)
	{
		cosv = 1.0f;
	}

	Theta = (float)acos(cosv);

	if (App->CL_Maths->Vector3_Normalize(&VecAxis) == 0.0f)
	{
		// If the resulting vector is 0 length, 
		// then a rotation about X will put us where we need to be.
		App->CL_Maths->XForm3d_SetIdentity(&t->XfmFaceAngle);
		App->CL_Maths->XForm3d_RotateX(&t->XfmFaceAngle, -Theta);
	}
	else
	{
		Ogre::Quaternion QRot;

		App->CL_Maths->Quaternion_SetFromAxisAngle(&QRot, &VecAxis, -Theta);
		App->CL_Maths->Quaternion_ToMatrix(&QRot, &t->XfmFaceAngle);
	}
}

// *************************************************************************
// *						Face_SetPlaneFromFace						   *
// *************************************************************************
signed int CL64_Face::Face_SetPlaneFromFace(Face* f)
{
	int		i;
	Ogre::Vector3 v1, v2;

	assert(f != NULL);

	//catches colinear points now
	for (i = 0; i < f->NumPoints; i++)
	{
		//gen a plane normal from the cross of edge vectors
		App->CL_Maths->Vector3_Subtract(&f->Points[i], &f->Points[(i + 1) % f->NumPoints], &v1);
		App->CL_Maths->Vector3_Subtract(&f->Points[(i + 2) % f->NumPoints], &f->Points[(i + 1) % f->NumPoints], &v2);

		App->CL_Maths->Vector3_CrossProduct(&v1, &v2, &f->Face_Plane.Normal);
		if (!App->CL_Maths->Vector3_Compare(&f->Face_Plane.Normal, &VecOrigin, VCOMPARE_EPSILON))
		{
			break;
		}

		//try the next three if there are three
	}
	if (i >= f->NumPoints)
	{
		return	false;
	}
	App->CL_Maths->Vector3_Normalize(&f->Face_Plane.Normal);
	f->Face_Plane.Dist = App->CL_Maths->Vector3_DotProduct(&f->Points[1], &f->Face_Plane.Normal);

	Face_SetTexInfoPlane(&f->Tex, &f->Face_Plane.Normal);
	return	true;
}

// *************************************************************************
// *						Face_SetTexInfoPlane						   *
// *************************************************************************
void CL64_Face::Face_SetTexInfoPlane(TexInfo* t,Ogre::Vector3 const* pNormal)
{
	t->VecNormal = *pNormal;

	t->DirtyFlag = true;
}

// *************************************************************************
// *							Face_SetSheet							   *
// *************************************************************************
void CL64_Face::Face_SetSheet(Face* f, const signed int bState)
{
	f->Flags = (bState) ? (f->Flags | FACE_SHEET) : (f->Flags & ~FACE_SHEET);
}

// *************************************************************************
// *							Face_GetPlane							   *
// *************************************************************************
const GPlane* CL64_Face::Face_GetPlane(const Face* f)
{
	return	&f->Face_Plane;
}

// *************************************************************************
// *						Face_IsFixedHull							   *
// *************************************************************************
signed int CL64_Face::Face_IsFixedHull(const Face* f)
{
	assert(f != NULL);

	return	(f->Flags & FACE_FIXEDHULL) ? true : false;
}

// *************************************************************************
// *						Face_CreateFromPlane						   *
// *************************************************************************
Face* CL64_Face::Face_CreateFromPlane(const GPlane* p, float Radius, int DibId)
{
	float	v;
	Ogre::Vector3	vup, vright, org, pnts[4];

	assert(p != NULL);

	//find the major axis of p->Normal
	App->CL_Maths->Vector3_Set(&vup, 0.0f, 0.0f, 1.0f);
	if ((fabs(p->Normal.z) > fabs(p->Normal.x))
		&& (fabs(p->Normal.z) > fabs(p->Normal.y)))
	{
		App->CL_Maths->Vector3_Set(&vup, 1.0f, 0.0f, 0.0f);
	}

	v = App->CL_Maths->Vector3_DotProduct(&vup, &p->Normal);
	App->CL_Maths->Vector3_AddScaled(&vup, &p->Normal, -v, &vup);
	App->CL_Maths->Vector3_Normalize(&vup);

	App->CL_Maths->Vector3_AddScaled(&VecOrigin, &p->Normal, p->Dist, &org);
	App->CL_Maths->Vector3_CrossProduct(&vup, &p->Normal, &vright);

	App->CL_Maths->Vector3_Scale(&vup, Radius, &vup);
	App->CL_Maths->Vector3_Scale(&vright, Radius, &vright);

	App->CL_Maths->Vector3_Subtract(&org, &vright, &pnts[0]);
	App->CL_Maths->Vector3_Add(&pnts[0], &vup, &pnts[0]);

	App->CL_Maths->Vector3_Add(&org, &vright, &pnts[1]);
	App->CL_Maths->Vector3_Add(&pnts[1], &vup, &pnts[1]);

	App->CL_Maths->Vector3_Add(&org, &vright, &pnts[2]);
	App->CL_Maths->Vector3_Subtract(&pnts[2], &vup, &pnts[2]);

	App->CL_Maths->Vector3_Subtract(&org, &vright, &pnts[3]);
	App->CL_Maths->Vector3_Subtract(&pnts[3], &vup, &pnts[3]);

	return	Face_Create(4, pnts, DibId);
}

// *************************************************************************
// *						Face_CopyFaceInfo							   *
// *************************************************************************
void CL64_Face::Face_CopyFaceInfo(const Face* src, Face* dst)
{
	assert(src);
	assert(dst);

	dst->Flags = src->Flags;
	dst->LightIntensity = src->LightIntensity;
	dst->MipMapBias = src->MipMapBias;
	dst->Translucency = src->Translucency;
	dst->Reflectivity = src->Reflectivity;
	dst->Tex = src->Tex;
	dst->LightXScale = src->LightXScale;
	dst->LightYScale = src->LightYScale;

	//make sure the texinfos plane and vecs are good
	Face_SetPlaneFromFace(dst);
}

// *************************************************************************
// *						Face_GetSplitInfo							   *
// *************************************************************************
void CL64_Face::Face_GetSplitInfo(const Face* f, const GPlane* p, float* dists, Ogre::uint8* sides, Ogre::uint8* cnt)
{
	int	i;

	cnt[0] = cnt[1] = cnt[2] = 0;

	for (i = 0; i < f->NumPoints; i++)
	{
		dists[i] = App->CL_Maths->Vector3_DotProduct(&f->Points[i], &p->Normal) - p->Dist;
		if (dists[i] > ON_EPSILON)
		{
			sides[i] = SIDE_FRONT;
		}
		else if (dists[i] < -ON_EPSILON)
		{
			sides[i] = SIDE_BACK;
		}
		else
		{
			sides[i] = SIDE_ON;
		}
		cnt[sides[i]]++;
	}
	sides[i] = sides[0];
	dists[i] = dists[0];
}

// *************************************************************************
// *							Face_Destroy							   *
// *************************************************************************
void CL64_Face::Face_Destroy(Face** f)
{
	if ((*f)->Points)
	{
		App->CL_Maths->Ram_Free((*f)->Points);
	}

	App->CL_Maths->Ram_Free(*f);
	*f = NULL;
}

// *************************************************************************
// *							Face_Clip								   *
// *************************************************************************
void CL64_Face::Face_Clip(Face* f, const GPlane* p, float* dists, Ogre::uint8* sides)
{
	Ogre::Vector3* p1, * p2, mid;
	int		nfp, nbp, i, j;
	float	dot;

	p1 = f->Points;
	for (i = nfp = nbp = 0; i < f->NumPoints; i++, p1++)
	{
		if (sides[i] == SIDE_ON)
		{
			App->CL_Maths->Vector3_Copy(p1, &spb[nbp]);
			nbp++;
			continue;
		}
		if (sides[i] == SIDE_BACK)
		{
			App->CL_Maths->Vector3_Copy(p1, &spb[nbp]);
			nbp++;
		}
		if (sides[i + 1] == SIDE_ON || sides[i + 1] == sides[i])
			continue;

		p2 = &f->Points[(i + 1) % f->NumPoints];
		dot = dists[i] / (dists[i] - dists[i + 1]);
		for (j = 0; j < 3; j++)
		{
			if (VectorToSUB(p->Normal, j) == 1)
			{
				VectorToSUB(mid, j) = p->Dist;
			}
			else if (VectorToSUB(p->Normal, j) == -1)
			{
				VectorToSUB(mid, j) = -p->Dist;
			}
			else
			{
				VectorToSUB(mid, j) = VectorToSUB(*p1, j) +
					dot * (VectorToSUB(*p2, j) - VectorToSUB(*p1, j));
			}
		}
		App->CL_Maths->Vector3_Copy(&mid, &spb[nbp]);
		nbp++;
	}
	App->CL_Maths->Ram_Free(f->Points);
	f->NumPoints = nbp;
	f->Points = (Ogre::Vector3*)App->CL_Maths->Ram_Allocate(sizeof(Ogre::Vector3) * nbp);
	memcpy(f->Points, spb, sizeof(Ogre::Vector3) * nbp);
}

// *************************************************************************
// *						Face_GetPoints							 	   *
// *************************************************************************
const Ogre::Vector3* CL64_Face::Face_GetPoints(const Face* f)
{
	assert(f != NULL);

	return	f->Points;
}

// *************************************************************************
// *					Face_GetNumPoints							 	   *
// *************************************************************************
int	CL64_Face::Face_GetNumPoints(const Face* f)
{
	return	f->NumPoints;
}

// *************************************************************************
// *						Face_GetTextureDibId					 	   *
// *************************************************************************
int	CL64_Face::Face_GetTextureDibId(const Face* f)
{
	return	f->Tex.Dib;
}

// *************************************************************************
// *							Face_Clone							 	   *
// *************************************************************************
Face* CL64_Face::Face_Clone(const Face* src)
{
	Face* dst;

	assert(src != NULL);
	assert(src->NumPoints > 0);
	assert(src->Points != NULL);

	dst = Face_Create(src->NumPoints, src->Points, Face_GetTextureDibId(src));
	if (dst)
	{
		Face_CopyFaceInfo(src, dst);
	}
	return	dst;
}

// *************************************************************************
// *							Face_SetLightScale					 	   *
// *************************************************************************
void CL64_Face::Face_SetLightScale(Face* f, const float xScale, const float yScale)
{
	f->LightXScale = xScale;
	f->LightYScale = yScale;
}

// *************************************************************************
// *							Face_SetTextureScale				 	   *
// *************************************************************************
void CL64_Face::Face_SetTextureScale(Face* f, const float xScale, const float yScale)
{
	f->Tex.xScale = xScale;
	f->Tex.yScale = yScale;

	f->Tex.DirtyFlag = true;
}

// *************************************************************************
// *						Face_SetTextureName						 	   *
// *************************************************************************
void CL64_Face::Face_SetTextureName(Face* f, const char* pName)
{
	// copy the name (safely), and then nul-terminate
	strncpy(f->Tex.Name, pName, sizeof(f->Tex.Name));
	f->Tex.Name[sizeof(f->Tex.Name) - 1] = '\0';
}

// *************************************************************************
// *							Face_SetTextureSize					 	   *
// *************************************************************************
void CL64_Face::Face_SetTextureSize(Face* f, const int txSize, const int tySize)
{
	f->Tex.txSize = txSize;
	f->Tex.tySize = tySize;
}

// *************************************************************************
// *							Face_GetTextureName					 	   *
// *************************************************************************
char const* CL64_Face::Face_GetTextureName(const Face* f)
{
	return	f->Tex.Name;
}

// *************************************************************************
// *							Face_GetTextureSize					 	   *
// *************************************************************************
void CL64_Face::Face_GetTextureSize(const Face* f, int* ptxSize, int* ptySize)
{
	*ptxSize = f->Tex.txSize;
	*ptySize = f->Tex.tySize;
}

signed int	Face_IsTextureLocked(const Face* f)
{
	return (f->Flags & FACE_TEXTURELOCKED) ? GE_TRUE : GE_FALSE;
}

static void Face_UpdateLockedTextureVecs(Face* f)
{
	int WhichAxis;
	Matrix3d XfmTexture;
	TexInfo* t = &f->Tex;

	assert(t != NULL);
	
	if (t->xScale == 0.0f) t->xScale = 1.0f;
	if (t->yScale == 0.0f) t->yScale = 1.0f;

	// the normal has to be normal, no?
	assert((t->VecNormal.X != 0.0f) ||
		(t->VecNormal.Y != 0.0f) ||
		(t->VecNormal.Z != 0.0f));

	// Compute rotation
	App->CL_Maths->Vector3_Clear(&t->XfmFaceAngle.Translation);
	
	WhichAxis = 0;		//	sides
	if (fabs(t->VecNormal.y) > fabs(t->VecNormal.x))
	{
		if (fabs(t->VecNormal.z) > fabs(t->VecNormal.y))
		{
			WhichAxis = 2;	// front / back
		}
		else
		{
			WhichAxis = 1;		//	top / bottom
		}
	}
	else if (fabs(t->VecNormal.z) > fabs(t->VecNormal.x))
	{
		WhichAxis = 2;
	}

	switch (WhichAxis)
	{
	case 0:			// sides
		App->CL_Maths->XForm3d_SetZRotation(&XfmTexture, Units_DegreesToRadians(t->Rotate));
		App->CL_Maths->XForm3d_Multiply(&t->XfmFaceAngle, &XfmTexture, &XfmTexture);
		App->CL_Maths->Vector3_Set(&t->TVecs.uVec, -XfmTexture.AX, -XfmTexture.BX, -XfmTexture.CX);
		App->CL_Maths->Vector3_Set(&t->TVecs.vVec, -XfmTexture.AY, -XfmTexture.BY, -XfmTexture.CY);
		break;
	case 1:			// top / bottom
		App->CL_Maths->XForm3d_SetZRotation(&XfmTexture, Units_DegreesToRadians(t->Rotate));
		App->CL_Maths->XForm3d_Multiply(&t->XfmFaceAngle, &XfmTexture, &XfmTexture);
		App->CL_Maths->Vector3_Set(&t->TVecs.uVec, XfmTexture.AX, XfmTexture.BX, XfmTexture.CX);
		App->CL_Maths->Vector3_Set(&t->TVecs.vVec, -XfmTexture.AY, -XfmTexture.BY, -XfmTexture.CY);
		break;
	case 2:			// front / back
		App->CL_Maths->XForm3d_SetZRotation(&XfmTexture, Units_DegreesToRadians(t->Rotate));
		App->CL_Maths->XForm3d_Multiply(&t->XfmFaceAngle, &XfmTexture, &XfmTexture);
		App->CL_Maths->Vector3_Set(&t->TVecs.uVec, XfmTexture.AX, XfmTexture.BX, XfmTexture.CX);
		App->CL_Maths->Vector3_Set(&t->TVecs.vVec, XfmTexture.AY, XfmTexture.BY, XfmTexture.CY);
		break;
	}
	// end change

		// and scale accordingly
	App->CL_Maths->Vector3_Scale(&t->TVecs.uVec, 1.0f / t->xScale, &t->TVecs.uVec);
	App->CL_Maths->Vector3_Scale(&t->TVecs.vVec, 1.0f / t->yScale, &t->TVecs.vVec);


	// compute offsets...
	{
		geFloat uOffset, vOffset;

		uOffset = App->CL_Maths->Vector3_DotProduct(&t->TVecs.uVec, &f->Tex.Pos);
		vOffset = App->CL_Maths->Vector3_DotProduct(&t->TVecs.vVec, &f->Tex.Pos);

		t->TVecs.uOffset = (float)(t->xShift - uOffset);
		t->TVecs.vOffset = (float)(t->yShift - vOffset);
	}
}


static void Face_UpdateWorldTextureVecs(Face* f)
{
	float	ang, sinv, cosv;
	Ogre::Vector3 uVec, vVec;
	int WhichAxis;
	TexInfo* t = &f->Tex;

	if (t->xScale == 0.0f) t->xScale = 1.0f;
	if (t->yScale == 0.0f) t->yScale = 1.0f;

	ang = UNITS_DEGREES_TO_RADIANS(-t->Rotate);
	sinv = (geFloat)sin(ang);
	cosv = (geFloat)cos(ang);

	// the normal has to be normal, no?
	assert((t->VecNormal.X != 0.0f) ||
		(t->VecNormal.Y != 0.0f) ||
		(t->VecNormal.Z != 0.0f));

	WhichAxis = 0;
	if (fabs(t->VecNormal.y) > fabs(t->VecNormal.x))
	{
		if (fabs(t->VecNormal.z) > fabs(t->VecNormal.y))
		{
			WhichAxis = 2;
		}
		else
		{
			WhichAxis = 1;
		}
	}
	else if (fabs(t->VecNormal.z) > fabs(t->VecNormal.x))
	{
		WhichAxis = 2;
	}

	switch (WhichAxis)
	{
	case 0:
		App->CL_Maths->Vector3_Set(&uVec, 0.0f, sinv, cosv);
		App->CL_Maths->Vector3_Set(&vVec, 0.0f, -cosv, sinv);
		break;
	case 1:
		App->CL_Maths->Vector3_Set(&uVec, cosv, 0.0f, sinv);
		App->CL_Maths->Vector3_Set(&vVec, -sinv, 0.0f, cosv);
		break;
	case 2:
		App->CL_Maths->Vector3_Set(&uVec, cosv, sinv, 0.0f);
		App->CL_Maths->Vector3_Set(&vVec, sinv, -cosv, 0.0f);
		break;
	}

	t->TVecs.uOffset = (geFloat)(t->xShift);
	t->TVecs.vOffset = (geFloat)(t->yShift);

	App->CL_Maths->Vector3_Scale(&uVec, (1.0f / t->xScale), &t->TVecs.uVec);
	App->CL_Maths->Vector3_Scale(&vVec, (1.0f / t->yScale), &t->TVecs.vVec);
}

static void Face_UpdateTextureVecs(Face* f)
{
	if (Face_IsTextureLocked(f))
	{
		Face_UpdateLockedTextureVecs(f);
	}
	else
	{
		Face_UpdateWorldTextureVecs(f);
	}
}



// *************************************************************************
// *							Face_GetTextureVecs					 	   *
// *************************************************************************
const TexInfo_Vectors* CL64_Face::Face_GetTextureVecs(const Face* f)
{

	// if info has been changed then we have to re-calculate the vecs...
	if (f->Tex.DirtyFlag)
	{
		//make sure the texinfos plane and vecs are good
		Face_SetPlaneFromFace((Face*)f);

		// The cast is kinda ugly, but we really want the parameter
		// to this function to be const!
		// mutable would be nice here, huh?
		Face_UpdateTextureVecs((Face*)f);
		((Face*)f)->Tex.DirtyFlag = GE_FALSE;
	}

	return &(f->Tex.TVecs);
}

// *************************************************************************
// *							Face_CloneReverse					 	   *
// *************************************************************************
Face* CL64_Face::Face_CloneReverse(const Face* src)
{
	int		i;
	Face* dst;
	Ogre::Vector3	pt;

	assert(src != NULL);
	assert(src->NumPoints > 0);
	assert(src->Points != NULL);

	dst = Face_Clone(src);
	if (dst)
	{
		dst->Face_Plane.Dist = -dst->Face_Plane.Dist;
		App->CL_Maths->Vector3_Inverse(&dst->Face_Plane.Normal);

		for (i = 0; i < dst->NumPoints / 2; i++)
		{
			pt = dst->Points[i];
			dst->Points[i] = dst->Points[dst->NumPoints - i - 1];
			dst->Points[dst->NumPoints - i - 1] = pt;
		}
		Face_SetPlaneFromFace(dst);
	}
	return dst;
}

// *************************************************************************
// *							Face_Split							 	   *
// *************************************************************************
void CL64_Face::Face_Split(const Face* f,const GPlane* p,Face** ff,Face** bf,float* dists, Ogre::uint8* sides)
{
	Ogre::Vector3* p1, * p2, mid;
	int		nfp, nbp, i, j;
	geFloat	dot;

	assert(f);
	assert(p);
	assert(ff);
	assert(bf);
	assert(*ff == NULL);
	assert(*bf == NULL);
	assert(dists);
	assert(sides);

	p1 = f->Points;
	for (i = nfp = nbp = 0; i < f->NumPoints; i++, p1++)
	{
		if (sides[i] == SIDE_ON)
		{
			App->CL_Maths->Vector3_Copy(p1, &spf[nfp]);
			App->CL_Maths->Vector3_Copy(p1, &spb[nbp]);
			nfp++;	nbp++;	//Dont ++ in params!
			continue;
		}
		if (sides[i] == SIDE_FRONT)
		{
			App->CL_Maths->Vector3_Copy(p1, &spf[nfp]);
			nfp++;
		}
		if (sides[i] == SIDE_BACK)
		{
			App->CL_Maths->Vector3_Copy(p1, &spb[nbp]);
			nbp++;
		}
		if (sides[i + 1] == SIDE_ON || sides[i + 1] == sides[i])
			continue;

		p2 = &f->Points[(i + 1) % f->NumPoints];
		dot = dists[i] / (dists[i] - dists[i + 1]);
		for (j = 0; j < 3; j++)
		{
			if (VectorToSUB(p->Normal, j) == 1)
			{
				VectorToSUB(mid, j) = p->Dist;
			}
			else if (VectorToSUB(p->Normal, j) == -1)
			{
				VectorToSUB(mid, j) = -p->Dist;
			}
			else
			{
				VectorToSUB(mid, j) = VectorToSUB(*p1, j) +
					dot * (VectorToSUB(*p2, j) - VectorToSUB(*p1, j));
			}
		}

		//split goes to both sides
		App->CL_Maths->Vector3_Copy(&mid, &spf[nfp]);
		nfp++;
		App->CL_Maths->Vector3_Copy(&mid, &spb[nbp]);
		nbp++;
	}
	*ff = Face_Create(nfp, spf, 0);
	*bf = Face_Create(nbp, spb, 0);

	if (*ff)
	{
		Face_CopyFaceInfo(f, *ff);
	}
	if (*bf)
	{
		Face_CopyFaceInfo(f, *bf);
	}
}

// *************************************************************************
// *							Face_Split							 	   *
// *************************************************************************
void CL64_Face::Face_MostlyOnSide(const Face* f, const GPlane* p, float* max, int* side)
{
	int		i;
	geFloat	d;

	for (i = 0; i < f->NumPoints; i++)
	{
		d = App->CL_Maths->Vector3_DotProduct(&f->Points[i], &p->Normal) - p->Dist;
		if (d > *max)
		{
			*max = d;
			*side = SIDE_FRONT;
		}
		if (-d > *max)
		{
			*max = -d;
			*side = SIDE_BACK;
		}
	}
}

// *************************************************************************
// *							Face_Move							 	   *
// *************************************************************************
void CL64_Face::Face_Move(Face* f, const Ogre::Vector3* trans)
{
	int i;

	for (i = 0; i < f->NumPoints; i++)
	{
		App->CL_Maths->Vector3_Add(&f->Points[i], trans, &f->Points[i]);
	}

	Face_SetPlaneFromFace(f);

	// Update position...
	App->CL_Maths->Vector3_Add(&f->Tex.Pos, trans, &f->Tex.Pos);
	f->Tex.DirtyFlag = GE_TRUE;
}

// *************************************************************************
// *						Face_SetSelected						 	   *
// *************************************************************************
void CL64_Face::Face_SetSelected(Face* f, const signed int bState)
{
	f->Flags = (bState) ? f->Flags | FACE_SELECTED : f->Flags & ~FACE_SELECTED;
}

// *************************************************************************
// *			( Static ) Face_UpdateFaceAngle						 	   *
// *************************************************************************
static void Face_UpdateFaceAngle(Face* f, const Ogre::Vector3* OldNormal)
{
	Ogre::Vector3 VecDest;
	Ogre::Vector3 VecAxis;
	geFloat cosv, Theta;
	Matrix3d Xfm;

	// Compute rotation from 
	VecDest = f->Tex.VecNormal;
	App->CL_Maths->Vector3_CrossProduct(&VecDest, OldNormal, &VecAxis);
	cosv = App->CL_Maths->Vector3_DotProduct(&VecDest, OldNormal);
	
	if (cosv > 1.0f)
	{
		cosv = 1.0f;
	}
	Theta = (geFloat)acos(cosv);
	if (App->CL_Maths->Vector3_Normalize(&VecAxis) == 0.0f)
	{
		App->CL_Maths->XForm3d_SetIdentity(&Xfm);
		App->CL_Maths->XForm3d_RotateX(& Xfm, -Theta);
	}
	else
	{
		Ogre::Quaternion QRot;

		App->CL_Maths->Quaternion_SetFromAxisAngle(&QRot, &VecAxis, -Theta);
		App->CL_Maths->Quaternion_ToMatrix(&QRot, &Xfm);
	}
	
	App->CL_Face->Face_XfmTexture(f, &Xfm);
}

// *************************************************************************
// *							Face_XfmTexture						 	   *
// *************************************************************************
void CL64_Face::Face_XfmTexture(Face* f, const Matrix3d* pXfm)
{
	assert(f != NULL);
	assert(pXfm != NULL);

	App->CL_Maths->XForm3d_Multiply(pXfm, &f->Tex.XfmFaceAngle, &f->Tex.XfmFaceAngle);
}

// *************************************************************************
// *								Face_Scale						 	   *
// *************************************************************************
signed int CL64_Face::Face_Scale(Face* f, const Ogre::Vector3* ScaleVec)
{
	signed int Success;
	//MRB END
	int i;

	assert(f);
	assert(ScaleVec);

	for (i = 0; i < f->NumPoints; i++)
	{
		//no magnitude operation in vec3d
		f->Points[i].x *= ScaleVec->x;
		f->Points[i].y *= ScaleVec->y;
		f->Points[i].z *= ScaleVec->z;
	}
	{
		Ogre::Vector3 OldNormal = f->Tex.VecNormal;
		//MRB BEGIN
		Success = Face_SetPlaneFromFace(f);

		Face_UpdateFaceAngle(f, &OldNormal);
	}
	f->Tex.DirtyFlag = GE_TRUE;
	
	return Success;
}

// *************************************************************************
// *						Face_GetTextureScale					 	   *
// *************************************************************************
void CL64_Face::Face_GetTextureScale(const Face* f, float* pxScale, float* pyScale)
{
	*pxScale = f->Tex.xScale;
	*pyScale = f->Tex.yScale;
}

// *************************************************************************
// *						Face_GetTextureShift					 	   *
// *************************************************************************
void CL64_Face::Face_GetTextureShift(const Face* f, int* pxShift, int* pyShift)
{
	*pxShift = f->Tex.xShift;
	*pyShift = f->Tex.yShift;
}

// *************************************************************************
// *						Face_GetTextureRotate					 	   *
// *************************************************************************
float CL64_Face::Face_GetTextureRotate(const Face* f)
{
	return	f->Tex.Rotate;
}
