/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OW3D_Mesh_Builder

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

#pragma once

#include "CL64_MapEditor.h"
#include "CL64_Maths.h"
#include "CL64_Box.h"
#include "CL64_Render.h"
#include "CL64_Level.h"
#include "CL64_Prefs.h"
#include "CL64_Ini_File.h"
#include "CL64_Doc.h"
#include "CL64_Utilities.h"
#include "CL64_Brush.h"
#include "CL64_FaceList.h"
#include "CL64_Face.h"
#include "CL64_BrushTemplate.h"
#include "CL64_SelBrushList.h"
#include "CL64_Array.h"
#include "CL64_SelFaceList.h"
#include "CL64_WadFile.h"
#include "CL64_Mesh_Mgr.h"
#include "CL64_Brush_X.h"
// ------------------------------
#include "CL64_Dialogs.h"
#include "CL64_Properties_Tabs.h"
#include "CL64_Properties_Templates.h"
#include "CL64_Properties_Brushes.h"
#include "CL64_Properties_Textures.h"
#include "CL64_Properties_Faces.h"
#include "CL64_Ogre.h"
#include "CL64_Grid.h"
#include "CL64_Panels.h"
#include "CL64_Keyboard.h"
#include "CL64_Model.h"
#include "CL64_Ogre3D.h"
#include "CL64_Top_Tabs.h"
#include "CL64_File.h"
#include "CL64_ParseFile.h"
#include "CL64_Export.h"
#include "CL64_File_IO.h"
#include "CL64_ImGui.h"
#include "CL64_TXL_Editor.h"
#include "CL64_Textures.h"
#include "CL64_Resources.h"
#include "CL64_Picking.h"
#include "CL64_Camera.h"

// Templates Dialogs
#include "A_CreateBoxDialog.h"
#include "A_CreateCylDialog.h"
#include "A_CreateConeDialog.h"
#include "A_CreateStaircaseDialog.h"
#include "A_CreateArchDialog.h"

class CL64_App
{
public:
	CL64_App(void);
	~CL64_App(void);

	CL64_MapEditor*				CL_MapEditor;
	CL64_Maths*					CL_Maths;
	CL64_Box*					CL_Box;
	CL64_Render*				CL_Render;
	CL64_Level*					CL_Level;
	CL64_Prefs*					CL_Prefs;
	CL64_Ini_File*				CL_Ini_File;
	CL64_Doc*					CL_Doc;
	CL64_Utilities*				CL_Utilities;
	CL64_Brush*					CL_Brush;
	CL64_FaceList*				CL_FaceList;
	CL64_Face*					CL_Face;
	CL64_BrushTemplate*			CL_BrushTemplate;
	CL64_SelBrushList*			CL_SelBrushList;
	CL64_Array*					CL_Array;
	CL64_SelFaceList*			CL_SelFaceList;
	CL64_Mesh_Mgr*				CL_Mesh_Mgr;
	CL64_Brush_X*				CL_Brush_X;
	// ------------------------------
	CL64_Dialogs*				CL_Dialogs;
	CL64_Properties_Tabs*		CL_Properties_Tabs;
	CL64_Properties_Templates*	CL_Properties_Templates;
	CL64_Properties_Brushes*	CL_Properties_Brushes;
	CL64_Properties_Textures*	CL_Properties_Textures;
	CL64_Properties_Faces*		CL_Properties_Faces;
	CL64_Ogre*					CL_Ogre;
	CL64_Grid*					CL_Grid;
	CL64_Panels*				CL_Panels;
	CL64_Keyboard*				CL_Keyboard;
	CL64_Model*					CL_Model;
	CL64_Ogre3D*				CL_Ogre3D;
	CL64_Top_Tabs*				CL_Top_Tabs;
	CL64_File*					CL_File;
	CL64_ParseFile*				CL_ParseFile;
	CL64_Export*				CL_Export;
	CL64_File_IO*				CL_File_IO;
	CL64_ImGui*					CL_ImGui;
	CL64_TXL_Editor*			CL_TXL_Editor;
	CL64_Textures*				CL_Textures;
	CL64_Resources*				CL_Resources;
	CL64_Picking*				CL_Picking;
	CL64_Camera*				CL_Camera;

	// Templates Dialogs
	A_CreateBoxDialog*			CL_CreateBoxDialog;
	A_CreateCylDialog*			CL_CreateCylDialog;
	A_CreateConeDialog*			CL_CreateConeDialog;
	A_CreateStaircaseDialog*	CL_CreateStaircaseDialog;
	A_CreateArchDialog*			CL_CreateArchDialog;

	void InitApp(void);
	void SetBrushes_Fonts(void);
	void LoadProgramResource(void);

	bool Custom_Button_Greyed(LPNMCUSTOMDRAW item);
	bool Custom_Button_Normal(LPNMCUSTOMDRAW item);
	bool Custom_Button_Toggle(LPNMCUSTOMDRAW item, bool Toggle);
	bool Custom_Button_Toggle_Tabs(LPNMCUSTOMDRAW item, bool Toggle);

	void Flash_Window();
	void SetMainWinCentre(void) const;

	void Report_Error(const char* Msg, ...);
	void Say(const char* Message, const char* Message2 = NULL);
	void Say_Int(int Value);
	void Say_Float(float Value);
	void Say_Vector3(Ogre::Vector3 Vector);

	void Say_Win(const char* Message);
	void Set_Title(const char* Title);

	HINSTANCE hInst;
	HWND MainHwnd;
	HBRUSH AppBackground;
	
	HWND ViewPLeaseWait;

	HBRUSH Brush_But_Pressed;
	HBRUSH Brush_But_Normal;
	HBRUSH Brush_But_Hover;
	HBRUSH Brush_Tabs_UnSelected;
	HBRUSH Brush_Tabs;

	HBRUSH BlackBrush;
	HBRUSH Brush_White;
	HBRUSH Brush_Green;

	HFONT Font_CB15;
	HFONT Font_CB18;
	HFONT Font_Arial20;
	HFONT Font_Banner;

	HANDLE Hnd_SolidBox_Bmp;
	HANDLE Hnd_HollowBox_Bmp;

	HANDLE Hnd_Stairs_Bmp;
	HANDLE Hnd_Ramp_Bmp;

	HANDLE Hnd_Arch_Icon;
	HANDLE Hnd_Cone_Icon;
	HANDLE Hnd_Cyl_Icon;
	HANDLE Hnd_Spheriod_Icon;
	HANDLE Hnd_Box_Icon;
	HANDLE Hnd_Stairs_Icon;

	HCURSOR CUR;
	HMENU mMenu;

	bool flag_Debug_App;
	bool flag_OgreStarted;

	int CursorPosX;
	int CursorPosY;

	char RB_Directory_FullPath[MAX_PATH];
	char App_Title[MAX_PATH];
};

extern CL64_App* App;

