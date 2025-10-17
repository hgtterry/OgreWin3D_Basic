/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OW3D_Mesh_Builder

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
class CreateConeDialog
{
public:
	CreateConeDialog(void);
	~CreateConeDialog(void);

	void Start_CreateCone_Dlg();

	BrushTemplate_Cone* pConeTemplate;

	int		m_Style;
	float	m_Width;
	float	m_Height;
	int		m_VerticalStrips;
	float	m_Thickness;
	bool	m_TCut;

	bool	m_UseCamPos;

private:

	static LRESULT CALLBACK Proc_CreateCone(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK OwnerEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

	void Capture_Edit_Boxes(HWND hDlg);
	void Remove_Edit_Boxes(HWND hDlg);

	void Set_Members();
	void Set_DLG_Members(HWND hDlg);
	void Get_DLG_Members(HWND hDlg);
	void Set_ConeTemplate();

	void Set_Defaults(HWND hDlg);

	void CreateCone();
	void CreateNewTemplateBrush(Brush* pBrush);

	char ConeName[MAX_PATH];

	bool flag_Solid_Flag_Dlg;
	bool flag_Hollow_Flag_Dlg;
	bool flag_Funnel_Flag_Dlg;
};

