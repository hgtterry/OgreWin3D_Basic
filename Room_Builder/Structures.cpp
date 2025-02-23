#include "pch.h"
#include "CL64_App.h"

#define Units_Trunc(n) ((int)(n))

#define Units_CentimetersToEngine(c) (((float)(c)) / 2.54f)
#define Units_EngineToCentimeters(i) (((float)(i)) * 2.54f)

#define CENTIMETERS_TO_ENGINE(c) Units_CentimetersToEngine(c)
#define ENGINE_TO_CENTIMETERS(e) Units_EngineToCentimeters(e)

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

enum
{
	GridSize_Centimeter = 1,
	GridSize_Decimeter = 10,
	GridSize_Meter = 100
};

// *************************************************************************
// *							GridInfo								   *
// *************************************************************************
typedef struct
{
	int GridType;
	int SnapType;
	int MetricSnapSize;
	int TexelSnapSize;
	int RotationSnap;

} GridInfo;

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

	GridInfo GridSettings;

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

typedef struct FindClosestInfoTag
{
    CL64_Doc* pDoc;
    ViewVars* v;
    Brush** ppFoundBrush;
    geFloat* pMinEdgeDist;
    const POINT* ptFrom;
} FindClosestInfo;

