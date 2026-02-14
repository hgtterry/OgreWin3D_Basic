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
class CL64_Properties_Textures
{
public:
	CL64_Properties_Textures();
	~CL64_Properties_Textures();

	void Reset_Class();

	void Show_Textures_Dialog(bool Show);
	void Start_TextureDialog();

	void Fill_ListBox();
	void Update_Texture();

	void Select_With_List_Index(int Index);
	void List_Selection_Changed();

	void Get_Selected_Face();
	void Select_With_TextureName(const char* TextureName);

	void Enable_FaceProps_Button(bool Enable);

	HBITMAP	Sel_BaseBitmap;
	long BasePicWidth;
	long BasePicHeight;

	int Selected_Index;

	bool Dialog_Created;
	bool Dialog_Textures_Visible;

	char m_CurrentTexture[MAX_PATH];
	char m_MaterialName[MAX_PATH];
	char mExport_PathAndName[MAX_PATH];

	HWND Textures_Dlg_Hwnd;
	Face* mSelected_Face;

private:
	static LRESULT CALLBACK Proc_TextureDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	
	static bool CALLBACK ViewerBasePic(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	bool RenderTexture_Blit(HDC hDC, HBITMAP Bmp, const RECT* SourceRect, const RECT* DestRect);

	void Init_Bmps_Globals(void);
	bool SelectBitmap();
	void Texture_To_HBITMP(char* TextureFileName);

	void Apply_Texture();
	int GetIndexFromTextureName(char* TextureName);

};

