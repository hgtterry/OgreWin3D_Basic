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
class CL64_Properties_Materials
{
public:
	CL64_Properties_Materials(void);
	~CL64_Properties_Materials(void);

	void Reset_Class();

	void Start_Materials_Dialog_Ogre();
	void Show_Materials_Dialog_Ogre(bool Show);
	void Get_First_Texture_Ogre();
	bool Update_Texture_Ogre_Dlg();
	bool View_Texture(char* TextureName, char* MaterialName);
	void Texture_To_HBITMP(char* TextureFileName);
	bool SelectBitmap();
	void Fill_Materials_ListBox();
	void List_Material_Changed();
	void Select_By_Index(int Index);
	void Fill_Textures_ListBox();

	char m_CurrentTexture_Ogre[MAX_PATH];
	char m_MaterialName_Ogre[MAX_PATH];
	char mExport_PathAndName_Ogre[MAX_PATH];

	int Selected_Material_Index;
	int Selected_Group;

	HBITMAP	Sel_BaseBitmap_Ogre;
	long BasePicWidth_Ogre;
	long BasePicHeight_Ogre;

	Ogre::String mFileString_Ogre;

	HWND Textures_Dlg_Hwnd_Ogre;

private:
	static LRESULT CALLBACK Proc_Materials_Dialog_Ogre(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	static bool CALLBACK ViewerBasePic_Ogre(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	bool RenderTexture_Blit_Ogre(HDC hDC, HBITMAP Bmp, const RECT* SourceRect, const RECT* DestRect);

};

