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

// *************************************************************************
// *		    Init_Doc:- Terry and Hazel Flanigan 2024 	    		   *
// *************************************************************************
const char* CL64_Doc::FindTextureLibrary(char const* WadName)
{
   static char WorkPath[MAX_PATH];
   /* const Prefs* pPrefs;*/

    /*::FilePath_AppendName(LastPath, WadName, WorkPath);
    if (_access(WorkPath, 0) == 0)
    {
        return WorkPath;
    }*/

   /* pPrefs = GetPrefs();
    if (FilePath_SearchForFile(WadName, Prefs_GetTxlSearchPath(pPrefs), WorkPath))*/
    {
        return WorkPath;
    }
    // changed QD 01/04
    //	return NULL;
    // code from RFEdit Pro
    //else
    {
        std::string	txlPathError = _T("World Editor checks for the default *.txl file when opening new worlds.\n\n");
        txlPathError += _T("World Editor now is looking for:\n\n");
        txlPathError += _T(WorkPath);
        txlPathError += _T("\n\nbut cannot find it.\n\n");
        txlPathError += _T("To avoid this message in the future, place or rename a *.txl file as: ");
        txlPathError += _T(WadName);
        txlPathError += _T("\ninto the above path. Or edit the WrldEdit.ini file to reflect\n");
        txlPathError += _T("the name of your default *.txl file.\n\n");
        txlPathError += _T("For now, please select a valid *.txl file");// or the application will exit.");

        MessageBox(NULL, txlPathError.c_str(), "World Editor cannot find default *.txl", MB_OK);

        //CFileDialog FileDlg(TRUE, "txl", WorkPath, OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR,
        //    "Texture Library Files (*.txl)|*.txl||");

        //FileDlg.m_ofn.lpstrTitle = "World Editor Texture Finder";

        //if (FileDlg.DoModal() == IDOK)
        //{
        //    CString	wadpath = FileDlg.GetPathName();
        //    CString wadFileName = FileDlg.GetFileName();
        //    strcpy(WorkPath, wadpath.GetBuffer(260));
        //    // set the Prefs wadfile to THIS one -- this session only
        //    Prefs_SetTxlName((Prefs*)pPrefs, wadFileName.GetBuffer(64));
        //}

        return WorkPath;
    }
    // end change
}

