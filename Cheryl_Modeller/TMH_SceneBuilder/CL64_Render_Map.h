/*
Copyright (c) 2024 - 2025 TMH_Software W.T.Flanigan M.Habib H.C.Flanigan

TMH_SceneBuilder

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
#include "CL64_Lib_Maths.h"
enum ViewTypes
{
	VIEWSOLID = 1,
	VIEWTEXTURE = 2,
	VIEWWIRE = 4,
	VIEWTOP = 8,
	VIEWFRONT = 16,
	VIEWSIDE = 32,
	VIEWOGRE = 64
};

typedef struct PlaneTag
{
	T_Vec3	Normal;
	float	Dist;
} GPlane;

typedef struct ViewVarsTag
{
	//HBITMAP				hDibSec;
	HWND				hDlg; // The Views HWND
	Ogre::uint32		Flags;
	//Ogre::uint8*		pBits;
	//Ogre::uint32*		pZBuffer;
	Ogre::uint32		ViewType;
	float				ZoomFactor = 5;

	//T_Vec3 Vpn, Vright, Vup, CamPos;
	T_Vec3 CamPos;
	//float	roll, pitch, yaw;
	GPlane	FrustPlanes[4];
	float	MaxScreenScaleInv;// FieldOfView;
	float	XCenter = 310;
	float	YCenter = 174;
	//float	MaxScale;
	//float	SpeedScale;
	float XScreenScale = 0;
	float YScreenScale = 0;

	long	Width = 310;
	long 	Height = 174;
	//long	FacesDone;
	char	Name[10];

} ViewVars;

typedef struct SizeInfoTag
{
	long	TexWidth, TexHeight;
	long	ScreenWidth, ScreenHeight;
	Ogre::uint8* TexData, * ScreenData;
	Ogre::uint32* ZData;
} SizeInfo;

class CL64_Render_Map
{
public:
	CL64_Render_Map(void);
	~CL64_Render_Map(void);

	void Pan_View(ViewVars* currentView, int startPosX, int startPosY);
	void Zoom_View(ViewVars* currentView, int startPosY, int startPosX, int cursorPositionY);

	int	Render_GetWidth(const ViewVars* v);
	int	Render_GetHeight(const ViewVars* v);
	int	Render_GetInidx(const ViewVars* v);

	bool Render_RenderOrthoGridFromSize(ViewVars* cv, int Interval, HDC hDC, RECT Rect);
	void Render_ViewToWorld(const ViewVars* cv, const int x, const int y, T_Vec3* wp);
	POINT Render_OrthoWorldToView(const ViewVars* cv, T_Vec3 const* wp);
	
	// Rotation
	float Render_GetXScreenScale(const ViewVars* v);
	void Render_ViewDeltaToRotation(const ViewVars* v, const float dx, T_Vec3* VecRotate);

	void Render_MoveCamPosOrtho(ViewVars* v, const T_Vec3* dv);
};

