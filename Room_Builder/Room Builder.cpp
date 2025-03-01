// Room Builder.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "framework.h"
#include "Room Builder.h"

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

LRESULT CALLBACK ViewerMain_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void StartOgre();
void Close_App();

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

    char* a_cwd = _getcwd(App->RB_Directory_FullPath, MAX_PATH);

    App->InitApp();

    App->CL_Prefs->Read_Preferences();

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_ROOMBUILDER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }


    App->mMenu = GetMenu(App->MainHwnd);
    App->LoadProgramResource();

    App->CL_Doc->Init_Doc();
    App->CL_MapEditor->Init_Map_Views();
    App->CL_MapEditor->Reset_Views();
    App->SetMainWinCentre();

    ShowWindow(App->MainHwnd, SW_SHOWMAXIMIZED);
    UpdateWindow(App->MainHwnd);

    App->CL_Ogre->Init_Ogre();

    App->CL_Top_Tabs->Start_Headers();
    App->CL_Properties_Tabs->Start_Tabs_Control_Dlg();

    App->CL_Resources->Load_Texture_Resources();

    SetTimer(App->MainHwnd, 1, 100, NULL);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ROOMBUILDER));

    MSG msg;

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ROOMBUILDER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(App->AppBackground);
    wcex.lpszMenuName   = MAKEINTRESOURCE(IDC_ROOMBUILDER);
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
      0, 0, 1200, 770, nullptr, nullptr, hInstance, nullptr);

   if (!App->MainHwnd)
   {
      return FALSE;
   }

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
            // Parse the menu selections:
            switch (wmId)
            {

            // ----------------------------- Debug
            case ID_DEBUG_GENERAL:
            {
               /* int Count = 0;

                while (Count < 3)
                {
                    App->CL_MapEditor->VCam[Count]->CamPos.x = 0;
                    App->CL_MapEditor->VCam[Count]->CamPos.y = 0;
                    App->CL_MapEditor->VCam[Count]->CamPos.z = 0;

                    App->CL_MapEditor->VCam[Count]->ZoomFactor = 0.3;

                    Count++;
                }

                App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);*/
               // App->CL_Resources->Load_Texture_Resources();
               /* if (App->CL_Ogre->OGL_Listener->Render_Mode == Enums::Render_Groups)
                {
                    App->CL_Ogre->OGL_Listener->Flag_Render_Brushes = 0;
                    App->CL_Ogre->OGL_Listener->Render_Mode = Enums::Render_Nothing;
                    App->CL_Mesh_Mgr->World_Node->setVisible(true);
                }
                else
                {
                    App->CL_Ogre->OGL_Listener->Flag_Render_Brushes = 1;
                    App->CL_Ogre->OGL_Listener->Render_Mode = Enums::Render_Groups;
                    App->CL_Mesh_Mgr->World_Node->setVisible(false);
                }

                RedrawWindow(App->CL_Properties_Textures->Textures_Dlg_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);*/
                App->CL_Utilities->UnZip_Test_2(NULL);

                return 1;
            }

            case  ID_DEBUG_SELECTIONSTATES:
            {
                if(App->CL_ImGui->flag_Show_Tool_ID_Debug == 1)
                {
                    App->CL_ImGui->flag_Show_Tool_ID_Debug = 0;
                }
                else
                {
                    App->CL_ImGui->flag_Show_Tool_ID_Debug = 1;
                }

                return 1;
            }

            case  ID_DEBUG_SHOWALLFACES3D:
            {
                if (App->CL_Ogre->OGL_Listener->Flag_Show_Selected_Brush == 1)
                {
                    App->CL_Ogre->OGL_Listener->Flag_Show_Selected_Brush = 0;
                }
                else
                {
                    App->CL_Ogre->OGL_Listener->Flag_Show_Selected_Brush = 1;
                }

                return 1;
            }

            case  ID_DEBUG_RENDERSELECTEDFACE:
            {
                if (App->CL_Ogre->OGL_Listener->Flag_Show_Selected_Face == 1)
                {
                    App->CL_Ogre->OGL_Listener->Flag_Show_Selected_Face = 0;
                }
                else
                {
                    App->CL_Ogre->OGL_Listener->Flag_Show_Selected_Face = 1;
                }

                return 1;
            }
            
            // ----------------------------- File
            case ID_FILE_OPEN:
            {
                App->CL_File->Start_Load(true);
                return 1;
            }
          
            case ID_OPENEXAMPLES_TESTSCENE:
            {
                strcpy(App->CL_File->PathFileName_3dt, App->RB_Directory_FullPath);
                strcat(App->CL_File->PathFileName_3dt, "\\Data\\Room_Builder\\Test.mtf");

                strcpy(App->CL_File->FileName_3dt, "Test.mtf");
    
                App->CL_File->Start_Load(false);

                return 1;
            }

            case ID_OPENEXAMPLES_CUTBRUSHES:
            {
                strcpy(App->CL_File->PathFileName_3dt, App->RB_Directory_FullPath);
                strcat(App->CL_File->PathFileName_3dt, "\\Data\\Room_Builder\\CutBrushs.mtf");

                strcpy(App->CL_File->FileName_3dt, "CutBrushs.mtf");

                App->CL_File->Start_Load(false);

                return 1;
            }

            case ID_OPENEXAMPLES_TWOROOMS:
            {
                strcpy(App->CL_File->PathFileName_3dt, App->RB_Directory_FullPath);
                strcat(App->CL_File->PathFileName_3dt, "\\Data\\Room_Builder\\Two_Rooms.mtf");

                strcpy(App->CL_File->FileName_3dt, "Two_Rooms.mtf");

                App->CL_File->Start_Load(false);

                return 1;
            }

            case ID_FILE_SAVE:
            {
                App->CL_File->Start_Save(false);
                return 1;
            }
           
            case ID_FILE_SAVEAS:
            {
                App->CL_File->Start_Save(true);
                return 1;
            }
           
            case ID_EXPORT_OGRE3D:
            {
                int BC = App->CL_Brush->Get_Brush_Count();
                if (BC > 0)
                {
                    App->CL_Export->Ogre_Export_Dlg();
                }
                else
                {
                    App->Say("No Geometry to Export");
                }

                return 1;
            }

            // ----------------------------- Edit
            case ID_EDIT_SELECTALL:
            {
                App->CL_Doc->SelectAll();
                App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);

                /*App->CL_TabsGroups_Dlg->Update_Dlg_SelectedBrushesCount();
                App->CL_TabsGroups_Dlg->Update_Dlg_Controls();
                App->CLSB_TopTabs->Update_Dlg_Controls();*/
                return 1;
            }

            case ID_EDIT_DESELECTALL:
            {
                App->CL_Panels->Deselect_All_Brushes_Update_Dlgs();
                return 1;
            }
            
            // ----------------------------- View
            case ID_VIEW_RESETVIEWS:
            {
                App->CL_MapEditor->Reset_Views();
                return 1;
            }
            
            case ID_VIEW_PATHS:
            {
                if (App->CL_ImGui->flag_Show_Paths == 1)
                {
                    App->CL_ImGui->flag_Show_Paths = 0;
                }
                else
                {
                    App->CL_ImGui->flag_Show_Paths = 1;
                }

                return 1;
            }
            
            // ----------------------------- Grid
            case ID_GRID_GRIDSNAP:
            {
                if (App->CL_Level->flag_UseGrid == 1)
                {
                    App->CL_Level->flag_UseGrid = 0;
                    CheckMenuItem(App->mMenu, ID_GRID_GRIDSNAP, MF_BYCOMMAND | MF_UNCHECKED);
                }
                else
                {
                    App->CL_Level->flag_UseGrid = 1;
                    CheckMenuItem(App->mMenu, ID_GRID_GRIDSNAP, MF_BYCOMMAND | MF_CHECKED);
                }

                return 1;
            }

            case ID_GRID_SNAPOPTOPNS:
            {
                App->CL_Dialogs->Dialog_SnapOptions();
                return 1;
            }
           
            // ----------------------------- Camera
			case ID_CAMERA_TRACKCAMERA:
			{
				if (App->CL_Doc->flag_Track_Camera == 1)
				{
					App->CL_Doc->flag_Track_Camera = 0;
					CheckMenuItem(App->mMenu, ID_CAMERA_TRACKCAMERA, MF_BYCOMMAND | MF_UNCHECKED);
				}
				else
				{
					App->CL_Doc->flag_Track_Camera = 1;
					CheckMenuItem(App->mMenu, ID_CAMERA_TRACKCAMERA, MF_BYCOMMAND | MF_CHECKED);
				}

                App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);

				return 1;
			}

            case ID_CAMERA_RESETCAMERAZERO:
            {
                App->CL_Ogre->Camera_Reset_Zero();
                App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
                return 1;
            }
           
            // ----------------------------- Templates
            case ID_TEMPLATES_INSERT:
            {
                if (App->CL_Doc->mModeTool == ID_TOOLS_TEMPLATE)
                {
                    if (App->CL_Brush->Get_Brush_Count() == 0)
                    {
                       App->CL_Doc->AddBrushToWorld();
                       App->CL_Doc->flag_Is_Modified = 1;
                    }
                    else
                    {
                       App->CL_Doc->AddBrushToWorld();
                       App->CL_Doc->flag_Is_Modified = 1;
                    }
                }

                App->CL_Properties_Brushes->Fill_ListBox();

                App->CL_Doc->DoGeneralSelect(true);
                App->CL_Doc->UpdateAllViews(Enums::UpdateViews_All);
                
                return 1;
            }

            // ----------------------------- Brushes
            case ID_BRUSH_SELECT:
            {
                int Bnum = App->CL_Brush->Get_Brush_Count();
                if (Bnum > 0)
                {
                    App->CL_Doc->DoGeneralSelect(false);
                    App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
                }

                return 1;
            }

            case ID_BRUSH_MOVE:
            {
                App->Say("Move Brush", (LPSTR)"");
                return 1;
            }

            case ID_BRUSH_SCALE:
            {
                App->Say("Scale Brush", (LPSTR)"");
                return 1;
            }

            case ID_BRUSH_SHEAR:
            {
                App->Say("Shear Brush", (LPSTR)"");
                return 1;
            }
            
            // ----------------------------- Faces
            case ID_FACES_SELECTALLFACESINBRUSH:
            {
                App->CL_Doc->SelectAllFacesInBrushes();
                return 1;
            }
            
            // ----------------------------- Tools
           
            case ID_TOOLS_RESOURCEVIEWER:
            {
                App->CL_Resources->Start_Resources();
                return 1;
            }

            case ID_TOOLS_TEXTURESEDITORNEW:
            {
                App->CL_TXL_Editor->Start_Texl_Dialog();
                return 1;
            }
            
           
            // ----------------------------- Windows
            case ID_WINDOW_PROPERTIES:
            {
                if (App->CL_Properties_Tabs->flag_Tabs_Dlg_Active == 1)
                {
                    App->CL_Properties_Tabs->Show_Tabs_Control_Dlg(false);
                    App->CL_Properties_Tabs->flag_Tabs_Dlg_Active = 0;
                    CheckMenuItem(App->mMenu, ID_WINDOW_PROPERTIES, MF_BYCOMMAND | MF_UNCHECKED);
                }
                else
                {
                    App->CL_Properties_Tabs->Show_Tabs_Control_Dlg(true);
                    App->CL_Properties_Tabs->flag_Tabs_Dlg_Active = 1;
                    CheckMenuItem(App->mMenu, ID_WINDOW_PROPERTIES, MF_BYCOMMAND | MF_CHECKED);
                }

                return 1;
            }
           
            // ---------------------------------------------
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;

            case IDM_EXIT:
            {
            
                /*if (App->CL_Scene->flag_Scene_Modified == 1)
                {
                    App->CL_Dialogs->Show_YesNo_Dlg((LPSTR)"Save Scene", (LPSTR)"Scene has been Modified", (LPSTR)"Do you wont to save changes");

                    bool Doit = App->CL_Dialogs->flag_Canceled;
                    if (Doit == 0)
                    {
                        App->CL_Project->Start_Save_Project_Dialog();
                    }
                }
                else*/
                {
                    App->CL_Dialogs->YesNo("Close Room Builder","Are you sure");

                    if (App->CL_Dialogs->flag_Dlg_Canceled == 1)
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
                
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

    case WM_SIZE:
    {
        RECT rcl;
        GetClientRect(App->MainHwnd, &rcl);
        MoveWindow(App->CL_MapEditor->Main_Dlg_Hwnd, 0, 50, rcl.right, rcl.bottom - 50, TRUE);
        App->CL_MapEditor->Init_Views();
       
        return 0;
    }
    
    case WM_CLOSE:
    {
 
        /*if (App->CL_Scene->flag_Scene_Modified == 1)
               {
                   App->CL_Dialogs->Show_YesNo_Dlg((LPSTR)"Save Scene", (LPSTR)"Scene has been Modified", (LPSTR)"Do you wont to save changes");

                   bool Doit = App->CL_Dialogs->flag_Canceled;
                   if (Doit == 0)
                   {
                       App->CL_Project->Start_Save_Project_Dialog();
                   }
               }
               else*/
        {
            App->CL_Dialogs->YesNo("Close Room Builder", "Are you sure");

            if (App->CL_Dialogs->flag_Dlg_Canceled == 1)
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
// *				StartOgre:- Terry and Hazel Flanigan 2025		 	   *
// *************************************************************************
void StartOgre()
{
    KillTimer(App->MainHwnd, 1);

   // App->CL_Ogre->Init_Ogre();

    Sleep(500);

    EndDialog(App->ViewPLeaseWait, LOWORD(0));

    App->flag_OgreStarted = 1;

    App->CL_Ogre->RenderFrame(5);
   
    App->CL_Ogre->Ogre_Render_Loop();

    Close_App();

    SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, TRUE, NULL, TRUE);
    PostQuitMessage(0);
}

// *************************************************************************
// *				Close_App:- Terry and Hazel Flanigan 2025	  		   *
// *************************************************************************
void Close_App()
{
    if (App->CL_Ogre->mRoot)
    {
        delete App->CL_Ogre->mRoot;
        App->CL_Ogre->mRoot = NULL;
    }

    // Delete Contents of working folder
    char mWorld_File_PathAndFile[MAX_PATH];
    strcpy(mWorld_File_PathAndFile, App->RB_Directory_FullPath);
    strcat(mWorld_File_PathAndFile, "\\");
    strcat(mWorld_File_PathAndFile, "Data");
    strcat(mWorld_File_PathAndFile, "\\");
    strcat(mWorld_File_PathAndFile, "World_Test");
    App->CL_Utilities->Delete_Folder_Contents(mWorld_File_PathAndFile);



    PostQuitMessage(0);
}

// *************************************************************************
// *				About:- Terry and Hazel Flanigan 2024	  			   *
// *************************************************************************
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

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
