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
#include "CL64_Doc.h"

CL64_Doc::CL64_Doc(void)
{
    LastTemplateTypeName[0] = 0;

}

CL64_Doc::~CL64_Doc(void)
{
}

// *************************************************************************
// *		    Init_Doc:- Terry and Hazel Flanigan 2024 	    		   *
// *************************************************************************
void CL64_Doc::Init_Doc()
{
	
    const char* DefaultWadName;
    strcpy(LastTemplateTypeName, "Box");

    DefaultWadName = App->CL_Prefs->Wad_File_Name;

    // strcpy(LastPath, Prefs_GetProjectDir(pPrefs));

    const char* WadPath = ""; //"FindTextureLibrary(DefaultWadName);

   // App->Say_Win(DefaultWadName);
}
