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

#pragma once

//#include "CL64_Brush.h"

typedef struct
{
	int			NumSlits;
	float		Thickness;
	float		Width;
	float		Radius;
	float		WallSize;
	int			Style;
	float		EndAngle;
	float		StartAngle;
	signed int	TCut;
	int			Sides;
	int			CW;
	int			Shape;
	float		Radius2;
	float		Height;
	signed int	Massive;
	signed int	Steps;
} BrushTemplate_Arch;

typedef struct
{
	int			Solid;
	signed int	TCut;
	signed int	TSheet;
	float		Thickness;
	float		XSizeTop;
	float		XSizeBot;
	float		YSize;
	float		ZSizeTop;
	float		ZSizeBot;
} BrushTemplate_Box;

typedef struct
{
	int			Style;
	float		Width;
	float		Height;
	int			VerticalStrips;
	float		Thickness;
	signed int	TCut;
} BrushTemplate_Cone;

typedef struct
{
	float		BotXOffset;
	float		BotXSize;
	float		BotZOffset;
	float		BotZSize;
	int			Solid;
	float		Thickness;
	float		TopXOffset;
	float		TopXSize;
	float		TopZOffset;
	float		TopZSize;
	int			VerticalStripes;
	float		YSize;
	float		RingLength;
	signed int	TCut;
} BrushTemplate_Cylinder;

typedef struct
{
	int			HorizontalBands;
	int			VerticalBands;
	float		YSize;
	int			Solid;
	float		Thickness;
	signed int	TCut;
} BrushTemplate_Spheroid;

typedef struct
{
	float		Height;
	float		Length;
	int			NumberOfStairs;
	float		Width;
	signed int	MakeRamp;
	signed int	TCut;
} BrushTemplate_Staircase;

class BrushTemplate
{
public:
	BrushTemplate(void);
	~BrushTemplate(void);

	void BrushTemplate_ArchDefaults(BrushTemplate_Arch* pArchTemplate);
	void BrushTemplate_BoxDefaults(BrushTemplate_Box* pBoxTemplate);
	void BrushTemplate_ConeDefaults(BrushTemplate_Cone* pConeTemplate);
	void BrushTemplate_CylinderDefaults(BrushTemplate_Cylinder* pCylinderTemplate);
	void BrushTemplate_SpheroidDefaults(BrushTemplate_Spheroid* pSpheroidTemplate);
	void BrushTemplate_StaircaseDefaults(BrushTemplate_Staircase* pStaircaseTemplate);

	Brush* BrushTemplate_CreateBox(const BrushTemplate_Box* pTemplate);
	Brush* BrushTemplate_CreateCylinder(const BrushTemplate_Cylinder* pTemplate);
	Brush* BrushTemplate_CreateCone(const BrushTemplate_Cone* pTemplate);
	Brush* BrushTemplate_CreateStaircase(const BrushTemplate_Staircase* pTemplate);
	Brush* BrushTemplate_CreateArch(const BrushTemplate_Arch* pTemplate);
};

