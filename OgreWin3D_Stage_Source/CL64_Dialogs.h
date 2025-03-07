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
class CL64_Dialogs
{
public:

	CL64_Dialogs(void);
	~CL64_Dialogs(void);

	void Message(char* pString, char* pString2);
	void Start_Import_Options_Dlg();
	void PleaseWait();
	void Show_YesNo_Dlg(char* Text, char* Text2, char* Text3);
	void Start_Speed_Camera_Dlg();
	void Start_FPSLock_Dlg();
	bool Dialog_Text(int Usage);
	void Start_TextureViewer_Dialog(char* TextureFile, HWND Owner_hDlg);
	void Start_FileViewer_Dialog(char* FFile, HWND Owner_hDlg);
	void Material_Search(char* ItemString);
	bool Dialog_TrueFlase(HWND Parent);
	void Dialog_Counter();
	bool UpDate_Counter_Dialog(HWND hDlg);
	void Set_Counter_Dialog(HWND hDlg, bool Enable);
	void Dialog_Text_Props();
	void Dialog_Int();
	void GameMode_StartPosition_Dlg();
	void Game_Config_Dlg();

	bool flag_Game_Start_Option;
	bool flag_TrueFlase;
	bool flag_Canceled;
	bool flag_Convert_to_Ogre;

	char btext[MAX_PATH];
	char Chr_Text[MAX_PATH];

	char Chr_Int[50];
	int mInt;

	int What_Check_Name;
	int CheckNames;

	HBITMAP	Sel_BaseBitmap;

	HWND FileViewer_Hwnd;
	HWND RightGroups_Hwnd;
	HWND FPSLock_Dlg_hWnd;

private:

	static LRESULT CALLBACK Proc_Message(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_Import_Options_Dlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_PleaseWait(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_YesNo(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_Speed_Camera(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_FPSLock(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_Dialog_Text(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_TextureViewer(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_FileViewer(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_Dialog_TrueFlase(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_Dialog_Counter(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_Dialog_Text_Props(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_Dialog_Int(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_GameMode_StartPosition_Dlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_Game_Config(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	static bool CALLBACK Proc_ViewerBasePic(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	void UnCheck_All_SpeedMouseOption();
	void Fill_Face_Combo(HWND hDlg);
	void Read_File(char* mFile, HWND hDlg);
	void Set_Counter_Dialog_Details(HWND hDlg);

	char Message_Text_Header[MAX_PATH];
	char Message_Text_Message[MAX_PATH];

	char MessageString[MAX_PATH];	// Yes No Dialog
	char MessageString2[MAX_PATH];	// Yes No Dialog
	char MessageString3[MAX_PATH];	// Yes No Dialog

	char mTextureFile[MAX_PATH];
	char mFile[MAX_PATH];

	bool flag_Mouse_Normal;
	bool flag_Mouse_Slow;
	bool flag_Mouse_VerySlow;
	bool flag_Mouse_Fast;
	bool flag_Dlg_Int_Active;
};

