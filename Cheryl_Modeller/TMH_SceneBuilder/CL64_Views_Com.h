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

#include "CL64_Render_Map.h"
#include "CX_Brush.h"

enum View
{
	V_None = -1,
	V_TL = 0,
	V_TR = 1,
	V_BL = 2,
	V_Ogre = 3
};


class CL64_Views_Com
{
public:
	CL64_Views_Com();
	~CL64_Views_Com();

	void Reset_Class();

	void Init_Map_Views();
	void Init_Views(int View);
	void ResizeOgreWindow(void);

	void Render_RenderBrushSelFacesOrtho(ViewVars* Cam, Brush* b, HDC ViewDC);
	
	void Context_3D_Menu(HWND hDlg);
	bool Context_3D_Command(WPARAM wParam);

	void Context_Grids_Menu(HWND hDlg);
	bool Context_Grids_Command(WPARAM wParam);

	void Set_View();
	void Reset_Views_All();
	void Reset_To_Camera();
	
	void Resize_Windows(HWND hDlg, int newWidth, int newDepth);

	void Set_Splitter_WidthDepth(int Width, int Depth);
	void Save_Splitter_Width_Depth();

	void Set_3D_FullView();
	void Set_Selected_View(int Selected_View);
	void Update_Selected_View();

	void On_Mouse_Move(POINT CursorPosition, HWND hDlg);
	void On_Left_Button_Up(POINT CursorPosition);
	void On_Left_Button_Down(POINT CursorPosition, HWND hDlg);

	HWND Main_View_Dlg_Hwnd; // Background of MDI

	float GridSize, GridSnapSize;
	
	// TODO hgtterry Make Private
	int nleftWnd_width;
	int nleftWnd_Depth;
	int Copy_Spliter_Depth;
	int Copy_Spliter_Width;
	int Mouse_Wheel_Selected_Speed;
	int Selected_Window;
	ViewVars* Current_View;

	Colour_Int Background_Colour;
	
	POINT mStartPoint;

	bool flag_Left_Button_Down;
	bool flag_Right_Button_Down;
	bool flag_Context_Menu_Active;
	bool flag_Wheel_Active;
	bool flag_Environment_On;
	
	HCURSOR	hcSizeEW;
	HCURSOR	hcSizeNS;
	HCURSOR	hcBoth;

	HMENU hMenu;

	HBRUSH Stock_Brush;
	HBRUSH BackGround_Brush;

	HPEN PenTemplate;
	HPEN PenSelected;
	HPEN Pen_Camera;
	HPEN PenSelectedFaces;
	HPEN PenCutBrush;
	HPEN PenEntity;
	
private:
	static LRESULT CALLBACK Proc_Main_Dlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void Create_Views();

	// Draw Routines
	
	void SetEditCursor(int Tool, const POINT* pMousePos);
	int GetCursorBoxPos(const POINT* ptMousePos);
	
	int LEFT_WINDOW_WIDTH;

	int LEFT_WINDOW_DEPTH;
	int TOP_POS_BOTLEFT;

	int RIGHT_MINIMUM_SPACE;
	int LEFT_MINIMUM_SPACE;

	bool Do_Width;
	bool Do_Depth;
	bool Do_All;
};

