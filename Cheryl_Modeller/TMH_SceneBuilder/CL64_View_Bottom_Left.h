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

typedef struct tagBrushDrawData_BL
{
	const Box3d* pViewBox;
	HDC 			pDC;
	ViewVars* v;
	int				GroupId;
	CL64_Doc* pDoc;
	BrushFlagTest	FlagTest;
	Ogre::uint32	Color;
} BrushDrawData_BL;

class CL64_View_Bottom_Left
{
public:
	CL64_View_Bottom_Left();
	~CL64_View_Bottom_Left();

	void Create_Bottom_Left_Window();
	void Redraw_Window_BL();

	static int Draw_Brush(Brush* pBrush, void* lParam);
	void Set_VCam_BL_Defaults();

	HWND Bottom_Left_Window_Hwnd;
	HWND Bottom_Left_Banner_Hwnd;

	signed int fdocShowBrush(Brush const* b, Box3d const* ViewBox);

	BrushDrawData_BL m_brushDrawData_BL;

	float m_GridSize;
	float m_GridSnapSize;
	float m_Zoom_Amount;

	HPEN Pen_Fine_Grid;
	HPEN m_Pen_Grid;
	HPEN Pen_Camera;

	HPEN PenBrushes;

	HDC	m_MemoryhDC_BL;
	T_Vec3 Saved_Cam_Position;

	ViewVars* VCam_BL;

private:
	static LRESULT CALLBACK Proc_Bottom_Left_Window(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void Draw_Screen_BL(HWND hwnd);
	void Draw_Faces_BL(Brush* SB);
};

