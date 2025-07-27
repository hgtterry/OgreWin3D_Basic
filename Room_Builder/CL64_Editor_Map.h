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
	V_Ogre = 3
};


class CL64_Editor_Map
{
public:
	CL64_Editor_Map();
	~CL64_Editor_Map();

	void Reset_Class();

	void Init_Map_Views();
	void Init_Views(int View);
	void Render_RenderBrushFacesOrtho(const ViewVars* Cam, Brush* b, HDC ViewDC);
	void Render_RenderBrushSelFacesOrtho(ViewVars* Cam, Brush* b, HDC ViewDC);
	
	bool Context_Command(WPARAM wParam);
	bool Context_Command_Ogre(WPARAM wParam);

	void Set_View();
	void Reset_Views_All();
	void Reset_To_Camera();
	
	void Resize_Windows(HWND hDlg, int NewWidth, int NewDepth);
	void Set_Splitter_WidthDepth(int Width, int Depth);
	
	void Context_Menu(HWND hDlg);
	void Context_Menu_Ogre(HWND hDlg);

	HWND Main_View_Dlg_Hwnd; // Background of MDI
	HWND Bottom_Right_Hwnd;

	HWND Left_Window_Hwnd;
	HWND Right_Window_Hwnd;
	HWND Bottom_Left_Hwnd;

	float GridSize, GridSnapSize;
	
	// TODO hgtterry Make Private
	int nleftWnd_width;
	int nleftWnd_Depth;
	
	ViewVars* VCam[4];
	ViewVars* Current_View;

	POINT mStartPoint;

	bool flag_Left_Button_Down;
	bool flag_Right_Button_Down;
	bool flag_Context_Menu_Active;

	bool flag_Environment_On;

	HCURSOR	hcSizeEW;
	HCURSOR	hcSizeNS;
	HCURSOR	hcBoth;

	HMENU hMenu;

private:
	static LRESULT CALLBACK Proc_Main_Dlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK Proc_Top_Left_Window(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_Top_Right_Window(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_Bottom_Left_Window(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_Ogre_BR(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK ViewerMain_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void Set_Views_Defaults(int Index,Ogre::int32 View, const char* Name);
	void Create_Top_Left_Window();
	void Create_Top_Right_Window();
	void Create_Bottom_Left_Window();
	void Create_Ogre_Bottom_Right();

	// Draw Routines
	void Draw_Screen(HWND hwnd);
	static signed int BrushDraw(Brush* pBrush, void* lParam);

	void Zoom_View(HWND hDlg, int Dx, int Dy);
	void Pan_View(HWND hDlg, int Dx, int Dy);

	void Draw_Camera(HDC ViewDC);

	void On_Mouse_Move(POINT CursorPosition, HWND hDlg);
	void On_Left_Button_Up(POINT CursorPosition);
	void On_Left_Button_Down(POINT CursorPosition, HWND hDlg);

	void SetEditCursor(int Tool, const POINT* pMousePos);
	int GetCursorBoxPos(const POINT* ptMousePos);
	signed int fdocShowBrush(Brush const* b, Box3d const* ViewBox);

	int LEFT_WINDOW_WIDTH;

	int LEFT_WINDOW_DEPTH;
	int TOP_POS_BOTLEFT;

	int RIGHT_MINIMUM_SPACE;
	int LEFT_MINIMUM_SPACE;

	bool Do_Width;
	bool Do_Depth;
	bool Do_All;

	HBRUSH BackGround_Brush;

	HPEN Pen_Fine_Grid;
	HPEN Pen_Grid;
	HPEN PenTemplate;
	HPEN PenBrushes;
	HPEN PenSelected;
	HPEN Pen_Camera;
	HPEN PenSelectedFaces;
	HPEN PenCutBrush;
	HPEN PenEntity;
	HDC	MemoryhDC;
};

