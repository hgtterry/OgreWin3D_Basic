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

class CL64_Dialogs
{
public:
	CL64_Dialogs(void);
	~CL64_Dialogs(void);

	void YesNo(const char* Text, const char* Text2);
	void Message(char* pString, char* pString2);
	void PleaseWait();
	void Start_Brush_Properties_Dlg();
	void Dialog_Text(int What_Check);
	void Dialog_SnapOptions();
	void Start_TextureViewer_Dialog(char* TextureFile, HWND Owner_hDlg);
	void Start_General_ListBox(int ListType);

	char btext[MAX_PATH];
	char Chr_Text[MAX_PATH];
	char mTextureFile[MAX_PATH];
	char MessageString[MAX_PATH];
	char MessageString2[MAX_PATH];

	bool flag_Dlg_Canceled;

	HWND TextureView_Hwnd;

	HBITMAP	Sel_BaseBitmap;

private:

	static LRESULT CALLBACK Proc_Message(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_PleaseWait(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_Brush_Properties(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_Dialog_Text(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_YesNo(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_SnapOptions(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_TextureViewer(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	
	static bool CALLBACK Proc_ViewerBasePic(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);;
	
	static LRESULT CALLBACK Proc_General_ListBox(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	// Brush Information
	void Fill_Brush_Combo(HWND hDlg);
	void List_BrushData(HWND hDlg);
	bool Show_Brush_Info(const Brush* b, HWND hDlg);
	bool Show_Brush_ListInfo(BrushList* BList, HWND hDlg);
	bool Show_Brush_Faces_Info(const FaceList* pList, HWND hDlg);


	bool Show_Face_Data(int Index, const Face* f, HWND hDlg);
	void List_Used_Textures(HWND List);

	int Check_What;
	int m_ListType;
	char Message_Text_Header[MAX_PATH];
	char Message_Text_Message[MAX_PATH];

	bool flag_boolBrush_Properties_Dialog_Active;

};

