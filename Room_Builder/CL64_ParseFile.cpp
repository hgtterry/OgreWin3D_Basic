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
#include "Structures.cpp"

CL64_ParseFile::CL64_ParseFile(void)
{
	str_buff_1[0] = 0;
	str_buff_2[0] = 0;
	Brush_Name[0] = 0;

	Tag_Float = 0;
	Tag_Int = 0;
}

CL64_ParseFile::~CL64_ParseFile(void)
{
}

// *************************************************************************
// *	    BrushList_CreateFromFile:- Terry and Hazel Flanigan 2025       *
// *************************************************************************
BrushList* CL64_ParseFile::BrushList_CreateFromFile(bool SubBrush)
{
	memset(App->CL_File->Read_Buffer, 0, MAX_PATH);

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

			pBrush = Brush_CreateFromFile(SubBrush);
			if (pBrush == NULL)
			{
				//App->CL_Brush->BrushList_Destroy(&blist);
				break;
			}
			else
			{
				App->CL_Brush->BrushList_Append(blist, pBrush);
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
Brush* CL64_ParseFile::Brush_CreateFromFile(bool SubBrush)
{
	//App->Say_Int(Index);
	memset(App->CL_File->Read_Buffer, 0, MAX_PATH);

	FaceList* fl;
	Brush* b;
	int			tmpFlags, tmpModelId, tmpGroupId, tmpType, tmpTranslucency;
	float		tmpHullSize = 0;
	BrushList* blist;
	char szTemp[MAX_PATH]{ 0 };

	assert(Parser != NULL);

	b = NULL;

	if (SubBrush == false)
	{
		if (!Get_String("Brush", szTemp)) { return NULL; }
		strcpy(Brush_Name, szTemp);
	}
	else
	{
		if (!Get_String("Brush", szTemp)) { return NULL; }
		strcpy(szTemp, Brush_Name);
	}

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
		blist = BrushList_CreateFromFile(true); // Recursive
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

	b = App->CL_Brush->Brush_Create(tmpType, fl, blist);
	if (b == NULL)
	{
		if (fl != NULL)
		{
			App->CL_FaceList->FaceList_Destroy(&fl);
		}
		if (blist != NULL)
		{
			App->CL_Brush->BrushList_Destroy(&blist);
		}
	}
	else
	{
		b->Flags = tmpFlags;
		b->HullSize = tmpHullSize;
		b->ModelId = tmpModelId;
		b->GroupId = tmpGroupId;
		App->CL_Brush->Brush_SetName(b, szTemp);
	}

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

				pFace->Real_Brush_Face_Index = 0;
				App->CL_FaceList->FaceList_AddFace(pList, pFace);
				//App->CL_Face->Face_SetTextureLock(pFace, true);
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
// *		   Face_CreateFromFile:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
Face* CL64_ParseFile::Face_CreateFromFile()
{
	Face* f = NULL;
	int		i, flg, NumPnts, xShift, yShift, Light;
	
	Ogre::Vector2 Shift;
	Ogre::Vector2 Scale;
	Ogre::Vector2 Vec_Light;

	float MipMapBias, Reflectivity, Translucency;
	float xScale, yScale, Rotate;
	T_Vec3* tmpPnts = NULL;
	signed int LoadResult;
	char szTemp[MAX_PATH]{ 0 };

	assert(Parser != NULL);

	LoadResult = false;
	
	if (!Get_Int("NumPoints", &NumPnts)) { return NULL; }
	if (!Get_Int("Flags", &flg)) { return NULL; }
	
	flg &= ~FACE_SELECTED;

	if (!Get_Int("Light", &Light)) { return NULL; }
	
	if (!Get_Float("MipMapBias", &MipMapBias)) { return NULL; }
	if (!Get_Float("Translucency", &Translucency)) { return NULL; }
	if (!Get_Float("Reflectivity", &Reflectivity)) { return NULL; }
	

	tmpPnts = (T_Vec3*)App->CL_Maths->Ram_Allocate(sizeof(T_Vec3) * NumPnts);
	if (tmpPnts)
	{
		float LightXScale = 1.0f;
		float LightYScale = 1.0f;
		
		for (i = 0; i < NumPnts; i++)
		{
			if (!Get_Vector3("Vec3d", &tmpPnts[i])) { return NULL; }
		}

		f = App->CL_Face->Face_Create(NumPnts, tmpPnts, 0);
		App->CL_Maths->Ram_Free(tmpPnts);
		tmpPnts = NULL;

		if (f)
		{
			//App->CL_Face->Face_SetTextureLock(f, true);
			f->Flags = flg;
			f->LightIntensity = Light;
			f->MipMapBias = MipMapBias;
			f->Reflectivity = Reflectivity;
			f->Translucency = Translucency;
			f->Real_Brush_Face_Index = 0;
		}

		Get_Text_Info("TexInfo", &Rotate, &Shift, &Scale, szTemp);

		xShift = (int)Shift.x;
		yShift = (int)Shift.y;

		xScale = Scale.x;
		yScale = Scale.y;

		if (!Get_Vector2("LightScale", &Vec_Light)) { return NULL; }
		LightXScale = Vec_Light.x;
		LightYScale = Vec_Light.y;

		if (f)
		{
			App->CL_Face->Face_InitTexInfo(&f->Tex, &f->Face_Plane.Normal);

			App->CL_Face->Face_SetTextureName(f, szTemp);
			App->CL_Face->Face_SetTextureRotate(f, Rotate);
			App->CL_Face->Face_SetTextureShift(f, xShift, yShift);
			App->CL_Face->Face_SetTextureScale(f, xScale, yScale);
			App->CL_Face->Face_SetTexturePos(f);

			f->LightXScale = LightXScale;
			f->LightYScale = LightYScale;
			
			if (!Get_Matrix3d("Transform", &f->Tex.XfmFaceAngle)) { Debug }
			
			//App->Say_Float(f->Tex.XfmFaceAngle.Translation.z);

			if (!Get_Vector3("Pos", &f->Tex.Pos)) { return NULL; }
		}

		LoadResult = true;
	}

//	if (LoadResult == false)
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
		strcpy(App->CL_File->WadPath, str_buff_2);
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
	memset(App->CL_File->Read_Buffer, 0, MAX_PATH);
	str_buff_1[0] = 0;
	str_buff_2[0] = 0;

	fgets(App->CL_File->Read_Buffer, sizeof(App->CL_File->Read_Buffer), App->CL_File->fp);
	(void)sscanf(App->CL_File->Read_Buffer, "%s %s", &str_buff_1, &str_buff_2);

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
	memset(App->CL_File->Read_Buffer, 0, MAX_PATH);
	str_buff_1[0] = 0;
	Tag_Int = 0;

	fgets(App->CL_File->Read_Buffer, sizeof(App->CL_File->Read_Buffer), App->CL_File->fp);
	(void)sscanf(App->CL_File->Read_Buffer, "%s %i", &str_buff_1, &Tag_Int);

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
	memset(App->CL_File->Read_Buffer, 0, MAX_PATH);
	str_buff_1[0] = 0;
	Tag_Float = 0;

	fgets(App->CL_File->Read_Buffer, sizeof(App->CL_File->Read_Buffer), App->CL_File->fp);
	(void)sscanf(App->CL_File->Read_Buffer, "%s %f", &str_buff_1, &Tag_Float);

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
bool CL64_ParseFile::Get_Vector3(const char* Should_Be, T_Vec3* Vec3_return)
{
	memset(App->CL_File->Read_Buffer, 0, MAX_PATH);
	str_buff_1[0] = 0;
	float x = 0;
	float y = 0;
	float z = 0;

	fgets(App->CL_File->Read_Buffer, sizeof(App->CL_File->Read_Buffer), App->CL_File->fp);
	(void)sscanf(App->CL_File->Read_Buffer, "%s %f %f %f", &str_buff_1, &x, &y, &z);

	if (!strcmp(str_buff_1, Should_Be))
	{
		Vec3_return->x = x;
		Vec3_return->y = y;
		Vec3_return->z = z;

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
// *	        Get_Vector2:- Terry and Hazel Flanigan 2025		           *
// *************************************************************************
bool CL64_ParseFile::Get_Vector2(const char* Should_Be, Ogre::Vector2* Vec2_return)
{
	memset(App->CL_File->Read_Buffer, 0, MAX_PATH);
	str_buff_1[0] = 0;
	float x = 0;
	float y = 0;
	
	fgets(App->CL_File->Read_Buffer, sizeof(App->CL_File->Read_Buffer), App->CL_File->fp);
	(void)sscanf(App->CL_File->Read_Buffer, "%s %f %f", &str_buff_1, &x, &y);

	if (!strcmp(str_buff_1, Should_Be))
	{
		*Vec2_return = Ogre::Vector2(x, y);
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
// *			 Get_Matrix3d:- Terry and Hazel Flanigan 2025		       *
// *************************************************************************
bool CL64_ParseFile::Get_Matrix3d(const char* Should_Be, Matrix3d* Matrix3d_return)
{
	memset(App->CL_File->Read_Buffer, 0, MAX_PATH);
	str_buff_1[0] = 0;

	float AX = 0;
	float AY = 0;
	float AZ = 0;

	float BX = 0;
	float BY = 0;
	float BZ = 0;

	float CX = 0;
	float CY = 0;
	float CZ = 0;

	float TX = 0;
	float TY = 0;
	float TZ = 0;
	

	fgets(App->CL_File->Read_Buffer, sizeof(App->CL_File->Read_Buffer), App->CL_File->fp);
	(void)sscanf(App->CL_File->Read_Buffer, "%s %f %f %f %f %f %f %f %f %f %f %f %f", &str_buff_1, &AX, &AY, &AZ, &BX, &BY, &BZ, &CX, &CY, &CZ, &TX, &TY, &TZ);

	if (!strcmp(str_buff_1, Should_Be))
	{
		Matrix3d_return->AX = AX;
		Matrix3d_return->AY = AY;
		Matrix3d_return->AZ = AZ;

		Matrix3d_return->BX = BX;
		Matrix3d_return->BY = BY;
		Matrix3d_return->BZ = BZ;

		Matrix3d_return->CX = CX;
		Matrix3d_return->CY = CY;
		Matrix3d_return->CZ = CZ;

		Matrix3d_return->Translation.x = TX;
		Matrix3d_return->Translation.y = TY;
		Matrix3d_return->Translation.z = TZ;

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
// *			Get_Text_Info:- Terry and Hazel Flanigan 2025	           *
// *************************************************************************
bool CL64_ParseFile::Get_Text_Info(const char* Should_Be, float* ret_Rotate, Ogre::Vector2* ret_Shift, Ogre::Vector2* ret_Scale, char* Chr_Texture)
{
	memset(App->CL_File->Read_Buffer, 0, MAX_PATH);
	str_buff_1[0] = 0;
	char Chr_Rotate[100]{ 0 };
	char Chr_Shift[100]{ 0 };
	char Chr_Scale[100]{ 0 };
	char Chr_Text_Name[100]{ 0 };
	char Chr_Texture_Name[100]{ 0 };

	bool Read_Error = 0;

	float Rx = 0;
	float Shx = 0;
	float Shy = 0;

	float Scx = 0;
	float Scy = 0;

	fgets(App->CL_File->Read_Buffer, sizeof(App->CL_File->Read_Buffer), App->CL_File->fp);
	(void)sscanf(App->CL_File->Read_Buffer, "%s %s %f %s %f %f %s %f %f %s %s", &str_buff_1, &Chr_Rotate, &Rx, Chr_Shift, &Shx, &Shy, &Chr_Scale, &Scx, &Scy, Chr_Text_Name, Chr_Texture_Name);

	if (!strcmp(str_buff_1, Should_Be))
	{
		// Rotate
		if (!strcmp(Chr_Rotate, "Rotate"))
		{
			*ret_Rotate = Rx;
			Read_Error = 0;
		}
		else
		{
			Read_Error = 1;
		}

		// Shift
		if (!strcmp(Chr_Shift, "Shift"))
		{
			*ret_Shift = Ogre::Vector2(Shx,Shy);
			Read_Error = 0;
		}
		else
		{
			Read_Error = 1;
		}

		// Scale
		if (!strcmp(Chr_Scale, "Scale"))
		{
			*ret_Scale = Ogre::Vector2(Scx,Scy);
			Read_Error = 0;
		}
		else
		{
			Read_Error = 1;
		}

		// Texture Name
		if (!strcmp(Chr_Text_Name, "Name"))
		{
			strcpy(Chr_Texture, Chr_Texture_Name);
			Read_Error = 0;
		}
		else
		{
			Read_Error = 1;
		}

		if (Read_Error == 1)
		{
			App->Say("Get_Text_Info Error", (LPSTR)Should_Be);
			return 0;
		}

		return 1;
	}
	else
	{
		App->Say("File Error", (LPSTR)Should_Be);
		return 0;
	}

	return 0;
}

