/*
Copyright (c) 2024 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OgreWin3D_Stage

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

enum ViewTypes
{
	VIEWSOLID = 1,
	VIEWTEXTURE = 2,
	VIEWWIRE = 4,
	VIEWTOP = 8,
	VIEWFRONT = 16,
	VIEWSIDE = 32
};

enum View
{
	V_None = -1,
	V_TL = 0,
	V_TR = 1,
	V_BL = 2,
	V_BR = 3
};

typedef struct PlaneTag
{
	Ogre::Vector3	Normal;
	float			Dist;
} GPlane;

typedef struct ViewVarsTag
{
	HBITMAP				hDibSec;
	Ogre::uint32		Flags;
	Ogre::uint8*		pBits;
	Ogre::uint32*		pZBuffer;
	Ogre::uint32		ViewType;
	float				ZoomFactor = 1;

	Ogre::Vector3 Vpn, Vright, Vup, CamPos;
	float	roll, pitch, yaw;
	GPlane		FrustPlanes[4];
	float	MaxScreenScaleInv, FieldOfView;
	float	XCenter = 310;
	float	YCenter = 174; 
	float	MaxScale;

	float	SpeedScale, YScreenScale, XScreenScale;
	long	Width = 310;
	long 	Height = 174;
	long		FacesDone;
	char Name[10];

} ViewVars;

class CL64_MapEditor
{
public:
	CL64_MapEditor();
	~CL64_MapEditor();

	void Start_Map_View_Dlg();

	ViewVars* VCam[4];
	ViewVars* Current_View;

	POINT		mStartPoint;

	bool flag_Map_Editor_Running;
	bool flag_Show_Areas;
	bool flag_Show_Camera;
	bool flag_Show_Colectables;
	bool flag_Show_Static_Objects;
	bool flag_Show_Dynamic_Objects;

	bool flag_Right_Button_Down;
	bool flag_Left_Button_Down;

	bool flag_IsDrawing;

private:

	static LRESULT CALLBACK Proc_Main_Dlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_Top_Left_Window(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_Top_Right_Window(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_Bottom_Left_Window(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Bottom_Right_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);


	void Init_Views();
	void Map_View_Main_Dlg();

	void Create_Top_Left_Window();
	void Create_Top_Right_Window();
	void Create_Bottom_Left_Window();
	void Create_Bottom_Right_Window();
	bool Resize_Windows(HWND hDlg, int NewWidth, int NewDepth);

	void Draw_Screen(HWND hwnd);
	bool Draw_Grid(HDC hDC, int Interval, RECT Rect);

	POINT m_Render_OrthoWorldToView(Ogre::Vector3 const* wp);

	void MeshData_Render_Faces(HDC ViewDC);
	void MeshData_Face_Groups(int Count, HDC ViewDC);
	void Render_ViewToWorld(const ViewVars* v, const int x, const int y, Ogre::Vector3* wp);
	void Draw_Camera(HDC ViewDC);

	void Zoom_View(HWND hDlg, int Dx, int Dy);
	void Pan_View(HWND hDlg, int Dx, int Dy);

	HPEN Pen_Fine_Grid;
	HPEN Pen_Grid;
	HPEN Pen_CutBrush;
	HPEN Pen_Camera;
	HPEN Pen_White;
	HPEN Pen_Colectables;
	HPEN Pen_Static_Objects;
	HPEN Pen_Dynamic_Objects;

	HWND Main_Dlg_Hwnd;

	HWND Left_Window_Hwnd;
	HWND Right_Window_Hwnd;
	HWND Bottom_Left_Hwnd;
	HWND Bottom_Right_Hwnd;

	HBRUSH BackGround_Brush;

	HMENU mMenu;

	int LEFT_WINDOW_WIDTH;

	int LEFT_WINDOW_DEPTH;
	int TOP_POS_BOTLEFT;

	int RIGHT_MINIMUM_SPACE;
	int LEFT_MINIMUM_SPACE;

	int nleftWnd_width;
	int nleftWnd_Depth;

	bool Do_Width;
	bool Do_Depth;
	bool Do_All;

	HDC	MemoryhDC;

};

