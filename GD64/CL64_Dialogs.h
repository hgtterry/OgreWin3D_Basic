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

	void Message(char* pString);
	void Start_Import_Options_Dlg();
	void PleaseWait();
	void Show_YesNo_Dlg(char* Text, char* Text2);
	void Start_Speed_Camera_Dlg();
	void Start_FPSLock_Dlg();

	bool Canceled;
	bool Flag_Convert_to_Ogre;


private:

	static LRESULT CALLBACK Message_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Import_Options_Dlg_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK PleaseWait_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK YesNo_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Speed_Camera_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK FPSLock_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void UnCheck_All_SpeedMouseOption();
	void Fill_Face_Combo(HWND hDlg);

	char Message_Text[MAX_PATH];

	char MessageString[MAX_PATH];	// Yes No Dialog
	char MessageString2[MAX_PATH];	// Yes No Dialog

	bool flag_Mouse_Normal;
	bool flag_Mouse_Slow;
	bool flag_Mouse_VerySlow;
	bool flag_Mouse_Fast;
};

