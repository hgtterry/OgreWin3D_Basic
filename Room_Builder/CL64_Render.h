#pragma once

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
	Ogre::Vector3	Normal;
	float			Dist;
} GPlane;

typedef struct ViewVarsTag
{
	HBITMAP				hDibSec;
	Ogre::uint32		Flags;
	Ogre::uint8* pBits;
	Ogre::uint32* pZBuffer;
	Ogre::uint32		ViewType;
	float				ZoomFactor = 1;

	Ogre::Vector3 Vpn, Vright, Vup, CamPos;
	float	roll, pitch, yaw;
	GPlane		FrustPlanes[4];
	float	MaxScreenScaleInv, FieldOfView;
	float	XCenter = 310;
	float	YCenter = 174;
	float	MaxScale;

	float	SpeedScale, YScreenScale, XScreenScale;
	long	Width = 310;
	long 	Height = 174;
	long		FacesDone;
	char Name[10];

} ViewVars;

class CL64_Render
{
public:
	CL64_Render(void);
	~CL64_Render(void);

	bool Render_RenderOrthoGridFromSize(ViewVars* cv, int Interval, HDC hDC, RECT Rect);
	void Render_ViewToWorld(const ViewVars* cv, const int x, const int y, Ogre::Vector3* wp);
	POINT Render_OrthoWorldToView(const ViewVars* cv, Ogre::Vector3 const* wp);

};

