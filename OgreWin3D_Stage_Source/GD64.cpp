/*
Copyright (c) OgreWin3D_Basic 2024 W.T.Flanigan H.C.Flanigan Inflanite_HGT

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely.

An acknowledgment in the product documentation would be
appreciated but is not required.
*/

#include "pch.h"
#include "framework.h"
#include "GD64.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
TCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void StartOgre();
void Close_App();
LRESULT CALLBACK Ogre3D_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ViewerMain_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

int Block_Call = 0;

CL64_App* App = NULL;

// *************************************************************************
// *				WinMain:- Terry and Hazel Flanigan 2024		  		   *
// *************************************************************************
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    InitCommonControls();

    App = new CL64_App();

	char* a_cwd = _getcwd(App->GD_Directory_FullPath, MAX_PATH);

    App->InitApp();
	
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_GD64, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

	App->mMenu = GetMenu(App->MainHwnd);
	App->CL_Ogre->Clear_ErrorLog();
	App->Init_Dialogs();
	
	MSG  msg;
	while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
    SetTimer(App->MainHwnd, 1, 100, NULL);

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GD64));

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

// *************************************************************************
// *			MyRegisterClass:- Terry and Hazel Flanigan 2024	  	 	   *
// *************************************************************************
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = 0;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GD64));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCE(IDC_GD64);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassEx(&wcex);
}

// *************************************************************************
// *			InitInstance:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   App->hInst = hInstance; // Store instance handle in our global variable

   App->MainHwnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
       0, 0, 1200, 770, nullptr, nullptr, App->hInst, nullptr);

   App->Fdlg = CreateDialog(App->hInst, (LPCTSTR)IDD_FILEVIEW, App->MainHwnd, (DLGPROC)ViewerMain_Proc);

   int cx = GetSystemMetrics(SM_CXSCREEN);
   int cy = GetSystemMetrics(SM_CYSCREEN);
   MoveWindow(App->Fdlg, 0, 0, cx, cy, TRUE);

   App->ViewGLhWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_VIEWER3D, App->Fdlg, (DLGPROC)Ogre3D_Proc);

   App->CL_Ogre->RenderHwnd = App->ViewGLhWnd;

   if (!App->MainHwnd)
   {
      return FALSE;
   }

   if (App->CL_Preferences->flag_Start_FullScreen == 1)
   {
	   ShowWindow(App->MainHwnd, SW_SHOWMAXIMIZED);
   }
   else
   {
	   ShowWindow(App->MainHwnd, nCmdShow);
   }
  
   UpdateWindow(App->MainHwnd);

   return TRUE;
}

// *************************************************************************
// *				WndProc:- Terry and Hazel Flanigan 2024	  			   *
// *************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);

		switch (wmId)
		{
		// Debug -------------------------------------------------------
		case ID_DEBUG_GENERAL:
		{
			char Test_Project[MAX_PATH];
			strcpy(Test_Project, App->GD_Directory_FullPath);
			strcat(Test_Project, "\\Projects\\First_Project_Prj\\Project.owproj");

			bool test = App->CL_Importers->Reload_FromResentFiles(Test_Project);
			if (test == 1)
			{
				App->CL_Scene->Set_Scene();
			}

			return TRUE;
		}
		
		case ID_DEBUG_APP:
		{
			if (App->CL_ImGui->flag_Show_App_Debug == 1)
			{
				App->CL_ImGui->flag_Show_App_Debug = 0;
				App->CL_FileView->Show_FileView(true);
			}
			else
			{
				App->CL_ImGui->flag_Show_App_Debug = 1;
				App->CL_FileView->Show_FileView(false);
			}
			return TRUE;
		}
			
		case ID_DEBUG_CAMERADATA:
		{	
			if (App->CL_ImGui->flag_Show_Camera_Data == 1)
			{
				App->CL_ImGui->flag_Show_Camera_Data = 0;
			}
			else
			{
				App->CL_ImGui->flag_Show_Camera_Data = 1;
			}
			return TRUE;
		}

		case ID_DEBUG_TESTPREFERANCE:
		{
			App->CL_Preferences->Read_Preferences();
			return TRUE;
		}

		case ID_IMGUI_IMGUIDEMO:
		{
			if (App->CL_ImGui->flag_Show_ImGui_Demo == 1)
			{
				App->CL_ImGui->flag_Show_ImGui_Demo = 0;
			}
			else
			{
				App->CL_ImGui->flag_Show_ImGui_Demo = 1;
			}

			return TRUE;
		}

		// Windows -------------------------------------------------------

		case ID_WINDOWS_MODELDATA:
		{
			return TRUE;
		}

		case ID_WINDOWS_TEXTURESDIALOG:
		{
			return TRUE;
		}
		
		case ID_WINDOWS_FILEVIEW:
		{
			if (App->CL_FileView->Flag_FileView_Active == 1)
			{
				App->CL_FileView->Show_FileView(false);
			}
			else
			{
				App->CL_FileView->Show_FileView(true);
			}

			return TRUE;
		}
		
		case ID_WINDOWS_FPS:
		{
			if (App->CL_ImGui->flag_Show_FPS == 1)
			{
				App->CL_ImGui->flag_Show_FPS = 0;
			}
			else
			{
				App->CL_ImGui->flag_Show_FPS = 1;
			}

			return TRUE;
		}

		case ID_WINDOWS_IMGUI:
		{
			if (App->CL_ImGui->flag_Show_ImGui_Demo == 1)
			{
				App->CL_TopDlg->Enable_ImGui_Demo_Panel(false);
			}
			else
			{
				App->CL_TopDlg->Enable_ImGui_Demo_Panel(true);
			}

			return TRUE;
		}

		case ID_WINDOWS_PROPERTIES:
		{

			if (App->CL_Properties->flag_Properties_Dlg_Active == 1)
			{
				App->CL_Properties->flag_Properties_Dlg_Active = 0;
				CheckMenuItem(App->mMenu, ID_WINDOWS_PROPERTIES, MF_BYCOMMAND | MF_UNCHECKED);

				ShowWindow(App->CL_Properties->Properties_Dlg_hWnd, 0);
			}
			else
			{
				App->CL_Properties->flag_Properties_Dlg_Active = 1;
				CheckMenuItem(App->mMenu, ID_WINDOWS_PROPERTIES, MF_BYCOMMAND | MF_CHECKED);

				ShowWindow(App->CL_Properties->Properties_Dlg_hWnd, 1);
			}

			return 1;
		}

		// Tools -------------------------------------------------------
		case ID_TOOLS_RESOURCEVIEWER:
		{
			App->CL_Resources->Start_Resources();
			return TRUE;
		}
		
		case ID_TOOLS_SOUNDPLAYER:
		{
			App->CL_SoundMgr->Dialog_SoundFile();
			return TRUE;
		}
		
		case ID_TOOLS_OGREWIN3D:
		{
			App->Open_Tool((LPSTR)"OgreWin3D_Assets.exe");

			return TRUE;
		}
		
		case ID_OPEN_LEVEL:
		{
			/*char Path[MAX_PATH];
			strcpy(Path,App->GD_Directory_FullPath);
			strcat(Path, "\\Levels\\Level_1\\World_1.mesh");
			
			strcpy(App->CL_File_IO->Model_FileName, "World_1.mesh");
			strcpy(App->CL_File_IO->Model_Path_FileName, Path);

			bool test = App->CL_Level->Load_Level(false);*/
			return TRUE;
		}
		
		// File Save ------------------------------------------------
		case ID_FILE_SAVE:
		{
			if (App->CL_Scene->flag_Scene_Loaded == 1)
			{
				App->CL_Project->Save_All();
			}
			return 1;
		}
		
		case ID_FILE_SAVEPROJECTAS:
		{
			if (App->CL_Scene->flag_Scene_Loaded == 1)
			{
				App->CL_Project->Start_Save_Project_Dialog();
			}
			return 1;
		}


		// File Open ------------------------------------------------
		case ID_OPEN_PROJECT:
		{
			App->CL_Importers->Load_Project();
			return 1;
		}

		// File Import ------------------------------------------------
		case ID_OGRE3D_MESH:
		{
			return TRUE;
		}

		case ID_OGRE3D_RELOADMESH:
		{
			return TRUE;
		}
		
		case ID_OGRE3D_RESOURCEFILE:
		{
			return TRUE;
		}
		
		case ID_OGRE3D_RELOADRESOURCEFILE:
		{
			return TRUE;
		}
		
		case ID_IMPORT_WAVEFRONTOBJ:
		{
			return TRUE;
		}

		case ID_IMPORT_AUTODESK3DS:
		{
			return TRUE;
		}

		case ID_IMPORT_MILKSHAPEMS3D:
		{
			return TRUE;
		}

		// File Others ------------------------------------------------
		case ID_FILE_CLEAR:
		{
			App->CL_Dialogs->Show_YesNo_Dlg((LPSTR) "Clear Scene/Model", (LPSTR)"All Data will be lost", (LPSTR) "Are you sure");

			if (App->CL_Dialogs->Canceled == 0)
			{
				App->CL_Scene->Clear_Level();
			}

			return TRUE;
		}

		// Camera -------------------------------------------------------
		case ID_MODE_MODEL:
		{
			App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_Model;
			//App->CL_Ogre->camNode->resetOrientation();
			return TRUE;
		}

		case ID_MODE_FREE:
		{
			App->CL_Ogre->Ogre3D_Listener->CameraMode = Enums::Cam_Mode_Free;
			return TRUE;
		}

		// PREFERENCES --------------------------------------------------
		case ID_OPTIONS_PREFERENCES:
		{
			App->CL_Preferences->Start_Preferences_Dlg();
			return TRUE;
		}

		case ID_OPTIONS_FPSLOCK:
		{
			App->CL_Dialogs->Start_FPSLock_Dlg();
			return TRUE;
		}
		
		case ID_VIEW_RESET:
		{
			App->CL_Camera->Reset_View();
			return TRUE;
		}

		case ID_VIEW_MAX3DVIEW:
		{
			
			return TRUE;
		}

		case ID_MOUSEANDKEYS:
		{
			App->CL_File_IO->Open_HTML((LPSTR)"Help\\MouseAndKeys.html");
			return 1;
		}

		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;

		case IDM_EXIT:

			if (App->CL_Scene->flag_Scene_Modified == 1)
			{
				App->CL_Dialogs->Show_YesNo_Dlg((LPSTR)"Save Scene", (LPSTR)"Scene has been Modified", (LPSTR)"Do you wish to save changes");

				bool Doit = App->CL_Dialogs->Canceled;
				if (Doit == 0)
				{
					App->CL_Project->Start_Save_Project_Dialog();
				}
			}
			else
			{
				App->CL_Dialogs->Show_YesNo_Dlg((LPSTR)"Close OgreWin3D", (LPSTR)"Are you sure", (LPSTR)"");

				if (App->CL_Dialogs->Canceled == 1)
				{
					return 1;
				}
			}

			if (App->CL_Ogre->Ogre3D_Listener->flag_StopOgre == 0)
			{
				App->CL_Ogre->Ogre3D_Listener->flag_StopOgre = 1;
			}

			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}

	case WM_MOVING:
	{
		App->CL_Panels->Resize_Fldg();
		App->CL_Panels->Resize_OgreWin();

		if (App->flag_OgreStarted == 1)
		{
			Root::getSingletonPtr()->renderOneFrame();
		}

		return 0;
	}

	case WM_SIZE:
	{
		App->CL_Panels->Resize_Fldg();
		App->CL_Panels->Resize_OgreWin();
		return 0;
	}

	case WM_WINDOWPOSCHANGED:
	{
		App->CL_Panels->Resize_Fldg();
		App->CL_Panels->Resize_OgreWin();
		return 0;
	}
	//break;
	case WM_CLOSE:
	{
		if (App->CL_Scene->flag_Scene_Modified == 1)
		{
			App->CL_Dialogs->Show_YesNo_Dlg((LPSTR)"Save Scene", (LPSTR)"Scene has been Modified", (LPSTR)"Do you wish to save changes");

			bool Doit = App->CL_Dialogs->Canceled;
			if (Doit == 0)
			{
				App->CL_Project->Start_Save_Project_Dialog();
			}
		}
		else
		{
			App->CL_Dialogs->Show_YesNo_Dlg((LPSTR)"Close OgreWin3D", (LPSTR)"Are you sure", (LPSTR)"");

			if (App->CL_Dialogs->Canceled == 1)
			{
				return 1;
			}
		}

		if (App->CL_Ogre->Ogre3D_Listener->flag_StopOgre == 0)
		{
			App->CL_Ogre->Ogre3D_Listener->flag_StopOgre = 1;
		}

		PostQuitMessage(0);
		break;
	}

	case WM_TIMER:
		if (wParam == 1)
		{
			if (App->flag_OgreStarted == 0)
			{
				App->CL_Dialogs->PleaseWait();

				if (Block_Call == 0)
				{
					Block_Call = 1;
					StartOgre();
				}
			}

			break;
		}

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// *************************************************************************
// *			Ogre3D_Proc:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
LRESULT CALLBACK Ogre3D_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:
	{
		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		if (App->flag_OgreStarted == 0)
		{
			return (LONG)App->BlackBrush;
		}

		return false;
	}
	case WM_MOUSEWHEEL:
	{
		if (App->CL_Ogre->Ogre3D_Listener->flag_LeftMouseDown == 0)
		{
			int zDelta = (short)HIWORD(wParam);    // wheel rotation

			if (zDelta > 0)
			{
				App->CL_Ogre->Ogre3D_Listener->Wheel = -1;
			}
			else if (zDelta < 0)
			{
				App->CL_Ogre->Ogre3D_Listener->Wheel = 1;
			}

			return 1;
		}
	}

	case WM_MOUSEMOVE: // ok up and running and we have a loop for mouse
	{
		POINT pos;
		GetCursorPos(&pos);
		ScreenToClient(App->ViewGLhWnd, &pos);

		if (App->CL_ImGui->flag_Imgui_Initialized == 1)
		{
			ImGuiIO& io = ImGui::GetIO();
			io.MousePos.x = static_cast<float>(pos.x);
			io.MousePos.y = static_cast<float>(pos.y);
		}

		if (GetCursorPos(&pos) && App->flag_OgreStarted == 1)// && App->CL10_Dimensions->Mouse_Move_Mode == Enums::Edit_Mouse_None)
		{
			if (ScreenToClient(App->ViewGLhWnd, &pos))
			{
				RECT rc;
				GetClientRect(App->ViewGLhWnd, &rc);
				int width = rc.right - rc.left;
				int height = rc.bottom - rc.top;

				float tx = ((float)width / 2) - (float)pos.x;
			}
		}

		SetFocus(App->ViewGLhWnd);

		break;
	}

	// Left Mouse Button
	case WM_LBUTTONDOWN: // BERNIE_HEAR_FIRE 
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[0] = true;

		if (!ImGui::GetIO().WantCaptureMouse)
		{

			{
				if (App->flag_OgreStarted == 1)
				{
					if (App->flag_Block_Mouse_Buttons == 0)
					{
						if (!ImGui::GetIO().WantCaptureMouse)
						{
							POINT p;
							GetCursorPos(&p);
							App->CursorPosX = p.x;
							App->CursorPosY = p.y;
							App->CL_Ogre->Ogre3D_Listener->Pl_Cent500X = p.x;
							App->CL_Ogre->Ogre3D_Listener->Pl_Cent500Y = p.y;

							SetCapture(App->ViewGLhWnd);// Bernie
							SetCursorPos(App->CursorPosX, App->CursorPosY);
							App->CL_Ogre->Ogre3D_Listener->flag_LeftMouseDown = 1;
							App->CUR = SetCursor(NULL);
						}
						else
						{
							App->CL_Ogre->Ogre3D_Listener->flag_LeftMouseDown = 1;
						}
					}

					return 1;
				}
			}
		}

		return 1;
	}

	case WM_LBUTTONUP:
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[0] = false;

		if (App->CL_Scene->flag_GameMode_Running_Flag == 0)
		{
			if (App->flag_OgreStarted == 1)
			{
				ReleaseCapture();
				App->CL_Ogre->Ogre3D_Listener->flag_LeftMouseDown = 0;
				SetCursor(App->CUR);
				return 1;
			}
		}

		return 1;
	}

	// Right Mouse Button
	case WM_RBUTTONDOWN: // BERNIE_HEAR_FIRE 
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[0] = true;
		
		//if (ImGui::GetIO().WantCaptureMouse)
		//{
			//App->Cl_FileView_V2->RightMouseDown = 1;
		//}

		if (!ImGui::GetIO().WantCaptureMouse)
		{

			if (App->CL_Scene->flag_GameMode_Running_Flag == 0)
			{
				if (App->flag_OgreStarted == 1)
				{
					POINT p;
					GetCursorPos(&p);
					App->CursorPosX = p.x;
					App->CursorPosY = p.y;
					App->CL_Ogre->Ogre3D_Listener->Pl_Cent500X = p.x;
					App->CL_Ogre->Ogre3D_Listener->Pl_Cent500Y = p.y;
					SetCapture(App->ViewGLhWnd);// Bernie
					SetCursorPos(App->CursorPosX, App->CursorPosY);
					App->CL_Ogre->Ogre3D_Listener->flag_RightMouseDown = 1;
					App->CUR = SetCursor(NULL);
					return 1;
				}
			}
		}
		return 1;
	}

	case WM_RBUTTONUP:
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[0] = false;
		//App->CL_Ogre->m_imgui.mouseReleased();

		if (App->CL_Scene->flag_GameMode_Running_Flag == 0)
		{
			if (App->flag_OgreStarted == 1)
			{
				ReleaseCapture();
				App->CL_Ogre->Ogre3D_Listener->flag_RightMouseDown = 0;
				SetCursor(App->CUR);
				return 1;
			}
		}

		return 1;
	}

	case WM_KEYUP:
	{
		
		ImGuiIO& io = ImGui::GetIO();
		//io.wWantCaptureKeyboard();
		//io.AddKeyEvent(ImGuiKey(ImGuiKey_None), false);
		
		return 1;
	}

	case WM_CHAR:
	{
		if (ImGui::GetIO().WantCaptureKeyboard)
		{
			ImGuiIO& io = ImGui::GetIO();
			io.AddInputCharacter((unsigned short)wParam);
		}
		return 1;
	}

	case WM_KEYDOWN:
	{
		ImGuiIO& io = ImGui::GetIO();
		
		if (wParam == VK_BACK)
		{
			//io.AddKeyEvent(ImGuiKey(ImGuiKey_LeftArrow),true);
			//io.AddKeyEvent(ImGuiKey(ImGuiKey_None), false);
		}

		if (wParam == VK_RIGHT)
		{
			//io.AddKeyEvent(ImGuiKey(ImGuiKey_RightArrow), true);
			//io.KeysDown[VK_RIGHT] = true;
		}

		if (wParam == VK_LEFT)
		{
			//io.AddKeyEvent(ImGuiKey(ImGuiKey_LeftArrow), true);
			//io.KeysDown[VK_LEFT] = true;
		}

		return 1;
	}

	}

	return FALSE;
}

// *************************************************************************
// *			ViewerMain_Proc:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
LRESULT CALLBACK ViewerMain_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{

	case WM_INITDIALOG:
	{
		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}
	case WM_COMMAND:
	{

	}
	break;
	}
	return FALSE;
}

// *************************************************************************
// *				StartOgre:- Terry and Hazel Flanigan 2024		 	   *
// *************************************************************************
void StartOgre()
{
	App->CL_Bullet->Init_Bullet();
    App->CL_Ogre->InitOgre();

	Sleep(500);

    EndDialog(App->ViewPLeaseWait, LOWORD(0));

    App->flag_OgreStarted = 1;

    KillTimer(App->MainHwnd, 1);

	//App->CL_Props_Textures->Get_First_Texture_Ogre();

	App->CL_Project->Load_Last_Project(); // Look At Terry if no level loaded

	App->CL_SoundMgr->Play_StartUp_Sound();

    App->CL_Ogre->Ogre_Render_Loop();

    Close_App();
    SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, TRUE, NULL, TRUE);
    PostQuitMessage(0);
}

// *************************************************************************
// *				Close_App:- Terry and Hazel Flanigan 2024	  		   *
// *************************************************************************
void Close_App()
{
	if (App->CL_Ini_File)
	{
		App->CL_Ini_File->Clean_Up();

		delete App->CL_Ini_File;
		App->CL_Ini_File = nullptr;
	}

	if (App->CL_Preferences)
	{
		App->CL_Preferences->Clean_Up();

		delete App->CL_Preferences;
		App->CL_Preferences = nullptr;
	}

	// And Finaly
    if (App->CL_Ogre->mRoot)
    {
        delete App->CL_Ogre->mRoot;
        App->CL_Ogre->mRoot = NULL;
    }

	App->CL_Bullet->ShutDown_Bullet();

    ImGui::DestroyContext();
}

// *************************************************************************
// *			About:- Terry and Hazel Flanigan 2024			  	 	   *
// *************************************************************************
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_ST_ABOUT_BANNER, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_ABOUT_VERSION, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_LIST_ABOUT_VERSIONS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		char buf[MAX_PATH];

		SetDlgItemText(hDlg, IDC_ST_ABOUT_VERSION, App->App_Version);

		strcpy(buf, "OgreWin3D_Stage Version:- ");
		strcat(buf, App->App_Version);
		strcat(buf, "  (64bit Build)");
		SendDlgItemMessage(hDlg, IDC_LIST_ABOUT_VERSIONS, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

		SendDlgItemMessage(hDlg, IDC_LIST_ABOUT_VERSIONS, LB_ADDSTRING, (WPARAM)0, (LPARAM)" ");

		sprintf(buf, "%s", "Ogre Version:- 14.2.5 (Tsathoggua)");
		SendDlgItemMessage(hDlg, IDC_LIST_ABOUT_VERSIONS, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

		sprintf(buf, "%s", "Imgui Version:- 1.90.4");
		SendDlgItemMessage(hDlg, IDC_LIST_ABOUT_VERSIONS, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

		sprintf(buf, "%s", "Bullet Version:- 2.86.1");
		SendDlgItemMessage(hDlg, IDC_LIST_ABOUT_VERSIONS, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);


		SendDlgItemMessage(hDlg, IDC_LIST_ABOUT_VERSIONS, LB_ADDSTRING, (WPARAM)0, (LPARAM)" ");

		sprintf(buf, "%s", "Instalation Path:- ");
		strcat(buf, App->GD_Directory_FullPath);
		SendDlgItemMessage(hDlg, IDC_LIST_ABOUT_VERSIONS, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);


		SendDlgItemMessage(hDlg, IDC_LIST_ABOUT_VERSIONS, LB_ADDSTRING, (WPARAM)0, (LPARAM)" ");

		sprintf(buf, "%s", "Terry and Hazel Flanigan (Inflanite_HGT)");
		SendDlgItemMessage(hDlg, IDC_LIST_ABOUT_VERSIONS, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

		return (INT_PTR)TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_ABOUT_BANNER) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_ABOUT_VERSION) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}
		
		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDOK)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}
		return CDRF_DODEFAULT;
	}

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }

    return (INT_PTR)FALSE;
}
