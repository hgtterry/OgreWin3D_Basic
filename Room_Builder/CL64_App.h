/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OW3D Scene Builder

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

#include "CL64_Editor_Map.h"

#include "G_Box.h"

#include "CL64_Render_Map.h"
#include "CL64_Level.h"

#include "Lib_Preference.h"
#include "Lib_Ini_File.h"

#include "CL64_Doc.h"
#include "CL64_Utilities.h"

#include "CX_Brush.h"

#include "CX_FaceList.h"

#include "CX_Face.h"

#include "BrushTemplate.h"

#include "CX_SelBrushList.h"
#include "CX_Array.h"

#include "CX_SelFaceList.h"
#include "Shapes_3D.h"

#include "CL64_WadFile.h"
#include "CL64_Mesh_Mgr.h"
#include "CL64_Brush_X.h"
// ------------------------------
#include "CL64_Dialogs.h"
#include "CL64_Properties_Tabs.h"
#include "CL64_Properties_Templates.h"
#include "CL64_Properties_Brushes.h"
#include "CL64_Properties_Textures.h"

#include "Face_Editor.h"
#include "CL64_Properties_Faces.h"

#include "CL64_Ogre.h"
#include "CL64_Panels.h"
#include "CL64_Keyboard.h"
#include "CL64_Scene.h"
#include "CL64_Ogre3D.h"
#include "CL64_Top_Tabs.h"
#include "CL64_File.h"
#include "CL64_ParseFile.h"
#include "CL64_Export.h"
#include "CL64_File_IO.h"
#include "CL64_ImGui.h"
#include "CL64_ImGui_Dialogs.h"
#include "CL64_ImGui_Editor.h"
#include "CL64_TXL_Editor.h"
#include "CL64_Textures.h"
#include "CL64_Resources.h"
#include "CL64_Picking.h"
#include "CL64_Camera.h"
#include "CL64_Physics.h"
#include "CL64_Entities.h"

#include "CL64_Com_Player.h"
#include "CL64_Com_Environments.h"
#include "CL64_Com_Objects.h"
#include "CL64_Com_Sounds.h"
#include "CL64_PB.h"

// Scene Editor
#include "CL64_Editor_Scene.h"
#include "CL64_FileView.h"
#include "CL64_Gui_Environment.h"
#include "CL64_Properties_Scene.h"

#include "CL64_Editor_Control.h"

// Templates Dialogs
#include "App_Templates.h"
#include "CreateBoxDialog.h"
#include "CreateConeDialog.h"
#include "CreateCylDialog.h"
#include "CreateStaircaseDialog.h"

// Project
#include "CL64_Project.h"
#include "CL64_Objects_Create.h"
#include "CL64_SoundMgr.h"
#include "CL64_Collision.h"
#include "CL64_Gizmos.h"
#include "CL64_Dimensions.h"
#include "CL64_Props_Dialogs.h"
#include "CL64_MeshViewer.h"

#include "CL64_Locations.h"
#include "CL64_Teleporters.h"

class CL64_App
{
public:
	CL64_App(void);
	~CL64_App(void);

	CL64_Editor_Map*			CL_Editor_Map;
	CL64_Lib_Maths*				CL_X_Maths;		// Maths Library

	CX_Box*						CL_X_Box;		// Box Library
	CL64_Render_Map*			CL_Render;
	CL64_Level*					CL_Level;

	Lib_Preference*				CL_X_Preference; // Preference Library
	
	Lib_Ini_File*				CL_X_Ini_File;	 // Ini_File Library

	CL64_Doc*					CL_Doc;
	CL64_Utilities*				CL_Utilities;

	CX_Brush*					CL_X_Brush;		 // Brush Library

	CX_FaceList*				CL_X_FaceList;	// FaceList Library

	CX_Face*					CL_X_Face;		 // Face Library
	
	BrushTemplate*				CL_X_BrushTemplate; // BrushTemplate Library
	CX_SelBrushList*			CL_X_SelBrushList;	// SelBrushList Library
	CX_Array*					CL_X_Array;			// Array Library

	CX_SelFaceList*				CL_X_SelFaceList;	// SelFaceList Library

	CL64_Mesh_Mgr*				CL_Mesh_Mgr;
	CL64_Brush_X*				CL_Brush_X;
	// ------------------------------
	CL64_Dialogs*				CL_Dialogs;
	CL64_Properties_Tabs*		CL_Properties_Tabs;
	CL64_Properties_Templates*	CL_Properties_Templates;
	CL64_Properties_Brushes*	CL_Properties_Brushes;
	CL64_Properties_Textures*	CL_Properties_Textures;

	Face_Editor*				CL_X_Face_Editor;
	CL64_Properties_Faces*		CL_Properties_Faces;

	CL64_Ogre*					CL_Ogre;
	CL64_Panels*				CL_Panels;
	CL64_Keyboard*				CL_Keyboard;
	CL64_Scene*					CL_Scene;
	CL64_Ogre3D*				CL_Ogre3D;
	CL64_Top_Tabs*				CL_Top_Tabs;
	CL64_File*					CL_File;
	CL64_ParseFile*				CL_ParseFile;
	CL64_Export*				CL_Export;
	CL64_File_IO*				CL_File_IO;
	CL64_ImGui*					CL_ImGui;
	CL64_ImGui_Dialogs*			CL_ImGui_Dialogs;
	CL64_ImGui_Editor*			CL_ImGui_Editor;
	CL64_TXL_Editor*			CL_TXL_Editor;
	CL64_Textures*				CL_Textures;
	CL64_Resources*				CL_Resources;
	CL64_Picking*				CL_Picking;
	CL64_Camera*				CL_Camera;
	CL64_Physics*				CL_Physics;
	CL64_Entities*				CL_Entities;

	CL64_Com_Player*			CL_Com_Player;
	CL64_Com_Environments*		CL_Com_Environments;
	CL64_Com_Objects*			CL_Com_Objects;
	CL64_Com_Sounds*			CL_Com_Sounds;
	CL64_Gui_Environment*		CL_Gui_Environment;
	CL64_Properties_Scene*		CL_Properties_Scene;
	CL64_PB*					CL_PB;

	// Scene Editor
	CL64_Editor_Scene*			CL_Editor_Scene;
	CL64_FileView*				CL_FileView;

	CL64_Editor_Control*		CL_Editor_Control;

	// Templates Dialogs Libraries
	App_Templates*				CL_App_Templates;
	CreateConeDialog*			CL_X_CreateConeDialog;
	CreateCylDialog*			CL_X_CreateCylDialog;
	CreateStaircaseDialog*		CL_X_CreateStaircaseDialog;
	Shapes_3D*					CL_X_Shapes_3D;

	// Project
	CL64_Project*				CL_Project;
	CL64_Objects_Create*		CL_Objects_Create;
	CL64_SoundMgr*				CL_SoundMgr;
	CL64_Collision*				CL_Collision;
	CL64_Gizmos*				CL_Gizmos;
	CL64_Dimensions*			CL_Dimensions;
	CL64_Props_Dialogs*			CL_Props_Dialogs;
	CL64_MeshViewer*			CL_MeshViewer;
	CL64_Locations*				CL_Locations;
	CL64_Teleporters*			CL_Teleporters;

	void InitApp(void);
	void Init_Dialogs(void);
	void Set_Brushes_Fonts(void);
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
	void BeepBeep();

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

	HFONT Font_CB10;
	HFONT Font_CB15;
	HFONT Font_CB18;
	HFONT Font_Arial20;
	HFONT Font_Banner;

	HANDLE Hnd_SolidBox_Bmp;
	HANDLE Hnd_HollowBox_Bmp;

	HANDLE Hnd_Stairs_Bmp;
	HANDLE Hnd_Ramp_Bmp;

	HANDLE Hnd_TR_Off_Bmp;
	HANDLE Hnd_TR_On_Bmp;
	HANDLE Hnd_Help_Bmp;

	HANDLE Hnd_Arch_Icon;
	HANDLE Hnd_Cone_Icon;
	HANDLE Hnd_Cyl_Icon;
	HANDLE Hnd_Spheriod_Icon;
	HANDLE Hnd_Box_Icon;
	HANDLE Hnd_Stairs_Icon;

	HCURSOR CUR;
	HMENU Menu_Map;
	HMENU Menu_Scene;

	HWND ViewGLhWnd;
	HWND ListPanel;

	bool flag_Debug_App;
	bool flag_OgreStarted;
	bool flag_Block_Mouse_Buttons;

	bool Development;

	int CursorPosX;
	int CursorPosY;

	char RB_Directory_FullPath[MAX_PATH];
	char App_Title[MAX_PATH];
};

extern CL64_App* App;

