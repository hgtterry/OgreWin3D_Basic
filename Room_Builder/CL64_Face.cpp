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

const Ogre::Vector3	VecOrigin = { 0.0f, 0.0f, 0.0f };

#define	VCOMPARE_EPSILON			(0.00001f)
#define MAX_POINTS					64
#define	ON_EPSILON					(0.1f)
#define	VectorToSUB(a, b)			(*((((geFloat *)(&a))) + (b)))
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
