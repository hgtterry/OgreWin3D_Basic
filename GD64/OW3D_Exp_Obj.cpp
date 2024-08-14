/*
Copyright (c) OgreWin3D_Basic 2024 W.T.Flanigan H.C.Flanigan Inflanite_HGT

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely.

An acknowledgment in the product documentation would be
appreciated but is not required.
*/

#include "pch.h"
#include "CL64_App.h"
#include "OW3D_Exp_Obj.h"

OW3D_Exp_Obj::OW3D_Exp_Obj(void)
{
}

OW3D_Exp_Obj::~OW3D_Exp_Obj(void)
{
}

// *************************************************************************
// *			Create_ObjectFile:- Terry and Hazel Flanigan 2024  	 	   *
// *************************************************************************
bool OW3D_Exp_Obj::Create_ObjectFile(void)
{

	strcpy(App->CL_File_IO->BrowserMessage, "Select Folder To Place Object Files a sub folder will be created");
	int Test = App->CL_File_IO->StartBrowser((LPSTR)"");

	if (Test == 0)
	{
		return 0;
	}

	strcpy(OutputFolder, "");

	char buff[1024];
	strcpy(buff, App->CL_File_IO->szSelectedDir);
	strcat(buff, "\\");
	strcat(buff, App->CL_Scene->JustName);
	strcat(buff, "_Object");
	strcat(buff, "\\");

	strcpy(OutputFolder, buff);

	CreateDirectory(OutputFolder, NULL);

	//App->CL_Textures->DecompileTextures(OutputFolder);

	strcpy(Object_FileName, OutputFolder);
	strcat(Object_FileName, App->CL_Scene->JustName);
	strcat(Object_FileName, ".obj");

	strcpy(mtl_FileName, OutputFolder);
	strcat(mtl_FileName, App->CL_Scene->JustName);
	strcat(mtl_FileName, ".mtl");

	strcpy(Just_mtl_FileName, App->CL_Scene->JustName);
	strcat(Just_mtl_FileName, ".mtl");

	Write_ObjectFile();

	WriteMTLFile();

	return 1;
}

// *************************************************************************
// *		Write_ObjectFile:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void OW3D_Exp_Obj::Write_ObjectFile(void)
{
	Write_OBJECTFILE = 0;

	//	char buf[1024];

	Write_OBJECTFILE = fopen(Object_FileName, "wt");

	if (!Write_OBJECTFILE)
	{
		return;
	}
	if (App->CL_Scene->Model_Type == Enums::LoadedFile_Assimp)
	{
		Write_ObjectFile_Commit();
	}

	fclose(Write_OBJECTFILE);
}

// *************************************************************************
// *		Write_ObjectFile_Commit:- Terry and Hazel Flanigan 2024  	   *
// *************************************************************************
void OW3D_Exp_Obj::Write_ObjectFile_Commit(void)
{

	fprintf(Write_OBJECTFILE, "#\n");
	fprintf(Write_OBJECTFILE, "# Wavefront OBJ file\n");
	fprintf(Write_OBJECTFILE, "# Created with Equity_XL\n");
	fprintf(Write_OBJECTFILE, "#\n");
	fprintf(Write_OBJECTFILE, "mtllib %s\n", Just_mtl_FileName);
	fprintf(Write_OBJECTFILE, "%s \n", " ");

	int	VertCount = 0;
	int GroupCount = 0;
	int VertTextCords = 0;
	int VertNormals = 0;
	int FaceCount = 0;
	int FaceIndex = 1;

	int GroupCountTotal = App->CL_Scene->GroupCount;

	//	char GroupName[255];
	char MaterialName[255];

	//--------------------------------------------- Vertices v-

	fprintf(Write_OBJECTFILE, "%s %i\n", "#", App->CL_Scene->VerticeCount);

	while (GroupCount < GroupCountTotal)
	{
		VertCount = 0;
		while (VertCount < App->CL_Scene->Group[GroupCount]->GroupVertCount)
		{
			float X = App->CL_Scene->Group[GroupCount]->vertex_Data[VertCount].x;
			float Y = App->CL_Scene->Group[GroupCount]->vertex_Data[VertCount].y;
			float Z = App->CL_Scene->Group[GroupCount]->vertex_Data[VertCount].z;

			fprintf(Write_OBJECTFILE, "v %.06f %.06f %.06f\n", X, Y, Z);

			VertCount++;
		}
		GroupCount++;
	}

	fprintf(Write_OBJECTFILE, "%s \n", " ");

	//--------------------------------------------- Texture Cords vt-

	fprintf(Write_OBJECTFILE, "%s %i\n", "#", App->CL_Scene->VerticeCount);

	GroupCount = 0;

	while (GroupCount < GroupCountTotal)
	{
		VertTextCords = 0;
		while (VertTextCords < App->CL_Scene->Group[GroupCount]->GroupVertCount)
		{
			float U = App->CL_Scene->Group[GroupCount]->MapCord_Data[VertTextCords].u;
			float V = App->CL_Scene->Group[GroupCount]->MapCord_Data[VertTextCords].v;

			fprintf(Write_OBJECTFILE, "vt %.06f %.06f\n", U, V);

			VertTextCords++;
		}
		GroupCount++;
	}

	fprintf(Write_OBJECTFILE, "%s \n", " ");

	//--------------------------------------------- Normals vn-

	fprintf(Write_OBJECTFILE, "%s %i\n", "#", App->CL_Scene->VerticeCount);

	GroupCount = 0;

	while (GroupCount < GroupCountTotal)
	{
		VertNormals = 0;
		while (VertNormals < App->CL_Scene->Group[GroupCount]->GroupVertCount)
		{
			float X = App->CL_Scene->Group[GroupCount]->Normal_Data[VertNormals].x;
			float Y = App->CL_Scene->Group[GroupCount]->Normal_Data[VertNormals].y;
			float Z = App->CL_Scene->Group[GroupCount]->Normal_Data[VertNormals].z;

			fprintf(Write_OBJECTFILE, "vn %.06f %.06f %.06f\n", X, Y, Z);

			VertNormals++;
		}
		GroupCount++;
	}

	fprintf(Write_OBJECTFILE, "%s \n", " ");

	GroupCount = 0;

	int Offset = 1;
	while (GroupCount < GroupCountTotal)
	{
		FaceCount = 0;
		int LineIndex = 0;

		strcpy(MaterialName, App->CL_Scene->Group[GroupCount]->MaterialName);
		fprintf(Write_OBJECTFILE, "usemtl %s\n", MaterialName);

		while (FaceCount < App->CL_Scene->Group[GroupCount]->GroupFaceCount)
		{
			int A0 = App->CL_Scene->Group[GroupCount]->Face_Data[FaceCount].a + Offset;
			int B0 = App->CL_Scene->Group[GroupCount]->Face_Data[FaceCount].a + Offset;
			int C0 = App->CL_Scene->Group[GroupCount]->Face_Data[FaceCount].a + Offset;

			int A1 = App->CL_Scene->Group[GroupCount]->Face_Data[FaceCount].b + Offset;
			int B1 = App->CL_Scene->Group[GroupCount]->Face_Data[FaceCount].b + Offset;
			int C1 = App->CL_Scene->Group[GroupCount]->Face_Data[FaceCount].b + Offset;

			int A2 = App->CL_Scene->Group[GroupCount]->Face_Data[FaceCount].c + Offset;
			int B2 = App->CL_Scene->Group[GroupCount]->Face_Data[FaceCount].c + Offset;
			int C2 = App->CL_Scene->Group[GroupCount]->Face_Data[FaceCount].c + Offset;

			FaceCount++;

			fprintf(Write_OBJECTFILE, "f %i/%i/%i %i/%i/%i %i/%i/%i\n", A0, B0, C0, A1, B1, C1, A2, B2, C2);

		}

		Offset = Offset + App->CL_Scene->Group[GroupCount]->GroupVertCount;

		fprintf(Write_OBJECTFILE, "%s \n", " ");
		GroupCount++;
	}
}

// *************************************************************************
// *				WriteMTLFile:- Terry and Hazel Flanigan 2024       	   *
// *************************************************************************
bool OW3D_Exp_Obj::WriteMTLFile(void)
{
	Write_MTLFile = 0;

	char buf[1024];

	Write_MTLFile = fopen(mtl_FileName, "wt");

	if (!Write_MTLFile)
	{
		return 0;
	}

	fprintf(Write_MTLFile, "#\n");
	fprintf(Write_MTLFile, "# Wavefront material file\n");
	fprintf(Write_MTLFile, "# Created with Equity_XL\n");
	fprintf(Write_MTLFile, "#\n");
	fprintf(Write_MTLFile, "%s \n", " ");

	int GroupCount = 0;
	int GroupCountTotal = App->CL_Scene->GroupCount;

	while (GroupCount < GroupCountTotal)
	{
		strcpy(buf, App->CL_Scene->Group[GroupCount]->MaterialName);
		fprintf(Write_MTLFile, "newmtl %s\n", buf);

		fprintf(Write_MTLFile, "illum 2\n");
		fprintf(Write_MTLFile, "Kd 0.800000 0.800000 0.800000\n");
		fprintf(Write_MTLFile, "Ka 0.200000 0.200000 0.200000\n");
		fprintf(Write_MTLFile, "Ks 0.000000 0.000000 0.000000\n");
		fprintf(Write_MTLFile, "Ke 0.000000 0.000000 0.000000\n");
		fprintf(Write_MTLFile, "Ns 0.000000\n");

		strcpy(buf, App->CL_Scene->Group[GroupCount]->Text_FileName);
		int Len = strlen(buf);
		buf[Len - 4] = 0;
		strcat(buf, ".jpg");

		fprintf(Write_MTLFile, "map_Kd %s\n", buf);

		fprintf(Write_MTLFile, "%s \n", " ");

		GroupCount++;
	}

	fclose(Write_MTLFile);

	return 1;
}
