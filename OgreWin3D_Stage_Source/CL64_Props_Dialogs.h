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
class CL64_Props_Dialogs
{
public:

	CL64_Props_Dialogs(void);
	~CL64_Props_Dialogs(void);

	void Start_Props_Dialogs();

	void Show_Physics_Test_Dlg(bool Show);
	void Show_Details_Goto_Dlg(bool Show);

private:

	static LRESULT CALLBACK Proc_Details_Goto(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_Dialog_PhysicsTest(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void Start_Details_Goto_Dlg(void);
	void Start_Dialog_PhysicsTest();

	HWND Details_Goto_Hwnd;
	HWND PhysicsTest_Dlg_hWnd;

};

