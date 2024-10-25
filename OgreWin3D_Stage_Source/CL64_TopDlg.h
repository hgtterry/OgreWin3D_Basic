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
class CL64_TopDlg
{
public:

	CL64_TopDlg(void);
	~CL64_TopDlg(void);

	void Reset_Class(void) const;

	void Start_TopBar();
	void Enable_Grid_Hair_Icons(bool Enable) const;
	void Enable_Info_Icon(bool Enable) const;
	void Enable_Info_Panel(bool Enable);
	void Enable_ImGui_Demo_Panel(bool Enable);
	void Enable_FPSLock_Dlg_Panel(bool Enable);

	void Update_Motions_By_Name(const char* Name);
	void Switch_To_Motions_Dlg(void);
	void Update_Speed_Combo(void);
	void Camera_Set_Free();

	char Selected_Motion_Name[MAX_PATH];

	bool flag_Toggle_Tabs_Game;
	bool flag_Toggle_Tabs_Camera;
	bool flag_Toggle_Tabs_Physics;
	bool flag_Toggle_Tabs_Motions;
	bool flag_Toggle_Tabs_Resources;
	bool flag_Toggle_Tabs_Locations;

	bool flag_Toggle_PhysicaDebug_Node;
	bool flag_FPS_Dlg_Running;
	
	bool flag_ShowOnlySubFaces_Saved;

	void Init_Bmps_Globals(void);

	HWND TabsHwnd;
	HWND Tabs_TB_hWnd;

	// Tab Options
	HWND Game_TB_hWnd;
	HWND Camera_TB_hWnd;
	HWND Physics_TB_hWnd;
	HWND Motions_TB_hWnd;
	HWND Locations_TB_hWnd;

private:

	static LRESULT CALLBACK Proc_TopBar(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_Tabs_Headers(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	// Tab Options
	static LRESULT CALLBACK Proc_Game_TB(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_Camera_TB(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_Physics_TB(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_Motions_TB(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_Locations_TB(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void Hide_Tabs(void);

	// Tab Options
	void Start_Tabs_Headers(void);
	void Start_Game_TB(void);
	void Start_Camera_TB(void);
	void Start_Physics_TB(void);
	void Start_Motions_TB(void);
	void Start_Locations_TB(void);

	bool flag_Toggle_Cam_ModelMode;
	bool flag_Toggle_Cam_FreeMode;
	bool flag_Toggle_Cam_FirstMode;

};

