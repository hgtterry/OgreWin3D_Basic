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

#include "pch.h"
#include "resource.h"
#include "CL64_App.h"
#include "CL64_PB.h"

CL64_PB::CL64_PB(void)
{
	ProgBarHwnd = nullptr;
	Dio = 0;
	Pani = 0;
	g_pos = 0;
	Bar = 0;
	Steps = 3000;
	flag_ClearBarDlg = 0;

}

CL64_PB::~CL64_PB(void)
{
}

// *************************************************************************
// *		Start_ProgressBar:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_PB::Start_ProgressBar()
{
	//App->CL_Panels->Disable_Panels(true);
	//App->CL_Ogre->Ogre3D_Listener->flag_Block_Mouse = 1;
	//App->CL_ImGui_Dialogs->flag_Disable_Physics_Console = 1;
	App->CL_Ogre->RenderFrame(8);

	ProgBarHwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_PROGRESS_BAR, App->MainHwnd, (DLGPROC)Proc_ProgressBar);

	Dio = 0;
	Pani = 0;
	return 1;
}
// *************************************************************************
// *		Proc_ProgressBar:- Terry and Hazel Flanigan 2024	 		   *
// *************************************************************************
LRESULT CALLBACK CL64_PB::Proc_ProgressBar(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HBRUSH holdBrush;
	HPEN hPen, holdPen;
	PAINTSTRUCT ps;
	RECT rect;

	HDC hdc;
	int till;
	int step, full;

	switch (message)
	{
	case WM_INITDIALOG:
	{

		SendDlgItemMessage(hDlg, IDC_PBBANNER, WM_SETFONT, (WPARAM)App->Font_Banner, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_PBACTION, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_PB_STATUS, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		App->CL_PB->g_pos = 0;
		App->CL_PB->Bar = GetDlgItem(hDlg, IDC_STBAR);
		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_STBAR) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_White;
		}

		if (GetDlgItem(hDlg, IDC_PBACTION) == (HWND)lParam)
		{
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_PBBANNER) == (HWND)lParam)
		{
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_PB_STATUS) == (HWND)lParam)
		{
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_PAINT:
	{

		hdc = BeginPaint(App->CL_PB->Bar, &ps);

		GetClientRect(App->CL_PB->Bar, &rect);

		till = (rect.right / App->CL_PB->Steps) * App->CL_PB->g_pos * 1;
		step = rect.right / 10.0;
		full = (rect.right / App->CL_PB->Steps);

		hPen = CreatePen(PS_NULL, 1, RGB(0, 0, 0));
		holdPen = (HPEN)SelectObject(hdc, hPen);


		if (App->CL_PB->flag_ClearBarDlg == 1)
		{
			holdBrush = (HBRUSH)SelectObject(hdc, App->Brush_White);
			::Rectangle(hdc, 0, 0, rect.right, 32);
			App->CL_PB->flag_ClearBarDlg = 0;
		}
		else
		{
			holdBrush = (HBRUSH)SelectObject(hdc, App->Brush_Green);
			::Rectangle(hdc, 0, 0, till, 32);
		}

		SelectObject(hdc, holdBrush);

		DeleteObject(hPen);

		EndPaint(hDlg, &ps);
		break;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDOK)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDOK));
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
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			/*App->CL_Panels->Disable_Panels(false);
			App->CL_ImGui_Dialogs->flag_Disable_Physics_Console = 0;
			App->CL_Ogre->Ogre3D_Listener->flag_Block_Mouse = 0;*/


			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}

// *************************************************************************
// *		Stop_Progress_Bar:- Terry and Hazel Flanigan 2024 			   *
// *************************************************************************
bool CL64_PB::Stop_Progress_Bar(char* ProcessText)
{
	EnableWindow(GetDlgItem(ProgBarHwnd, IDOK), 1);

	SetDlgItemText(ProgBarHwnd, IDC_PBBANNER, (LPCTSTR)"Finished");
	SetDlgItemText(ProgBarHwnd, IDC_ST_PB_STATUS, (LPCTSTR)ProcessText);

	InvalidateRect(ProgBarHwnd, NULL, FALSE);

	MSG msg;
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	InvalidateRect(ProgBarHwnd, NULL, FALSE);

	return 1;
}

// *************************************************************************
// *			Set_Progress:- Terry and Hazel Flanigan 2024 			   *
// *************************************************************************
bool CL64_PB::Set_Progress(char* ProcessText, float TotalSteps)
{
	MSG msg;
	char buff[1024];
	strcpy(buff, "Processing :- ");
	strcat(buff, ProcessText);

	g_pos = 0;
	Steps = TotalSteps;

	SetDlgItemText(ProgBarHwnd, IDC_PBACTION, (LPCTSTR)buff);

	App->CL_PB->flag_ClearBarDlg = 1;

	InvalidateRect(ProgBarHwnd, NULL, FALSE);

	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	InvalidateRect(ProgBarHwnd, NULL, FALSE);
	return 1;
}

// *************************************************************************
// *		Set_Progress_Text:- Terry and Hazel Flanigan 2024 			   *
// *************************************************************************
bool CL64_PB::Set_Progress_Text(char* ProcessText)
{
	MSG msg;
	char buff[1024];
	strcpy(buff, "Processing :- ");
	strcat(buff, ProcessText);

	SetDlgItemText(ProgBarHwnd, IDC_PBACTION, (LPCTSTR)buff);
	SetDlgItemText(ProgBarHwnd, IDC_ST_PB_STATUS, (LPCTSTR)ProcessText);

	InvalidateRect(ProgBarHwnd, NULL, FALSE);
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	InvalidateRect(ProgBarHwnd, NULL, FALSE);
	return 1;
}

// *************************************************************************
// *				Nudge:- Terry and Hazel Flanigan 2024 				   *
// *************************************************************************
bool CL64_PB::Nudge(char* Message)
{
	Set_Progress_Text(Message);

	MSG msg;
	g_pos++;
	InvalidateRect(ProgBarHwnd, NULL, FALSE);

	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	Sleep(100);

	return 1;
}

// *************************************************************************
// *				Close:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
bool CL64_PB::Close()
{
	DestroyWindow(ProgBarHwnd);
	return 1;
}

