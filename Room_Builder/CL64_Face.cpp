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
	geXForm3d XfmFaceAngle;	// face rotation angle
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
