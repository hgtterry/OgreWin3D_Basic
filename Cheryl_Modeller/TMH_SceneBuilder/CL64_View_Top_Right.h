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

typedef struct tagBrushDrawData_TR
{
	const Box3d* pViewBox;
	HDC 			pDC;
	ViewVars* v;
	int				GroupId;
	CL64_Doc* pDoc;
	BrushFlagTest	FlagTest;
	Ogre::uint32	Color;
} BrushDrawData_TR;

class CL64_View_Top_Right
{
public:
	CL64_View_Top_Right();
	~CL64_View_Top_Right();

	void Create_Top_Right_Window();
	void Redraw_Window_TR();

	static int Draw_Brush(Brush* pBrush, void* lParam);
	void Draw_Brush_Faces_Ortho(const ViewVars* Cam, Brush* b);
	void Set_VCam_TR_Defaults();

	HWND Top_Right_Window_Hwnd;

	signed int fdocShowBrush(Brush const* b, Box3d const* ViewBox);

	BrushDrawData_TR m_brushDrawData_TR;

	float m_GridSize, m_GridSnapSize;

	HPEN m_Pen_Grid;
	HPEN Pen_Camera;

	HDC	m_MemoryhDC_TR;
	ViewVars* VCam_TR;

private:
	static LRESULT CALLBACK Proc_Top_Right_Window(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void Draw_Screen_TR(HWND hwnd);

};

