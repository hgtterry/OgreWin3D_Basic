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
//#include "vfile.h"

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
	/*if (pe->LockedBitmap != NULL)
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
	}*/
}

// *************************************************************************
// *								 Setup								   *
// *************************************************************************
signed int CL64_WadFile::Setup()
{
		DestroyBitmapArray();

		int Count = 0;
		int nFiles = App->CL_TXL_Editor->Texture_Count;

		if (nFiles > 0)
		{
			mBitmaps = (WadFileEntry*)App->CL_Maths->Ram_Allocate(nFiles * sizeof(WadFileEntry));

			while (Count < nFiles)
			{
				WadFileEntry* pe;
				pe = &mBitmaps[mBitmapCount];
				pe->Height = App->CL_TXL_Editor->Texture_List[Count]->Height;
				pe->Width = App->CL_TXL_Editor->Texture_List[Count]->Width;
				strcpy(pe->Name, App->CL_TXL_Editor->Texture_List[Count]->Name);

				++mBitmapCount;
				Count++;
			}

		}

	return true;
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
