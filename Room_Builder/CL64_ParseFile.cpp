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


CL64_ParseFile::CL64_ParseFile(void)
{
	Read_Buffer[0] = 0;
	WadPath[0] = 0;

	str_buff_1[0] = 0;
	str_buff_2[0] = 0;
	Tag_Float = 0;

	fp = NULL;
}

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

		pLevel->Brushes = BrushList_CreateFromFile(fp);
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
BrushList* CL64_ParseFile::BrushList_CreateFromFile(FILE* mFile)
{
	char mBuffer[MAX_PATH];
	memset(mBuffer, 0, MAX_PATH);

	int NumBrushes;
	BrushList* blist = { 0 };

	fgets(Read_Buffer, sizeof(Read_Buffer), mFile);
	(void)sscanf(Read_Buffer, "%s %i", &str_buff_1, &Tag_Int);
	if (!strcmp(str_buff_1, "Brushlist"))
	{
		//App->Say_Int(Tag_Int);
		NumBrushes = Tag_Int;
	}
	else
	{
		App->Say("Error in File");
		return NULL;
	}
	//if (!Parse3dt_GetInt(Parser, (*Expected = "Brushlist"), &NumBrushes)) return NULL;
	
	/*blist = BrushList_Create();
	if (blist != NULL)
	{
		int i;

		for (i = 0; i < NumBrushes; ++i)
		{
			Brush* pBrush;

			pBrush = Brush_CreateFromFile(Parser, VersionMajor, VersionMinor, Expected);
			if (pBrush == NULL)
			{
				BrushList_Destroy(&blist);
				break;
			}
			else
			{
				BrushList_Append(blist, pBrush);
			}
		}
	}*/

	return NULL;// blist;
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
			App->Say_Float(Tag_Float);
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

