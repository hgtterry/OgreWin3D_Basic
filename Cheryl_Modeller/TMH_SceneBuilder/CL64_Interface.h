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
class CL64_Interface
{
public:
	CL64_Interface();
	~CL64_Interface();

	void Reset_All_Dialogs(void);

	void Move_FileView_Window(void);

	void Show_Properties_Panel(bool Show);

	bool Position_Properties_Dlg(void);
	void Position_Tabs_Dlg(void);

	void Position_Motions_Dlg(void);
	void Position_Face_Options_Dlg(void);
	
	bool Resize_FileView(void);
	void Unselect_Brush_And_Set_Dlgs(void);

	void Menu_Enable_OgreExport(bool option);

	void Show_file_view(bool show);

	void Show_Motions_Dlg(bool show);
	void Enable_Change_Textures_Button(bool option);


	void Set_Menu_Items_Startup();
	void Set_Menu_Items_Design();
	void Set_Menu_Items_Import();

	void Set_Title(bool Clear);

	void Show_Grid_Windows(bool Show);

	// Top Tabs brush panel options
	void Show_Faces_Panel_Control(bool show);
	void Enable_TopTabs_Faces_Buttons(bool option);
	void Enable_TopTabs_Buttons(bool option);

	void Select_Tab(int Tab_ID);
	void Hide_Tab_Dialogs();
	void Show_Brushes_Dialog(bool Show);
	void Show_Textures_Dialog(bool Show);
	void Show_Materials_Dialog(bool Show);
	void Show_TemplatesDialog(bool Show);

	bool flag_Tab_Texture;
	bool flag_Tab_Templates;
	bool flag_Tab_Group;
	bool flag_Dark_Mode;

	bool flag_Properties_Dlg_Active;
	bool Materials_Dlg_Active;
	bool Motions_Dlg_Active;
	bool Textures_Dlg_Assimp_Active;
	bool flag_Grids_Are_Visible;
	bool flag_Faces_Con_Dlg_Active;
};

