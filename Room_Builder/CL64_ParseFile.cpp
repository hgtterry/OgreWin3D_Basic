/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

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
#include "CL64_ParseFile.h"

enum FaceFlags
{
	FACE_MIRROR = (1 << 0),
	FACE_FULLBRIGHT = (1 << 1),
	FACE_SKY = (1 << 2),
	FACE_LIGHT = (1 << 3),
	FACE_SELECTED = (1 << 4),
	FACE_FIXEDHULL = (1 << 5),		//doesn't expand (rings)
	FACE_GOURAUD = (1 << 6),
	FACE_FLAT = (1 << 7),
	FACE_TEXTURELOCKED = (1 << 8),
	FACE_VISIBLE = (1 << 9),
	FACE_SHEET = (1 << 10),		//visible from both sides
	FACE_TRANSPARENT = (1 << 11)		//use transparency value for something
};

struct tag_Level
{
	BrushList* Brushes;
	//CEntityArray* Entities;
	char* WadPath;
	char* HeadersDir;
	// changed QD Actors
	char* ActorsDir;
	//geBoolean ShowActors;
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

	Ogre::Vector3 TemplatePos;

	float DrawScale;		// default draw scale
	float LightmapScale;	// default lightmap scale
};

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
	Matrix3d XfmFaceAngle;	// face rotation angle
} TexInfo;

typedef struct FaceTag
{
	int			NumPoints;
	int			Flags;
	GPlane		Face_Plane;
	int			LightIntensity;
	float		Reflectivity;
	float		Translucency;
	float		MipMapBias;
	float		LightXScale, LightYScale;
	TexInfo		Tex;
	Ogre::Vector3* Points;
} Face;

CL64_ParseFile::CL64_ParseFile(void)
{
	Read_Buffer[0] = 0;
	WadPath[0] = 0;

	str_buff_1[0] = 0;
	str_buff_2[0] = 0;
	Tag_Float = 0;

	fp = NULL;
}

struct tag_FaceList
{
	int NumFaces;
	int Limit;
	Face** Faces;
	geBoolean Dirty;
	Box3d Bounds;
};

CL64_ParseFile::~CL64_ParseFile(void)
{
}

// *************************************************************************
// *	          Load_File:- Terry and Hazel Flanigan 2025	               *
// *************************************************************************
void CL64_ParseFile::Load_File(char* FileName)
{
	Level* pLevel = NULL;
	
	int Count = 0;

	fp = fopen(FileName, "r");
	if (!fp)
	{
		return;
	}

	memset(Read_Buffer, 0, MAX_PATH);

	while (fgets(Read_Buffer, sizeof(Read_Buffer),fp))
	{
		Tag_Float = 0;

		if (Get_Version(Read_Buffer) == 0)
		{
			break;
		}

		fgets(Read_Buffer, sizeof(Read_Buffer), fp);
		if (Get_TextureLib(Read_Buffer) == 0)
		{
			break;
		}

		/*if (App->CL_Doc->pLevel->Brushes != NULL)
		{
			App->CL_Brush->BrushList_Destroy(&App->CL_Doc->pLevel->Brushes);
			App->CL_Doc->pLevel->Brushes = NULL;
		}*/

		pLevel = App->CL_Level->Level_Create(WadPath, NULL, NULL, NULL);
		if (pLevel == NULL)
		{
			App->Say("Can not Create Level");
			break;
		}

		pLevel->Brushes = BrushList_CreateFromFile();
		if (pLevel->Brushes == NULL)
		{
			App->Say("Can not Create Brushes");
			break;
		}

		break;

		Count++;
		memset(Read_Buffer, 0, MAX_PATH);
	}

	fclose(fp);

	App->Say("Closed");
}

// *************************************************************************
// *	    BrushList_CreateFromFile:- Terry and Hazel Flanigan 2025       *
// *************************************************************************
BrushList* CL64_ParseFile::BrushList_CreateFromFile()
{
	memset(Read_Buffer, 0, MAX_PATH);

	int NumBrushes = 0;
	BrushList* blist = { 0 };

	if (!Get_Int("Brushlist", &NumBrushes)){ return NULL;};

	blist = App->CL_Brush->BrushList_Create();
	if (blist != NULL)
	{
		int i;

		for (i = 0; i < NumBrushes; ++i)
		{
			Brush* pBrush;

			pBrush = Brush_CreateFromFile();
			if (pBrush == NULL)
			{
				//BrushList_Destroy(&blist);
				break;
			}
			else
			{
				//BrushList_Append(blist, pBrush);
			}
		}
	}
	else
	{
		App->Say("Can not create brush list");
		return NULL;
	}

	return blist;
}

// *************************************************************************
// *		 Brush_CreateFromFile:- Terry and Hazel Flanigan 2025		    *
// *************************************************************************
Brush* CL64_ParseFile::Brush_CreateFromFile()
{
	memset(Read_Buffer, 0, MAX_PATH);

	FaceList* fl;
	Brush* b;
	int			tmpFlags, tmpModelId, tmpGroupId, tmpType, tmpTranslucency;
	float		tmpHullSize = 0;
	BrushList* blist;
	char szTemp[MAX_PATH]{ 0 };

	assert(Parser != NULL);

	b = NULL;

	if (!Get_String("Brush", szTemp)) { return NULL; }
	if (!Get_Int("Flags", &tmpFlags)) { return NULL; }
	if (!Get_Int("ModelId", &tmpModelId)) { return NULL; }
	if (!Get_Int("GroupId", &tmpGroupId)) { return NULL; }
	if (!Get_Float("HullSize", &tmpHullSize)) { return NULL; }
	
	if (tmpHullSize < 1.0f)
	{
		tmpHullSize = 1.0f;
	}

	tmpTranslucency = 0;

	tmpType = BRUSH_LEAF;	// default is leaf brush

	if (!Get_Int("Type", &tmpType)) { return NULL; };
	
	fl = NULL;
	blist = NULL;
	switch (tmpType)
	{
	case BRUSH_LEAF:
	{
		fl = FaceList_CreateFromFile();
		if (fl == NULL)
		{
			App->Say("Can not create face");
			return NULL;
		}
		break;
	}
	case BRUSH_MULTI:
		blist = BrushList_CreateFromFile(); // Recursive
		if (blist == NULL)
		{
			App->Say("Can not create BrushList");
			return NULL;
		}
		break;
	default:
		assert(0);		//bad stuff here
		return NULL;
	}

	//if (tmpFlags & BRUSH_TRANSLUCENT)
	//{
	//	// set faces as translucent
	//	if (fl != NULL)
	//	{
	//		FaceList_SetTransparent(fl, GE_TRUE);
	//	}
	//	tmpFlags &= ~BRUSH_TRANSLUCENT;
	//}

	//b = Brush_Create(tmpType, fl, blist);
	//if (b == NULL)
	//{
	//	if (fl != NULL)
	//	{
	//		FaceList_Destroy(&fl);
	//	}
	//	if (blist != NULL)
	//	{
	//		BrushList_Destroy(&blist);
	//	}
	//}
	//else
	//{
	//	b->Flags = tmpFlags;
	//	b->HullSize = tmpHullSize;
	//	b->ModelId = tmpModelId;
	//	b->GroupId = tmpGroupId;
	//	Brush_SetName(b, szTemp);

	//	//FaceList_SetTextureLock(fl, true);
	//}

	return	b;
}

// *************************************************************************
// *	     FaceList_CreateFromFile:- Terry and Hazel Flanigan 2025       *
// *************************************************************************
FaceList* CL64_ParseFile::FaceList_CreateFromFile()
{
	int NumFaces;
	FaceList* pList = NULL;

	if (!Get_Int("BrushFaces", &NumFaces)) { return NULL; };
	
	pList = App->CL_FaceList->FaceList_Create(NumFaces);
	if (pList != NULL)
	{
		int i;
		for (i = 0; i < NumFaces; i++)
		{
			Face* pFace;

			pFace = Face_CreateFromFile();
			if (pFace != NULL)
			{
				App->CL_FaceList->FaceList_AddFace(pList, pFace);
			}
		}
		if (pList->NumFaces < 4)
		{
			App->CL_FaceList->FaceList_Destroy(&pList);
			return NULL;
		}
		pList->Dirty = GE_TRUE;
	}

	return pList;
}

// *************************************************************************
// *	     FaceList_CreateFromFile:- Terry and Hazel Flanigan 2025       *
// *************************************************************************
Face* CL64_ParseFile::Face_CreateFromFile()
{
	Face* f = NULL;
	int		i, flg, NumPnts, xShift, yShift, Light;// changed QD
	
	Ogre::Vector2 Shift;
	Ogre::Vector2 Scale;

	float MipMapBias, Reflectivity, Translucency;
	float	fVal, xScale, yScale, Rotate;// changed QD
	Ogre::Vector3* tmpPnts = NULL;
	signed int LoadResult;
	char szTemp[MAX_PATH]{ 0 };

	assert(Parser != NULL);

	LoadResult = false;
	
	if (!Get_Int("NumPoints", &NumPnts)) { return NULL; }
	if (!Get_Int("Flags", &flg)) { return NULL; }
	
	// don't allow selected faces on load
	flg &= ~FACE_SELECTED;

	if (!Get_Int("Light", &Light)) { return NULL; }
	
	if (!Get_Float("MipMapBias", &MipMapBias)) { return NULL; }
	if (!Get_Float("Translucency", &Translucency)) { return NULL; }
	if (!Get_Float("Reflectivity", &Reflectivity)) { return NULL; }
	

	tmpPnts = (Ogre::Vector3*)App->CL_Maths->Ram_Allocate(sizeof(Ogre::Vector3) * NumPnts);
	if (tmpPnts)
	{
		float	LightXScale = 1.0f;
		float LightYScale = 1.0f;
//		
		for (i = 0; i < NumPnts; i++)
		{
			if (!Get_Vector3("Vec3d", &tmpPnts[i])) { return NULL; }
			App->Say_Float(tmpPnts[i].x);
		}

		f = App->CL_Face->Face_Create(NumPnts, tmpPnts, 0);
		App->CL_Maths->Ram_Free(tmpPnts);
		tmpPnts = NULL;

		if (f)
		{
			//App->Say("Face Created");
			f->Flags = flg;
			f->LightIntensity = Light;
			f->MipMapBias = MipMapBias;
			f->Reflectivity = Reflectivity;
			f->Translucency = Translucency;
		}

		Get_Text_Info("TexInfo", &Rotate, &Shift, &Scale, szTemp);

		xShift = (int)Shift.x;
		yShift = (int)Shift.y;

		xScale = Scale.x;
		yScale = Scale.y;


		//if (!Parse3dt_ScanExpectingText(Parser, (*Expected = "TexInfo"))) goto DoneLoad;
		//// changed QD
		//if (!Parse3dt_GetFloat(Parser, (*Expected = "Rotate"), &Rotate)) goto DoneLoad;
		//// end change
		//if (!Parse3dt_GetInt(Parser, (*Expected = "Shift"), &xShift)) goto DoneLoad;
		//if (!Parse3dt_GetInt(Parser, NULL, &yShift)) goto DoneLoad;
		//if (!Parse3dt_GetFloat(Parser, (*Expected = "Scale"), &xScale)) goto DoneLoad;
		//if (!Parse3dt_GetFloat(Parser, NULL, &yScale)) goto DoneLoad;
		//if (!Parse3dt_GetLiteral(Parser, (*Expected = "Name"), szTemp)) goto DoneLoad;

		//if (!Parse3dt_GetFloat(Parser, (*Expected = "LightScale"), &LightXScale)) goto DoneLoad;
		//if (!Parse3dt_GetFloat(Parser, NULL, &LightYScale)) goto DoneLoad;

		

//		if (f)
//		{
//			Face_InitTexInfo(&f->Tex, &f->Face_Plane.Normal);
//
//			Face_SetTextureName(f, szTemp);
//			Face_SetTextureRotate(f, Rotate);// changed QD
//			Face_SetTextureShift(f, xShift, yShift);
//			Face_SetTextureScale(f, xScale, yScale);
//			Face_SetTexturePos(f);
//
//			if ((VersionMajor == 1) && (VersionMinor <= 16))
//			{
//				f->LightXScale = xScale;
//				f->LightYScale = yScale;
//			}
//			else
//			{
//				f->LightXScale = LightXScale;
//				f->LightYScale = LightYScale;
//			}
//			if ((VersionMajor == 1) && (VersionMinor < 29))
//			{
//				Face_SetVisible(f, GE_TRUE);
//			}
//			if (((VersionMajor == 1) && (VersionMinor > 31)))
//			{
//				if (!Parse3dt_GetXForm3d(Parser, (*Expected = "Transform"), &f->Tex.XfmFaceAngle)) goto DoneLoad;
//				if (!Parse3dt_GetVec3d(Parser, (*Expected = "Pos"), &f->Tex.Pos)) goto DoneLoad;
//			}
//		}
//		LoadResult = GE_TRUE;
	}
//DoneLoad:
//	if (LoadResult == GE_FALSE)
//	{
//		if (f != NULL)
//		{
//			Face_Destroy(&f);
//		}
//		if (tmpPnts != NULL)
//		{
//			geRam_Free(tmpPnts);
//		}
//	}
	return f;
}

// *************************************************************************
// *	          Get_Version:- Terry and Hazel Flanigan 2025	           *
// *************************************************************************
bool CL64_ParseFile::Get_Version(char* Buffer)
{
	(void)sscanf(Buffer, "%s %f", &str_buff_1, &Tag_Float);
	if (!strcmp(str_buff_1, "MTF_Version"))
	{
		if (Tag_Float == 1.000000)
		{
			//App->Say_Float(Tag_Float);
			return 1;
		}
		else
		{
			App->Say("Wrong Version");
			return 0;
		}
	}

	return 1;
}

// *************************************************************************
// *	         Get_TextureLib:- Terry and Hazel Flanigan 2025	           *
// *************************************************************************
bool CL64_ParseFile::Get_TextureLib(char* Buffer)
{
	(void)sscanf(Buffer, "%s %s", &str_buff_1, &str_buff_2);
	if (!strcmp(str_buff_1, "TextureLib"))
	{
		strcpy(WadPath, str_buff_2);
		return 1;
	}
	else
	{
		App->Say("No Texture File Found");
		return 0;
	}

	return 1;
}

// *************************************************************************
// *	        Get_String:- Terry and Hazel Flanigan 2025                 *
// *************************************************************************
bool CL64_ParseFile::Get_String(const char* Should_Be,char* Chr_return)
{
	memset(Read_Buffer, 0, MAX_PATH);
	str_buff_1[0] = 0;
	str_buff_2[0] = 0;

	fgets(Read_Buffer, sizeof(Read_Buffer), fp);
	(void)sscanf(Read_Buffer, "%s %s", &str_buff_1, &str_buff_2);

	if (!strcmp(str_buff_1, Should_Be))
	{
		strcpy(Chr_return, str_buff_2);
		return 1;
	}
	else
	{
		App->Say("File Error", (LPSTR)Should_Be);
		return 0;
	}

	return 0;
}

// *************************************************************************
// *	        Get_int:- Terry and Hazel Flanigan 2025			           *
// *************************************************************************
bool CL64_ParseFile::Get_Int(const char* Should_Be, int* Int_return)
{
	memset(Read_Buffer, 0, MAX_PATH);
	str_buff_1[0] = 0;
	Tag_Int = 0;

	fgets(Read_Buffer, sizeof(Read_Buffer), fp);
	(void)sscanf(Read_Buffer, "%s %i", &str_buff_1, &Tag_Int);

	if (!strcmp(str_buff_1, Should_Be))
	{
		*Int_return = Tag_Int;
		return 1;
	}
	else
	{
		App->Say("File Error", (LPSTR)Should_Be);
		return 0;
	}

	return 0;
}

// *************************************************************************
// *	        Get_Float:- Terry and Hazel Flanigan 2025		           *
// *************************************************************************
bool CL64_ParseFile::Get_Float(const char* Should_Be, float* Float_return)
{
	memset(Read_Buffer, 0, MAX_PATH);
	str_buff_1[0] = 0;
	Tag_Float = 0;

	fgets(Read_Buffer, sizeof(Read_Buffer), fp);
	(void)sscanf(Read_Buffer, "%s %f", &str_buff_1, &Tag_Float);

	if (!strcmp(str_buff_1, Should_Be))
	{
		*Float_return = Tag_Float;
		return 1;
	}
	else
	{
		App->Say("File Error", (LPSTR)Should_Be);
		return 0;
	}

	return 0;
}

// *************************************************************************
// *	        Get_Vector3:- Terry and Hazel Flanigan 2025		           *
// *************************************************************************
bool CL64_ParseFile::Get_Vector3(const char* Should_Be, Ogre::Vector3* Vec3_return)
{
	memset(Read_Buffer, 0, MAX_PATH);
	str_buff_1[0] = 0;
	float x = 0;
	float y = 0;
	float z = 0;

	fgets(Read_Buffer, sizeof(Read_Buffer), fp);
	(void)sscanf(Read_Buffer, "%s %f %f %f", &str_buff_1, &x, &y, &z);

	if (!strcmp(str_buff_1, Should_Be))
	{
		*Vec3_return = Ogre::Vector3(x, y, z);
		return 1;
	}
	else
	{
		App->Say("File Error", (LPSTR)Should_Be);
		return 0;
	}

	return 0;
}

// *************************************************************************
// *	        Get_Vector3:- Terry and Hazel Flanigan 2025		           *
// *************************************************************************
bool CL64_ParseFile::Get_Text_Info(const char* Should_Be, float* ret_Rotate, Ogre::Vector2* ret_Shift, Ogre::Vector2* ret_Scale, char* Chr_Texture)
{
	memset(Read_Buffer, 0, MAX_PATH);
	str_buff_1[0] = 0;
	char Chr_Rotate[100]{ 0 };
	char Chr_Shift[100]{ 0 };
	char Char_Scale[100]{ 0 };
	char Texture_Name[100]{ 0 };

	float Rx = 0;
	float Shx = 0;
	float Shy = 0;

	float Scx = 0;
	float Scy = 0;

	fgets(Read_Buffer, sizeof(Read_Buffer), fp);
	(void)sscanf(Read_Buffer, "%s %s %f %s %f %f %s %f %f %s", &str_buff_1, &Chr_Rotate, &Rx, Chr_Shift, &Shx, &Shy, &Char_Scale, &Scx, &Scy, Texture_Name);

	if (!strcmp(str_buff_1, Should_Be))
	{
		//*Vec3_return = Ogre::Vector3(x, y, z);
		return 1;
	}
	else
	{
		App->Say("File Error", (LPSTR)Should_Be);
		return 0;
	}

	return 0;
}

