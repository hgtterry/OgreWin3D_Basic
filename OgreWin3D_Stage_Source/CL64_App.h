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
#include "CL64_Scene.h"
#include "CL64_Importers.h"
#include "CL64_File_IO.h"
#include "CL64_ImGui.h"
#include "CL64_ImGui_Dialogs.h"
#include "CL64_TopDlg.h"
#include "CL64_Panels.h"
#include "CL64_Bullet.h"
#include "CL64_Com_Player.h"
#include "CL64_Dialogs.h"
#include "CL64_Resources.h"
#include "CL64_Textures.h"
#include "CL64_Dimensions.h"

#include "CL64_FileView.h"
#include "CL64_Properties.h"
#include "CL64_Project.h"
#include "CL64_Com_Area.h"
#include "CL64_Objects_Create.h"
#include "CL64_Com_Objects.h"
#include "CL64_Physics.h"

#include "CL64_Com_Collectables.h"
#include "CL64_Com_Teleporters.h"
#include "CL64_Com_Environments.h"
#include "CL64_Com_Messages.h"
#include "CL64_Com_MoveEntity.h"
#include "CL64_Com_Particles.h"
#include "CL64_Com_Sounds.h"
#include "CL64_SB_Collision.h"
#include "CL64_SoundMgr.h"
#include "CL64_Com_Counters.h"
#include "CL64_LookUps.h"

#include "CL64_Gui_Environment.h"
#include "CL64_Props_Dialogs.h"
#include "CL64_Gizmos.h"
#include "CL64_MeshViewer.h"
#include "CL64_Materials.h"
#include "CL64_Locations.h"
#include "CL64_Project_Create.h"
#include "CL64_Picking.h"
#include "CL64_Build_Game.h"
#include "CL64_PB.h"
#include "CL64_Motions.h"
#include "CL64_Utilities.h"
#include "Bass_Front_Dialog.h"
#include "CL64_MapEditor.h"

class CL64_App
{

public:
	CL64_App(void);
	~CL64_App(void);

	CL64_Ogre*				CL_Ogre;
	CL64_Grid*				CL_Grid;
	CL64_Keyboard*			CL_Keyboard;
	CL64_Com_Cameras*		CL_Com_Cameras;
	CL64_Ini_File*			CL_Ini_File;
	CL64_Preferences*		CL_Preferences;
	CL64_Scene*				CL_Scene;
	CL64_Importers*			CL_Importers;
	CL64_File_IO*			CL_File_IO;
	CL64_ImGui*				CL_ImGui;
	CL64_ImGui_Dialogs*		CL_ImGui_Dialogs;
	CL64_TopDlg*			CL_TopDlg;
	CL64_Panels*			CL_Panels;
	CL64_Bullet*			CL_Bullet;
	CL64_Com_Player*		CL_Com_Player;
	CL64_Dialogs*			CL_Dialogs;
	CL64_Resources*			CL_Resources;
	CL64_Textures*			CL_Textures;

	CL64_Dimensions*		CL_Dimensions;

	// Stage
	CL64_FileView*			CL_FileView;
	CL64_Properties*		CL_Properties;
	CL64_Project*			CL_Project;
	CL64_Com_Area*			CL_Com_Area;
	CL64_Objects_Create*	CL_Objects_Create;
	CL64_Com_Objects*		CL_Com_Objects;
	CL64_Physics*			CL_Physics;
	CL64_Com_Collectables*	CL_Com_Collectables;
	CL64_Com_Teleporters*	CL_Com_Teleporters;
	CL64_Com_Environments*	CL_Com_Environments;
	CL64_Com_Messages*		CL_Com_Messages;
	CL64_Com_MoveEntity*	CL_Com_MoveEntity;
	CL64_Com_Particles*		CL_Com_Particles;
	CL64_Com_Sounds*		CL_Com_Sounds;
	CL64_Collision*			CL_Collision;
	CL64_SoundMgr*			CL_SoundMgr;
	CL64_Com_Counters*		CL_Com_Counters;
	CL64_LookUps*			CL_LookUps;
	CL64_Gui_Environment*	CL_Gui_Environment;
	CL64_Props_Dialogs*		CL_Props_Dialogs;
	CL64_Gizmos*			CL_Gizmos;
	CL64_MeshViewer*		CL_MeshViewer;
	CL64_Materials*			CL_Materials;
	CL64_Locations*			CL_Locations;
	CL64_Project_Create*	CL_Project_Create;
	CL64_Picking*			CL_Picking;
	CL64_Build_Game*		CL_Build_Game;
	CL64_PB*				CL_PB;
	CL64_Motions*			CL_Motions;
	CL64_Utilities*			CL_Utilities;
	CL64_MapEditor*			CL_MapEditor;

	Bass_Front_Dialog*		CL_Front_Dialog;
	

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

	void Check_Menu_Camera_Data(bool Enable);

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
	HFONT Font_Banner;

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
	HBRUSH DialogBackGround;
	HBRUSH Brush_Panel;
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

