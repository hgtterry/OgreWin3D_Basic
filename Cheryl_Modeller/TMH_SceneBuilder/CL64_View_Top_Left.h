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

typedef struct tagBrushDrawData_TL
{
	const Box3d* pViewBox;
	HDC 			pDC;
	ViewVars* v;
	int				GroupId;
	CL64_Doc* pDoc;
	BrushFlagTest	FlagTest;
	Ogre::uint32	Color;
} BrushDrawData_TL;

class CL64_View_Top_Left
{
public:
	CL64_View_Top_Left();
	~CL64_View_Top_Left();

	void Create_Top_Left_Window();
	void Redraw_Window_TL();
	void Set_VCam_TL_Defaults();

	HWND Top_Left_Window_Hwnd;
	HWND Top_Left_Banner_Hwnd;

	signed int fdocShowBrush(Brush const* b, Box3d const* ViewBox);

	BrushDrawData_TL m_brushDrawData_TL;

	float m_GridSize;
	float m_GridSnapSize;
	float m_Zoom_Amount;

	HPEN Pen_Fine_Grid;
	HPEN m_Pen_Grid;
	HPEN Pen_Camera;

	HPEN PenBrushes;

	HDC	m_MemoryhDC_TL;
	T_Vec3 Saved_Cam_Position;

	ViewVars* VCam_TL;

private:
	static LRESULT CALLBACK Proc_Top_Left_Window(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void Draw_Screen_TL(HWND hwnd);

	static int Draw_Brush_2D(Brush* pBrush, void* lParam);
	void Draw_Faces_TL(Brush* SB);

};

