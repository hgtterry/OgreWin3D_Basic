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
#include "CL64_WadFile.h"
#include "vfile.h"

CL64_WadFile::CL64_WadFile()
{
	mBitmapCount = 0;
	mBitmaps = NULL;
}

CL64_WadFile::~CL64_WadFile()
{
}

// *************************************************************************
// *						WadFileEntry_Free							   *
// *************************************************************************
static void WadFileEntry_Free(WadFileEntry* pe)
{
	if (pe->LockedBitmap != NULL)
	{
		geBitmap_UnLock(pe->LockedBitmap);
	}
	if (pe->bmp != NULL)
	{
		geBitmap_Destroy(&pe->bmp);
	}
	if (pe->Name != NULL)
	{
		App->CL_Maths->Ram_Free(pe->Name);
	}
}

// *************************************************************************
// *						 wadCountFiles								   *
// *************************************************************************
static int wadCountFiles(geVFile* vfs, const char* fspec)
{
	int nFiles = 0;
	geVFile_Finder* Finder;

	// count the number of files
	Finder = geVFile_CreateFinder(vfs, fspec);
	if (Finder != NULL)
	{
		while (geVFile_FinderGetNextFile(Finder) != GE_FALSE)
		{
			++nFiles;
		}
		geVFile_DestroyFinder(Finder);
	}
	return nFiles;
}

// *************************************************************************
// *								 Setup								   *
// *************************************************************************
signed int CL64_WadFile::Setup(const char* Filename)
{
	char Path[MAX_PATH];

	strcpy(Path, App->RB_Directory_FullPath);
	strcat(Path, "\\Data\\Room_Builder\\Default.txl");

	geVFile* Library;

	signed int	NoErrors = GE_FALSE;

	Library = geVFile_OpenNewSystem(NULL, GE_VFILE_TYPE_VIRTUAL, Path, NULL, GE_VFILE_OPEN_READONLY | GE_VFILE_OPEN_DIRECTORY);
	if (Library != NULL)
	{
		NoErrors = GE_TRUE;

		DestroyBitmapArray();

		int nFiles = wadCountFiles(Library, "*.*");

		if (nFiles > 0)
		{
			// make new array and fill it with loaded bitmaps
			mBitmaps = (WadFileEntry*)App->CL_Maths->Ram_Allocate(nFiles * sizeof(WadFileEntry));

			// and fill array with filenames
			NoErrors = GE_FALSE;
			geVFile_Finder* Finder = geVFile_CreateFinder(Library, "*.*");
			if (Finder != NULL)
			{
				NoErrors = GE_TRUE;
				geVFile_Properties Props;
			
				while (geVFile_FinderGetNextFile(Finder) != GE_FALSE)
				{
					geVFile_FinderGetProperties(Finder, &Props);

					// load the file and create a DibBitmap from it
					geVFile* BmpFile = geVFile_Open(Library, Props.Name, GE_VFILE_OPEN_READONLY);
					geBitmap* TheBitmap;

					if (BmpFile == NULL)
					{
						NoErrors = GE_FALSE;
					}
					else
					{
						TheBitmap = geBitmap_CreateFromFile(BmpFile);
						geVFile_Close(BmpFile);
						if (TheBitmap == NULL)
						{
							NoErrors = GE_FALSE;
						}
						else
						{
							if (geBitmap_SetFormat(TheBitmap, GE_PIXELFORMAT_16BIT_555_RGB, GE_FALSE, 0, NULL) != GE_FALSE)
							{
								WadFileEntry* pe;
								geBitmap_Info info, info2;

								geBitmap_GetInfo(TheBitmap, &info, &info2);
								pe = &mBitmaps[mBitmapCount];
								pe->bmp = TheBitmap;
								pe->Height = info.Height;
								pe->Width = info.Width;
								strcpy(pe->Name,Props.Name);
								geBitmap_LockForReadNative(pe->bmp, &pe->LockedBitmap, 0, 0);
								pe->BitsPtr = geBitmap_GetBits(pe->LockedBitmap);

								++mBitmapCount;
							}
							else
							{
								geBitmap_Destroy(&TheBitmap);
								NoErrors = GE_FALSE;
							}
						}
					}
				}

				geVFile_DestroyFinder(Finder);
			}
		}
		geVFile_Close(Library);
	}

	return GE_TRUE;
}

// *************************************************************************
// *						 DestroyBitmapArray							   *
// *************************************************************************
void CL64_WadFile::DestroyBitmapArray()
{
	if (mBitmaps != NULL)
	{
		for (; mBitmapCount > 0; --mBitmapCount)
		{
			WadFileEntry_Free(&mBitmaps[mBitmapCount - 1]);
		}
		App->CL_Maths->Ram_Free(mBitmaps);
	}
}
