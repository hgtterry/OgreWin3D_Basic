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
#include "CL64_Ogre3D.h"

CL64_Ogre3D::CL64_Ogre3D(void)
{
	Export_Manual = nullptr;
	World_Manual = nullptr;

	mJustName[0] = 0;
	mFolder_Path[0] = 0;
	mSelected_Directory[0] = 0;
	mDirectory_Name[0] = 0;
	mExport_Path[0] = 0;
	mExport_PathAndFile_Mesh[0] = 0;
	mExport_PathAndFile_Material[0] = 0;

	mWorld_Mesh_JustName[0] = 0;
	mWorld_File_PathAndFile[0] = 0;
	mWorld_File_Path[0] = 0;
	mExport_Just_Name[0] = 0;
	Material_PathAndFile[0] = 0;

	x, y, z = 0;
	nx, ny, nz = 0;
	u, v = 0;

	NameIndex = 0;
}

CL64_Ogre3D::~CL64_Ogre3D(void)
{
}

// *************************************************************************
// *		Set_World_Paths:- Terry and Hazel Flanigan 2025			 	   *
// *************************************************************************
void CL64_Ogre3D::Set_World_Paths(void)
{
	char Num[100];
	_itoa(NameIndex, Num, 10);

	strcpy(mWorld_Mesh_JustName, "World");
	strcat(mWorld_Mesh_JustName, Num);

	strcpy(mWorld_File_PathAndFile, App->RB_Directory_FullPath);
	strcat(mWorld_File_PathAndFile, "\\");
	strcat(mWorld_File_PathAndFile, "Data");
	strcat(mWorld_File_PathAndFile, "\\");
	strcat(mWorld_File_PathAndFile, "World_Test");

	strcpy(mWorld_File_Path, mWorld_File_PathAndFile);

	strcat(mWorld_File_PathAndFile, "\\");
	strcat(mWorld_File_PathAndFile, mWorld_Mesh_JustName);
	strcat(mWorld_File_PathAndFile, ".mesh");

	strcpy(mExport_Just_Name, mWorld_Mesh_JustName);

	NameIndex++;

	x, y, z = 0;
	nx, ny, nz = 0;
	u, v = 0;
}

// *************************************************************************
// *		Set_Export_Paths:- Terry and Hazel Flanigan 2025		 	   *
// *************************************************************************
void CL64_Ogre3D::Set_Export_Paths(void)
{
	strcpy(mSelected_Directory, App->CL_Export->mFolder_Path);
	strcpy(mDirectory_Name, App->CL_Export->mDirectory_Name);
	strcpy(mExport_Just_Name, App->CL_Export->mJustName);

	strcpy(mExport_Path, mSelected_Directory);
	strcat(mExport_Path, "\\");
	strcat(mExport_Path, mDirectory_Name);
	strcat(mExport_Path, "\\");

	strcpy(mExport_PathAndFile_Mesh, mExport_Path);
	strcat(mExport_PathAndFile_Mesh, mExport_Just_Name);
	strcat(mExport_PathAndFile_Mesh, ".mesh");

	strcpy(mExport_PathAndFile_Material, mExport_Path);
	strcat(mExport_PathAndFile_Material, mExport_Just_Name);
	strcat(mExport_PathAndFile_Material, ".material");

	x, y, z = 0;
	nx, ny, nz = 0;
	u, v = 0;

}

// *************************************************************************
// *	  		Export_To_Ogre3D:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Ogre3D::Export_To_Ogre3D()
{
	Set_Export_Paths();

	CreateDirectory(mExport_Path, NULL);

	if (Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(App->CL_Ogre->Export_Resource_Group))
	{
		Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup(App->CL_Ogre->Export_Resource_Group);
	}

	Ogre::ResourceGroupManager::getSingleton().createResourceGroup(App->CL_Ogre->Export_Resource_Group);
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(App->CL_Ogre->Export_Resource_Group);

	Export_MaterialFile(mExport_PathAndFile_Material);
	DecompileTextures_TXL(mExport_Path);

	Export_Manual = App->CL_Ogre->mSceneMgr->createManualObject("OgreManual2");
	
	int A = 0;
	int B = 0;
	int C = 0;

	Export_Manual->setDynamic(false);
	Export_Manual->setCastShadows(false);

	Export_Manual->estimateVertexCount(App->CL_Model->VerticeCount);
	Export_Manual->estimateIndexCount(App->CL_Model->FaceCount);

	char MaterialNumber[MAX_PATH];
	char MatName[MAX_PATH];

	int GroupCountTotal = App->CL_Model->GroupCount;
	int Count = 0;
	int FaceCount = 0;
	int FaceIndex = 0;

	while (Count < GroupCountTotal)
	{
		_itoa(Count, MaterialNumber, 10);
		strcpy(MatName, mExport_Just_Name);
		strcat(MatName, "_Material_");
		strcat(MatName, MaterialNumber);

		Export_Manual->begin(MatName, Ogre::RenderOperation::OT_TRIANGLE_LIST,App->CL_Ogre->Export_Resource_Group);

		FaceCount = 0;
		FaceIndex = 0;

		while (FaceCount < App->CL_Model->Group[Count]->GroupFaceCount)
		{
			A = App->CL_Model->Group[Count]->Face_Data[FaceCount].a;
			B = App->CL_Model->Group[Count]->Face_Data[FaceCount].b;
			C = App->CL_Model->Group[Count]->Face_Data[FaceCount].c;

			// --------------------------------------------------

			Get_Data(Count, A);

			Export_Manual->position(Ogre::Vector3(x, y, z));
			Export_Manual->textureCoord(Ogre::Vector2(u, 1 - v));
			Export_Manual->normal(Ogre::Vector3(nx, ny, nz));
			Export_Manual->index(FaceIndex);
			FaceIndex++;

			Get_Data(Count, B);

			Export_Manual->position(Ogre::Vector3(x, y, z));
			Export_Manual->textureCoord(Ogre::Vector2(u, 1 - v));
			Export_Manual->normal(Ogre::Vector3(nx, ny, nz));
			Export_Manual->index(FaceIndex);
			FaceIndex++;

			Get_Data(Count, C);

			Export_Manual->position(Ogre::Vector3(x, y, z));
			Export_Manual->textureCoord(Ogre::Vector2(u, 1 - v));
			Export_Manual->normal(Ogre::Vector3(nx, ny, nz));
			Export_Manual->index(FaceIndex);

			FaceIndex++;
			FaceCount++;
		}

		Export_Manual->end();

		Count++;
	}


	if (Export_Manual->getNumSections() == 0)
	{
		App->Say("Can not create Ogre Sections");
		return;
	}

	Ogre::MeshPtr mesh = Export_Manual->convertToMesh("TestMesh",App->CL_Ogre->Export_Resource_Group);

	mesh->setAutoBuildEdgeLists(true);

	if (App->CL_Export->flag_Build_Edge_List == 1)
	{
		mesh->buildEdgeList();
	}
	
	App->CL_Ogre->mSceneMgr->destroyManualObject(Export_Manual);

	//Ogre::MeshVersion::

	Ogre::MeshSerializer* ms = new Ogre::MeshSerializer();
	ms->exportMesh(mesh.get(), mExport_PathAndFile_Mesh, Ogre::MeshVersion::MESH_VERSION_LATEST);
	delete(ms);

	char File[MAX_PATH];
	strcpy(File, mExport_Just_Name);
	strcat(File, ".mesh");
	App->Say(File,(LPSTR)"Exported");

}

// *************************************************************************
// *		Export_MaterialFile:- Terry and Hazel Flanigan 2025		   	   *
// *************************************************************************
void CL64_Ogre3D::Export_MaterialFile(char* MatFileName)
{
	char MatName[255];
	char File[255];
	char MaterialNumber[255];

	Ogre::String OMatFileName = MatFileName;
	Ogre::String OFile;
	Ogre::String OMatName;

	int numMaterials = App->CL_Model->GroupCount;

	Ogre::MaterialManager& matMgrSgl = Ogre::MaterialManager::getSingleton();

	Ogre::MaterialSerializer matSer;

	for (int i = 0; i < numMaterials; ++i)
	{
		_itoa(i, MaterialNumber, 10);
		strcpy(MatName, mExport_Just_Name);
		strcat(MatName, "_Material_");
		strcat(MatName, MaterialNumber);

		strcpy(File, App->CL_Model->Group[i]->Text_FileName);

		OMatName = MatName;
		OFile = File;

		Ogre::MaterialPtr ogremat = matMgrSgl.create(OMatName,App->CL_Ogre->Export_Resource_Group);


		if (0 < strlen(File))
		{
			ogremat->getTechnique(0)->getPass(0)->createTextureUnitState(File);

			if (_stricmp(File + strlen(File) - 4, ".TGA") == 0)
			{
				ogremat->getTechnique(0)->getPass(0)->setAlphaRejectSettings(Ogre::CMPF_GREATER, 128);
			}
		}

		matSer.queueForExport(ogremat);
	}

	matSer.exportQueued(OMatFileName);
}

// *************************************************************************
// *	  		Convert_ToOgre3D:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Ogre3D::Convert_ToOgre3D(bool Create)
{

	if (App->CL_Mesh_Mgr->World_Ent)
	{
		App->CL_Ogre->mSceneMgr->destroyEntity(App->CL_Mesh_Mgr->World_Ent);
		App->CL_Mesh_Mgr->World_Ent = nullptr;
	}

	if (Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(App->CL_Ogre->World_Resource_Group))
	{
		Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup(App->CL_Ogre->World_Resource_Group);
	}

	Ogre::ResourceGroupManager::getSingleton().createResourceGroup(App->CL_Ogre->World_Resource_Group);
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(App->CL_Ogre->World_Resource_Group);

	Set_World_Paths();

	CreateMaterialFile();

	if (Create == 1)
	{
		World_Manual = App->CL_Ogre->mSceneMgr->createManualObject("OgreManual2");
		World_Manual->setRenderQueueGroup(2);
	}

	int A = 0;
	int B = 0;
	int C = 0;

	World_Manual->setDynamic(false);
	World_Manual->setCastShadows(false);

	World_Manual->estimateVertexCount(App->CL_Model->VerticeCount);
	World_Manual->estimateIndexCount(App->CL_Model->FaceCount);

	char MaterialNumber[255];
	char MatName[255];

	int GroupCountTotal = App->CL_Model->GroupCount;
	int Count = 0;
	int FaceCount = 0;
	int FaceIndex = 0;
	int TotalFaces = 0;

	if (App->CL_Model->GroupCount == 0)
	{
		App->Say("No Groups");
		return;
	}

	while (Count < GroupCountTotal)
	{
		_itoa(Count, MaterialNumber, 10);
		strcpy(MatName, mWorld_Mesh_JustName);
		strcat(MatName, "_Material_");
		strcat(MatName, MaterialNumber);

		World_Manual->begin(MatName, Ogre::RenderOperation::OT_TRIANGLE_LIST, App->CL_Ogre->World_Resource_Group);

		FaceCount = 0;
		FaceIndex = 0;

		TotalFaces = TotalFaces + App->CL_Model->Group[Count]->GroupFaceCount;
		if (App->CL_Model->Group[Count]->GroupFaceCount == 0)
		{
			//App->Say("No Faces"); hgtterry Debug
		}

		while (FaceCount < App->CL_Model->Group[Count]->GroupFaceCount)
		{
			A = App->CL_Model->Group[Count]->Face_Data[FaceCount].a;
			B = App->CL_Model->Group[Count]->Face_Data[FaceCount].b;
			C = App->CL_Model->Group[Count]->Face_Data[FaceCount].c;

			// --------------------------------------------------

			Get_Data(Count, A);

			World_Manual->position(Ogre::Vector3(x, y, z));
			World_Manual->textureCoord(Ogre::Vector2(u, 1 - v));
			World_Manual->normal(Ogre::Vector3(nx, ny, nz));
			World_Manual->index(FaceIndex);
			FaceIndex++;

			Get_Data(Count, B);

			World_Manual->position(Ogre::Vector3(x, y, z));
			World_Manual->textureCoord(Ogre::Vector2(u, 1 - v));
			World_Manual->normal(Ogre::Vector3(nx, ny, nz));
			World_Manual->index(FaceIndex);
			FaceIndex++;

			Get_Data(Count, C);

			World_Manual->position(Ogre::Vector3(x, y, z));
			World_Manual->textureCoord(Ogre::Vector2(u, 1 - v));
			World_Manual->normal(Ogre::Vector3(nx, ny, nz));
			World_Manual->index(FaceIndex);

			FaceIndex++;
			FaceCount++;
		}

		World_Manual->end();

		Count++;
	}

	if (World_Manual->getNumSections() == 0)
	{
		App->Say("Can not create Ogre Sections");
		App->CL_Ogre->mSceneMgr->destroyManualObject(World_Manual);
		return;
	}

	
	Ogre::MeshPtr mesh = World_Manual->convertToMesh("TestMesh", App->CL_Ogre->World_Resource_Group);

	mesh->setAutoBuildEdgeLists(true);
	mesh->buildEdgeList();

	App->CL_Ogre->mSceneMgr->destroyManualObject(World_Manual);

	Ogre::MeshSerializer* ms = new Ogre::MeshSerializer();
	ms->exportMesh(mesh.get(), mWorld_File_PathAndFile);
	delete(ms);

	char OutputFolder[MAX_PATH];
	strcpy(OutputFolder, mWorld_File_Path);
	strcat(OutputFolder, "\\");

	DecompileTextures_TXL(OutputFolder);

	char Name[MAX_PATH];
	strcpy(Name, mWorld_Mesh_JustName);
	strcat(Name, ".mesh");

	if (App->CL_Mesh_Mgr->World_Ent)
	{
		App->CL_Mesh_Mgr->World_Node->detachAllObjects();

		App->CL_Ogre->mSceneMgr->destroySceneNode(App->CL_Mesh_Mgr->World_Node);
		App->CL_Ogre->mSceneMgr->destroyEntity(App->CL_Mesh_Mgr->World_Ent);

		App->CL_Mesh_Mgr->World_Node = NULL;
		App->CL_Mesh_Mgr->World_Ent = NULL;

		Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup(App->CL_Ogre->World_Resource_Group);
		Ogre::ResourceGroupManager::getSingleton().createResourceGroup(App->CL_Ogre->World_Resource_Group);

		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mWorld_File_Path, "FileSystem", App->CL_Ogre->World_Resource_Group);
		Ogre::ResourceGroupManager::getSingleton().clearResourceGroup(App->CL_Ogre->World_Resource_Group);
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(App->CL_Ogre->World_Resource_Group);

	}
	else
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mWorld_File_Path, "FileSystem", App->CL_Ogre->World_Resource_Group);
	}

	App->CL_Mesh_Mgr->World_Ent = App->CL_Ogre->mSceneMgr->createEntity(Name);
	App->CL_Mesh_Mgr->World_Node = App->CL_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();

	App->CL_Mesh_Mgr->World_Node->attachObject(App->CL_Mesh_Mgr->World_Ent);

	App->CL_Mesh_Mgr->World_Node->setPosition(0, 0, 0);
	App->CL_Mesh_Mgr->World_Node->setVisible(true);
	App->CL_Mesh_Mgr->World_Node->setScale(1, 1, 1);
	//App->CL_Mesh_Mgr->World_Node->showBoundingBox(true);
	// 

	remove(mWorld_File_PathAndFile);
	remove(Material_PathAndFile);
}

// *************************************************************************
// *		CreateMaterialFile:- Terry and Hazel Flanigan 2025		   	   *
// *************************************************************************
void CL64_Ogre3D::CreateMaterialFile()
{
	Material_PathAndFile[0] = 0;

	strcpy(Material_PathAndFile, mWorld_File_Path);
	strcat(Material_PathAndFile, "\\");
	strcat(Material_PathAndFile, mWorld_Mesh_JustName);
	strcat(Material_PathAndFile, ".material");

	char MatName[255];
	char File[255];
	char MaterialNumber[255];

	Ogre::String OMatFileName = Material_PathAndFile;
	Ogre::String OFile;
	Ogre::String OMatName;

	int numMaterials = App->CL_Model->GroupCount;

	Ogre::MaterialManager& matMgrSgl = Ogre::MaterialManager::getSingleton();

	Ogre::MaterialSerializer matSer;

	for (int i = 0; i < numMaterials; ++i)
	{
		_itoa(i, MaterialNumber, 10);
		strcpy(MatName, mExport_Just_Name);
		strcat(MatName, "_Material_");
		strcat(MatName, MaterialNumber);

		strcpy(File, App->CL_Model->Group[i]->Text_FileName);
		//App->Say_Win(File);

		OMatName = MatName;
		OFile = File;

		Ogre::MaterialPtr ogremat = matMgrSgl.create(OMatName, App->CL_Ogre->World_Resource_Group);


		if (0 < strlen(File))
		{
			ogremat->getTechnique(0)->getPass(0)->createTextureUnitState(File);

			if (_stricmp(File + strlen(File) - 4, ".TGA") == 0)
			{
				ogremat->getTechnique(0)->getPass(0)->setAlphaRejectSettings(Ogre::CMPF_GREATER, 128);
			}
		}

		matSer.queueForExport(ogremat);
	}

	matSer.exportQueued(OMatFileName);
}

// *************************************************************************
// *			Get_Data:- Terry and Hazel Flanigan 2025  			   	   *
// *************************************************************************
void CL64_Ogre3D::Get_Data(int Index, int FaceIndex)
{
	x = App->CL_Model->Group[Index]->vertex_Data[FaceIndex].x;
	y = App->CL_Model->Group[Index]->vertex_Data[FaceIndex].y;
	z = App->CL_Model->Group[Index]->vertex_Data[FaceIndex].z;

	u = App->CL_Model->Group[Index]->MapCord_Data[FaceIndex].u;
	v = App->CL_Model->Group[Index]->MapCord_Data[FaceIndex].v;

	nx = App->CL_Model->Group[Index]->Normal_Data[FaceIndex].x;
	ny = App->CL_Model->Group[Index]->Normal_Data[FaceIndex].y;
	nz = App->CL_Model->Group[Index]->Normal_Data[FaceIndex].z;
}

// *************************************************************************
// *		DecompileTextures_TXL:- Terry and Hazel Flanigan 2025  	   	   *
// *************************************************************************
bool CL64_Ogre3D::DecompileTextures_TXL(char* PathAndFile)
{
	
	char buf[MAX_PATH];

	int GroupCount = 0;
	int GroupCountTotal = App->CL_Model->GroupCount;

	while (GroupCount < GroupCountTotal)
	{
		strcpy(buf, App->CL_Model->Group[GroupCount]->Text_FileName);
		int Len = strlen(buf);
		buf[Len - 4] = 0;

		Extract_TXL_Texture(buf, PathAndFile);

		GroupCount++;
	}

	return 1;
}

// *************************************************************************
// *		Extract_TXL_Texture:- Terry and Hazel Flanigan 2023    	 	   *
// *************************************************************************
bool CL64_Ogre3D::Extract_TXL_Texture(char* Name, char* Folder)
{
	WadFileEntry* BitmapPtr = App->CL_Doc->GetDibBitmap(Name);

	int nErrorCode;

	char Name2[MAX_PATH];
	strcpy(Name2, Folder);

	if (geBitmap_HasAlpha(BitmapPtr->bmp))
	{
		//LoadTextures_TXL(Name);

		////App->Say(BitmapPtr->Name);

		//char Buf1[200];
		//strcpy(Buf1, BitmapPtr->Name);
		//strcat(Buf1, ".tga");

		//strcat(Name2, Buf1);
		//nErrorCode = WriteTGA(Name2, Temp_RF_Bitmap);
	}
	else
	{
		char Buf1[200];
		strcpy(Buf1, BitmapPtr->Name);
		strcat(Buf1, ".bmp");

		strcat(Name2, Buf1);

		nErrorCode = WriteBMP8(Name2, BitmapPtr->bmp);
	}

	return 1;
}

typedef struct tagMY_BITMAPINFO
{
	BITMAPINFOHEADER bmiHeader;
	RGBQUAD          bmiColors[256];
} MY_BITMAPINFO, * pMY_BITMAPINFO, ** ppMY_BITMAPINFO;

#define PAD32(i)     (((i)+31)/32*4)

// *************************************************************************
// *		Extract_TXL_Texture:- Terry and Hazel Flanigan 2025    	 	   *
// *************************************************************************
int CL64_Ogre3D::WriteBMP8(const char* pszFile, geBitmap* pBitmap)
{
	geBitmap* pLock = NULL;
	gePixelFormat    Format;
	geBitmap_Info    BitmapInfo;
	int    nErrorCode = 0;// nErrorCode = TPACKERROR_UNKNOWN;      // Return code
	BITMAPFILEHEADER BmpHeader;                            // bitmap file-header 
	MY_BITMAPINFO    BmInfo;
	Ogre::uint32           nBytesPerPixel;
	void* pPixelData;
	Ogre::uint8* pOut = NULL;
	Ogre::uint8* pTmp = NULL;
	int              nNewStride = 0;
	int              nOldStride = 0;
	int              i;
	HANDLE           hFile = NULL;
	DWORD            nBytesWritten;
	Ogre::uint8* pNew;
	Ogre::uint8* pOld;
	int    y;

	// Create the .BMP file.
	hFile = CreateFile(pszFile,
		GENERIC_READ | GENERIC_WRITE,
		(DWORD)0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		(HANDLE)NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		App->Say("Error");
	}
	
	// get 8-bit palettized bitmap
	Format = GE_PIXELFORMAT_8BIT;

	if (geBitmap_GetBits(pBitmap))
	{
		pLock = pBitmap;
	}
	else
	{
		if (!geBitmap_LockForRead(pBitmap, &pLock, 0, 0, Format, GE_FALSE, 0))
		{
			return FALSE;
		}
	}

	geBitmap_GetInfo(pLock, &BitmapInfo, NULL);
	if (BitmapInfo.Format != Format)
	{
		App->Say("Error");
		goto ExitWriteBitmap;
	}


	for (i = 0; i < 256; i++)
	{
		int r, g, b, a;
		geBitmap_Palette_GetEntryColor(BitmapInfo.Palette, i, &r, &g, &b, &a);

		BmInfo.bmiColors[i].rgbRed = (Ogre::uint8)r;
		BmInfo.bmiColors[i].rgbGreen = (Ogre::uint8)g;
		BmInfo.bmiColors[i].rgbBlue = (Ogre::uint8)b;
		BmInfo.bmiColors[i].rgbReserved = (Ogre::uint8)0;
	}

	nBytesPerPixel = gePixelFormat_BytesPerPel(Format);
	pPixelData = geBitmap_GetBits(pLock);

	// Build bitmap info
	BmInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	BmInfo.bmiHeader.biWidth = BitmapInfo.Width;
	BmInfo.bmiHeader.biHeight = BitmapInfo.Height;    // Bitmap are bottom-up
	BmInfo.bmiHeader.biPlanes = 1;
	BmInfo.bmiHeader.biBitCount = (WORD)8;
	BmInfo.bmiHeader.biCompression = BI_RGB;
	BmInfo.bmiHeader.biSizeImage = 0;
	BmInfo.bmiHeader.biXPelsPerMeter = BmInfo.bmiHeader.biYPelsPerMeter = 0;   // 10000;

	if (BmInfo.bmiHeader.biBitCount < 24)
		BmInfo.bmiHeader.biClrUsed = (1 << BmInfo.bmiHeader.biBitCount);
	else
		BmInfo.bmiHeader.biClrUsed = 0;

	BmInfo.bmiHeader.biClrImportant = 0;

	nNewStride = PAD32(BitmapInfo.Width * BmInfo.bmiHeader.biBitCount);
	nOldStride = BitmapInfo.Width * nBytesPerPixel;

	BmInfo.bmiHeader.biSizeImage = nNewStride * BitmapInfo.Height;

	if (nNewStride == nOldStride)
	{
		pTmp = (Ogre::uint8*)pPixelData;
	}

	// Allocate new pixel buffer.
	else
	{
		pTmp = (Ogre::uint8*)App->CL_Maths->Ram_Allocate(nNewStride * BitmapInfo.Height);
		if (pTmp == (Ogre::uint8*)0)
		{
			App->Say("Error");
			goto ExitWriteBitmap;
		}


		pNew = (Ogre::uint8*)pTmp;
		pOld = (Ogre::uint8*)pPixelData;

		// Copy old to new
		for (y = 0; y < BitmapInfo.Height; y++)
		{
			memcpy(pNew, pOld, nOldStride);

			// Next row
			pOld += nOldStride;
			pNew += nNewStride;
		}
	}

	pOut = (Ogre::uint8*)App->CL_Maths->Ram_Allocate(nNewStride * BitmapInfo.Height);
	if (pOut == (Ogre::uint8*)0)
	{
		App->Say("Error");
		goto ExitWriteBitmap;
	}

	pNew = (Ogre::uint8*)pOut;
	pOld = (Ogre::uint8*)(pTmp + (nNewStride * (BitmapInfo.Height - 1)));

	// Copy old to new
	for (y = 0; y < BitmapInfo.Height; y++)
	{
		memcpy(pNew, pOld, nNewStride);

		// Next row
		pOld -= nNewStride;
		pNew += nNewStride;
	}

	if (pTmp && nNewStride != nOldStride)
		App->CL_Maths->Ram_Free(pTmp);

	// Build the file header
	BmpHeader.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M" 

	// Compute the size of the entire file. 
	BmpHeader.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) +
		BmInfo.bmiHeader.biSize +
		(BmInfo.bmiHeader.biClrUsed * sizeof(RGBQUAD)) +
		(nNewStride * BitmapInfo.Height));
	BmpHeader.bfReserved1 = 0;
	BmpHeader.bfReserved2 = 0;

	// Compute the offset to the array of color indices. 
	BmpHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) +
		BmInfo.bmiHeader.biSize +
		(BmInfo.bmiHeader.biClrUsed * sizeof(RGBQUAD));

	// Write the BMP file header
	if (!WriteFile(hFile, (LPVOID)&BmpHeader, sizeof(BITMAPFILEHEADER), (LPDWORD)&nBytesWritten, (NULL)))
	{
		App->Say("Error");
		goto ExitWriteBitmap;
	}

	// Write the Bitmap infor header and palette
	if (!WriteFile(hFile, (LPVOID)&BmInfo, sizeof(MY_BITMAPINFO), (LPDWORD)&nBytesWritten, (NULL)))
	{
		App->Say("Error");
		goto ExitWriteBitmap;
	}

	// Write the pixel data
	if (!WriteFile(hFile, (LPVOID)pOut, nNewStride * BitmapInfo.Height, (LPDWORD)&nBytesWritten, (NULL)))
	{
		App->Say("Error");
		goto ExitWriteBitmap;
	}

	CloseHandle(hFile);
	hFile = NULL;

	// Success!
	//App->Say("Success");

ExitWriteBitmap:

	// Clean-up
	//------------------------------------
	// Make sure the file gets closed
	if (hFile)
		CloseHandle(hFile);

	App->CL_Maths->Ram_Free(pOut);

	// Unlock the geBitmap
	if (pLock != pBitmap)
	{
		geBitmap_UnLock(pLock);
	}
	return 1;// nErrorCode;
}

// *************************************************************************
// *			LoadTextures_TXL:- Terry and Hazel Flanigan 2025 		   *
// *************************************************************************
bool CL64_Ogre3D::LoadTextures_TXL(char* Name)
{
	//VFS = NULL;

	//geVFile* File = NULL;
	//geVFile_Finder* Finder = NULL;

	////NameCount = 0;

	//VFS = geVFile_OpenNewSystem(NULL, GE_VFILE_TYPE_VIRTUAL, App->CL_World->mCurrent_TXL_FilePath, NULL, GE_VFILE_OPEN_READONLY | GE_VFILE_OPEN_DIRECTORY);
	//if (!VFS)
	//{
	//	App->Say("Could not open file", App->CL_World->mCurrent_TXL_FilePath);
	//	return 0;
	//}

	//Finder = geVFile_CreateFinder(VFS, "*.*");
	//if (!Finder)
	//{
	//	App->Say("Could not create Finder *.* ");

	//	geVFile_Close(VFS);
	//	return 0;
	//}


	//if (VFS)
	//{
	//	File = geVFile_Open(VFS, Name, GE_VFILE_OPEN_READONLY);
	//}

	//if (!File)
	//{
	//	App->Say("Could not open", Name);
	//	return 0;
	//}

	//Temp_RF_Bitmap = geBitmap_CreateFromFile(File);

	////geBitmap_Destroy(&Temp_RF_Bitmap);

	//geVFile_Close(File);
	//geVFile_Close(VFS);

	return 1;
}
