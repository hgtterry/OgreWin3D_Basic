/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OW3D_Mesh_Builder

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
#include "BrushTemplate.h"
#include "Structures.cpp"

BrushTemplate::BrushTemplate(void)
{
}

BrushTemplate::~BrushTemplate(void)
{
}

void BrushTemplate::BrushTemplate_ArchDefaults(BrushTemplate_Arch* pArchTemplate)
{
	pArchTemplate->NumSlits = 3;
	pArchTemplate->Thickness = 150;
	pArchTemplate->Width = 100;
	pArchTemplate->Radius = 200;
	pArchTemplate->WallSize = 16;
	pArchTemplate->Style = 0;
	pArchTemplate->EndAngle = 180.0f;
	pArchTemplate->StartAngle = 0.0f;
	pArchTemplate->TCut = false;
	pArchTemplate->Sides = 3;
	pArchTemplate->CW = 0;
	pArchTemplate->Shape = 0;
	pArchTemplate->Radius2 = 64;
	pArchTemplate->Height = 0;
	pArchTemplate->Massive = false;
	pArchTemplate->Steps = false;
}

void BrushTemplate::BrushTemplate_BoxDefaults(BrushTemplate_Box* pBoxTemplate)
{
	pBoxTemplate->Solid = 1;		// hollow
	pBoxTemplate->TCut = false;
	pBoxTemplate->Thickness = 16.0f;
	pBoxTemplate->XSizeBot = 768.0f;
	pBoxTemplate->XSizeTop = 768.0f;
	pBoxTemplate->YSize = 256.0f;
	pBoxTemplate->ZSizeBot = 512.0f;
	pBoxTemplate->ZSizeTop = 512.0f;
}

void BrushTemplate::BrushTemplate_ConeDefaults(BrushTemplate_Cone* pConeTemplate)
{
	pConeTemplate->Style = 0;
	pConeTemplate->Width = 200;
	pConeTemplate->Height = 300;
	pConeTemplate->VerticalStrips = 4;
	pConeTemplate->Thickness = 16;
	pConeTemplate->TCut = false;
}

void BrushTemplate::BrushTemplate_CylinderDefaults(BrushTemplate_Cylinder* pCylinderTemplate)
{
	pCylinderTemplate->BotXOffset = 0.0;
	pCylinderTemplate->BotXSize = 128.0;
	pCylinderTemplate->BotZOffset = 0.0;
	pCylinderTemplate->BotZSize = 128.0;
	pCylinderTemplate->Solid = 0;
	pCylinderTemplate->Thickness = 16.0;
	pCylinderTemplate->TopXOffset = 0.0;
	pCylinderTemplate->TopXSize = 128.0;
	pCylinderTemplate->TopZOffset = 0.0;
	pCylinderTemplate->TopZSize = 128.0;
	pCylinderTemplate->VerticalStripes = 6;
	pCylinderTemplate->YSize = 512.0;
	pCylinderTemplate->RingLength = 0.0;
	pCylinderTemplate->TCut = false;
}

void BrushTemplate::BrushTemplate_SpheroidDefaults(BrushTemplate_Spheroid* pSpheroidTemplate)
{
	pSpheroidTemplate->HorizontalBands = 4;
	pSpheroidTemplate->VerticalBands = 8;
	pSpheroidTemplate->YSize = 256.0;
	pSpheroidTemplate->Solid = 0;
	pSpheroidTemplate->Thickness = 16;
	pSpheroidTemplate->TCut = false;
}

void BrushTemplate::BrushTemplate_StaircaseDefaults(BrushTemplate_Staircase* pStaircaseTemplate)
{
	pStaircaseTemplate->Height = 128.0;
	pStaircaseTemplate->Length = 128.0;
	pStaircaseTemplate->NumberOfStairs = 8;
	pStaircaseTemplate->Width = 64.0;
	pStaircaseTemplate->MakeRamp = false;
	pStaircaseTemplate->TCut = false;
}
#include "Room Builder.h"

// *************************************************************************
// *					BrushTemplate_CreateBox							   *
// *************************************************************************
Brush* BrushTemplate::BrushTemplate_CreateBox(const BrushTemplate_Box* pTemplate)
{
	T_Vec3	Verts[8];
	T_Vec3	FaceVerts[4];
	FaceList* fl;
	Face* f;
	Brush* b;

	fl = App->CL_FaceList->FaceList_Create(6);

	// Vertices 0 to 3 are the 4 corners of the top face
	App->CL_Maths->Vector3_Set(&Verts[0], (float)-(pTemplate->XSizeTop / 2), (float)(pTemplate->YSize / 2), (float)-(pTemplate->ZSizeTop / 2));
	App->CL_Maths->Vector3_Set(&Verts[1], (float)-(pTemplate->XSizeTop / 2), (float)(pTemplate->YSize / 2), (float)(pTemplate->ZSizeTop / 2));
	App->CL_Maths->Vector3_Set(&Verts[2], (float)(pTemplate->XSizeTop / 2), (float)(pTemplate->YSize / 2), (float)(pTemplate->ZSizeTop / 2));
	App->CL_Maths->Vector3_Set(&Verts[3], (float)(pTemplate->XSizeTop / 2), (float)(pTemplate->YSize / 2), (float)-(pTemplate->ZSizeTop / 2));

	// Vertices 4 to 7 are the 4 corners of the bottom face
	App->CL_Maths->Vector3_Set(&Verts[4], (float)-(pTemplate->XSizeBot / 2), (float)-(pTemplate->YSize / 2), (float)-(pTemplate->ZSizeBot / 2));
	App->CL_Maths->Vector3_Set(&Verts[5], (float)(pTemplate->XSizeBot / 2), (float)-(pTemplate->YSize / 2), (float)-(pTemplate->ZSizeBot / 2));
	App->CL_Maths->Vector3_Set(&Verts[6], (float)(pTemplate->XSizeBot / 2), (float)-(pTemplate->YSize / 2), (float)(pTemplate->ZSizeBot / 2));
	App->CL_Maths->Vector3_Set(&Verts[7], (float)-(pTemplate->XSizeBot / 2), (float)-(pTemplate->YSize / 2), (float)(pTemplate->ZSizeBot / 2));

	FaceVerts[3] = Verts[0];
	FaceVerts[2] = Verts[1];
	FaceVerts[1] = Verts[2];
	FaceVerts[0] = Verts[3];

	f = App->CL_X_Face->Face_Create(4, FaceVerts, 0);
	if (f)
	{
		f->Real_Brush_Face_Index = 1;
		App->CL_FaceList->FaceList_AddFace(fl, f);
		App->CL_X_Face->Face_SetTextureLock(f, true);
	}

	FaceVerts[3] = Verts[4];
	FaceVerts[2] = Verts[5];
	FaceVerts[1] = Verts[6];
	FaceVerts[0] = Verts[7];

	f = App->CL_X_Face->Face_Create(4, FaceVerts, 0);
	if (f)
	{
		f->Real_Brush_Face_Index = 2;
		App->CL_FaceList->FaceList_AddFace(fl, f);
		App->CL_X_Face->Face_SetTextureLock(f, true);
	}

	FaceVerts[3] = Verts[1];
	FaceVerts[2] = Verts[7];
	FaceVerts[1] = Verts[6];
	FaceVerts[0] = Verts[2];

	f = App->CL_X_Face->Face_Create(4, FaceVerts, 0);
	if (f)
	{
		f->Real_Brush_Face_Index = 3;
		App->CL_FaceList->FaceList_AddFace(fl, f);
		App->CL_X_Face->Face_SetTextureLock(f, true);
	}

	FaceVerts[3] = Verts[0];
	FaceVerts[2] = Verts[3];
	FaceVerts[1] = Verts[5];
	FaceVerts[0] = Verts[4];

	f = App->CL_X_Face->Face_Create(4, FaceVerts, 0);
	if (f)
	{
		f->Real_Brush_Face_Index = 4;
		App->CL_FaceList->FaceList_AddFace(fl, f);
		App->CL_X_Face->Face_SetTextureLock(f, true);
	}

	FaceVerts[3] = Verts[0];
	FaceVerts[2] = Verts[4];
	FaceVerts[1] = Verts[7];
	FaceVerts[0] = Verts[1];

	f = App->CL_X_Face->Face_Create(4, FaceVerts, 0);
	if (f)
	{
		f->Real_Brush_Face_Index = 5;
		App->CL_FaceList->FaceList_AddFace(fl, f);
		App->CL_X_Face->Face_SetTextureLock(f, true);
	}

	FaceVerts[3] = Verts[3];
	FaceVerts[2] = Verts[2];
	FaceVerts[1] = Verts[6];
	FaceVerts[0] = Verts[5];

	f = App->CL_X_Face->Face_Create(4, FaceVerts, 0);
	if (f)
	{
		f->Real_Brush_Face_Index = 6;
		App->CL_FaceList->FaceList_AddFace(fl, f);
		App->CL_X_Face->Face_SetTextureLock(f, true);
	}

	if (!pTemplate->Solid)
	{
		b = App->CL_X_Brush->Brush_Create(BRUSH_LEAF, fl, 0);
		if (b)
		{
			Brush_SetSubtract(b, pTemplate->TCut); // TODO hgtterry Problem
			App->CL_X_Brush->Brush_SetSheet(b, pTemplate->TSheet);

		}
		return	b;
	}
	else
	{
		// hollow brush
		BrushList* bl = App->CL_X_Brush->BrushList_Create();
		Brush* bh, * bm;

		b = App->CL_X_Brush->Brush_Create(BRUSH_LEAF, fl, 0);
		if (b)
		{
			Brush_SetHollow(b, true);
			Brush_SetHullSize(b, (float)pTemplate->Thickness);
			bh = App->CL_X_Brush->Brush_CreateHollowFromBrush(b);
			if (bh)
			{
				App->CL_X_Brush->Brush_SetHollowCut(bh, true);
				App->CL_X_Brush->BrushList_Append(bl, b);
				App->CL_X_Brush->BrushList_Append(bl, bh);

				bm = App->CL_X_Brush->Brush_Create(BRUSH_MULTI, 0, bl);
				if (bm)
				{
					Brush_SetHollow(bm, true);
					Brush_SetSubtract(bm, pTemplate->TCut);
					Brush_SetHullSize(bm, (float)pTemplate->Thickness);
					return	bm;
				}
			}
			else
			{
				App->CL_X_Brush->Brush_Destroy(&b);
				App->CL_X_Brush->BrushList_Destroy(&bl);
			}
		}
		else
		{
			App->CL_X_Brush->BrushList_Destroy(&bl);
		}
	}

	return	0;
}

// *************************************************************************
// *					BrushTemplate_CreateCylinder					   *
// *************************************************************************
Brush* BrushTemplate::BrushTemplate_CreateCylinder(const BrushTemplate_Cylinder* pTemplate)
{
	double		CurrentXDiameter, CurrentZDiameter;
	double		DeltaXDiameter, DeltaZDiameter;
	double		CurrentXOffset, CurrentZOffset;
	double		DeltaXOffset, DeltaZOffset, sqrcheck;
	double		EllipseZ;
	int			NumVerticalBands, HBand, VBand;
	int			VertexCount = 0;
	T_Vec3* Verts, * TopPoints;
	T_Vec3	Current, Final, Delta;
	Matrix3d YRotation;
	FaceList* fl;
	Face* f;
	Brush* b;

	NumVerticalBands = (int)(pTemplate->VerticalStripes);

	if (NumVerticalBands < 3)
	{
		return	0;
	}


	Verts = (T_Vec3*)App->CL_Maths->Ram_Allocate(sizeof(T_Vec3) * NumVerticalBands * 2);
	TopPoints = (T_Vec3*)App->CL_Maths->Ram_Allocate(sizeof(T_Vec3) * NumVerticalBands);
	fl = App->CL_FaceList->FaceList_Create(NumVerticalBands + 2);

	if (!Verts || !TopPoints || !fl)
	{
		return	0;
	}

	App->CL_Maths->XForm3d_SetIdentity(&YRotation);
	App->CL_Maths->XForm3d_SetYRotation(&YRotation, (M_PI * 2.0f) / (float)NumVerticalBands);

	// Start with the top of cylinder
	CurrentXDiameter = pTemplate->TopXSize;
	CurrentZDiameter = pTemplate->TopZSize;
	DeltaXDiameter = (pTemplate->BotXSize - pTemplate->TopXSize);
	DeltaZDiameter = (pTemplate->BotZSize - pTemplate->TopZSize);

	// Get the offset amounts
	CurrentXOffset = pTemplate->TopXOffset;
	CurrentZOffset = pTemplate->TopZOffset;
	DeltaXOffset = (pTemplate->BotXOffset - pTemplate->TopXOffset);
	DeltaZOffset = (pTemplate->BotZOffset - pTemplate->TopZOffset);

	// Get the band positions and deltas
	App->CL_Maths->Vector3_Set(&Current, (float)(pTemplate->TopXSize / 2), (float)(pTemplate->YSize / 2), 0.0);
	App->CL_Maths->Vector3_Set(&Delta, (float)((pTemplate->BotXSize / 2) - Current.x), (float)(-(pTemplate->YSize / 2) - Current.y), 0.0);

	for (HBand = 0; HBand <= 1; HBand++)
	{
		Final = Current;
		for (VBand = 0; VBand < NumVerticalBands; VBand++)
		{
			// Get the elliptical Z value
			// (x^2/a^2) + (z^2/b^2) = 1
			// z = sqrt(b^2(1 - x^2/a^2))
			sqrcheck = (((CurrentZDiameter / 2) * (CurrentZDiameter / 2))
				* (1.0 - (Final.x * Final.x)
					/ ((CurrentXDiameter / 2) * (CurrentXDiameter / 2))));
			if (sqrcheck < 0.0)
				sqrcheck = 0.0;
			EllipseZ = sqrt(sqrcheck);

			// Check if we need to negate this thing
			if (VBand > (NumVerticalBands / 2))
				EllipseZ = -EllipseZ;

			App->CL_Maths->Vector3_Set
			(
				&Verts[VertexCount],
				(float)(Final.x + CurrentXOffset),
				Final.y,
				(float)(EllipseZ + CurrentZOffset)
			);
			VertexCount++;

			// Rotate the point around the Y to get the next vertical band
			App->CL_Maths->XForm3d_Rotate(&YRotation, &Final, &Final);
		}

		CurrentXDiameter += DeltaXDiameter;
		CurrentZDiameter += DeltaZDiameter;
		CurrentXOffset += DeltaXOffset;
		CurrentZOffset += DeltaZOffset;

		App->CL_Maths->Vector3_Add(&Current, &Delta, &Current);
	}

	for (VBand = 0; VBand < NumVerticalBands; VBand++)
	{
		TopPoints[VBand] = Verts[VBand];
	}
	f = App->CL_X_Face->Face_Create(NumVerticalBands, TopPoints, 0);

	if (f)
	{
		if (pTemplate->Solid > 1)
		{
			App->CL_X_Face->Face_SetFixedHull(f, GE_TRUE);
		}
		App->CL_FaceList->FaceList_AddFace(fl, f);
		App->CL_X_Face->Face_SetTextureLock(f, true);
	}

	for (VBand = NumVerticalBands - 1, HBand = 0; VBand >= 0; VBand--, HBand++)
	{
		TopPoints[HBand] = Verts[VBand + NumVerticalBands];
	}
	f = App->CL_X_Face->Face_Create(NumVerticalBands, TopPoints, 0);

	if (f)
	{
		if (pTemplate->Solid > 1)
		{
			App->CL_X_Face->Face_SetFixedHull(f, GE_TRUE);
		}
		App->CL_FaceList->FaceList_AddFace(fl, f);
		App->CL_X_Face->Face_SetTextureLock(f, true);
	}

	// Generate the polygons
	for (HBand = 0; HBand < 1; HBand++)
	{
		for (VBand = 0; VBand < NumVerticalBands; VBand++)
		{
			T_Vec3 Points[4];
			Points[3] = Verts[(HBand * NumVerticalBands) + VBand];
			Points[2] = Verts[(HBand * NumVerticalBands) + ((VBand + 1) % NumVerticalBands)];
			Points[1] = Verts[((HBand + 1) * NumVerticalBands) + ((VBand + 1) % NumVerticalBands)];
			Points[0] = Verts[((HBand + 1) * NumVerticalBands) + VBand];
			f = App->CL_X_Face->Face_Create(4, Points, 0);

			if (f)
			{
				App->CL_FaceList->FaceList_AddFace(fl, f);
				App->CL_X_Face->Face_SetTextureLock(f, true);
			}
		}
	}
	//	geRam_Free(Verts);
	//	geRam_Free(TopPoints);

	if (!pTemplate->Solid)
	{
		b = App->CL_X_Brush->Brush_Create(BRUSH_LEAF, fl, 0);
		if (b)
		{
			Brush_SetSubtract(b, pTemplate->TCut);
		}
		return	b;
	}
	else
	{
		BrushList* bl = App->CL_X_Brush->BrushList_Create();
		Brush* bh, * bm;

		b = App->CL_X_Brush->Brush_Create(BRUSH_LEAF, fl, 0);
		if (b)
		{
			Brush_SetHollow(b, GE_TRUE);
			Brush_SetHullSize(b, (float)pTemplate->Thickness);
			bh = App->CL_X_Brush->Brush_CreateHollowFromBrush(b);
			if (bh)
			{
				App->CL_X_Brush->Brush_SetHollowCut(bh, GE_TRUE);
				App->CL_X_Brush->BrushList_Append(bl, b);
				App->CL_X_Brush->BrushList_Append(bl, bh);

				bm = App->CL_X_Brush->Brush_Create(BRUSH_MULTI, 0, bl);
				if (bm)
				{
					Brush_SetHollow(bm, GE_TRUE);
					Brush_SetSubtract(bm, pTemplate->TCut);
					Brush_SetHullSize(bm, (float)pTemplate->Thickness);
					return	bm;
				}
			}
			else
			{
				App->CL_X_Brush->Brush_Destroy(&b);
				App->CL_X_Brush->BrushList_Destroy(&bl);
			}
		}
		else
		{
			App->CL_X_Brush->BrushList_Destroy(&bl);
		}
	}

	return	0;
}

// *************************************************************************
// *					BrushTemplate_CreateCone						   *
// *************************************************************************
Brush* BrushTemplate::BrushTemplate_CreateCone(const BrushTemplate_Cone* pTemplate)
{
	int			index, BottomCount;
	T_Vec3		StartPoint, CurPoint, OldPoint, OuterFocus;
	T_Vec3		FaceVerts[3], * BottomVerts;
	FaceList* fl;
	Face* f;
	Brush* b;

	double CurAngle;
	double AngleDeltaDegrees = 360.0f / (float)pTemplate->VerticalStrips;
	double AngleDelta = UNITS_DEGREES_TO_RADIANS(AngleDeltaDegrees);


	fl = App->CL_FaceList->FaceList_Create(pTemplate->VerticalStrips + 1);

	App->CL_Maths->Vector3_Set(&OuterFocus, 0, (float)(pTemplate->Height / 2), 0);
	App->CL_Maths->Vector3_Set(&StartPoint, (float)(pTemplate->Width / 2), (float)-(pTemplate->Height / 2), 0);

	CurPoint = OldPoint = StartPoint;
	BottomVerts = (T_Vec3*)App->CL_Maths->Ram_Allocate(sizeof(T_Vec3) * pTemplate->VerticalStrips);
	BottomVerts[0] = CurPoint;

	CurAngle = BottomCount = 0;
	for (index = 1; index < pTemplate->VerticalStrips; index++)
	{
		//	Rotate around to create our successive points...
		CurAngle += AngleDelta;

		App->CL_Maths->Vector3_Set
		(
			&CurPoint,
			(float)((StartPoint.x * cos(CurAngle)) + (StartPoint.z * sin(CurAngle))),
			StartPoint.y,
			(float)((StartPoint.z * cos(CurAngle)) - (StartPoint.x * sin(CurAngle)))
		);

		FaceVerts[2] = OuterFocus;
		FaceVerts[1] = OldPoint;
		FaceVerts[0] = CurPoint;

		f = App->CL_X_Face->Face_Create(3, FaceVerts, 0);
		if (f)
		{
			App->CL_FaceList->FaceList_AddFace(fl, f);
			App->CL_X_Face->Face_SetTextureLock(f, true);
		}

		OldPoint = CurPoint;

		//	Assign the current point to bottom plane...
		BottomVerts[++BottomCount] = CurPoint;
	}

	//	Create the final polygon...
	CurAngle += AngleDelta;

	App->CL_Maths->Vector3_Set
	(
		&CurPoint,
		(float)((StartPoint.x * cos(CurAngle)) + (StartPoint.z * sin(CurAngle))),
		StartPoint.y,
		(float)((StartPoint.z * cos(CurAngle)) - (StartPoint.x * sin(CurAngle)))
	);

	FaceVerts[2] = OuterFocus;
	FaceVerts[1] = OldPoint;
	FaceVerts[0] = CurPoint;

	f = App->CL_X_Face->Face_Create(3, FaceVerts, 0);
	if (f)
	{
		App->CL_FaceList->FaceList_AddFace(fl, f);
		App->CL_X_Face->Face_SetTextureLock(f, true);
	}

	f = App->CL_X_Face->Face_Create(pTemplate->VerticalStrips, BottomVerts, 0);

	if (f)
	{
		if (pTemplate->Style > 1)	//default to hollow (if they make hollow later)
		{
			App->CL_X_Face->Face_SetFixedHull(f, GE_TRUE);
		}
		App->CL_FaceList->FaceList_AddFace(fl, f);
		App->CL_X_Face->Face_SetTextureLock(f, true);
	}
	//	geRam_Free(BottomVerts);

	if (!pTemplate->Style)
	{
		b = App->CL_X_Brush->Brush_Create(BRUSH_LEAF, fl, 0);
		if (b)
		{
			Brush_SetSubtract(b, pTemplate->TCut);
		}
		return	b;
	}
	else
	{
		BrushList* bl = App->CL_X_Brush->BrushList_Create();
		Brush* bh, * bm;

		b = App->CL_X_Brush->Brush_Create(BRUSH_LEAF, fl, 0);
		if (b)
		{
			Brush_SetHollow(b, GE_TRUE);
			Brush_SetHullSize(b, (float)pTemplate->Thickness);
			bh = App->CL_X_Brush->Brush_CreateHollowFromBrush(b);
			if (bh)
			{
				App->CL_X_Brush->Brush_SetHollowCut(bh, GE_TRUE);
				App->CL_X_Brush->BrushList_Append(bl, b);
				App->CL_X_Brush->BrushList_Append(bl, bh);

				bm = App->CL_X_Brush->Brush_Create(BRUSH_MULTI, 0, bl);
				if (bm)
				{
					Brush_SetHollow(bm, GE_TRUE);
					Brush_SetSubtract(bm, pTemplate->TCut);
					Brush_SetHullSize(bm, (float)pTemplate->Thickness);
					return	bm;
				}
			}
			else
			{
				App->CL_X_Brush->Brush_Destroy(&b);
				App->CL_X_Brush->BrushList_Destroy(&bl);
			}
		}
		else
		{
			App->CL_X_Brush->BrushList_Destroy(&bl);
		}
	}
	return	0;
}

// *************************************************************************
// *					BrushTemplate_CreateStaircase					   *
// *************************************************************************
Brush* BrushTemplate::BrushTemplate_CreateStaircase(const BrushTemplate_Staircase* pTemplate)
{
	int			i;
	float		HalfWidth = (float)(pTemplate->Width / 2);
	float		HalfHeight = (float)(pTemplate->Height / 2);
	float		HalfLength = (float)(pTemplate->Length / 2);
	Brush* b, * b2;
	BrushList* MBList = App->CL_X_Brush->BrushList_Create();
	FaceList* fl;
	Face* f;
	T_Vec3		v, FaceVerts[4];


	if (pTemplate->MakeRamp)
	{
		fl = App->CL_FaceList->FaceList_Create(5);

		App->CL_Maths->Vector3_Set(&(FaceVerts[3]), -HalfWidth, -HalfHeight, HalfLength);
		App->CL_Maths->Vector3_Set(&(FaceVerts[2]), HalfWidth, -HalfHeight, HalfLength);
		App->CL_Maths->Vector3_Set(&(FaceVerts[1]), HalfWidth, HalfHeight, HalfLength);
		App->CL_Maths->Vector3_Set(&(FaceVerts[0]), -HalfWidth, HalfHeight, HalfLength);
		f = App->CL_X_Face->Face_Create(4, FaceVerts, 0);
		if (f)
		{
			App->CL_FaceList->FaceList_AddFace(fl, f);
			App->CL_X_Face->Face_SetTextureLock(f, true);
		}

		App->CL_Maths->Vector3_Set(&(FaceVerts[3]), HalfWidth, -HalfHeight, HalfLength);
		App->CL_Maths->Vector3_Set(&(FaceVerts[2]), -HalfWidth, -HalfHeight, HalfLength);
		App->CL_Maths->Vector3_Set(&(FaceVerts[1]), -HalfWidth, -HalfHeight, -HalfLength);
		App->CL_Maths->Vector3_Set(&(FaceVerts[0]), HalfWidth, -HalfHeight, -HalfLength);
		f = App->CL_X_Face->Face_Create(4, FaceVerts, 0);
		if (f)
		{
			App->CL_FaceList->FaceList_AddFace(fl, f);
			App->CL_X_Face->Face_SetTextureLock(f, true);
		}

		App->CL_Maths->Vector3_Set(&(FaceVerts[3]), -HalfWidth, HalfHeight, HalfLength);
		App->CL_Maths->Vector3_Set(&(FaceVerts[2]), HalfWidth, HalfHeight, HalfLength);
		App->CL_Maths->Vector3_Set(&(FaceVerts[1]), HalfWidth, -HalfHeight, -HalfLength);
		App->CL_Maths->Vector3_Set(&(FaceVerts[0]), -HalfWidth, -HalfHeight, -HalfLength);
		f = App->CL_X_Face->Face_Create(4, FaceVerts, 0);
		if (f)
		{
			App->CL_FaceList->FaceList_AddFace(fl, f);
			App->CL_X_Face->Face_SetTextureLock(f, true);
		}

		App->CL_Maths->Vector3_Set(&(FaceVerts[2]), HalfWidth, HalfHeight, HalfLength);
		App->CL_Maths->Vector3_Set(&(FaceVerts[1]), HalfWidth, -HalfHeight, HalfLength);
		App->CL_Maths->Vector3_Set(&(FaceVerts[0]), HalfWidth, -HalfHeight, -HalfLength);
		f = App->CL_X_Face->Face_Create(3, FaceVerts, 0);
		if (f)
		{
			App->CL_FaceList->FaceList_AddFace(fl, f);
			App->CL_X_Face->Face_SetTextureLock(f, true);
		}

		App->CL_Maths->Vector3_Set(&(FaceVerts[0]), -HalfWidth, HalfHeight, HalfLength);
		App->CL_Maths->Vector3_Set(&(FaceVerts[1]), -HalfWidth, -HalfHeight, HalfLength);
		App->CL_Maths->Vector3_Set(&(FaceVerts[2]), -HalfWidth, -HalfHeight, -HalfLength);
		f = App->CL_X_Face->Face_Create(3, FaceVerts, 0);
		if (f)
		{
			App->CL_FaceList->FaceList_AddFace(fl, f);
			App->CL_X_Face->Face_SetTextureLock(f, true);
		}

		b = App->CL_X_Brush->Brush_Create(BRUSH_LEAF, fl, 0);
	}
	else
	{
		float	StairYSize = (float)pTemplate->Height / (float)pTemplate->NumberOfStairs;
		float	DZ = (float)pTemplate->Length / (float)pTemplate->NumberOfStairs;
		float	ZSize = (float)pTemplate->Length;
		BrushTemplate_Box BoxTemplate;

		BoxTemplate.Solid = 0;
		BoxTemplate.TCut = pTemplate->TCut;
		BoxTemplate.Thickness = 0.0f;
		BoxTemplate.XSizeTop = pTemplate->Width;
		BoxTemplate.XSizeBot = pTemplate->Width;
		BoxTemplate.YSize = StairYSize;

		for (i = 0; i < pTemplate->NumberOfStairs; i++)
		{
			BoxTemplate.ZSizeTop = ZSize;
			BoxTemplate.ZSizeBot = ZSize;
			BoxTemplate.TSheet = GE_FALSE;	// nasty, nasty, nasty
			b2 = BrushTemplate_CreateBox(&BoxTemplate);
			ZSize -= DZ;
			App->CL_Maths->Vector3_Set(&v, 0.0f, i * StairYSize, (i * DZ) / 2);
			App->CL_X_Brush->Brush_Move(b2, &v);
			App->CL_X_Brush->BrushList_Append(MBList, b2);
		}
		b = App->CL_X_Brush->Brush_Create(BRUSH_MULTI, 0, MBList);
	}

	if (b)
	{
		Brush_SetSubtract(b, pTemplate->TCut);
	}

	return	b;
}

// *************************************************************************
// *					BrushTemplate_CreateArch						   *
// *************************************************************************
Brush* BrushTemplate::BrushTemplate_CreateArch(const BrushTemplate_Arch* pTemplate)
{
	Brush* b, * b2;
	BrushList* MBList = App->CL_X_Brush->BrushList_Create();
	FaceList* fl;
	Face* f;
	T_Vec3		FaceVerts[4];

	int		i, NumSlits = pTemplate->NumSlits;
	// changed QD 11/03
	int		NumCrossSections = NumSlits;//+2;
	float Height = pTemplate->Height;
	float HeightDelta = 0;
	int		NumSides = pTemplate->Sides;
	float Radius2 = pTemplate->Radius2;
	int		CW = pTemplate->CW;
	signed int Steps = pTemplate->Steps;
	signed int Massive = pTemplate->Massive;
	// end change
	float	Thickness = pTemplate->Thickness;
	float	Width = pTemplate->Width;
	float	InnerRadius = pTemplate->Radius;
	//	geFloat	WallSize			=pTemplate->WallSize;
	double	StartAngleDegrees = pTemplate->StartAngle;
	double	EndAngleDegrees = pTemplate->EndAngle;
	double	AngleDelta = 0;
	double	CurAngle = 0;
	double	StartAngle = Units_DegreesToRadians(StartAngleDegrees);
	double	EndAngle = Units_DegreesToRadians(EndAngleDegrees);
	double	Temp;
	T_Vec3	TopInnerPoint;
	T_Vec3	TopOuterPoint;
	T_Vec3	FinalTopInnerPoint;
	T_Vec3	FinalTopOuterPoint;
	T_Vec3	FinalBottomInnerPoint;
	T_Vec3	FinalBottomOuterPoint;
	T_Vec3	OldTopInner;
	T_Vec3	OldTopOuter;
	T_Vec3	OldBottomInner;
	T_Vec3	OldBottomOuter;

	//If angles are equal, we have an empty shape...
	if (StartAngle == EndAngle)
	{
		return	0;
	}

	//	Put the angles in order...
	if (StartAngle > EndAngle)
	{
		Temp = StartAngle;
		StartAngle = EndAngle;
		EndAngle = Temp;
	}

	AngleDelta = (EndAngle - StartAngle) / (NumCrossSections); // changed QD 11/03
	CurAngle = StartAngle + AngleDelta;

	// changed QD 11/03
	HeightDelta = Height / NumCrossSections;
	if (CW)
		HeightDelta = -HeightDelta;

	////////////////
	// Shape = Round
	if (pTemplate->Shape > 0)
	{
		int			index, BottomCount;
		T_Vec3		StartPoint, CurPoint;
		T_Vec3* CrossVerts, * OldVerts, * StartVerts;

		double CurAngle2;
		double AngleDelta2Degrees = 360.0f / (float)NumSides;
		double AngleDelta2 = Units_DegreesToRadians(AngleDelta2Degrees);

		App->CL_Maths->Vector3_Set(&StartPoint, -(float)Radius2, 0.0, 0.0);

		CurPoint = StartPoint;
		CrossVerts = (T_Vec3*)App->CL_Maths->Ram_Allocate(sizeof(T_Vec3) * NumSides);
		OldVerts = (T_Vec3*)App->CL_Maths->Ram_Allocate(sizeof(T_Vec3) * NumSides);
		StartVerts = (T_Vec3*)App->CL_Maths->Ram_Allocate(sizeof(T_Vec3) * NumSides);
		StartVerts[0] = CurPoint;
		StartVerts[0].x += InnerRadius + Radius2;
		//		if(CW)
		//			StartVerts[0].Z+=Height;
		OldVerts[0] = StartVerts[0];

		CurAngle2 = BottomCount = 0;
		for (index = 1; index < NumSides; index++)
		{
			//	Rotate around to create our successive points...
			CurAngle2 += AngleDelta2;

			App->CL_Maths->Vector3_Set
			(
				&CurPoint,
				(float)((StartPoint.x * cos(CurAngle2)) + (StartPoint.z * sin(CurAngle2))),
				StartPoint.y,
				(float)((StartPoint.z * cos(CurAngle2)) - (StartPoint.x * sin(CurAngle2)))
			);

			CurPoint.x += InnerRadius + Radius2;
			//	CurPoint.Z+=Height;

			App->CL_Maths->Vector3_Set
			(
				&(StartVerts[index]),
				(float)((CurPoint.x * cos(StartAngle)) - (CurPoint.y * sin(StartAngle))),
				(float)((CurPoint.x * sin(StartAngle)) + (CurPoint.y * cos(StartAngle))),
				CurPoint.z
			);

			OldVerts[index] = StartVerts[index];

		}

		/////////////////////////////////
		for (i = 0; i < NumCrossSections; i++) //changed QD 11/03
		{
			for (index = 0; index < NumSides; index++)
			{
				App->CL_Maths->Vector3_Set
				(
					&(CrossVerts[index]),
					(float)((StartVerts[index].x * cos(CurAngle)) - (StartVerts[index].y * sin(CurAngle))),
					(float)((StartVerts[index].x * sin(CurAngle)) + (StartVerts[index].y * cos(CurAngle))),
					StartVerts[index].z + (i + 1) * HeightDelta
				);
			}

			CurAngle += AngleDelta;

			//			if(InnerRadius>0.0f)
			fl = App->CL_FaceList->FaceList_Create(NumSides + 2);
			//			else
			//				fl	=FaceList_Create(2*NumSides);

						// Sides ...
			for (index = 1; index < NumSides - 1; index++)
			{

				//	geVec3d FaceVerts2[3];
				FaceVerts[0] = CrossVerts[index];//FinalTopInnerPoint;
				FaceVerts[1] = CrossVerts[index + 1];//FinalTopOuterPoint;
				FaceVerts[2] = OldVerts[index + 1];
				FaceVerts[3] = OldVerts[index];
				f = App->CL_X_Face->Face_Create(4, FaceVerts, 0);

				if (f)
				{
					App->CL_FaceList->FaceList_AddFace(fl, f);
					App->CL_X_Face->Face_SetTextureLock(f, true);
				}

				/*				FaceVerts2[0] = CrossVerts[index];//FinalTopInnerPoint;
								//FaceVerts2[1] = CrossVerts[index+1];//FinalTopOuterPoint;
								FaceVerts2[1] = OldVerts[index+1];
								FaceVerts2[2] = OldVerts[index];
								f = Face_Create(3, FaceVerts2, 0);

								if(f)
								{
									FaceList_AddFace(fl, f);
								}*/
			}

			//first side, last side are triangles if radius=0
			if (InnerRadius > 0.0f)
			{
				//first side
			//	geVec3d FaceVerts2[3];
				FaceVerts[0] = CrossVerts[0];
				FaceVerts[1] = CrossVerts[1];
				FaceVerts[2] = OldVerts[1];
				FaceVerts[3] = OldVerts[0];
				f = App->CL_X_Face->Face_Create(4, FaceVerts, 0);

				if (f)
				{
					App->CL_FaceList->FaceList_AddFace(fl, f);
					App->CL_X_Face->Face_SetTextureLock(f, true);
				}

				/*			FaceVerts2[0] = CrossVerts[0];
							//FaceVerts2[1] = CrossVerts[1];
							FaceVerts2[1] = OldVerts[1];
							FaceVerts2[2] = OldVerts[0];
							f = Face_Create(3, FaceVerts2, 0);

							if(f)
							{
								FaceList_AddFace(fl, f);
							}*/

							//last side
				FaceVerts[0] = CrossVerts[NumSides - 1];
				FaceVerts[1] = CrossVerts[0];
				FaceVerts[2] = OldVerts[0];
				FaceVerts[3] = OldVerts[NumSides - 1];
				f = App->CL_X_Face->Face_Create(4, FaceVerts, 0);

				if (f)
				{
					App->CL_FaceList->FaceList_AddFace(fl, f);
					App->CL_X_Face->Face_SetTextureLock(f, true);
				}

				/*				FaceVerts2[0] = CrossVerts[NumSides-1];
								//FaceVerts2[1] = CrossVerts[0];
								FaceVerts2[1] = OldVerts[0];
								FaceVerts2[2] = OldVerts[NumSides-1];
								f = Face_Create(3, FaceVerts2, 0);

								if(f)
								{
									FaceList_AddFace(fl, f);
								}*/
			}
			else
			{
				T_Vec3 FaceVerts2[3];

				FaceVerts2[0] = CrossVerts[0];
				FaceVerts2[1] = CrossVerts[1];
				FaceVerts2[2] = OldVerts[1];
				f = App->CL_X_Face->Face_Create(3, FaceVerts2, 0);

				if (f)
				{
					App->CL_FaceList->FaceList_AddFace(fl, f);
					App->CL_X_Face->Face_SetTextureLock(f, true);
				}

				FaceVerts2[0] = CrossVerts[NumSides - 1];
				FaceVerts2[1] = CrossVerts[0];
				FaceVerts2[2] = OldVerts[NumSides - 1];
				f = App->CL_X_Face->Face_Create(3, FaceVerts2, 0);

				if (f)
				{
					App->CL_FaceList->FaceList_AddFace(fl, f);
					App->CL_X_Face->Face_SetTextureLock(f, true);
				}
			}

			//make the end faces
			f = App->CL_X_Face->Face_Create(NumSides, OldVerts, 0);

			if (f)
			{
				if (pTemplate->Style < 2)	//default to hollow (if they make hollow later)
				{
					if (i)
					{
						App->CL_X_Face->Face_SetFixedHull(f, GE_TRUE);
					}
				}
				else
				{
					App->CL_X_Face->Face_SetFixedHull(f, GE_TRUE);
				}
				App->CL_FaceList->FaceList_AddFace(fl, f);
				App->CL_X_Face->Face_SetTextureLock(f, true);
			}

			//need reverse order for the other end
			for (index = 0; index < NumSides; index++)
				OldVerts[NumSides - index - 1] = CrossVerts[index];

			f = App->CL_X_Face->Face_Create(NumSides, OldVerts, 0);

			if (f)
			{
				if (pTemplate->Style < 2)	//default to hollow (if they make hollow later)
				{
					if (i < (NumCrossSections - 1)) // changed QD
					{
						App->CL_X_Face->Face_SetFixedHull(f, GE_TRUE);
					}
				}
				else
				{
					App->CL_X_Face->Face_SetFixedHull(f, GE_TRUE);
				}
				App->CL_FaceList->FaceList_AddFace(fl, f);
				App->CL_X_Face->Face_SetTextureLock(f, true);
			}

			if (!pTemplate->Style)
			{
				b2 = App->CL_X_Brush->Brush_Create(BRUSH_LEAF, fl, 0);
				if (b2)
				{
					Brush_SetSubtract(b2, pTemplate->TCut);
				}
				App->CL_X_Brush->BrushList_Append(MBList, b2);
			}
			else
			{
				BrushList* bl = App->CL_X_Brush->BrushList_Create();
				Brush* bh, * bm;

				b2 = App->CL_X_Brush->Brush_Create(BRUSH_LEAF, fl, 0);
				if (b2)
				{
					Brush_SetHollow(b2, GE_TRUE);
					Brush_SetHullSize(b2, pTemplate->WallSize);
					bh = App->CL_X_Brush->Brush_CreateHollowFromBrush(b2);
					if (bh)
					{
						App->CL_X_Brush->Brush_SetHollowCut(bh, GE_TRUE);
						App->CL_X_Brush->BrushList_Append(bl, b2);
						App->CL_X_Brush->BrushList_Append(bl, bh);

						bm = App->CL_X_Brush->Brush_Create(BRUSH_MULTI, 0, bl);
						if (bm)
						{
							Brush_SetHollow(bm, GE_TRUE);
							Brush_SetSubtract(bm, pTemplate->TCut);
							Brush_SetHullSize(bm, pTemplate->WallSize);

							App->CL_X_Brush->BrushList_Append(MBList, bm);
						}
					}
					else
					{
						App->CL_X_Brush->Brush_Destroy(&b2);
						App->CL_X_Brush->BrushList_Destroy(&bl);
					}
				}
				else
				{
					App->CL_X_Brush->BrushList_Destroy(&bl);
				}
			}

			//	Set old points...
			for (index = 0; index < NumSides; index++)
				OldVerts[index] = CrossVerts[index];

		}

		//free allocated verts
//		geRam_Free(StartVerts);
//		geRam_Free(OldVerts);
//		geRam_Free(CrossVerts);
	}
	//////////////////////
	// Shape = Rectangular
	else
	{
		// end change
		App->CL_Maths->Vector3_Set(&TopInnerPoint, (float)InnerRadius, 0.0, (float)(Width / 2));
		App->CL_Maths->Vector3_Set(&TopOuterPoint, (float)(InnerRadius + Thickness), 0.0, (float)(Width / 2));

		//	Create first cross section of 4 vertices ( outer face @ start angle)...
		App->CL_Maths->Vector3_Set
		(
			&FinalTopInnerPoint,
			(float)((TopInnerPoint.x * cos(StartAngle)) - (TopInnerPoint.y * sin(StartAngle))),
			(float)((TopInnerPoint.x * sin(StartAngle)) + (TopInnerPoint.y * cos(StartAngle))),
			TopInnerPoint.z
		);
		App->CL_Maths->Vector3_Set
		(
			&FinalTopOuterPoint,
			(float)((TopOuterPoint.x * cos(StartAngle)) - (TopInnerPoint.y * sin(StartAngle))),
			(float)((TopOuterPoint.x * sin(StartAngle)) + (TopInnerPoint.y * cos(StartAngle))),
			TopOuterPoint.z
		);

		FinalBottomInnerPoint = FinalTopInnerPoint;
		FinalBottomInnerPoint.z = -FinalTopInnerPoint.z;
		FinalBottomOuterPoint = FinalTopOuterPoint;
		FinalBottomOuterPoint.z = -FinalTopOuterPoint.z;
		// changed QD 11/03
		if (CW)
		{
			FinalTopInnerPoint.z += Height;
			FinalTopOuterPoint.z += Height;
			if (!Massive)
			{
				FinalBottomInnerPoint.z += Height;
				FinalBottomOuterPoint.z += Height;
			}
			else if (Steps)
			{
				FinalBottomInnerPoint.z -= HeightDelta;
				FinalBottomOuterPoint.z -= HeightDelta;
			}
		}
		// end change
		OldTopInner = FinalTopInnerPoint;
		OldTopOuter = FinalTopOuterPoint;
		OldBottomInner = FinalBottomInnerPoint;
		OldBottomOuter = FinalBottomOuterPoint;

		//Create the other cross sections and assign verts to polys after each...
		for (i = 0; i < NumCrossSections; i++) // changed QD 11/03
		{
			App->CL_Maths->Vector3_Set
			(
				&FinalTopInnerPoint,
				(float)((TopInnerPoint.x * cos(CurAngle)) - (TopInnerPoint.y * sin(CurAngle))),
				(float)((TopInnerPoint.x * sin(CurAngle)) + (TopInnerPoint.y * cos(CurAngle))),
				TopInnerPoint.z
			);
			App->CL_Maths->Vector3_Set
			(
				&FinalTopOuterPoint,
				(float)((TopOuterPoint.x * cos(CurAngle)) - (TopInnerPoint.y * sin(CurAngle))),
				(float)((TopOuterPoint.x * sin(CurAngle)) + (TopInnerPoint.y * cos(CurAngle))),
				TopOuterPoint.z
			);
			FinalBottomInnerPoint = FinalTopInnerPoint;
			FinalBottomInnerPoint.z = -FinalTopInnerPoint.z;
			// changed QD 11/03
			if (CW)
			{
				if (!Massive)
					FinalBottomInnerPoint.z += Height;
				else if (Steps)
					FinalBottomInnerPoint.z -= HeightDelta;
				FinalTopInnerPoint.z += Height;
			}
			if (!Steps)
			{
				if (!Massive)
					FinalBottomInnerPoint.z += ((i + 1) * HeightDelta);
				FinalTopInnerPoint.z += ((i + 1) * HeightDelta);


			}
			else if (i > 0)
			{
				if (!Massive)
					FinalBottomInnerPoint.z += ((i)*HeightDelta);
				FinalTopInnerPoint.z += ((i)*HeightDelta);
			}
			// end change
			FinalBottomOuterPoint = FinalTopOuterPoint;
			FinalBottomOuterPoint.z = -FinalTopOuterPoint.z;
			// changed QD 11/03
			if (CW)
			{
				if (!Massive)
					FinalBottomOuterPoint.z += Height;
				else if (Steps)
					FinalBottomOuterPoint.z -= HeightDelta;
				FinalTopOuterPoint.z += Height;
			}
			if (!Steps)
			{
				if (!Massive)
					FinalBottomOuterPoint.z += ((i + 1) * HeightDelta);
				FinalTopOuterPoint.z += ((i + 1) * HeightDelta);
			}
			else if (i > 0)//Steps&&i>0)
			{
				if (!Massive)
					FinalBottomOuterPoint.z += ((i)*HeightDelta);
				FinalTopOuterPoint.z += ((i)*HeightDelta);
			}
			// end change

			CurAngle += AngleDelta;

			// changed QD 11/03
			if (InnerRadius > 0.0f)
			{
				if (Height > 0.0f && !Steps)
				{
					if (Massive)
						fl = App->CL_FaceList->FaceList_Create(7);
					else
						fl = App->CL_FaceList->FaceList_Create(8);
				}
				else
				{
					fl = App->CL_FaceList->FaceList_Create(6);
				}
			}
			else
			{
				if (Height > 0.0f && !Steps)

				{
					if (Massive)
						fl = App->CL_FaceList->FaceList_Create(6);
					else
						fl = App->CL_FaceList->FaceList_Create(7);
				}
				else
				{
					fl = App->CL_FaceList->FaceList_Create(5);
				}
			}

			//Assign points to the 4 outer poly faces...

				//Top face...
			if (Height > 0.0f && !Steps)
			{
				if (CW)
				{
					FaceVerts[0] = FinalTopInnerPoint;
					FaceVerts[1] = FinalTopOuterPoint;
					FaceVerts[2] = OldTopInner;
					f = App->CL_X_Face->Face_Create(3, FaceVerts, 0);
					if (f)
					{
						App->CL_FaceList->FaceList_AddFace(fl, f);
						App->CL_X_Face->Face_SetTextureLock(f, true);
					}

					FaceVerts[0] = FinalTopOuterPoint;
					FaceVerts[1] = OldTopOuter;
					FaceVerts[2] = OldTopInner;
					f = App->CL_X_Face->Face_Create(3, FaceVerts, 0);
					if (f)
					{
						App->CL_FaceList->FaceList_AddFace(fl, f);
						App->CL_X_Face->Face_SetTextureLock(f, true);
					}
				}
				else
				{
					FaceVerts[0] = FinalTopInnerPoint;
					FaceVerts[1] = FinalTopOuterPoint;
					FaceVerts[2] = OldTopOuter;
					f = App->CL_X_Face->Face_Create(3, FaceVerts, 0);
					if (f)
					{
						App->CL_FaceList->FaceList_AddFace(fl, f);
						App->CL_X_Face->Face_SetTextureLock(f, true);
					}

					FaceVerts[0] = FinalTopInnerPoint;
					FaceVerts[1] = OldTopOuter;
					FaceVerts[2] = OldTopInner;
					f = App->CL_X_Face->Face_Create(3, FaceVerts, 0);
					if (f)
					{
						App->CL_FaceList->FaceList_AddFace(fl, f);
						App->CL_X_Face->Face_SetTextureLock(f, true);
					}
				}
			}
			else
			{
				if (InnerRadius > 0.0f)
				{
					FaceVerts[0] = FinalTopInnerPoint;
					FaceVerts[1] = FinalTopOuterPoint;
					FaceVerts[2] = OldTopOuter;
					FaceVerts[3] = OldTopInner;
					f = App->CL_X_Face->Face_Create(4, FaceVerts, 0);
				}
				else
				{
					FaceVerts[0] = FinalTopInnerPoint;
					FaceVerts[1] = FinalTopOuterPoint;
					FaceVerts[2] = OldTopOuter;
					f = App->CL_X_Face->Face_Create(3, FaceVerts, 0);
				}

				if (f)
				{
					App->CL_FaceList->FaceList_AddFace(fl, f);
					App->CL_X_Face->Face_SetTextureLock(f, true);
				}
			}


			//	Bottom face...
			if (Height > 0.0f && !Steps && !Massive)
			{
				if (CW)
				{
					FaceVerts[2] = FinalBottomInnerPoint;
					FaceVerts[1] = FinalBottomOuterPoint;
					FaceVerts[0] = OldBottomOuter;
					f = App->CL_X_Face->Face_Create(3, FaceVerts, 0);
					if (f)
					{
						App->CL_FaceList->FaceList_AddFace(fl, f);
						App->CL_X_Face->Face_SetTextureLock(f, true);
					}

					FaceVerts[2] = FinalBottomInnerPoint;
					FaceVerts[1] = OldBottomOuter;
					FaceVerts[0] = OldBottomInner;
					f = App->CL_X_Face->Face_Create(3, FaceVerts, 0);
					if (f)
					{
						App->CL_FaceList->FaceList_AddFace(fl, f);
						App->CL_X_Face->Face_SetTextureLock(f, true);
					}
				}
				else
				{
					FaceVerts[2] = FinalBottomInnerPoint;
					FaceVerts[1] = FinalBottomOuterPoint;
					FaceVerts[0] = OldBottomInner;
					f = App->CL_X_Face->Face_Create(3, FaceVerts, 0);
					if (f)
					{
						App->CL_FaceList->FaceList_AddFace(fl, f);
						App->CL_X_Face->Face_SetTextureLock(f, true);
					}

					FaceVerts[2] = FinalBottomOuterPoint;
					FaceVerts[1] = OldBottomOuter;
					FaceVerts[0] = OldBottomInner;
					f = App->CL_X_Face->Face_Create(3, FaceVerts, 0);
					if (f)
					{
						App->CL_FaceList->FaceList_AddFace(fl, f);
						App->CL_X_Face->Face_SetTextureLock(f, true);
					}
				}
			}
			else
			{
				if (InnerRadius > 0.0f)
				{
					FaceVerts[3] = FinalBottomInnerPoint;
					FaceVerts[2] = FinalBottomOuterPoint;
					FaceVerts[1] = OldBottomOuter;
					FaceVerts[0] = OldBottomInner;
					f = App->CL_X_Face->Face_Create(4, FaceVerts, 0);
				}
				else
				{
					FaceVerts[2] = FinalBottomInnerPoint;
					FaceVerts[1] = FinalBottomOuterPoint;
					FaceVerts[0] = OldBottomOuter;
					f = App->CL_X_Face->Face_Create(3, FaceVerts, 0);
				}
				if (f)
				{
					App->CL_FaceList->FaceList_AddFace(fl, f);
					App->CL_X_Face->Face_SetTextureLock(f, true);
				}
			}

			if (InnerRadius > 0.0f)
			{
				//	Inner side face...
				FaceVerts[0] = FinalTopInnerPoint;
				FaceVerts[1] = OldTopInner;
				FaceVerts[2] = OldBottomInner;
				FaceVerts[3] = FinalBottomInnerPoint;
				f = App->CL_X_Face->Face_Create(4, FaceVerts, 0);
				if (f)
				{
					App->CL_FaceList->FaceList_AddFace(fl, f);
					App->CL_X_Face->Face_SetTextureLock(f, true);
				}
			}
			// end change

						//	Outer side face...
			FaceVerts[3] = FinalTopOuterPoint;
			FaceVerts[2] = OldTopOuter;
			FaceVerts[1] = OldBottomOuter;
			FaceVerts[0] = FinalBottomOuterPoint;
			f = App->CL_X_Face->Face_Create(4, FaceVerts, 0);
			if (f)
			{
				App->CL_FaceList->FaceList_AddFace(fl, f);
				App->CL_X_Face->Face_SetTextureLock(f, true);
			}

			//make the end faces
			FaceVerts[0] = OldTopOuter;
			FaceVerts[1] = OldBottomOuter;
			FaceVerts[2] = OldBottomInner;
			FaceVerts[3] = OldTopInner;
			f = App->CL_X_Face->Face_Create(4, FaceVerts, 0);

			if (f)
			{
				if (pTemplate->Style < 2)	//default to hollow (if they make hollow later)
				{
					if (i)
					{
						App->CL_X_Face->Face_SetFixedHull(f, GE_TRUE);
					}
				}
				else
				{
					App->CL_X_Face->Face_SetFixedHull(f, GE_TRUE);
				}

				App->CL_FaceList->FaceList_AddFace(fl, f);
				App->CL_X_Face->Face_SetTextureLock(f, true);
			}

			FaceVerts[3] = FinalTopOuterPoint;
			FaceVerts[2] = FinalBottomOuterPoint;
			FaceVerts[1] = FinalBottomInnerPoint;
			FaceVerts[0] = FinalTopInnerPoint;
			f = App->CL_X_Face->Face_Create(4, FaceVerts, 0);

			if (f)
			{
				if (pTemplate->Style < 2)	//default to hollow (if they make hollow later)
				{
					if (i < (NumCrossSections - 1)) // changed QD 11/03
					{
						App->CL_X_Face->Face_SetFixedHull(f, GE_TRUE);
					}
				}
				else
				{
					App->CL_X_Face->Face_SetFixedHull(f, GE_TRUE);
				}

				App->CL_FaceList->FaceList_AddFace(fl, f);
				App->CL_X_Face->Face_SetTextureLock(f, true);
			}

			if (!pTemplate->Style)
			{
				b2 = App->CL_X_Brush->Brush_Create(BRUSH_LEAF, fl, 0);
				if (b2)
				{
					Brush_SetSubtract(b2, pTemplate->TCut);
				}
				App->CL_X_Brush->BrushList_Append(MBList, b2);
			}
			else
			{
				BrushList* bl = App->CL_X_Brush->BrushList_Create();
				Brush* bh, * bm;

				b2 = App->CL_X_Brush->Brush_Create(BRUSH_LEAF, fl, 0);
				if (b2)
				{
					Brush_SetHollow(b2, GE_TRUE);
					Brush_SetHullSize(b2, pTemplate->WallSize);
					bh = App->CL_X_Brush->Brush_CreateHollowFromBrush(b2);
					if (bh)
					{
						App->CL_X_Brush->Brush_SetHollowCut(bh, GE_TRUE);
						App->CL_X_Brush->BrushList_Append(bl, b2);
						App->CL_X_Brush->BrushList_Append(bl, bh);

						bm = App->CL_X_Brush->Brush_Create(BRUSH_MULTI, 0, bl);
						if (bm)
						{
							Brush_SetHollow(bm, GE_TRUE);
							Brush_SetSubtract(bm, pTemplate->TCut);
							Brush_SetHullSize(bm, pTemplate->WallSize);

							App->CL_X_Brush->BrushList_Append(MBList, bm);
						}
					}
					else
					{
						App->CL_X_Brush->Brush_Destroy(&b2);
						App->CL_X_Brush->BrushList_Destroy(&bl);
					}
				}
				else
				{
					App->CL_X_Brush->BrushList_Destroy(&bl);
				}
			}

			//	Set old points...
			OldTopInner = FinalTopInnerPoint;
			OldTopOuter = FinalTopOuterPoint;
			OldBottomInner = FinalBottomInnerPoint;
			OldBottomOuter = FinalBottomOuterPoint;
			// changed QD 11/03
			if (Steps)
			{
				OldTopInner.z += HeightDelta;
				OldTopOuter.z += HeightDelta;
				if (!Massive)
				{
					OldBottomInner.z += HeightDelta;
					OldBottomOuter.z += HeightDelta;
				}
			}
			// end change
		}
	}
	b = App->CL_X_Brush->Brush_Create(BRUSH_MULTI, 0, MBList);

	if (b)
	{
		Brush_SetSubtract(b, pTemplate->TCut);
	}

	return	b;
}
