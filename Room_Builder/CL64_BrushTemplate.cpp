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
#include "CL64_BrushTemplate.h"

CL64_BrushTemplate::CL64_BrushTemplate(void)
{
}

CL64_BrushTemplate::~CL64_BrushTemplate(void)
{
}

void CL64_BrushTemplate::BrushTemplate_ArchDefaults(BrushTemplate_Arch* pArchTemplate)
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

void CL64_BrushTemplate::BrushTemplate_BoxDefaults(BrushTemplate_Box* pBoxTemplate)
{
	pBoxTemplate->Solid = 1;		// hollow
	pBoxTemplate->TCut = false;
	// emperically derived default sizes
	pBoxTemplate->Thickness = 16.0f;
	pBoxTemplate->XSizeBot = 680.0f;
	pBoxTemplate->XSizeTop = 680.0f;
	pBoxTemplate->YSize = 360.0f;
	pBoxTemplate->ZSizeBot = 560.0f;
	pBoxTemplate->ZSizeTop = 560.0f;
}

void CL64_BrushTemplate::BrushTemplate_ConeDefaults(BrushTemplate_Cone* pConeTemplate)
{
	pConeTemplate->Style = 0;
	pConeTemplate->Width = 200;
	pConeTemplate->Height = 300;
	pConeTemplate->VerticalStrips = 4;
	pConeTemplate->Thickness = 16;
	pConeTemplate->TCut = false;
}

void CL64_BrushTemplate::BrushTemplate_CylinderDefaults(BrushTemplate_Cylinder* pCylinderTemplate)
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

void CL64_BrushTemplate::BrushTemplate_SpheroidDefaults(BrushTemplate_Spheroid* pSpheroidTemplate)
{
	pSpheroidTemplate->HorizontalBands = 4;
	pSpheroidTemplate->VerticalBands = 8;
	pSpheroidTemplate->YSize = 256.0;
	pSpheroidTemplate->Solid = 0;
	pSpheroidTemplate->Thickness = 16;
	pSpheroidTemplate->TCut = false;
}

void CL64_BrushTemplate::BrushTemplate_StaircaseDefaults(BrushTemplate_Staircase* pStaircaseTemplate)
{
	pStaircaseTemplate->Height = 128.0;
	pStaircaseTemplate->Length = 128.0;
	pStaircaseTemplate->NumberOfStairs = 8;
	pStaircaseTemplate->Width = 64.0;
	pStaircaseTemplate->MakeRamp = false;
	pStaircaseTemplate->TCut = false;
}
#include "Room Builder.h"
Brush* CL64_BrushTemplate::BrushTemplate_CreateBox(const BrushTemplate_Box* pTemplate)
{
	Ogre::Vector3	Verts[8];
	Ogre::Vector3	FaceVerts[4];
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

	f = App->CL_Face->Face_Create(4, FaceVerts, 0);
	if (f)
	{
		App->CL_FaceList->FaceList_AddFace(fl, f);
	}

	FaceVerts[3] = Verts[4];
	FaceVerts[2] = Verts[5];
	FaceVerts[1] = Verts[6];
	FaceVerts[0] = Verts[7];

	f = App->CL_Face->Face_Create(4, FaceVerts, 0);
	if (f)
	{
		App->CL_FaceList->FaceList_AddFace(fl, f);
	}

	FaceVerts[3] = Verts[1];
	FaceVerts[2] = Verts[7];
	FaceVerts[1] = Verts[6];
	FaceVerts[0] = Verts[2];

	f = App->CL_Face->Face_Create(4, FaceVerts, 0);
	if (f)
	{
		App->CL_FaceList->FaceList_AddFace(fl, f);
	}

	FaceVerts[3] = Verts[0];
	FaceVerts[2] = Verts[3];
	FaceVerts[1] = Verts[5];
	FaceVerts[0] = Verts[4];

	f = App->CL_Face->Face_Create(4, FaceVerts, 0);
	if (f)
	{
		App->CL_FaceList->FaceList_AddFace(fl, f);
	}

	FaceVerts[3] = Verts[0];
	FaceVerts[2] = Verts[4];
	FaceVerts[1] = Verts[7];
	FaceVerts[0] = Verts[1];

	f = App->CL_Face->Face_Create(4, FaceVerts, 0);
	if (f)
	{
		App->CL_FaceList->FaceList_AddFace(fl, f);
	}

	FaceVerts[3] = Verts[3];
	FaceVerts[2] = Verts[2];
	FaceVerts[1] = Verts[6];
	FaceVerts[0] = Verts[5];

	f = App->CL_Face->Face_Create(4, FaceVerts, 0);
	if (f)
	{
		App->CL_FaceList->FaceList_AddFace(fl, f);
	}

	if (!pTemplate->Solid)
	{
		b = App->CL_Brush->Brush_Create(BRUSH_LEAF, fl, 0);
		if (b)
		{
			Brush_SetSubtract(b, pTemplate->TCut); // hgtterry Problem
			App->CL_Brush->Brush_SetSheet(b, pTemplate->TSheet);
			
		}
		return	b;
	}
	else
	{
		// hollow brush
		BrushList* bl = App->CL_Brush->BrushList_Create();
		Brush* bh, * bm;

		b = App->CL_Brush->Brush_Create(BRUSH_LEAF, fl, 0);
		if (b)
		{
			Brush_SetHollow(b, true);
			Brush_SetHullSize(b, (float)pTemplate->Thickness);
			bh = App->CL_Brush->Brush_CreateHollowFromBrush(b);
			if (bh)
			{
				App->CL_Brush->Brush_SetHollowCut(bh, true);
				App->CL_Brush->BrushList_Append(bl, b);
				App->CL_Brush->BrushList_Append(bl, bh);

				bm = App->CL_Brush->Brush_Create(BRUSH_MULTI, 0, bl);
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
				App->CL_Brush->Brush_Destroy(&b);
				App->CL_Brush->BrushList_Destroy(&bl);
			}
		}
		else
		{
			App->CL_Brush->BrushList_Destroy(&bl);
		}
	}

	return	0;
}
