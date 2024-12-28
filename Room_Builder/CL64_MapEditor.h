/*
Copyright (c) 2024 Inflanite_HGT W.T.Flanigan H.C.Flanigan

Room Builder

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

#define	M_PI		((float)3.14159265358979323846f)
#define	TOP_POS					8
#define	BOTTOM_POS				400
#define	SPLITTER_BAR_WIDTH		5
#define WIDTH_ADJUST			2

#define BOTTOM_POS_BOTLEFT		5

class CL64_MapEditor
{
public:
	CL64_MapEditor();
	~CL64_MapEditor();

	void Init_Map_Views();
	void Init_Views();

	HWND Main_Dlg_Hwnd;

private:
	static LRESULT CALLBACK Proc_Main_Dlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK Proc_Top_Left_Window(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_Top_Right_Window(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_Bottom_Left_Window(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Bottom_Right_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);



	void Create_Top_Left_Window();
	void Create_Top_Right_Window();
	void Create_Bottom_Left_Window();
	void Create_Bottom_Right_Window();

	void Resize_Windows(HWND hDlg, int NewWidth, int NewDepth);

	int LEFT_WINDOW_WIDTH;

	int LEFT_WINDOW_DEPTH;
	int TOP_POS_BOTLEFT;


	int RIGHT_MINIMUM_SPACE;
	int LEFT_MINIMUM_SPACE;

	int nleftWnd_width;
	int nleftWnd_Depth;

	bool Do_Width;
	bool Do_Depth;
	bool Do_All;

	HWND Left_Window_Hwnd;
	HWND Right_Window_Hwnd;
	HWND Bottom_Left_Hwnd;
	HWND Bottom_Right_Hwnd;

	HBRUSH BackGround_Brush;

};

