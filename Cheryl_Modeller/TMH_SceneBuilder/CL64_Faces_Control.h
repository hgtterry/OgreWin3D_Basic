/*
Copyright (c) 2024 - 2026 HGT_Software W.T.Flanigan H.C.Flanigan

Cheryl 3D Modeller

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
class CL64_Faces_Control
{
public:
	CL64_Faces_Control(void);
	~CL64_Faces_Control(void);

	void Start_Faces_Control_Dlg();

	void Update_Faces_Dialog();

	void Unselect_All_Face();
	void Select_All_Face();
	void Select_Next_Face();
	void Select_Prev_Face();
	void Select_Face();

	void Reset_Face_Buttons();
	void Reset_Brush_Buttons();
	void Select_Brush_Faces(Brush* b);

	int Selected_Face_Index;

	bool flag_Brush_Select;
	bool flag_Brush_Move;
	bool flag_Brush_Rotate;
	bool flag_Brush_Scale;

	bool flag_No_Faces;
	bool flag_All_Faces;
	bool flag_Next_Face;
	bool flag_Prev_Face;

	HWND Faces_Control_Dlg_hWnd;

private:
	static LRESULT CALLBACK Proc_Top_Tabs_Faces(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void Set_Brush_Mode(int Mode, int Dlg_Selection);
};

