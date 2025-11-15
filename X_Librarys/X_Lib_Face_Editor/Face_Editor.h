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
class Face_Editor
{
public:
	Face_Editor(void);
	~Face_Editor(void);

	void Start_FaceDialog();
	void Change_Selection();
	void Close_Faces_Dialog();
	void List_Face_Data(HWND List);

	HWND FaceDlg_Hwnd;

	int m_NumberOfFaces;

	bool flag_FaceDlg_Active;

private:
	static LRESULT CALLBACK Proc_FaceDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void Update_Faces();
	void Update_Face_List(HWND hDlg);
	void Update_Face_Members();
	void UpdateDialog(HWND hDlg);
	void Update_Face_Info(HWND hDlg);

	void Fill_ComboBox_AngleValues(HWND hDlg);
	void Fill_ComboBox_ScaleValues(HWND hDlg);
	void Fill_ComboBox_OffSetValues(HWND hDlg);

	static signed int FlipVertical(Face* pFace, void*);
	static signed int FlipHorizontal(Face* pFace, void*);

	static signed int ChangeTextureAngle(Face* pFace, void* lParam);
	static signed int ChangeTextureXScale(Face* pFace, void* lParam);
	static signed int ChangeTextureYScale(Face* pFace, void* lParam);
	static signed int ChangeXOffset(Face* pFace, void* lParam);
	static signed int ChangeYOffset(Face* pFace, void* lParam);

	HWND Slider_Scale_X_hWnd;
	HWND Slider_Scale_Y_hWnd;
	HWND Slider_Offset_X_hWnd;
	HWND Slider_Offset_Y_hWnd;
	HWND Slider_Rotation_hWnd;

	Face* m_Selected_Face;

	float m_TextureAngle_Copy;
	float m_TextureAngle;
	float m_TextureAngle_Delta;

	float m_TextureXScale_Copy;
	float m_TextureXScale;
	float ScaleX_Delta;

	float m_TextureYScale_Copy;
	float m_TextureYScale;
	float ScaleY_Delta;

	int m_TextureXOffset_Copy;
	int m_TextureXOffset;
	int m_TextureXOffset_Delta;

	int m_TextureYOffset_Copy;
	int m_TextureYOffset;
	int m_TextureYOffset_Delta;

	int m_Selected_Face_Index;
};

