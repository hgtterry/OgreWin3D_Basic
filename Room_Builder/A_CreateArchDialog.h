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

class A_CreateArchDialog
{
public:
	A_CreateArchDialog(void);
	~A_CreateArchDialog(void);

	void Start_CreateArch_Dlg();

	BrushTemplate_Arch *pArchTemplate;

	int		m_NumSlits;
	float	m_Thickness;
	float	m_Width;
	float	m_Radius;
	float	m_WallSize;
	int		m_Style;
	float	m_EndAngle;
	float	m_StartAngle;
	bool	m_TCut;
	float	m_Height;
	float	m_Radius2;
	bool	m_Massive;
	int		m_Shape;
	int		m_Sides;
	bool	m_Steps;
	int		m_CW;

	bool	m_UseCamPos;

private:

	static LRESULT CALLBACK CreateArch_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void Set_Members();
	void Set_DLG_Members(HWND hDlg);
	void Get_DLG_Members(HWND hDlg);
	void Set_ArchTemplate();

	void Set_Defaults(HWND hDlg);

	void CreateArch();
	void CreateNewTemplateBrush(Brush *pBrush);
	void Zero_Dlg_Flags(HWND hDlg);

	bool flag_Solid_Flag;
	bool flag_Hollow_Flag;
	bool flag_Ring_Flag;
	bool flag_Cut_Flag;
	bool flag_Rectangle_Flag;
	bool flag_Round_Flag;

	char ArchName[MAX_PATH];
};
