/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OgreWin3D

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
#include "CL64_App.h"
#include "resource.h"
#include "CL64_SoundMgr.h"

CL64_SoundMgr::CL64_SoundMgr(void)
{
	SoundEngine = irrklang::createIrrKlangDevice();
	SndFile = nullptr;
	SndVolume = 0.5;

	strcpy(mSoundFile, "");
	strcpy(Default_Folder, App->RB_Directory_FullPath);

	SoundFile_Count = 0;
	//Current_Object_Sound[0] = 0;

	Access_File[0] = 0;
	m_Current_Sound_file[0] = 0;

	flag_IsCancelled = 0;
	flag_Accessed = 0;
}

CL64_SoundMgr::~CL64_SoundMgr(void)
{
}

// *************************************************************************
// *		Play_StartUp_Sound:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_SoundMgr::Play_StartUp_Sound()
{
	char Sound[MAX_PATH];
	strcpy(Sound, Default_Folder);
	strcat(Sound, "\\Media\\Sounds\\");
	strcat(Sound, "welcome.ogg");

	SndFile = SoundEngine->play2D(Sound, false, true, true);
	SndFile->setVolume(0.5);
	SndFile->setIsPaused(false);

	return 1;
}

// *************************************************************************
// *	  		Show_Sound_Player:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_SoundMgr::Show_Sound_Player()
{
	flag_IsCancelled = 0;
	SndFile = nullptr;

	DialogBox(App->hInst, (LPCTSTR)IDD_SOUNDPLAYER, App->MainHwnd, (DLGPROC)Proc_Dialog_SoundFile);
}

// *************************************************************************
// *		Proc_Dialog_SoundFile:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
LRESULT CALLBACK CL64_SoundMgr::Proc_Dialog_SoundFile(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_TITLENAME, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_SOUNDLIST, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_EDITINT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_PLAY, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_PAUSE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_STOP, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_VOLBOX, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STVOLUME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SetDlgItemText(hDlg, IDC_TITLENAME, (LPCTSTR)"Sound Player");


		App->CL_SoundMgr->GetSoundFiles(hDlg, (LPSTR)"*.ogg");
		App->CL_SoundMgr->GetSoundFiles(hDlg, (LPSTR)"*.wav");
		App->CL_SoundMgr->GetSoundFiles(hDlg, (LPSTR)"*.mp3");

		char Sound[MAX_PATH]{ 0 };
		HWND ListHwnd = GetDlgItem(hDlg, IDC_SOUNDLIST);

		SendMessage(ListHwnd, LB_SETCURSEL, 0, (LPARAM)(LPCTSTR)0);
		SendDlgItemMessage(hDlg, IDC_SOUNDLIST, LB_GETTEXT, (WPARAM)0, (LPARAM)Sound);
		SetDlgItemText(hDlg, IDC_EDITINT, (LPTSTR)Sound);

		int Sel = SendMessage(GetDlgItem(hDlg, IDC_SOUNDLIST), LB_SELECTSTRING, -1, (LPARAM)App->CL_SoundMgr->Access_File);

		char buff[MAX_PATH]{ 0 };
		SendDlgItemMessage(hDlg, IDC_SOUNDLIST, LB_GETTEXT, (WPARAM)Sel, (LPARAM)buff);
		SetDlgItemText(hDlg, IDC_EDITINT, (LPTSTR)buff);

		int VolPer = int(App->CL_SoundMgr->SndVolume * 10);

		HWND Slider = GetDlgItem(hDlg, IDC_SLVOLUME);
		SendMessage(Slider, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, 10));
		SendMessage(Slider, TBM_SETPOS, (WPARAM)1, (LPARAM)VolPer);

		VolPer = VolPer * 10;

		char buf[10];
		sprintf(buf, "%i", VolPer);
		SetDlgItemText(hDlg, IDC_VOLBOX, (LPCTSTR)buf);

		if (App->CL_SoundMgr->flag_Accessed == 1)
		{
			SendMessage(GetDlgItem(hDlg, IDC_SOUNDLIST), LB_SELECTSTRING, -1, (LPARAM)App->CL_SoundMgr->Access_File);
		}

		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_STVOLUME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_VOLBOX) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_White;
		}

		if (GetDlgItem(hDlg, IDC_EDITINT) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_White;
		}

		if (GetDlgItem(hDlg, IDC_SLVOLUME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_TITLENAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
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

	case WM_HSCROLL:
	{
		HWND Slider = GetDlgItem(hDlg, IDC_SLVOLUME);
		int pos = 0;
		int Percent = 0;
		char buf[20];
		pos = SendMessage(Slider, TBM_GETPOS, (WPARAM)0, (LPARAM)0);
		Percent = pos * 10;
		sprintf(buf, "%i", Percent);
		SetDlgItemText(hDlg, IDC_VOLBOX, (LPCTSTR)buf);

		float sum2 = float(Percent) / 100;
		App->CL_SoundMgr->SndVolume = sum2;

		if (App->CL_SoundMgr->SndFile)
		{
			App->CL_SoundMgr->SndFile->setVolume(sum2);
		}

		return 1;
	}

	case WM_NOTIFY:
	{
		LPNMHDR nmhdr = (LPNMHDR)lParam;

		if (nmhdr->idFrom == IDC_PLAY)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)nmhdr;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (nmhdr->idFrom == IDC_BT_PAUSE)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)nmhdr;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (nmhdr->idFrom == IDC_BT_STOP)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)nmhdr;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (nmhdr->idFrom == IDOK)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)nmhdr;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (nmhdr->idFrom == IDCANCEL)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)nmhdr;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

	}break;

	case WM_COMMAND:

		if (LOWORD(wParam) == IDC_PLAY)
		{

			GetDlgItemText(hDlg, IDC_EDITINT, (LPTSTR)App->CL_SoundMgr->m_Current_Sound_file, MAX_PATH);

			if (App->CL_SoundMgr->SndFile == NULL)
			{
			}
			else
			{
				App->CL_SoundMgr->SndFile->stop();
				App->CL_SoundMgr->SndFile = NULL;
			}

			int result = 1;
			result = strcmp(App->CL_SoundMgr->m_Current_Sound_file, "");
			if (result == 0)
			{

			}
			else
			{
				result = strcmp(App->CL_SoundMgr->m_Current_Sound_file, "None");
				if (result == 0)
				{

				}
				else
				{
					char Sound[1024];
					strcpy(Sound, App->CL_SoundMgr->Default_Folder);
					strcat(Sound, "\\Media\\Sounds\\");
					strcat(Sound, App->CL_SoundMgr->m_Current_Sound_file);

					App->CL_SoundMgr->SndFile = App->CL_SoundMgr->SoundEngine->play2D(Sound, false, true, true);
					App->CL_SoundMgr->SndFile->setVolume(App->CL_SoundMgr->SndVolume);
					App->CL_SoundMgr->SndFile->setIsPaused(false);
				}
			}
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_STOP)
		{
			App->CL_SoundMgr->SndFile->stop();

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_PAUSE)
		{
			bool Paused = App->CL_SoundMgr->SndFile->getIsPaused();
			if (Paused == 1)
			{
				App->CL_SoundMgr->SndFile->setIsPaused(0);
			}
			else
			{
				App->CL_SoundMgr->SndFile->setIsPaused(1);
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_SOUNDLIST)
		{
			char buff[MAX_PATH] { 0 };
			int Index = 0;
			Index = SendDlgItemMessage(hDlg, IDC_SOUNDLIST, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);

			if (Index == -1)
			{
				return 1;
			}

			SendDlgItemMessage(hDlg, IDC_SOUNDLIST, LB_GETTEXT, (WPARAM)Index, (LPARAM)buff);

			SetDlgItemText(hDlg, IDC_EDITINT, (LPTSTR)buff);

			if (App->CL_SoundMgr->flag_Accessed == 1)
			{
				strcpy(App->CL_SoundMgr->Access_File, buff);
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDOK)
		{
			if (App->CL_SoundMgr->SndFile == NULL)
			{
			}
			else
			{
				App->CL_SoundMgr->SndFile->stop();
				App->CL_SoundMgr->SndFile = NULL;
			}

			App->CL_SoundMgr->flag_IsCancelled = 0;

			char file[256];
			GetDlgItemText(hDlg, IDC_EDITINT, (LPTSTR)file, 256);
			strcpy(App->CL_SoundMgr->mSoundFile, file);
			App->CL_SoundMgr->Remeber_SoundFile(file);

			App->CL_SoundMgr->flag_Accessed = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			if (App->CL_SoundMgr->SndFile == NULL)
			{
			}
			else
			{
				App->CL_SoundMgr->SndFile->stop();
				App->CL_SoundMgr->SndFile->drop();
				App->CL_SoundMgr->SndFile = NULL;

			}

			App->CL_SoundMgr->flag_Accessed = 0;
			App->CL_SoundMgr->flag_IsCancelled = 1;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;

	}
	return FALSE;

}

// *************************************************************************
// *					GetSoundFiles Terry Bernie						   *
// *************************************************************************
void CL64_SoundMgr::GetSoundFiles(HWND hDlg, char* Extention)
{
	HWND ListHwnd = GetDlgItem(hDlg, IDC_SOUNDLIST);

	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	char SearchName[1024];
	strcpy(SearchName, Default_Folder);
	strcat(SearchName, "\\Media\\Sounds\\");
	strcat(SearchName, Extention);

	hFind = FindFirstFile(SearchName, &ffd);

	SendMessage(ListHwnd, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)ffd.cFileName);

	while (FindNextFile(hFind, &ffd) != 0)
	{
		SendMessage(ListHwnd, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)ffd.cFileName);
	}
}

// *************************************************************************
// *					Remeber_SoundFile Terry Bernie					   *
// *************************************************************************
void CL64_SoundMgr::Remeber_SoundFile(char* File)
{
	Sound_File.push_back(File);
	SoundFile_Count = Sound_File.size();

}
