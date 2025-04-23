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
// *		Check_Directory_Exists:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Utilities::Check_Directory_Exists(char* szPath)
{
	DWORD dwAttrib = GetFileAttributes(szPath);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
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
// *			Extract_Textures:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Utilities::Extract_Textures(bool Extract_All,const char* Exclude_File)
{
	Ogre::String mFileString;

	Ogre::FileInfoListPtr RFI = ResourceGroupManager::getSingleton().listResourceFileInfo(App->CL_Ogre->Texture_Resource_Group, false);
	Ogre::FileInfoList::const_iterator i, iend;
	iend = RFI->end();

	for (i = RFI->begin(); i != iend; ++i)
	{
		if (i->filename == Exclude_File && Extract_All == 0)
		{

		}
		else
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
}

// *************************************************************************
// *	  		Zip_Assets:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
bool CL64_Utilities::Zip_Assets(char* SourceFolder, char* DestinationFolder)
{
	HZIP hz;

	char poop[MAX_PATH];
	strcpy(poop, DestinationFolder);
	strcat(poop, "Assets.zip");

	hz = CreateZip(_T(poop), 0);

	char SourceFile[MAX_PATH];
	char DestinationFile[MAX_PATH];

	char Path[MAX_PATH];
	strcpy(Path, SourceFolder);
	strcat(Path, "*.*");

	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(Path, &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {

			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				strcpy(SourceFile, SourceFolder);
				strcat(SourceFile, fd.cFileName);

				strcpy(DestinationFile, DestinationFolder);
				strcat(DestinationFile, fd.cFileName);
				ZipAdd(hz, _T(fd.cFileName), _T(SourceFile));
			}

		} while (::FindNextFile(hFind, &fd));

		::FindClose(hFind);
	}

	CloseZip(hz);

	return 1;
}

