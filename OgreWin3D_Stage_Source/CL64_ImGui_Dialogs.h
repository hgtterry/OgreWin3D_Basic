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
class CL64_ImGui_Dialogs
{
public:

	CL64_ImGui_Dialogs(void);
	~CL64_ImGui_Dialogs(void);

	void Close_All_Dialogs(void);
	void BackGround_Render_Loop(void);

	void Start_Dialog_Float(float Step, int Combo_Step, float StartValue, char* Banner);
	void Dialog_Float(void);

	void Start_Dialog_MessageEditor(int Index);
	void Dialog_MessageEditor(void);

	void Start_Move_Entity_Editor(int Index);
	void Move_Entity_Editor(void);

	void Start_Dialog_List();
	void Dialog_List_Gui(void);

	void Physics_Console_Gui(void);

	bool flag_Disable_Panels;
	// List Dialog
	float List_PosX;
	float List_PosY;
	bool flag_Show_Dialog_list;
	bool flag_List_StartPos;
	bool flag_List_Canceled;
	char List_Banner[MAX_PATH];
	std::vector<std::string> List_Strings;
	int List_Count;
	int List_Index;
	int item_current_idx;

	// -------------- Float Dialog
	bool flag_Show_Dialog_Float;
	float Float_Step;
	bool flag_Float_Canceld;
	bool flag_Float_StartPos;
	float Float_PosX;
	float Float_PosY;
	float m_Dialog_Float;
	float m_Dialog_Float_Copy;
	char Float_Banner[MAX_PATH];
	bool flag_Float_Exit;
	bool flag_Float_Altetered;
	int Float_Combo_Step;

	// -------------- General
	ImVec4 Float_Colour;
	
	// -------------- Message Editor Dialog
	float Message_Editor_PosX;
	float Message_Editor_PosY;
	int Message_Index;
	ImVec4 BackGround_color;

	bool flag_Centre_X_Selected;
	bool flag_Centre_Y_Selected;
	bool flag_Show_Dialog_MessageEditor;
	bool flag_MessageEditor_Canceld;
	bool flag_Message_Editor_StartPos;
	

	// -------------- Move Entity Editor
	float Move_Ent_Editor_PosX;
	float Move_Ent_Editor_PosY;
	int Move_Ent_Index;

	std::vector<std::string> V_Move_Ent_Object_List;
	int Move_Ent_ObjectCount;
	int Move_Ent_item_current_idx;

	bool flag_Show_Move_Ent_Editor;
	bool flag_Move_Ent_Canceld;
	bool flag_Move_Ent_Editor_StartPos;
	bool flag_Move_Ent_PosX_Selected;
	bool flag_Move_Ent_PosY_Selected;
	bool flag_Move_Ent_PosZ_Selected;

	// -------------- Physics Console
	bool flag_Disable_Physics_Console;
	bool flag_Show_Physics_Console;
	float Physics_PosX;
	float Physics_PosY;
	bool flag_Physics_Console_StartPos;

private:


};

