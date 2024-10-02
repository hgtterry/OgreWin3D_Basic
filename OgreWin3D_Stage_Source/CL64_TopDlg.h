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
class CL64_TopDlg
{
public:

	CL64_TopDlg(void);
	~CL64_TopDlg(void);

	void Reset_Class(void) const;

	void Start_TopBar();
	void Start_Tabs_Headers(void);
	void Hide_Tabs(void);
	void Enable_Grid_Hair_Icons(bool Enable) const;
	void Enable_Info_Icon(bool Enable) const;
	void Enable_Info_Panel(bool Enable);
	void Enable_ImGui_Demo_Panel(bool Enable);
	void Enable_FPSLock_Dlg_Panel(bool Enable);

	// Tab Options
	void Start_Camera_TB(void);
	void Start_Physics_TB(void);
	void Start_Motions_TB(void);

	void Update_Motions_By_Name(const char* Name);
	void Switch_To_Motions_Dlg(void);
	void Update_Speed_Combo(void);

	char Selected_Motion_Name[MAX_PATH];

	bool flag_Toggle_Tabs_Camera;
	bool flag_Toggle_Tabs_Physics;
	bool flag_Toggle_Tabs_Motions;
	bool flag_Toggle_Tabs_Resources;

	bool flag_Toggle_PhysicaDebug_Node;
	bool flag_FPS_Dlg_Running;
	
	bool flag_ShowOnlySubFaces_Saved;

	void Init_Bmps_Globals(void);

	HWND TabsHwnd;
	HWND Tabs_TB_hWnd;

	// Tab Options
	HWND Camera_TB_hWnd;
	HWND Physics_TB_hWnd;
	HWND Motions_TB_hWnd;

private:

	static LRESULT CALLBACK TopBar_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Tabs_Headers_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	// Tab Options
	static LRESULT CALLBACK Camera_TB_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Physics_TB_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Motions_TB_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	bool flag_Toggle_Cam_ModelMode;
	bool flag_Toggle_Cam_FreeMode;
	bool flag_Toggle_Cam_FirstMode;

	/*bool flag_Toggle_Demos_Demo_1;
	bool flag_Toggle_Demos_Demo_2;*/
	
};

