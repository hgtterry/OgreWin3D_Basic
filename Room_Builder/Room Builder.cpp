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

    App->LoadProgramResource();

    ShowWindow(App->MainHwnd, SW_SHOWMAXIMIZED);
    UpdateWindow(App->MainHwnd);

    App->CL_Doc->Init_Doc();
    App->CL_MapEditor->Init_Map_Views();

    App->SetMainWinCentre();
    App->CL_Top_Tabs->Start_Headers();
    App->CL_Properties_Tabs->Start_Tabs_Control_Dlg();

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

   /*ShowWindow(App->MainHwnd, nCmdShow);
   UpdateWindow(App->MainHwnd);*/

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
                App->CL_Doc->RebuildTrees();        // Build Brushes
                App->CL_Mesh_Mgr->Update_World();   // Build Ogre

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
                App->CL_Doc->ResetAllSelections();
               // App->CL_SelBrushList->SelBrushList_RemoveAll(App->CL_Doc->pSelBrushes);
               // App->CL_SelFaceList->SelFaceList_RemoveAll(App->CL_Doc->pSelFaces);
                App->CL_Doc->UpdateSelected();

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

                App->CL_Doc->DoGeneralSelect();
                App->CL_Doc->UpdateAllViews(Enums::UpdateViews_All);
                
                return 1;
            }

            // ----------------------------- Brushes
            case ID_BRUSH_SELECT:
            {
                int Bnum = App->CL_Brush->Get_Brush_Count();
                if (Bnum > 0)
                {
                    App->CL_Doc->DoGeneralSelect();
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
            
            // ---------------------------------------------
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;

            case IDM_EXIT:
            {
            
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

    App->CL_Ogre->Init_Ogre();

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
    App->Say("Close Room Builder", (LPSTR)"");
    // And Finaly
    if (App->CL_Ogre->mRoot)
    {
        delete App->CL_Ogre->mRoot;
        App->CL_Ogre->mRoot = NULL;
    }

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
