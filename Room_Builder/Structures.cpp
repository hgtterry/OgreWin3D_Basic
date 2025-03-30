#include "pch.h"
#include "CL64_App.h"

#define Units_Trunc(n) ((int)(n))

#define Units_CentimetersToEngine(c) (((float)(c)) / 2.54f)
#define Units_EngineToCentimeters(i) (((float)(i)) * 2.54f)

#define CENTIMETERS_TO_ENGINE(c) Units_CentimetersToEngine(c)
#define ENGINE_TO_CENTIMETERS(e) Units_EngineToCentimeters(e)

enum BrushFlags
{
	BRUSH_SOLID = 0x0001,
	BRUSH_WINDOW = 0x0002,
	BRUSH_WAVY = 0x0004,
	BRUSH_DETAIL = 0x0008,	//not included in vis calculations
	BRUSH_HOLLOWCUT = 0x0010,
	BRUSH_TRANSLUCENT = 0x0020,
	BRUSH_EMPTY = 0x0040,
	BRUSH_SUBTRACT = 0x0080,
	BRUSH_CLIP = 0x0100,
	BRUSH_FLOCKING = 0x0200,
	BRUSH_HOLLOW = 0x0400,
	BRUSH_SHEET = 0x0800,
	BRUSH_HIDDEN = 0x1000,
	BRUSH_LOCKED = 0x2000,
	BRUSH_HINT = 0x4000,
	BRUSH_AREA = 0x8000
};

enum FaceFlags
{
	FACE_MIRROR = (1 << 0),
	FACE_FULLBRIGHT = (1 << 1),
	FACE_SKY = (1 << 2),
	FACE_LIGHT = (1 << 3),
	FACE_SELECTED = (1 << 4),
	FACE_FIXEDHULL = (1 << 5),			//doesn't expand (rings)
	FACE_GOURAUD = (1 << 6),
	FACE_FLAT = (1 << 7),
	FACE_TEXTURELOCKED = (1 << 8),
	FACE_VISIBLE = (1 << 9),
	FACE_SHEET = (1 << 10),				//visible from both sides
	FACE_TRANSPARENT = (1 << 11)		//use transparency value for something
};

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
	char			Brush_Name[MAX_PATH];
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
    float* pMinEdgeDist;
    const POINT* ptFrom;
} FindClosestInfo;

typedef struct
{
	CL64_Doc* pDoc;
	const char* TexName;
} BrushTexSetData;

struct fdocFaceScales
{
	float DrawScale;
	float LightmapScale;
};

// *************************************************************************
// *			       ( Static ) fdocSetFaceScales	                   	   *
// *************************************************************************
static signed int fdocSetFaceScales(Face* pFace, void* lParam)
{
	fdocFaceScales* pScales = (fdocFaceScales*)lParam;

	App->CL_Face->Face_SetTextureScale(pFace, pScales->DrawScale, pScales->DrawScale);
	App->CL_Face->Face_SetLightScale(pFace, pScales->LightmapScale, pScales->LightmapScale);

	return false;
}

// *************************************************************************
// *			               BrushTexSetCB	                       	   *
// *************************************************************************
static signed int BrushTexSetCB(Brush* b, void* lParam)
{
	int			i;
	BrushTexSetData* pData;

	pData = (BrushTexSetData*)lParam;

	//	Brush_SetName(b, pData->TexName);
	App->CL_Brush->Brush_SetName(b, pData->pDoc->LastTemplateTypeName);
	//	char const * const BrushName = Brush_GetName (b);
	const int NumFaces = App->CL_Brush->Brush_GetNumFaces(b);

	//copy face TexInfos
	for (i = 0; i < NumFaces; i++) // hgtterry Debug
	{
		Face* f = App->CL_Brush->Brush_GetFace(b, i);
		WadFileEntry* pbmp;
		// 
		App->CL_Face->Face_SetTextureName(f, pData->TexName);
		App->CL_Face->Face_SetTextureDibId(f, App->CL_Level->Level_GetDibId(App->CL_Doc->pLevel, pData->TexName));
		pbmp = App->CL_Level->Level_GetWadBitmap(App->CL_Doc->pLevel, pData->TexName);
		if (pbmp != NULL)
		{
			App->CL_Face->Face_SetTextureSize(f, pbmp->Width, pbmp->Height);
		}
	}

	App->CL_Brush->Brush_SetFaceListDirty(b);

	return true;
}

// *************************************************************************
// *      ( Static ) SelAllBrushFaces:- Terry and Hazel Flanigan 2025      *
// *************************************************************************
static signed int SelAllBrushFaces(Brush* pBrush, void* lParam)
{
	int iFace, nFaces;
	char buff[MAX_PATH];

	nFaces = App->CL_Brush->Brush_GetNumFaces(pBrush);
	for (iFace = 0; iFace < nFaces; ++iFace)
	{
		Face* pFace;
		pFace = App->CL_Brush->Brush_GetFace(pBrush, iFace);

		strcpy(buff, App->CL_Brush->Brush_GetName(App->CL_Doc->CurBrush));
		App->CL_Face->Face_SetBrushName(pFace, buff);

		App->CL_Face->Face_SetSelected(pFace, GE_TRUE);
		App->CL_SelFaceList->SelFaceList_Add(App->CL_Doc->pSelFaces, pFace);
	}

	return GE_TRUE;
}

// *************************************************************************
// *      ( Static ) SelAllBrushFaces:- Terry and Hazel Flanigan 2025      *
// *************************************************************************
static signed int Set_BrushFaces_Name(Brush* pBrush, void* lParam)
{
	int iFace, nFaces;
	char buff[MAX_PATH];

	nFaces = App->CL_Brush->Brush_GetNumFaces(pBrush);
	for (iFace = 0; iFace < nFaces; ++iFace)
	{
		Face* pFace;
		pFace = App->CL_Brush->Brush_GetFace(pBrush, iFace);

		strcpy(buff, App->CL_Brush->Brush_GetName(App->CL_Doc->CurBrush));
		App->CL_Face->Face_SetBrushName(pFace, buff);
	}

	return GE_TRUE;
}
