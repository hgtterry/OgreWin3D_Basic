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
class CL64_PB
{
public:

	CL64_PB(void);
	~CL64_PB(void);

	bool Start_ProgressBar();
	bool Set_Progress(char* ProcessText, float TotalSteps);
	bool Set_Progress_Text(char* ProcessText);
	bool Stop_Progress_Bar(char* ProcessText);
	bool Nudge(char* Message);
	bool Close();

	HWND ProgBarHwnd;

	HWND Dio;
	int Pani;

	int g_pos;

	HWND Bar;

	float Steps;
	bool flag_ClearBarDlg;


protected:

	static LRESULT CALLBACK Proc_ProgressBar(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);


};

