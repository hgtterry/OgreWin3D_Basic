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

class A_CreateBoxDialog
{
public:
	A_CreateBoxDialog(void);
	~A_CreateBoxDialog(void);

	void Start_CreateBox_Dlg();
	void CreateDefault_TemplateCube();

	void CreateCube();

	void CreateNewTemplateBrush(Brush* pBrush);

	BrushTemplate_Box *pBoxTemplate;

	float	m_YSize;
	int		m_Solid;
	float	m_XSizeBot;
	float	m_XSizeTop;
	float	m_ZSizeBot;
	float	m_ZSizeTop;
	bool	m_TCut;
	float	m_Thickness;
	bool	m_TSheet;

	bool	m_UseCamPos;

private:

	static LRESULT CALLBACK Proc_CreateBox(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void SetMembers();
	void Get_Dialog_Members(HWND hDlg);
	void Set_Dialog_Members(HWND hDlg);
	
	void Set_BoxTemplate();

	void SetDefaults(HWND hDlg);
	void SetRoom(HWND hDlg);

	void Zero_Dlg_Flags(HWND hDlg);

	bool Solid_Flag;
	bool Hollow_Flag;
	bool Cut_Flag;

	bool flag_Default;
	bool flag_Room;

	char BoxName[MAX_PATH];
};
