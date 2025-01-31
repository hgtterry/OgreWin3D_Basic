#include "pch.h"
#include "CL64_App.h"

// *************************************************************************
// *						Texture Information							   *
// *************************************************************************
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

// *************************************************************************
// *							Face Structure							   *
// *************************************************************************
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

// *************************************************************************
// *							Face List								   *
// *************************************************************************
struct tag_FaceList
{
	int NumFaces;
	int Limit;
	Face** Faces;
	bool Dirty;
	Box3d Bounds;
};

// *************************************************************************
// *							Brush List								   *
// *************************************************************************
struct tag_BrushList
{
	Brush* First;
	Brush* Last;
};

// *************************************************************************
// *							Level Structure							   *
// *************************************************************************
struct tag_Level
{
	BrushList* Brushes;
	char* WadPath;
	char* HeadersDir;
	char* ActorsDir;
	char* PawnIniPath;
	SizeInfo* WadSizeInfos;
	CL64_WadFile* WadFile;
	int GroupVisSetting;

	BrushTemplate_Arch ArchTemplate;
	BrushTemplate_Box	BoxTemplate;
	BrushTemplate_Cone	ConeTemplate;
	BrushTemplate_Cylinder CylinderTemplate;
	BrushTemplate_Spheroid	SpheroidTemplate;
	BrushTemplate_Staircase StaircaseTemplate;

	T_Vec3 TemplatePos;

	float DrawScale;		// default draw scale
	float LightmapScale;	// default lightmap scale
};

// *************************************************************************
// *						Selected Brush List							   *
// *************************************************************************
struct tag_SelBrushList
{
	Array* pItems;
	int FirstFree;
};

// *************************************************************************
// *						Selected Face List							   *
// *************************************************************************
struct tag_SelFaceList
{
	Array* pItems;
	int FirstFree;
};
