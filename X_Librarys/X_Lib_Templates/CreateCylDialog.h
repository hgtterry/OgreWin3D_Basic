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
class CreateCylDialog
{
public:
	CreateCylDialog(void);
	~CreateCylDialog(void);

	char* GetVersion();
	void Start_CreateCyl_Dlg();

	BrushTemplate_Cylinder* pCylinderTemplate;

	float	m_BotXOffset;
	float	m_BotXSize;
	float	m_BotZOffset;
	float	m_BotZSize;
	int		m_Solid;
	float	m_TopXOffset;
	float	m_TopXSize;
	float	m_TopZOffset;
	float	m_TopZSize;
	float	m_YSize;
	float	m_RingLength;
	BOOL	m_TCut;
	int		m_VerticalStripes;
	float	m_Thickness;

	bool	m_UseCamPos;

private:

	static LRESULT CALLBACK Proc_Create_Cylinder(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK OwnerEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

	void Capture_Edit_Boxes(HWND hDlg);
	void Remove_Edit_Boxes(HWND hDlg);

	void Init_Bmps_Globals(HWND hDlg);

	void Set_Members();
	void Set_DLG_Members(HWND hDlg);
	void Get_DLG_Members(HWND hDlg);
	void Set_CylinderTemplate();

	void Set_Defaults(HWND hDlg);

	void CreateCylinder_New();
	void CreateNewTemplateBrush(Brush* pBrush);

	void Update(void);

	char CylinderName[MAX_PATH];

	bool flag_Solid_Flag_Dlg;
	bool flag_Hollow_Flag_Dlg;
	bool flag_Ring_Flag_Dlg;
};

