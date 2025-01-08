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

#include "CL64_Render.h"
#include "CL64_Brush.h"

enum View
{
	V_None = -1,
	V_TL = 0,
	V_TR = 1,
	V_BL = 2,
	V_BR = 3
};


class CL64_MapEditor
{
public:
	CL64_MapEditor();
	~CL64_MapEditor();

	void Init_Map_Views();
	void Init_Views();
	void Render_RenderBrushFacesOrtho(const ViewVars* Cam, Brush* b, HDC ViewDC);

	HWND Bottom_Right_Hwnd;

	float GridSize, GridSnapSize;

	ViewVars* VCam[4];
	ViewVars* Current_View;

	bool flag_Left_Button_Down;

	HWND Main_Dlg_Hwnd;

private:
	static LRESULT CALLBACK Proc_Main_Dlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK Proc_Top_Left_Window(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_Top_Right_Window(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_Bottom_Left_Window(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_Bottom_Right_Ogre(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void Create_Top_Left_Window();
	void Create_Top_Right_Window();
	void Create_Bottom_Left_Window();
	void Create_Bottom_Right_Ogre();

	void Resize_Windows(HWND hDlg, int NewWidth, int NewDepth);

	void Draw_Screen(HWND hwnd);

	static signed int BrushDraw(Brush* pBrush, void* lParam);

	void On_Mouse_Move(POINT CursorPosition);
	void On_Left_Button_Up(POINT CursorPosition);
	void On_Left_Button_Down(POINT CursorPosition);

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
	

	HBRUSH BackGround_Brush;

	HPEN Pen_Fine_Grid;
	HPEN Pen_Grid;
	HPEN PenTemplate;
	HPEN PenBrushes;
	HPEN PenSelected;

	HDC	MemoryhDC;
};

