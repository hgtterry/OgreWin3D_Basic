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
class CL64_Properties_Templates
{
public:
	CL64_Properties_Templates();
	~CL64_Properties_Templates();

	void Enable_Insert_Button(bool Enable);
	void Start_TemplatesDialog();
	void Show_TemplatesDialog(bool Show);
	void Insert_Template();

	char LastCreated_ShapeName[20];

	HWND TemplatesDlg_Hwnd;

private:
	static LRESULT CALLBACK Proc_Templates(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void Set_Icons();
	
	bool flag_Insert_Enabled;
};

