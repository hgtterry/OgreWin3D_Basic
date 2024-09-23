/*
Copyright (c) 2024 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OgreWin3D_Stage

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

#include "CL64_Ogre.h"
#include "CL64_Grid.h"
#include "CL64_Keyboard.h"
#include "CL64_Camera.h"
#include "CL64_Ini_File.h"
#include "CL64_Preferences.h"
#include "CL64_Assimp.h"
#include "CL64_Scene.h"
#include "CL64_Importers.h"
#include "CL64_File_IO.h"
#include "CL64_ImGui.h"
#include "CL64_Textures.h"
#include "CL64_TopDlg.h"
#include "CL64_Panels.h"
#include "CL64_Converters.h"
#include "CL64_Bullet.h"
#include "CL64_Player.h"
#include "CL64_Dialogs.h"
#include "CL64_Resources.h"

#include "CL64_Import_Ogre3D.h"
#include "CL64_Motioins.h"

#include "CL64_Exp_Ogre3D.h"
#include "CL64_Exporters.h"
#include "CL64_Dimensions.h"
#include "CL64_Exp_Obj.h"
#include "CL64_Props_Textures.h"

#include "CL64_Mesh_Manager.h"
#include "CL64_Level.h"
#include "CL64_FileView.h"
#include "CL64_Properties.h"
#include "CL64_Project.h"

class CL64_App
{

public:
	CL64_App(void);
	~CL64_App(void);

	CL64_Ogre*				CL_Ogre;
	CL64_Grid*				CL_Grid;
	CL64_Keyboard*			CL_Keyboard;
	CL64_Camera*			CL_Camera;
	CL64_Ini_File*			CL_Ini_File;
	CL64_Preferences*		CL_Preferences;
	CL64_Assimp*			CL_Assimp;
	CL64_Scene*				CL_Scene;
	CL64_Importers*			CL_Importers;
	CL64_File_IO*			CL_File_IO;
	CL64_ImGui*				CL_ImGui;
	CL64_Textures*			CL_Textures;
	CL64_TopDlg*			CL_TopDlg;
	CL64_Panels*			CL_Panels;
	CL64_Converters*		CL_Converters;
	CL64_Bullet*			CL_Bullet;
	CL64_Player*			CL_Player;
	CL64_Dialogs*			CL_Dialogs;
	CL64_Resources*			CL_Resources;
	
	CL64_Imp_Ogre3D*		CL_Import_Ogre3D;
	CL64_Motioins*			CL_Motions;

	CL64_Exp_Ogre3D*		CL_Exp_Ogre;
	CL64_Exporters*			CL_Exporters;
	CL64_Dimensions*		CL_Dimensions;
	CL64_Exp_Obj*			CL_Exp_Obj;
	CL64_Props_Textures*	CL_Props_Textures;
	CL64_Mesh_Manager*		CL_Mesh_Manager;
	CL64_Level*				CL_Level;
	CL64_FileView*			CL_FileView;
	CL64_Properties*		CL_Properties;
	CL64_Project*			CL_Project;

	void InitApp(void);
	void Init_Dialogs(void);
	void Open_Tool(char* Tool);
	void Say(const char* Message, char* Message2 = NULL);
	void Say_Win(const char* Message);
	void Say_Int(int Value);
	void Say_Float(float Value);
	void Set_Title();

	bool SetMainWinCentre(void) const;
	void Flash_Window();
	void Debug_Text();

	void LoadProgramResource(void);
	void SetBrushes_Fonts(void);

	void Custom_Button_Toggle(LPNMCUSTOMDRAW item, bool Toggle) const;
	void Custom_Button_Toggle_Tabs(LPNMCUSTOMDRAW item, bool Toggle) const;
	void Custom_Button_Normal(LPNMCUSTOMDRAW item) const;
	void Custom_Button_Globals(LPNMCUSTOMDRAW item) const;
	void Custom_Button_Greyed(LPNMCUSTOMDRAW item);

	HINSTANCE hInst;
	HWND MainHwnd;
	HWND Fdlg;
	HWND ViewGLhWnd;
	HWND ViewPLeaseWait;
	HWND ListPanel;

	int CursorPosX;
	int CursorPosY;

	char GD_Directory_FullPath[MAX_PATH];
	char App_Version[MAX_PATH];

	bool flag_OgreStarted;			//Has Ogre3D started properly
	bool flag_Block_Mouse_Buttons;	//Block Mouse Buttons
	bool flag_Debug_App;			//App Debug Mode 
	bool flag_Debug_Textures;		//App Debug Textures

	HFONT Font_CB15;
	HFONT Font_CB18;
	HFONT Font_Arial20;

	HANDLE Hnd_TexturesOn_Bmp;
	HANDLE Hnd_TexturesOff_Bmp;
	HANDLE Hnd_MeshOn_Bmp;
	HANDLE Hnd_MeshOff_Bmp;
	HANDLE Hnd_MeshPointsOn_Bmp;
	HANDLE Hnd_MeshPointsOff_Bmp;
	HANDLE Hnd_BBOn_Bmp;
	HANDLE Hnd_BBOff_Bmp;
	HANDLE Hnd_BonesOn_Bmp;
	HANDLE Hnd_BonesOff_Bmp;
	HANDLE Hnd_HairOn_Bmp;
	HANDLE Hnd_HairOff_Bmp;
	HANDLE Hnd_GridOn_Bmp;
	HANDLE Hnd_GridOff_Bmp;
	HANDLE Hnd_ModelInfo_Bmp;
	HANDLE Hnd_NormalsOn_Bmp;
	HANDLE Hnd_NormalsOff_Bmp;
	HANDLE Hnd_ModelInfoOn_Bmp;

	HPEN BlackPen;
	HPEN GreenPen;
	HPEN WhitePen;

	HBRUSH AppBackground;
	HBRUSH BlackBrush;
	HBRUSH Brush_White;
	HBRUSH Brush_But_Normal;
	HBRUSH Brush_But_Hover;
	HBRUSH Brush_But_Pressed;
	HBRUSH Brush_Green;
	HBRUSH Brush_Tabs;
	HBRUSH Brush_Tabs_UnSelected;

	HBRUSH HotBrush_1;
	HBRUSH HotBrush_2;

	HCURSOR CUR;
	HMENU mMenu;
};

extern CL64_App* App;

