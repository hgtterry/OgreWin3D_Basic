#pragma once
#include "CL64_Maths.h"
enum ViewTypes
{
	VIEWSOLID = 1,
	VIEWTEXTURE = 2,
	VIEWWIRE = 4,
	VIEWTOP = 8,
	VIEWFRONT = 16,
	VIEWSIDE = 32
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
	float				ZoomFactor = 1;

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

class CL64_Render
{
public:
	CL64_Render(void);
	~CL64_Render(void);

	int	Render_GetWidth(const ViewVars* v);
	int	Render_GetHeight(const ViewVars* v);
	int	Render_GetInidx(const ViewVars* v);

	bool Render_RenderOrthoGridFromSize(ViewVars* cv, int Interval, HDC hDC, RECT Rect);
	void Render_ViewToWorld(const ViewVars* cv, const int x, const int y, T_Vec3* wp);
	POINT Render_OrthoWorldToView(const ViewVars* cv, T_Vec3 const* wp);
	void Render_MoveCamPosOrtho(ViewVars* v, const T_Vec3* dv);
};

