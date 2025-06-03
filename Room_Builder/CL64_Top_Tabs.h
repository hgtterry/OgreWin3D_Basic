/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

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
class CL64_Top_Tabs
{
public:
	CL64_Top_Tabs(void);
	~CL64_Top_Tabs(void);

	void Reset_Class();

	void Start_Headers();

	void Enable_TopBar_Brush_Buttons(bool Enable, bool Active);
	void Enable_TopBar_Face_Buttons(bool Enable);

	void Enable_Select_Button(bool Enable, bool Active);
	void Enable_Move_Button(bool Enable, bool Active);
	void Deselect_Faces_Dlg_Buttons();
	void Reset_Brush_Buttons();

	void Select_Face();
	void Update_Faces_Combo();

	void Set_Brush_Mode(int Mode, int Dlg_Selection);
	void Show_TopTabs(bool Enable);


	int Copy_Spliter_Depth;
	int Copy_Spliter_Width;

	bool flag_Brush_Select;
	bool flag_Brush_Move;
	bool flag_Brush_Rotate;

	bool flag_Full_View_3D;
	bool flag_View_Top_Left;
	bool flag_View_Top_Right;
	bool flag_View_Bottom_Left;

	bool flag_All_Faces;

	bool flag_TopTabs_Active;

	HWND Headers_hWnd;

private:
	static LRESULT CALLBACK Proc_Headers(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void Init_Bmps_Globals(void);

	bool flag_Brush_Scale;

	bool flag_Next_Face;
	bool flag_Prev_Face;

};

