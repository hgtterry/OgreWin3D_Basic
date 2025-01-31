#include "pch.h"
#include "CL64_App.h"

typedef struct TexInfoTag
{
	T_Vec3 VecNormal;
	float xScale, yScale;
	int xShift, yShift;
	float	Rotate;			// texture rotation angle in degrees
	TexInfo_Vectors TVecs;
	int Dib;				// index into the wad
	char Name[16];
	signed int DirtyFlag;
	T_Vec3 Pos;
	int txSize, tySize;		// texture size (not currently used)
	Matrix3d XfmFaceAngle;	// face rotation angle
} TexInfo;

typedef struct FaceTag
{
	int				NumPoints;
	int				Flags;
	bool			Selected;
	GPlane			Face_Plane;
	int				LightIntensity;
	float			Reflectivity;
	float			Translucency;
	float			MipMapBias;
	float			LightXScale, LightYScale;
	TexInfo			Tex;
	T_Vec3* Points;

} Face;