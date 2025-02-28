/*
Copyright (c) 2024 Inflanite_HGT W.T.Flanigan H.C.Flanigan

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
#include "CL64_Utilities.h"

CL64_Utilities::CL64_Utilities(void)
{
	JustFileName[0] = 0;
}

CL64_Utilities::~CL64_Utilities(void)
{
}

// *************************************************************************
// *		Check_File_Exist:- Terry and Hazel Flanigan 2024		 	   *
// *************************************************************************
bool CL64_Utilities::Check_File_Exist(char* Full_Path)
{
	char pSearchPath[MAX_PATH];

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	strcpy(pSearchPath, Full_Path);

	hFind = FindFirstFile(pSearchPath, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	else
	{
		FindClose(hFind);
		return 1;
	}

	return 0;
}

// *************************************************************************
// *		Delete_Folder_Contents:- Terry and Hazel Flanigan 2024	 	   *
// *************************************************************************
void CL64_Utilities::Delete_Folder_Contents(char* Folder)
{
	char pSearchPath[MAX_PATH];
	char DeleteFile[MAX_PATH];

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	strcpy(pSearchPath, Folder);
	strcat(pSearchPath, "\\");
	strcat(pSearchPath, "*.*");

	hFind = FindFirstFile(pSearchPath, &FindFileData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		strcpy(DeleteFile, Folder);
		strcat(DeleteFile, "\\");
		strcat(DeleteFile, FindFileData.cFileName);
		remove(DeleteFile);
		
		while (FindNextFile(hFind, &FindFileData) != 0)
		{
			strcpy(DeleteFile, Folder);
			strcat(DeleteFile, "\\");
			strcat(DeleteFile, FindFileData.cFileName);
			remove(DeleteFile);
		}
	}
	
	FindClose(hFind);
}

// *************************************************************************
// *		Get_FileName_FromPath:- Terry and Hazel Flanigan 2025	  	   *
// *************************************************************************
void CL64_Utilities::Get_FileName_FromPath(char* pString, char* FileName)
{
	JustFileName[0] = 0;

	int Count = 0;
	int Mark = 0;
	bool Test = 0;

	while (*pString != 0)
	{
		if (*pString == '\\' || *pString == '/')
		{
			Test = 1;
			Mark = Count;
		}

		Count++;
		pString++;
	}

	if (Mark == 0 && Test == 0)
	{
		strcpy(JustFileName, FileName);
	}
	else
	{
		if (Mark == 0 && Test == 1)
		{
			Mark = 1;
			strcpy(JustFileName, (FileName + Mark));
		}
		else
		{
			strcpy(JustFileName, (FileName + Mark) + 1);
		}
	}
}

// *************************************************************************
// *			UnZip_Test:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Utilities::UnZip_Test()
{
	char BufPath[MAX_PATH];
	strcpy(BufPath, App->RB_Directory_FullPath);
	strcat(BufPath, "\\Data\\Room_Builder\\Default.zip");

	char OutDir[MAX_PATH];
	strcpy(OutDir, App->RB_Directory_FullPath);
	strcat(OutDir, "\\Data\\Poo\\");

	SetCurrentDirectory(OutDir);

	HZIP hz = OpenZip(BufPath, 0);
	ZIPENTRY ze; GetZipItem(hz, -1, &ze); int numitems = ze.index;
	// -1 gives overall information about the zipfile
	for (int zi = 0; zi < numitems; zi++)
	{
		ZIPENTRY ze; GetZipItem(hz, zi, &ze); // fetch individual details
		ZRESULT test = UnzipItem(hz, zi, ze.name);         // e.g. the item's name.

		char buff[MAX_PATH];
		FormatZipMessage(test, buff,MAX_PATH);
		App->Say(buff);
	}

	CloseZip(hz);
	
	//RemoveFileFromZip(const TCHAR * zip, const TCHAR * name);
	//AddFileToZip(const TCHAR * zip, const TCHAR * name, const TCHAR * fn);
}

// *************************************************************************
// *			UnZip_Test_2:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Utilities::UnZip_Test_2()
{
	Ogre::String mFileString;

	char BufPath[MAX_PATH];
	strcpy(BufPath, App->RB_Directory_FullPath);
	strcat(BufPath, "\\Data\\Room_Builder\\Default.zip");

	char OutDir[MAX_PATH];
	strcpy(OutDir, App->RB_Directory_FullPath);
	strcat(OutDir, "\\Data\\Poo\\");

	Ogre::FileInfoListPtr RFI = ResourceGroupManager::getSingleton().listResourceFileInfo(App->CL_Ogre->Texture_Resource_Group, false);
	Ogre::FileInfoList::const_iterator i, iend;
	iend = RFI->end();

	for (i = RFI->begin(); i != iend; ++i)
	{
		Ogre::DataStreamPtr ff = i->archive->open(i->filename);

		mFileString = ff->getAsString();

		char mFileName[MAX_PATH];
		strcpy(mFileName, App->RB_Directory_FullPath);
		strcat(mFileName, "\\Data\\Texture_Test\\");
		strcat(mFileName, i->filename.c_str());

		std::ofstream outFile;
		outFile.open(mFileName, std::ios::binary);
		outFile << mFileString;
		outFile.close();

		mFileString.clear();
	}
}

// *************************************************************************
// *			RemoveFileFromZip:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
ZRESULT CL64_Utilities::RemoveFileFromZip(const TCHAR* zipfn, const TCHAR* zename)
{
	return AddFileToZip(zipfn, zename, 0);
}

// *************************************************************************
// *								AddFileToZip						   *
// *************************************************************************
ZRESULT CL64_Utilities::AddFileToZip(const TCHAR* zipfn, const TCHAR* zename, const TCHAR* zefn)
{
	if (GetFileAttributes(zipfn) == 0xFFFFFFFF || (zefn != 0 && GetFileAttributes(zefn) == 0xFFFFFFFF)) return ZR_NOFILE;
	// Expected size of the new zip will be the size of the old zip plus the size of the new file
	HANDLE hf = CreateFile(zipfn, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0); if (hf == INVALID_HANDLE_VALUE) return ZR_NOFILE; DWORD size = GetFileSize(hf, 0); CloseHandle(hf);
	if (zefn != 0) { hf = CreateFile(zefn, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0); if (hf == INVALID_HANDLE_VALUE) return ZR_NOFILE; size += GetFileSize(hf, 0); CloseHandle(hf); }
	size *= 2; // just to be on the safe side.
	//
	HZIP hzsrc = OpenZip(zipfn, 0); if (hzsrc == 0) return ZR_READ;
	HZIP hzdst = CreateZip(0, size, 0); if (hzdst == 0) { CloseZip(hzsrc); return ZR_WRITE; }
	// hzdst is created in the system pagefile
	// Now go through the old zip, unzipping each item into a memory buffer, and adding it to the new one
	char* buf = 0; unsigned int bufsize = 0; // we'll unzip each item into this memory buffer
	ZIPENTRY ze; ZRESULT zr = GetZipItem(hzsrc, -1, &ze); int numitems = ze.index; if (zr != ZR_OK) { CloseZip(hzsrc); CloseZip(hzdst); return zr; }
	for (int i = 0; i < numitems; i++)
	{
		zr = GetZipItem(hzsrc, i, &ze); if (zr != ZR_OK) { CloseZip(hzsrc); CloseZip(hzdst); return zr; }
		if (_stricmp(ze.name, zename) == 0) continue; // don't copy over the old version of the file we're changing
		if (ze.attr & FILE_ATTRIBUTE_DIRECTORY) { zr = ZipAddFolder(hzdst, ze.name); if (zr != ZR_OK) { CloseZip(hzsrc); CloseZip(hzdst); return zr; } continue; }
		if (ze.unc_size > (long)bufsize) { if (buf != 0) delete[] buf; bufsize = ze.unc_size * 2; buf = new char[bufsize]; }
		zr = UnzipItem(hzsrc, i, buf, bufsize); if (zr != ZR_OK) { CloseZip(hzsrc); CloseZip(hzdst); return zr; }
		zr = ZipAdd(hzdst, ze.name, buf, bufsize); if (zr != ZR_OK) { CloseZip(hzsrc); CloseZip(hzdst); return zr; }
	}
	delete[] buf;
	// Now add the new file
	if (zefn != 0) { zr = ZipAdd(hzdst, zename, zefn); if (zr != ZR_OK) { CloseZip(hzsrc); CloseZip(hzdst); return zr; } }
	zr = CloseZip(hzsrc); if (zr != ZR_OK) { CloseZip(hzdst); return zr; }
	//
	// The new file has been put into pagefile memory. Let's store it to disk, overwriting the original zip
	zr = ZipGetMemory(hzdst, (void**)&buf, &size); if (zr != ZR_OK) { CloseZip(hzdst); return zr; }
	hf = CreateFile(zipfn, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0); if (hf == INVALID_HANDLE_VALUE) { CloseZip(hzdst); return ZR_WRITE; }
	DWORD writ; WriteFile(hf, buf, size, &writ, 0); CloseHandle(hf);
	zr = CloseZip(hzdst); if (zr != ZR_OK) return zr;

	return ZR_OK;
}