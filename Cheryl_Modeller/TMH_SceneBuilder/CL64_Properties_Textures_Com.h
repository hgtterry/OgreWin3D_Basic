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
class CL64_Properties_Textures_Com
{
public:

	CL64_Properties_Textures_Com(void);
	~CL64_Properties_Textures_Com(void);

	void Reset_Class(void);

	void Show_Materials_Dialog(bool Show);
	bool Start_Texture_Editor_Dialog();

	bool Start_Props_Textures_Dialog();

	bool RenderTexture_Blit(HDC hDC, HBITMAP Bmp, const RECT* SourceRect, const RECT* DestRect);

	void Get_First_Texture();
	bool View_Texture(char* TextureName, char* MaterialName);
	void Texture_To_HBITMP(char* TextureFileName);

	void Fill_Textures_ListBox();
	void Fill_Materials_ListBox();

	void Update_Dlg_Bmp_Texture();

	void List_Material_Changed(int index);
	void List_Texture_Changed(int Index);

	bool Change_Texture(void);

	int Selected_Group;

	long BasePicWidth;
	long BasePicHeight;

	char m_Current_TextureName[MAX_PATH];
	char m_Current_MaterialName[MAX_PATH];
	char m_Export_PathAndName[MAX_PATH];

	bool Textures_Editor_Dlg_Active;

	int Slider_Index;
	int Slider_Index_Copy;

	HBITMAP	Sel_BaseBitmap;

	HWND Textures_Dlg_Hwnd_Assimp;
	HWND Texture_Editor_Dlg_Hwnd;
	

//private:
	static bool CALLBACK ViewerBasePic(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:

	static LRESULT CALLBACK Proc_Textures_Dialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_Texture_Editor(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

};


