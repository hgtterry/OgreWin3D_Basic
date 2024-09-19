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

#pragma once
class CL64_Dialogs
{
public:

	CL64_Dialogs(void);
	~CL64_Dialogs(void);

	void Message(char* pString, char* pString2);
	void Start_Import_Options_Dlg();
	void PleaseWait();
	void Show_YesNo_Dlg(char* Text, char* Text2);
	void Start_Speed_Camera_Dlg();
	void Start_FPSLock_Dlg();
	bool Dialog_Text(int What_Check);
	void Start_TextureViewer_Dialog(char* TextureFile, HWND Owner_hDlg);
	void Start_FileViewer_Dialog(char* FFile, HWND Owner_hDlg);
	void Material_Search(char* ItemString);

	void Show_Reset_View_Dlg();
	void Close_Reset_View_Dlg();


	bool Canceled;
	bool Flag_Convert_to_Ogre;

	bool flag_Reset_View_Dlg_Active;

	char btext[MAX_PATH];
	char Chr_Text[MAX_PATH];

	int What_Check_Name;

	HBITMAP	Sel_BaseBitmap;

	HWND FileViewer_Hwnd;
	HWND RightGroups_Hwnd;
	HWND FPSLock_Dlg_hWnd;
	HWND Reset_View_Dlg_Hwnd;

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
	static LRESULT CALLBACK Proc_Reset_View_Dlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	static bool CALLBACK Proc_ViewerBasePic(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	void UnCheck_All_SpeedMouseOption();
	void Fill_Face_Combo(HWND hDlg);
	void Read_File(char* mFile, HWND hDlg);

	char Message_Text_Header[MAX_PATH];
	char Message_Text_Message[MAX_PATH];

	char MessageString[MAX_PATH];	// Yes No Dialog
	char MessageString2[MAX_PATH];	// Yes No Dialog

	char mTextureFile[MAX_PATH];
	char mFile[MAX_PATH];

	bool flag_Mouse_Normal;
	bool flag_Mouse_Slow;
	bool flag_Mouse_VerySlow;
	bool flag_Mouse_Fast;
};

