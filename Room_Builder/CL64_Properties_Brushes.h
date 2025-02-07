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
class CL64_Properties_Brushes
{
public:
	CL64_Properties_Brushes();
	~CL64_Properties_Brushes();

	void Show_Brushes_Dialog(bool Show);
	void Enable_Options_Buttons(bool Enable);

	void Start_Brush_Tabs_Dialog();
	void Fill_ListBox();
	void Get_Index(const Brush* b);
	void OnSelchangeBrushlist(int Index, bool Clear);
	void Update_SelectedBrushesCount_Dlg();

	int Selected_Index;
	Brush* Selected_Brush;

	HWND BrushesDlg_Hwnd;

private:
	static LRESULT CALLBACK Proc_Brush_Tabs(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_Dimensions_Dlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void List_Selection_Changed(bool Clear);
	void Start_Dimensions_Dlg();

	void Update_Deltas_Dlg(HWND hDlg);
	void Fill_ComboBox_PosDelta(HWND hDlg);
	void Fill_ComboBox_RotDelta(HWND hDlg);

	void Update_From_Brush_Dlg(HWND hDlg);
	void Get_Brush();

	void Move_Brush();
	void Rotate_Brush(float SX, float SY, float SZ);
	void Scale_Brush(float SX, float SY, float SZ);

	float PosX_Delta;
	float PosY_Delta;
	float PosZ_Delta;

	float RotX_Delta;
	float RotY_Delta;
	float RotZ_Delta;

	float ScaleX_Delta;
	float ScaleY_Delta;
	float ScaleZ_Delta;

	T_Vec3 FinalScale;
	T_Vec3 FinalRot;
	T_Vec3 CenterOfSelection;
	T_Vec3 Rotation;
	T_Vec3 Size;

	HWND Dimensions_Dlg_hWnd;

	bool flag_Brushes_Dlg_Created;
	bool flag_Dimension_Dlg_Active;
};

