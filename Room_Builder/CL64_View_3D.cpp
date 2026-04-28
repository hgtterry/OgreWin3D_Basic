/*
Copyright (c) 2024 - 2026 HGT_Software W.T.Flanigan H.C.Flanigan

Cheryl 3D Scene Builder

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
#include "resource.h"
#include "CL64_App.h"
#include "CL64_View_3D.h"

CL64_View_3D::CL64_View_3D()
{
	Bottom_Right_Window_Hwnd = nullptr;
	RenderWin3D_hWnd = nullptr;
	Bottom_3D_Banner = nullptr;
}

CL64_View_3D::~CL64_View_3D()
{
}

// *************************************************************************
// *		 Create_Ogre_Bottom_Right:- Terry and Hazel Flanigan 2024	   *
// *************************************************************************
void CL64_View_3D::Create_Ogre_Bottom_Right()
{
	App->CL_Views_Com->VCam[V_Ogre] = new ViewVars;
	App->CL_Views_Com->Set_Views_Defaults(V_Ogre, VIEWOGRE, "Ogre_Window");

	Bottom_Right_Window_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_MAP_BOTTOM_RIGHT, App->CL_Views_Com->Main_View_Dlg_Hwnd, (DLGPROC)Proc_ViewerMain);

	App->CL_Views_Com->VCam[V_Ogre]->hDlg = Bottom_Right_Window_Hwnd;

	App->CL_Ogre->RenderHwnd = App->ViewGLhWnd;
}

// *************************************************************************
// *			ViewerMain_Proc:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
LRESULT CALLBACK CL64_View_3D::Proc_ViewerMain(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_ST_3D_TITLE, WM_SETFONT, (WPARAM)App->Font_CB10, MAKELPARAM(TRUE, 0));
		App->CL_View_3D->Bottom_3D_Banner = GetDlgItem(hDlg, IDC_ST_3D_TITLE);

		App->ViewGLhWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_MAP_RENDER_WINDOW, hDlg, (DLGPROC)Proc_Ogre_BR);
		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_3D_TITLE) == (HWND)lParam)
		{
			if (App->CL_Views_Com->Selected_Window == Enums::Selected_Map_View_3D)
			{
				SetBkColor((HDC)wParam, RGB(0, 255, 0));
				SetTextColor((HDC)wParam, RGB(0, 0, 0));
				SetBkMode((HDC)wParam, TRANSPARENT);
				return (UINT)App->Brush_Green;
			}
			else
			{
				SetBkColor((HDC)wParam, RGB(0, 255, 0));
				SetTextColor((HDC)wParam, RGB(0, 0, 0));
				SetBkMode((HDC)wParam, TRANSPARENT);
				return (UINT)App->AppBackground;
			}
		}

		return FALSE;
	}

	// Left Mouse Down
	case WM_LBUTTONDOWN:
	{
		App->CL_Views_Com->Current_View = App->CL_Views_Com->VCam[V_Ogre];

		if (App->CL_Views_Com->Selected_Window != Enums::Selected_Map_View_3D)
		{
			App->CL_Views_Com->Set_Selected_View(Enums::Selected_Map_View_3D);
		}
	}

	// Right Mouse Down
	case WM_RBUTTONDOWN:
	{
		App->CL_Views_Com->Current_View = App->CL_Views_Com->VCam[V_Ogre];

		if (App->CL_Views_Com->Selected_Window != Enums::Selected_Map_View_3D)
		{
			App->CL_Views_Com->Set_Selected_View(Enums::Selected_Map_View_3D);
		}
		return 1;
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
// *		Proc_Ogre_BR:- Terry and Hazel Flanigan 2024 				   *
// *************************************************************************
LRESULT CALLBACK CL64_View_3D::Proc_Ogre_BR(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		if (App->flag_3D_Started == 0)
		{
			return (LONG)App->BlackBrush;
		}
		break;
	}

	case WM_SETCURSOR:
	{
		if (App->CL_Editor_Control->flag_Scene_Game_Running == true)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	case WM_COMMAND:
	{
		bool isSceneEditorActive = (App->CL_Editor_Control->flag_Scene_Editor_Active == 1);
		bool commandHandled = false;

		if (isSceneEditorActive)
		{
			commandHandled = App->CL_Editor_Scene->Context_Command_Ogre(LOWORD(wParam));
		}
		else
		{
			commandHandled = App->CL_Views_Com->Context_Command_Ogre(LOWORD(wParam));
		}

		return commandHandled ? TRUE : FALSE;
	}

	case WM_MOUSEWHEEL:
	{
		if (App->CL_Editor_Control->flag_PreviewMode_Active == 1 &&
			App->CL_Ogre->Ogre3D_Listener->flag_LeftMouseDown == 0)
		{
			int zDelta = static_cast<short>(HIWORD(wParam)); // wheel rotation

			App->CL_Ogre->Ogre3D_Listener->Wheel = (zDelta > 0) ? -1 : (zDelta < 0) ? 1 : App->CL_Ogre->Ogre3D_Listener->Wheel;

			return 1;
		}
	}

	case WM_MOUSEMOVE:
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

		SetFocus(App->ViewGLhWnd);

		break;
	}

	// Left Mouse Down
	case WM_LBUTTONDOWN:
	{
		App->CL_Views_Com->Current_View = App->CL_Views_Com->VCam[V_Ogre];

		if (App->CL_Views_Com->Selected_Window != Enums::Selected_Map_View_3D)
		{
			App->CL_Views_Com->Set_Selected_View(Enums::Selected_Map_View_3D);
		}

		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[0] = true;

		if (App->flag_3D_Started == 1)
		{
			if (App->flag_Block_Mouse_Buttons == false)
			{
				if (!ImGui::GetIO().WantCaptureMouse)
				{
					POINT p;
					GetCursorPos(&p);
					App->CL_Views_Com->mStartPoint = p;

					GetCursorPos(&p);
					App->CursorPosX = p.x;
					App->CursorPosY = p.y;
					App->CL_Ogre->Ogre3D_Listener->Pl_Cent500X = p.x;
					App->CL_Ogre->Ogre3D_Listener->Pl_Cent500Y = p.y;

					SetCapture(App->ViewGLhWnd);
					SetCursorPos(App->CursorPosX, App->CursorPosY);
					App->CL_Ogre->Ogre3D_Listener->flag_LeftMouseDown = 1;
					App->CUR = SetCursor(NULL);

					App->CL_Camera->Camera_Save_Location();

				}
				else
				{
					//App->CL_Ogre->Ogre3D_Listener->flag_LeftMouseDown = 1;
				}
			}
		}

		return 1;
	}

	// Left Mouse Up
	case WM_LBUTTONUP:
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[0] = false;

		if (App->flag_3D_Started == 1)
		{
			if (App->flag_Block_Mouse_Buttons == 0)
			{
				ReleaseCapture();
				App->CL_Ogre->Ogre3D_Listener->flag_LeftMouseDown = 0;
				SetCursor(App->CUR);
				App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);

				Ogre::Quaternion cameraRotation = App->CL_Ogre->camNode->getOrientation();

				int cameraComparison = App->CL_X_Maths->Ogre_Quaternion_Compare(&cameraRotation, &App->CL_Camera->Saved_Rotation, 0);

				// If Mouse has not moved select Brush and Face
				if (cameraComparison == 1)
				{
					if (App->CL_Editor_Control->flag_PreviewMode_Active == 0 && App->CL_Editor_Control->flag_Scene_Editor_Active == 0)
					{
						App->CL_Picking->Mouse_Pick_Entity(false);
					}
				}
				//App->BeepBeep();
				return 1;
			}
		}

		return 1;
	}

	// Right Mouse Down
	case WM_RBUTTONDOWN:
	{
		App->CL_Views_Com->Current_View = App->CL_Views_Com->VCam[V_Ogre];

		if (App->CL_Views_Com->Selected_Window != Enums::Selected_Map_View_3D)
		{
			App->CL_Views_Com->Set_Selected_View(Enums::Selected_Map_View_3D);
		}

		if (App->flag_3D_Started == 1)
		{
			if (App->flag_Block_Mouse_Buttons == 0)
			{
				POINT cursorPosition;
				GetCursorPos(&cursorPosition);

				App->CursorPosX = cursorPosition.x;
				App->CursorPosY = cursorPosition.y;

				auto& listener = App->CL_Ogre->Ogre3D_Listener;
				listener->Pl_Cent500X = cursorPosition.x;
				listener->Pl_Cent500Y = cursorPosition.y;

				SetCapture(App->ViewGLhWnd);
				SetCursorPos(App->CursorPosX, App->CursorPosY);

				listener->flag_RightMouseDown = 1;
				App->CUR = SetCursor(NULL);

				App->CL_Camera->Camera_Save_Location();

				return 1;
			}
		}

		return 1;
	}

	// Right Mouse Up
	case WM_RBUTTONUP:
	{
		if (App->flag_3D_Started == 1)
		{
			if (App->flag_Block_Mouse_Buttons == 0)
			{
				ReleaseCapture();
				App->CL_Ogre->Ogre3D_Listener->flag_RightMouseDown = 0;
				SetCursor(App->CUR);

				if (GetAsyncKeyState(VK_CONTROL) < 0 && App->CL_Editor_Control->flag_PreviewMode_Active == 0)
				{
					App->CL_Picking->Mouse_Pick_Entity(true);
					int index = App->CL_TXL_Editor->GetIndex_From_FileName(App->CL_Picking->m_Texture_FileName);

					if (index > -1)
					{
						App->CL_Properties_Textures->Select_With_TextureName(App->CL_TXL_Editor->Texture_List[index]->Name);
						if (App->CL_Properties_Textures->Dialog_Textures_Visible == 0)
						{
							App->CL_Properties_Tabs->Select_Textures_Tab();
						}
					}
				}
				else
				{
					bool isSceneEditorActive = App->CL_Editor_Control->flag_Scene_Editor_Active == 1;
					bool isPreviewModeRunning = App->CL_Editor_Control->flag_PreviewMode_Active == 0;

					Ogre::Vector3 cameraPosition = App->CL_Ogre->camNode->getPosition();
					int cameraComparison = App->CL_X_Maths->Ogre_Vector3_Compare(&cameraPosition, &App->CL_Camera->Saved_Cam_Pos, 0);

					if (cameraComparison == 1)
					{
						App->CL_Views_Com->Current_View = App->CL_Views_Com->VCam[V_TR];
						if (isSceneEditorActive)
						{
							App->CL_Editor_Scene->Context_Menu_Ogre(hDlg);
						}
						else if (isPreviewModeRunning)
						{
							App->CL_Views_Com->Context_Menu_Ogre(hDlg);
						}
					}
				}

				App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
				return 1;
			}
		}
	}

	}

	return FALSE;
}
