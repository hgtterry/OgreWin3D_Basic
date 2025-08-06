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

    // Create Main App Control
    App = new CL64_App();

    // Get current working directory
    char* a_cwd = _getcwd(App->RB_Directory_FullPath, MAX_PATH);

    // Initialize application
    App->InitApp();
    App->CL_X_Preference->Read_Preferences();
    App->CL_X_Preference->Init_Configuration();

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize global strings
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_ROOMBUILDER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    // Load menus
    App->Menu_Map = GetMenu(App->MainHwnd);
    App->Menu_Scene = LoadMenuA(App->hInst, (LPCSTR)IDR_SCENE_MENU);

    // Load program resources
    App->LoadProgramResource();

    // Initialize document and views
    App->CL_Doc->Init_Doc();
    App->CL_Editor_Map->Init_Map_Views();
    App->SetMainWinCentre();

    // Show and update main window
    ShowWindow(App->MainHwnd, SW_SHOWMAXIMIZED);
    UpdateWindow(App->MainHwnd);

    // Reset views and initialize components
    App->CL_Editor_Map->Reset_Views_All();
    App->CL_Physics->Init_Bullet();
    App->CL_Ogre->Init_Ogre();
    App->CL_Picking->Init_Picking();

    // Load default WAD  --- file Zipped Texture file
    char DefaultWad[MAX_PATH];
    strcpy(DefaultWad, App->RB_Directory_FullPath);
    strcat(DefaultWad, "\\Data\\Room_Builder\\Default.zip");
    App->CL_Doc->Load_Wad_File(DefaultWad); // Needs Ogre at the Moment
   
    // Start headers and tabs
    App->CL_Top_Tabs->Start_Headers();
    App->CL_Editor_Scene->Start_Headers_Scene();
    App->CL_Properties_Tabs->Start_Tabs_Control_Dlg();
    App->CL_FileView->Start_FileView();
    App->CL_FileView->Init_FileView();

    // Initialize dialogs
    App->Init_Dialogs();


  //  App->CL_Com_Player->Create_Player_Object();

    // Set timer for the main window
    SetTimer(App->MainHwnd, 1, 100, NULL);

    // Start main window/proc loop
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
               // App->CL_Editor_Control->Set_Map_Editor_Select_Dlg();

                App->Say_Win(App->CL_X_Preference->GetVersion());

                /*if (App->CL_ImGui_Editor->flag_Show_Visuals == true)
                {
                    App->CL_ImGui_Editor->flag_Show_Visuals = false;
                }
                else
                {
                    App->CL_ImGui_Editor->flag_Show_Visuals = true;
                }*/

				return 1;
			}

            case ID_DEBUG_LIBRARIES:
            {
                App->CL_Dialogs->Start_General_ListBox(Enums::ListBox_Libraries);

                return 1;
            }
            
            case ID_DEBUG_DEBUGIMGUI:
            {
                if (App->CL_ImGui->flag_Show_Listbox == true)
                {
                    App->CL_ImGui->flag_Show_Listbox = false;
                }
                else
                {
                    App->CL_Editor_Map->Init_Views(Enums::Selected_Map_View_3D);
                    App->CL_Editor_Map->Resize_Windows(App->CL_Editor_Map->Main_View_Dlg_Hwnd, App->CL_Editor_Map->nleftWnd_width, App->CL_Editor_Map->nleftWnd_Depth);

                    App->CL_ImGui->flag_Show_Listbox = true;
                }

                return 1;
            }
           
            case ID_DEBUG_QUICKLOADSCENEEDITOR:
            {
                strcpy(App->CL_File->PathFileName_3dt, App->RB_Directory_FullPath);
                strcat(App->CL_File->PathFileName_3dt, "\\Data\\Room_Builder\\Test_owp\\Test.mtf");

                strcpy(App->CL_File->FileName_3dt, "Test.mtf");

                App->CL_File->Start_Load(false);

                App->CL_Editor_Control->Start_Editor_Scene();

                return 1;
            }
           
            case  ID_DEBUG_SHOWALLFACES3D:
            {
                if (App->CL_Ogre->OGL_Listener->flag_Show_Selected_Brush == 1)
                {
                    App->CL_Ogre->OGL_Listener->flag_Show_Selected_Brush = 0;
                }
                else
                {
                    App->CL_Ogre->OGL_Listener->flag_Show_Selected_Brush = 1;
                }

                return 1;
            }

            case  ID_DEBUG_RENDERSELECTEDFACE:
            {
                if (App->CL_Ogre->OGL_Listener->flag_Show_Selected_Face == 1)
                {
                    App->CL_Ogre->OGL_Listener->flag_Show_Selected_Face = 0;
                }
                else
                {
                    App->CL_Ogre->OGL_Listener->flag_Show_Selected_Face = 1;
                }

                return 1;
            }
            
            case  ID_DEBUG_APPSTATS:
            {
                if (App->CL_ImGui->flag_Show_App_Stats == 1)
                {
                    App->CL_ImGui->flag_Show_App_Stats = 0;

                    App->CL_Top_Tabs->flag_Full_View_3D = 0;

                    App->CL_Editor_Map->Set_Splitter_WidthDepth(App->CL_Editor_Map->Copy_Spliter_Width, App->CL_Editor_Map->Copy_Spliter_Depth);

                    App->CL_Editor_Map->Resize_Windows(App->CL_Editor_Map->Main_View_Dlg_Hwnd, App->CL_Editor_Map->nleftWnd_width, App->CL_Editor_Map->nleftWnd_Depth);

                }
                else
                {
                    App->CL_Top_Tabs->flag_Full_View_3D = 1;
                    App->CL_Top_Tabs->flag_View_Top_Left = 0;
                    App->CL_Top_Tabs->flag_View_Top_Right = 0;
                    App->CL_Top_Tabs->flag_View_Bottom_Left = 0;

                    App->CL_Editor_Map->Init_Views(Enums::Selected_Map_View_3D);
                    App->CL_Editor_Map->Resize_Windows(App->CL_Editor_Map->Main_View_Dlg_Hwnd, App->CL_Editor_Map->nleftWnd_width, App->CL_Editor_Map->nleftWnd_Depth);

                    App->CL_ImGui->flag_Show_App_Stats = 1;
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
                strcat(App->CL_File->PathFileName_3dt, "\\Data\\Room_Builder\\Test_owp\\Test.mtf");

                strcpy(App->CL_File->FileName_3dt, "Test.mtf");
    
                App->CL_File->Start_Load(false);

                return 1;
            }

            case ID_OPENEXAMPLES_CUTBRUSHES:
            {
                strcpy(App->CL_File->PathFileName_3dt, App->RB_Directory_FullPath);
                strcat(App->CL_File->PathFileName_3dt, "\\Data\\Room_Builder\\CutBrushes.owp\\CutBrushs.mtf");

                strcpy(App->CL_File->FileName_3dt, "CutBrushs.mtf");

                App->CL_File->Start_Load(false);

                return 1;
            }

            case ID_OPENEXAMPLES_TWOROOMS:
            {
                strcpy(App->CL_File->PathFileName_3dt, App->RB_Directory_FullPath);
                strcat(App->CL_File->PathFileName_3dt, "\\Data\\Room_Builder\\Two_Rooms_owp\\Two_Rooms.mtf");

                strcpy(App->CL_File->FileName_3dt, "Two_Rooms.mtf");

                App->CL_File->Start_Load(false);

                return 1;
            }

            case ID_FILE_SAVE:
            {
                if (App->CL_Level->flag_File_Been_Saved == 0)
                {
                    App->Say("This Project is new", "Will use Save As for this first time save");
                    App->CL_File->Start_Save(true);
                    return 1;
                }

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

            case ID_EXPORT_OBJECTWAVEFRONT:
            {
                int BC = App->CL_Brush->Get_Brush_Count();
                if (BC > 0)
                {
                    App->CL_Export->Object_Export_Dlg();
                }
                else
                {
                    App->Say("No Geometry to Export");
                }

                return 1;
            }

            case ID_FILE_CLEAR:
            {
                /*App->CL_Dialogs->YesNo("Clear Editor", "Are you sure");

                if (App->CL_Dialogs->flag_Dlg_Canceled == true)
                {
                    return 1;
                }

                App->CL_Scene->Clear_Level(false);*/
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
                App->CL_Dialogs->YesNo("Are you Sure", "All Views will be Reset to Default");

                if (App->CL_Dialogs->flag_Dlg_Canceled == false)
                {
                    App->CL_Editor_Map->Reset_Views_All();
                }

                return 1;
            }
            
            case ID_VIEW_TOPLEFT:
            {
                App->CL_Editor_Map->Init_Views(Enums::Selected_Map_View_TL);
                App->CL_Editor_Map->Resize_Windows(App->CL_Editor_Map->Main_View_Dlg_Hwnd, App->CL_Editor_Map->nleftWnd_width, App->CL_Editor_Map->nleftWnd_Depth);
                App->CL_Top_Tabs->Set_View_Buttons(Enums::Selected_Map_View_TL);
                return true;
            }

            case ID_VIEW_TOPRIGHT:
            {
                App->CL_Editor_Map->Init_Views(Enums::Selected_Map_View_TR);
                App->CL_Editor_Map->Resize_Windows(App->CL_Editor_Map->Main_View_Dlg_Hwnd, App->CL_Editor_Map->nleftWnd_width, App->CL_Editor_Map->nleftWnd_Depth);
                App->CL_Top_Tabs->Set_View_Buttons(Enums::Selected_Map_View_TR);
                return true;
            }

            case ID_VIEW_BOTTOMLEFT:
            {
                App->CL_Editor_Map->Init_Views(Enums::Selected_Map_View_BL);
                App->CL_Editor_Map->Resize_Windows(App->CL_Editor_Map->Main_View_Dlg_Hwnd, App->CL_Editor_Map->nleftWnd_width, App->CL_Editor_Map->nleftWnd_Depth);
                App->CL_Top_Tabs->Set_View_Buttons(Enums::Selected_Map_View_BL);
                return true;
            }

            case ID_VIEW_3DVIEW:
            {
                App->CL_Editor_Map->Set_3D_FullView();
                App->CL_Top_Tabs->Set_View_Buttons(Enums::Selected_Map_View_3D);
                return true;
            }

            case ID_VIEW_RESTOREVIEWS:
            {
                App->CL_Editor_Map->Set_Splitter_WidthDepth(App->CL_Editor_Map->Copy_Spliter_Width, App->CL_Editor_Map->Copy_Spliter_Depth);
                App->CL_Editor_Map->Resize_Windows(App->CL_Editor_Map->Main_View_Dlg_Hwnd, App->CL_Editor_Map->nleftWnd_width, App->CL_Editor_Map->nleftWnd_Depth);
                App->CL_Top_Tabs->Set_View_Buttons(Enums::Selected_Map_View_None);
                return true;
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
                    CheckMenuItem(App->Menu_Map, ID_GRID_GRIDSNAP, MF_BYCOMMAND | MF_UNCHECKED);
                }
                else
                {
                    App->CL_Level->flag_UseGrid = 1;
                    CheckMenuItem(App->Menu_Map, ID_GRID_GRIDSNAP, MF_BYCOMMAND | MF_CHECKED);
                }

                return 1;
            }

            case ID_GRID_SNAPOPTOPNS:
            {
                App->CL_Dialogs->Dialog_SnapOptions();
                return 1;
            }
           
            // ----------------------------- Camera
            case ID_CAMERA_TEXTURED:
            {
                App->CL_Camera->Camera_Textured();
                return 1;
            }

            case ID_CAMERA_WIREFRAMED:
            {
                App->CL_Camera->Camera_Wired();
                return 1;
            }
            
			case ID_CAMERA_TRACKCAMERA:
			{
                App->CL_Camera->Track_Camera();
				return 1;
			}

            case ID_CAMERA_RESETCAMERAZERO:
            {
                App->CL_Camera->Camera_Reset_Zero();
                return 1;
            }

            case ID_SPEED_NORMAL:
            {
                App->CL_Camera->Camera_Speed_Normal();
                return 1;
            }

            case ID_SPEED_SLOW:
            {
                App->CL_Camera->Camera_Speed_Slow();
                return 1;
            }
            
            case ID_SPEED_VERYSLOW:
            {
                App->CL_Camera->Camera_Speed_Very_Slow();
                return 1;
            }

            case ID_SPEED_FAST:
            {
                App->CL_Camera->Camera_Speed_Fast();
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
                       App->CL_Level->flag_Level_is_Modified = true;
                    }
                    else
                    {
                       App->CL_Doc->AddBrushToWorld();
                       App->CL_Level->flag_Level_is_Modified = true;
                    }
                }

                App->CL_Properties_Brushes->Fill_ListBox();

                App->CL_Doc->Do_General_Select_Dlg(true);
                App->CL_Doc->UpdateAllViews(Enums::UpdateViews_All);
                
                return 1;
            }

            // ----------------------------- Brushes
            case ID_BRUSH_SELECT:
            {
                int Bnum = App->CL_Brush->Get_Brush_Count();
                if (Bnum > 0)
                {
                    App->CL_Doc->Do_General_Select_Dlg(false);
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
            
            // ----------------------------- Textures
            case ID_TEXTURES_TEXTURESFILEEDITOR:
            {
                App->CL_TXL_Editor->Start_Texl_Dialog();
                return 1;
            }
         
            case ID_TEXTURES_LOCKALLTEXTURES:
            {
                App->CL_Textures->Lock_All_Textures();
                return 1;
            }

            case ID_TEXTURES_USEDTEXTURES:
            {
                App->CL_Dialogs->Start_General_ListBox(Enums::ListBox_Used_Textures);
                return 1;
            }

            // ----------------------------- Options
            case ID_OPTIONS_SETTINGS:
            {
                App->CL_X_Preference->Start_Options_Dlg();
                return 1;
            }
            
            // ----------------------------- Tools
            case ID_TOOLS_RESOURCEVIEWER:
            {
                App->CL_Resources->Start_Resources();
                return 1;
            }

            case  ID_TOOLS_MESHDATA:
            {
                App->CL_Mesh_Mgr->Start_Mesh_Viewer();
                return 1;
            }
 
            // ----------------------------- Windows
            case ID_WINDOW_PROPERTIES:
            {
                if (App->CL_Properties_Tabs->flag_Tabs_Dlg_Active == 1)
                {
                    App->CL_Properties_Tabs->Show_Tabs_Control_Dlg(false);
                    App->CL_Properties_Tabs->flag_Tabs_Dlg_Active = 0;
                    CheckMenuItem(App->Menu_Map, ID_WINDOW_PROPERTIES, MF_BYCOMMAND | MF_UNCHECKED);
                }
                else
                {
                    App->CL_Properties_Tabs->Show_Tabs_Control_Dlg(true);
                    App->CL_Properties_Tabs->flag_Tabs_Dlg_Active = 1;
                    CheckMenuItem(App->Menu_Map, ID_WINDOW_PROPERTIES, MF_BYCOMMAND | MF_CHECKED);
                }

                return 1;
            }
           
            // ----------------------------- Scene Editor Tools
            case ID_TOOLS_SOUNDPLAYER:
            {
                App->CL_SoundMgr->Show_Sound_Player();
                return 1;
            }

            case ID_INFO_PLAYERDATA:
            {
                if (App->CL_ImGui->flag_Show_Player_Data == 1)
                {
                    App->CL_ImGui->flag_Show_Player_Data = 0;  
                }
                else
                {
                    App->CL_ImGui->flag_Show_Player_Data = 1;
                }
                return 1;
            }

            case ID_INFO_PHYSICSOBJECTSDEBUG:
            {
                if (App->CL_Editor_Scene->flag_Show_Physics_Objects == 1)
                {
                    App->CL_Physics->Show_Debug_Objects(false);
                    App->CL_Editor_Scene->flag_Show_Physics_Objects = 0;
                }
                else
                {
                    App->CL_Physics->Show_Debug_Objects(true);
                    App->CL_Editor_Scene->flag_Show_Physics_Objects = 1;
                }

                return 1;
            }

            case ID_ADDITEM_OBJECT:
            {
               
                if (App->CL_Scene->flag_Project_Resources_Created == true)
                {
                    App->CL_Dialogs->YesNo((LPSTR)"Add Object", (LPSTR)"Do you want to add a new Object Entity");

                    if (App->CL_Dialogs->flag_Dlg_Canceled == false)
                    {
                        App->CL_MeshViewer->Mesh_Viewer_Mode = Enums::Mesh_Viewer_Objects;
                        App->CL_MeshViewer->Start_MeshViewer_Dlg();
                    }
                }
                else
                {
                    if (App->CL_Level->flag_File_Been_Saved == 0)
                    {
                        App->Say("This Project is new", "Will use Save As for this first time save");
                        App->CL_File->Start_Save(true);

                        if (App->CL_Scene->flag_Project_Resources_Created == true)
                        {
                            App->CL_Dialogs->YesNo((LPSTR)"Add Object", (LPSTR)"Do you want to add a new Object Entity");

                            if (App->CL_Dialogs->flag_Dlg_Canceled == false)
                            {
                                App->CL_MeshViewer->Mesh_Viewer_Mode = Enums::Mesh_Viewer_Objects;
                                App->CL_MeshViewer->Start_MeshViewer_Dlg();
                            }
                        }

                        return 1;
                    }

                }

                return 1;
            }

            case ID_ADDITEM_SOUNDENTITY:
            {

                App->CL_Dialogs->YesNo((LPSTR)"Add Sound Entity", (LPSTR)"Do you want to add a new Sound Entity");

                if (App->CL_Dialogs->flag_Dlg_Canceled == false)
                {
                    App->CL_Com_Sounds->Add_New_Sound();
                }

                return 1;
            }

            case ID_ADDITEM_ENVIRONMENT:
            {

                App->CL_Dialogs->YesNo((LPSTR)"Add Environment Entity", (LPSTR)"Do you want to add a new Environ Entity");

                if (App->CL_Dialogs->flag_Dlg_Canceled == false)
                {
                    App->CL_Com_Environments->Add_New_Environ_Entity(false);
                }

                return 1;
            }
            
           
            // ---------------------------------------------
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;

            case IDM_EXIT:
            {
            
                if (App->CL_Level->flag_Level_is_Modified == true)
                {
                    char Text[200];
                    strcpy(Text, "Save Changes To ");
                    strcat(Text, App->CL_Level->MTF_Just_FileName);

                    App->CL_Dialogs->YesNoCancel((LPSTR)"File has been Modified", Text);

                    if (App->CL_Dialogs->YesNoCancel_Result == 1)
                    {
                        App->CL_File->Start_Save(true);
                    }

                    if (App->CL_Dialogs->YesNoCancel_Result == 2)
                    {

                    }

                    if (App->CL_Dialogs->YesNoCancel_Result == 3)
                    {
                        return 1;
                    }

                }
                else
                {
                    App->CL_Dialogs->YesNo("Close OgreWin3D","Are you sure");

                    if (App->CL_Dialogs->flag_Dlg_Canceled == 1)
                    {
                        return 1;
                    }
                }

                App->CL_ImGui_Dialogs->Close_All_Dialogs();

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

        if (App->CL_Editor_Control->flag_Scene_Editor_Active == 0)
        {
            MoveWindow(App->CL_Editor_Map->Main_View_Dlg_Hwnd, 0, 50, rcl.right, rcl.bottom - 50, TRUE);
            App->CL_Editor_Map->Init_Views(Enums::Selected_Map_View_None);
            App->CL_Editor_Map->Save_Splitter_Width_Depth();

            if (App->flag_OgreStarted == 1)
            {
                App->CL_Ogre->RenderFrame(2);
            }
        }
        else
        {
            GetClientRect(App->CL_Editor_Map->Bottom_Ogre_Right_Hwnd, &rcl);

            SetWindowPos(App->ViewGLhWnd, NULL, 0, 0, rcl.right, rcl.bottom, SWP_NOZORDER);
            App->CL_Ogre->mWindow->windowMovedOrResized();
            App->CL_Ogre->mCamera->setAspectRatio((Ogre::Real)App->CL_Ogre->mWindow->getWidth() / (Ogre::Real)App->CL_Ogre->mWindow->getHeight());
           
            if (App->flag_OgreStarted == 1)
            {
                App->CL_Ogre->RenderFrame(2);
            }

        }
       
        return 0;
    }
    
    case WM_CLOSE:
    {
        if (App->CL_Level->flag_Level_is_Modified == true)
        {
            char Text[200];
            strcpy(Text, "Save Changes To ");
            strcat(Text, App->CL_Level->MTF_Just_FileName);

            App->CL_Dialogs->YesNoCancel((LPSTR)"File has been Modified", Text);

            if (App->CL_Dialogs->YesNoCancel_Result == 1)
            {
                App->CL_File->Start_Save(true);
            }

            if (App->CL_Dialogs->YesNoCancel_Result == 2)
            {
                
            }

            if (App->CL_Dialogs->YesNoCancel_Result == 3)
            {
                return 1;
            }

        }
        else
        {
            App->CL_Dialogs->YesNo("Close OgreWin3D", "Are you sure");

            if (App->CL_Dialogs->flag_Dlg_Canceled == 1)
            {
                return 1;
            }
        }

        App->CL_ImGui_Dialogs->Close_All_Dialogs();

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
   
    if (App->CL_X_Preference->flag_OpenLastFile)
    {
        // Compare the last opened file with "New_Room.mtf"
        if (strcmp(App->CL_X_Preference->Prefs_PathAndFile, "New_Room.mtf") != 0)
        {
            // Copy the path and filename from preferences to the file structure
            strcpy(App->CL_File->PathFileName_3dt, App->CL_X_Preference->Prefs_PathAndFile);
            strcpy(App->CL_File->FileName_3dt, App->CL_X_Preference->Prefs_JustFileName);

            // Initiate the loading process
            App->CL_File->Start_Load(false);
        }
    }

   if (App->CL_X_Preference->flag_SceneEditor == true)
   {
       App->CL_Editor_Control->Start_Editor_Scene();
   }

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

    App->CL_Physics->ShutDown_Bullet();

    ImGui::DestroyContext();

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
    {
        SendDlgItemMessage(hDlg, IDC_ST_ABOUT_BANNER, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
        SendDlgItemMessage(hDlg, IDC_ST_ABOUT_VERSION, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
        SendDlgItemMessage(hDlg, IDC_LIST_ABOUT_VERSIONS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
        SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
       
        char buf[MAX_PATH];

        SetDlgItemText(hDlg, IDC_ST_ABOUT_VERSION, App->App_Title);

        strcpy(buf, "OgreWin3D Mesh Builder:- ");
        strcat(buf, App->App_Title);
        strcat(buf, "  (64bit Build)");
        SendDlgItemMessage(hDlg, IDC_LIST_ABOUT_VERSIONS, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

        SendDlgItemMessage(hDlg, IDC_LIST_ABOUT_VERSIONS, LB_ADDSTRING, (WPARAM)0, (LPARAM)" ");

        sprintf(buf, "%s", "Ogre Version:- Version 14.4.0 (Tsathoggua)");
        SendDlgItemMessage(hDlg, IDC_LIST_ABOUT_VERSIONS, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

        ImGui::GetVersion();
        sprintf(buf, "Imgui Version:- " "%s", ImGui::GetVersion()); //"Imgui Version:- 1.91.2");
        SendDlgItemMessage(hDlg, IDC_LIST_ABOUT_VERSIONS, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

        sprintf(buf, "%s", "Bullet Version:- 2.86.1");
        SendDlgItemMessage(hDlg, IDC_LIST_ABOUT_VERSIONS, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);


        SendDlgItemMessage(hDlg, IDC_LIST_ABOUT_VERSIONS, LB_ADDSTRING, (WPARAM)0, (LPARAM)" ");

        sprintf(buf, "%s", "Instalation Path:- ");
        strcat(buf, App->RB_Directory_FullPath);
        SendDlgItemMessage(hDlg, IDC_LIST_ABOUT_VERSIONS, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

        char buff[MAX_PATH];
        App->CL_Ogre->Get_OpenGL_Version(buff);
        SendDlgItemMessage(hDlg, IDC_LIST_ABOUT_VERSIONS, LB_ADDSTRING, (WPARAM)0, (LPARAM)buff);

        SendDlgItemMessage(hDlg, IDC_LIST_ABOUT_VERSIONS, LB_ADDSTRING, (WPARAM)0, (LPARAM)" ");

        sprintf(buf, "%s", "Terry and Hazel Flanigan (Inflanite_HGT)");
        SendDlgItemMessage(hDlg, IDC_LIST_ABOUT_VERSIONS, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

        App->CL_Properties_Tabs->Enable_Tabs_Dlg(false);

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
            App->CL_Properties_Tabs->Enable_Tabs_Dlg(true);
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
