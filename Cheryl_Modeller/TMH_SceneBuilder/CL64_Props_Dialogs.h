/*
Copyright (c) 2024 - 2025 TMH_Software W.T.Flanigan M.Habib H.C.Flanigan

TMH_SceneBuilder

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
class CL64_Props_Dialogs
{
public:

	CL64_Props_Dialogs(void);
	~CL64_Props_Dialogs(void);

	void Start_Props_Dialogs();
	void Start_Materials_PropsPanel();
	bool Start_Camera_PropsPanel();
	bool Start_Player_PropsPanel();
	void Start_Override_Counter(void);

	void Show_Physics_Test_Dlg(bool Show);
	void Show_Details_Goto_Dlg(bool Show);
	void Show_Materials_Dlg(bool Show);
	void Show_Cameras_Dlg(bool Show);
	void Show_Player_Dlg(bool Show);
	void Show_Override_Counter_Dlg(bool Show);

	bool flag_isHighlighted;

	HWND Player_Props_HWND;
	
private:

	static LRESULT CALLBACK Proc_Details_Goto(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_Dialog_PhysicsTest(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Prop_Materials_PropsPanel(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_Camera_PropsPanel(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_Player_PropsPanel(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_Override_Counter(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void Start_Details_Goto_Dlg(void);
	void Start_Dialog_PhysicsTest();
	
	bool flag_Show_Area_Physics_Debug;
	bool flag_Toggle_Objects;
	bool flag_Toggle_Physics;
	bool flag_Toggle_OverrideCounter;

	HWND Override_Counter_Goto_Hwnd;
	HWND Details_Goto_Hwnd;
	HWND PhysicsTest_Dlg_hWnd;
	HWND Material_Props_Hwnd;
	HWND Cam_Props_HWND;

};

