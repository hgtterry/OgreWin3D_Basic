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

	void Start_TopBar();
	void Start_Tabs_Headers(void);
	void Hide_Tabs(void);
	void Enable_TestMesh_Button(bool Enable);

	// Tab Options
	void Start_Debug_TB(void);
	void Start_Camera_TB(void);
	void Start_Demos_TB(void);
	void Start_Physics_TB(void);

	bool flag_Toggle_Tabs_Debug;
	bool flag_Toggle_Tabs_Camera;
	bool flag_Toggle_Tabs_Demos;
	bool flag_Toggle_Tabs_Physics;
	
	bool flag_Toggle_PhysicaDebug_Node;

	bool flag_Demo_1_Running;
	bool flag_Demo_2_Running;

	void Init_Bmps_Globals(void);

	HWND TabsHwnd;
	HWND Tabs_TB_hWnd;

	// Tab Options
	HWND Debug_TB_hWnd;
	HWND Camera_TB_hWnd;
	HWND Demos_TB_hWnd;
	HWND Physics_TB_hWnd;

private:

	static LRESULT CALLBACK TopBar_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Tabs_Headers_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	// Tab Options
	static LRESULT CALLBACK Debug_TB_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Camera_TB_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Demos_TB_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Physics_TB_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	bool flag_Toggle_Cam_ModelMode;
	bool flag_Toggle_Cam_FreeMode;
	bool flag_Toggle_Cam_FirstMode;

	bool flag_Toggle_Demos_Demo_1;
	bool flag_Toggle_Demos_Demo_2;
	bool flag_Toggle_Demos_Options;

};

