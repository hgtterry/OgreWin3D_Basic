/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

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
#include "resource.h"
#include "CL64_App.h"
#include "CL64_Top_Tabs.h"

CL64_Top_Tabs::CL64_Top_Tabs(void)
{
	Headers_hWnd = nullptr;
	flag_Brush_Select = 1;
	flag_Brush_Move = 0;
	flag_Brush_Scale = 0;
}

CL64_Top_Tabs::~CL64_Top_Tabs(void)
{
}

// *************************************************************************
// *	  			Message:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
void CL64_Top_Tabs::Start_Headers()
{
	Headers_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_TOP_TABS_HEADERS, App->MainHwnd, (DLGPROC)Proc_Headers);
}

// *************************************************************************
// *        	Message_Proc:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
LRESULT CALLBACK CL64_Top_Tabs::Proc_Headers(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_BT_BRUSH_SELECT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_BRUSH_MOVE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_BRUSH_SCALE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_BRUSH_SHEAR, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_BT_ALLFACES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_NEXTFACE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_PREVFACE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{
		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BT_BRUSH_SELECT)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_BRUSH_SELECT));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle_Tabs(item, App->CL_Top_Tabs->flag_Brush_Select);
			}

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_BRUSH_MOVE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_BRUSH_MOVE));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle_Tabs(item, App->CL_Top_Tabs->flag_Brush_Move);
			}

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_BRUSH_SCALE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_BRUSH_SCALE));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle_Tabs(item, App->CL_Top_Tabs->flag_Brush_Scale);
			}

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_BRUSH_SHEAR)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_BRUSH_SHEAR));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Normal(item);
			}

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_ALLFACES)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_ALLFACES));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Normal(item);
			}

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_NEXTFACE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_NEXTFACE));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Normal(item);
			}

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_PREVFACE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_PREVFACE));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Normal(item);
			}

			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_BT_BRUSH_SELECT)
		{
			App->CL_Top_Tabs->Reset_Brush_Buttons();
			App->CL_Top_Tabs->flag_Brush_Select = 1;
			RedrawWindow(App->CL_Top_Tabs->Headers_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			App->CL_Doc->DoGeneralSelect(false);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_BRUSH_MOVE)
		{
			SetCursor(App->CL_MapEditor->hcBoth);

			App->CL_Top_Tabs->Reset_Brush_Buttons();
			App->CL_Top_Tabs->flag_Brush_Move = 1;
			RedrawWindow(App->CL_Top_Tabs->Headers_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			App->CL_Doc->mCurrentTool = CURTOOL_NONE;
			App->CL_Doc->mModeTool = ID_TOOLS_BRUSH_MOVEROTATEBRUSH;
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_BRUSH_SCALE)
		{
			SetCursor(App->CL_MapEditor->hcBoth);

			App->CL_Top_Tabs->Reset_Brush_Buttons();
			App->CL_Top_Tabs->flag_Brush_Scale = 1;
			RedrawWindow(App->CL_Top_Tabs->Headers_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			
			App->CL_Doc->mCurrentTool = CURTOOL_NONE;
			App->CL_Doc->mModeTool = ID_TOOLS_BRUSH_SCALEBRUSH;
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_ALLFACES)
		{
			App->CL_Doc->SelectAllFacesInBrushes();
			App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
			App->CL_Properties_Tabs->Select_Textures_Tab();

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_NEXTFACE)
		{
			if (App->CL_SelFaceList->SelFaceList_GetSize(App->CL_Doc->pSelFaces) == 0)
			{
				App->CL_Doc->SelectAllFacesInBrushes();
				App->CL_Face->Select_Next_Face();
			}
			else
			{
				App->CL_Face->Select_Next_Face();
			}

			//App->CL_Doc->UpdateSelected(Enums::UpdateViews_Grids);
			App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
			App->CL_Properties_Tabs->Select_Textures_Tab();

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_PREVFACE)
		{
			if (App->CL_SelFaceList->SelFaceList_GetSize(App->CL_Doc->pSelFaces) == 0)
			{
				App->CL_Doc->SelectAllFacesInBrushes();
				App->CL_Face->Select_Previous_Face();
			}
			else
			{
				App->CL_Face->Select_Previous_Face();
			}

			//App->CL_Doc->UpdateSelected(Enums::UpdateViews_Grids);
			App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
			App->CL_Properties_Tabs->Select_Textures_Tab();

			return TRUE;
		}
		
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
	}
	}

	return FALSE;
}

// *************************************************************************
// *	Enable_Brush_Options_Buttons:- Terry and Hazel Flanigan 2025   	   *
// *************************************************************************
void CL64_Top_Tabs::Enable_Brush_Options_Buttons(bool Enable, bool Active)
{
	EnableWindow(GetDlgItem(Headers_hWnd, IDC_BT_BRUSH_MOVE), Enable);
	flag_Brush_Move = Active;

	EnableWindow(GetDlgItem(Headers_hWnd, IDC_BT_BRUSH_SCALE), Enable);
	flag_Brush_Scale = Active;

	EnableWindow(GetDlgItem(Headers_hWnd, IDC_BT_ALLFACES), Enable);
	//flag_Brush_Scale = Active;

	EnableWindow(GetDlgItem(Headers_hWnd, IDC_BT_NEXTFACE), Enable);
	//flag_Brush_Scale = Active;

	EnableWindow(GetDlgItem(Headers_hWnd, IDC_BT_PREVFACE), Enable);
	//flag_Brush_Scale = Active;
}

// *************************************************************************
// *		Enable_Select_Button:- Terry and Hazel Flanigan 2025   	  	   *
// *************************************************************************
void CL64_Top_Tabs::Enable_Select_Button(bool Enable, bool Active)
{
	EnableWindow(GetDlgItem(Headers_hWnd, IDC_BT_BRUSH_SELECT), Enable);
	flag_Brush_Select = Active;
}

// *************************************************************************
// *		Enable_Move_Button:- Terry and Hazel Flanigan 2025   	  	   *
// *************************************************************************
void CL64_Top_Tabs::Enable_Move_Button(bool Enable, bool Active)
{
	EnableWindow(GetDlgItem(Headers_hWnd, IDC_BT_BRUSH_MOVE), Enable);
	flag_Brush_Move = Active;
}

// *************************************************************************
// *	  	Reset_Brush_Buttons:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Top_Tabs::Reset_Brush_Buttons()
{
	flag_Brush_Select = 0;
	flag_Brush_Move = 0;
	flag_Brush_Scale = 0;

	RedrawWindow(Headers_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}
