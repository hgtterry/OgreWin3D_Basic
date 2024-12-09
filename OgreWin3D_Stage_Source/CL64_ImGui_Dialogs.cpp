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

#include "pch.h"
#include "CL64_App.h"
#include "resource.h"
#include "CL64_ImGui_Dialogs.h"

CL64_ImGui_Dialogs::CL64_ImGui_Dialogs(void)
{
	flag_Disable_Panels = 0;

	// List Dialog
	List_PosX = 0;
	List_PosY = 0;
	flag_Show_Dialog_list = 0;
	strcpy(List_Banner, "Banner");
	List_Count = 0;
	List_Index = 0;
	flag_List_StartPos = 0;
	flag_List_Canceled = 1;
	item_current_idx = 0;


	// -------------- Float Dialog
	Show_Dialog_Float = 0;
	Float_StartPos = 0;
	Float_PosX = 0;
	Float_PosY = 0;
	Float_Step = 0.05f;
	Float_Canceld = 0;
	Float_Exit = 0;
	strcpy(Float_Banner, "Banner");
	m_Dialog_Float_Copy = 0;
	m_Dialog_Float = 10.222;
	Float_Combo_Step = 0;

	// Mesage Editor
	flag_Centre_X_Selected = 0;
	flag_Centre_Y_Selected = 0;
	flag_MessageEditor_Canceld = 0;
	flag_Show_Dialog_MessageEditor = 0;
	flag_Message_Editor_StartPos = 0;
	Message_Editor_PosX = 10;
	Message_Editor_PosY = 10;
	Message_Index = 0;

	// -------------- Move Entity Editor
	flag_Show_Move_Ent_Editor = 0;
	flag_Move_Ent_Canceld = 0;
	flag_Move_Ent_Editor_StartPos = 0;
	flag_Move_Ent_PosX_Selected = 0;
	flag_Move_Ent_PosY_Selected = 0;
	flag_Move_Ent_PosZ_Selected = 0;
	Move_Ent_ObjectCount = 0;
	Move_Ent_Editor_PosX = 10;
	Move_Ent_Editor_PosY = 10;
	Move_Ent_Index = 0;
	Move_Ent_item_current_idx = 0;

	// -------------- Physics Console
	flag_Disable_Physics_Console = 0;
	Show_Physics_Console = 0;
	Physics_PosX = 500;
	Physics_PosY = 500;
	Physics_Console_StartPos = 0;

}

CL64_ImGui_Dialogs::~CL64_ImGui_Dialogs(void)
{
}

// *************************************************************************
// *			Close_All_Dialogs:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_ImGui_Dialogs::Close_All_Dialogs(void)
{
	Show_Dialog_Float = 0;
	flag_Show_Dialog_MessageEditor = 0;
	flag_Show_Move_Ent_Editor = 0;
	flag_Show_Dialog_list = 0;
	//Show_ColourPicker = 0;
	
}

// *************************************************************************
// *		BackGround_Render_Loop:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_ImGui_Dialogs::BackGround_Render_Loop(void)
{
	MSG  msg;
	while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (App->CL_Ogre->mWindow->isClosed()) return;

	if (App->CL_Ogre->FPStimer.getMilliseconds() > 6)
	{
		App->CL_Ogre->FPStimer.reset();

		App->CL_Ogre->mRoot->_fireFrameStarted();
		App->CL_Ogre->mRoot->_updateAllRenderTargets();
		App->CL_Ogre->mRoot->_fireFrameEnded();

	}
}

// *************************************************************************
// *		Start_Dialog_Float:- Terry and Hazel Flanigan 2024  		   *
// *************************************************************************
void CL64_ImGui_Dialogs::Start_Dialog_Float(float Step,int Combo_Step, float StartValue, char* Banner)
{
	Float_Exit = 0;
	Float_Canceld = 0;
	Float_Step = Step;
	Float_Combo_Step = Combo_Step;
	m_Dialog_Float = StartValue;
	strcpy(Float_Banner, Banner);

	m_Dialog_Float_Copy = StartValue;

	App->CL_Panels->Disable_Panels(true);

	Float_PosX = ((float)App->CL_Ogre->Ogre3D_Listener->View_Width / 2) - (200 / 2);
	Float_PosY = ((float)App->CL_Ogre->Ogre3D_Listener->View_Height / 2) - (130 / 2);

	Float_StartPos = 0;

	Show_Dialog_Float = 1;
}

// *************************************************************************
// *			Dialog_Float:- Terry and Hazel Flanigan 2024  			   *
// *************************************************************************
void CL64_ImGui_Dialogs::Dialog_Float(void)
{
	ImGui::SetNextWindowPos(ImVec2(Float_PosX, Float_PosY), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(210, 160), ImGuiCond_FirstUseEver);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(239, 239, 239, 255));

	if (!ImGui::Begin(Float_Banner, &Show_Dialog_Float, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize))
	{
		ImGui::End();
	}
	else
	{
		if (Float_StartPos == 0)
		{
			Float_PosX = ((float)App->CL_Ogre->Ogre3D_Listener->View_Width / 2) - (200 / 2);
			Float_PosY = ((float)App->CL_Ogre->Ogre3D_Listener->View_Height / 2) - (130 / 2);
			ImGui::SetWindowPos(Float_Banner, ImVec2(Float_PosX, Float_PosY));

			Float_StartPos = 1;
		}

		float spacingX = ImGui::GetStyle().ItemInnerSpacing.x;

		ImGui::Indent();
		ImGui::Spacing();

		ImGui::InputFloat("", &m_Dialog_Float, Float_Step, 0, "%.3f");

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::SetNextItemWidth(100);
		const char* XitemsPosXX[] = { "0.001","0.01","0.1","0.5","1", "2", "5", "10", "20"};
		bool ChangedPosX = ImGui::Combo("Step", &Float_Combo_Step, XitemsPosXX, IM_ARRAYSIZE(XitemsPosXX));
		if (ChangedPosX == 1)
		{
			Float_Step = (float)atof(XitemsPosXX[Float_Combo_Step]);
		}

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Indent();

		if (ImGui::Button("Apply"))
		{
			Float_Exit = 1;
			Show_Dialog_Float = 0;
			Float_StartPos = 0;
			Float_Canceld = 0;
			ImGui::PopStyleColor();
			ImGui::End();
		}

		ImGui::SameLine(0.0f, spacingX);

		if (ImGui::Button("Cancel"))
		{
			Float_StartPos = 0;
			Float_Exit = 1;
			Show_Dialog_Float = 0;
			Float_Canceld = 1;
			ImGui::PopStyleColor();
			ImGui::End();
		}

		if (Float_Exit == 0)
		{
			Float_Canceld = 1;
			ImGui::PopStyleColor();
			ImGui::End();
		}
	}
}

// *************************************************************************
// *	 Start_Dialog_MessageEditor:- Terry and Hazel Flanigan 2024  	   *
// *************************************************************************
void CL64_ImGui_Dialogs::Start_Dialog_MessageEditor(int Index)
{

	Float_Exit = 0;
	flag_MessageEditor_Canceld = 0;

	Float_Step = 1;

	App->CL_Panels->Disable_Panels(true);
	App->CL_Panels->Show_FileView(false);
	App->CL_Panels->Show_Properties(false);

	Message_Editor_PosX = 10;
	Message_Editor_PosY = 10;
	flag_Message_Editor_StartPos = 0;
	Message_Index = Index;

	flag_Centre_X_Selected = App->CL_Scene->B_Object[Message_Index]->S_Message[0]->PosXCentre_Flag;
	flag_Centre_Y_Selected = App->CL_Scene->B_Object[Message_Index]->S_Message[0]->PosYCentre_Flag;

	Float_Colour = ImVec4(App->CL_Scene->B_Object[Message_Index]->S_Message[0]->Text_Colour.x / 255.0f,
		App->CL_Scene->B_Object[Message_Index]->S_Message[0]->Text_Colour.y / 255.0f,
		App->CL_Scene->B_Object[Message_Index]->S_Message[0]->Text_Colour.z / 255.0f,
		255);

	BackGround_color = ImVec4(App->CL_Scene->B_Object[Message_Index]->S_Message[0]->BackGround_Colour.x / 255.0f,
		App->CL_Scene->B_Object[Message_Index]->S_Message[0]->BackGround_Colour.y / 255.0f,
		App->CL_Scene->B_Object[Message_Index]->S_Message[0]->BackGround_Colour.z / 255.0f,
		255);

	App->CL_Scene->B_Object[Index]->Show_Message_Flag = 1;

	flag_Show_Dialog_MessageEditor = 1;
}

// *************************************************************************
// *		Dialog_MessageEditor:- Terry and Hazel Flanigan 2024  		   *
// *************************************************************************
void CL64_ImGui_Dialogs::Dialog_MessageEditor(void)
{

	ImGui::SetNextWindowPos(ImVec2(Message_Editor_PosX, Message_Editor_PosY), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(200, 340), ImGuiCond_FirstUseEver);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(239, 239, 239, 255));

	if (!ImGui::Begin("Message Editor", &flag_Show_Dialog_MessageEditor, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize))
	{
		ImGui::End();
	}
	else
	{
		if (flag_Message_Editor_StartPos == 0)
		{
			ImVec2 Size = ImGui::GetWindowSize();
			Message_Editor_PosX = (((float)App->CL_Ogre->Ogre3D_Listener->View_Width - Size.x) - 10);
			Message_Editor_PosY = 10;

			ImGui::SetWindowPos("Message Editor", ImVec2(Message_Editor_PosX, Message_Editor_PosY));

			flag_Message_Editor_StartPos = 1;
		}

		float spacingX = ImGui::GetStyle().ItemInnerSpacing.x;

		ImGui::Indent();
		ImGui::Spacing();

		// ------------------------------------------------------------- Pos X
		ImGui::InputFloat("X", &App->CL_Scene->B_Object[Message_Index]->S_Message[0]->Message_PosX, Float_Step, 0, "%.3f");

		ImGui::Checkbox("Centre X", &flag_Centre_X_Selected);

		if (flag_Centre_X_Selected)
		{
			App->CL_Scene->B_Object[Message_Index]->S_Message[0]->PosXCentre_Flag = 1;
		}
		else
		{
			App->CL_Scene->B_Object[Message_Index]->S_Message[0]->PosXCentre_Flag = 0;
		}

		ImGui::Separator();
		// ------------------------------------------------------------ - Pos Y
		ImGui::InputFloat("Y", &App->CL_Scene->B_Object[Message_Index]->S_Message[0]->Message_PosY, Float_Step, 0, "%.3f");

		ImGui::Checkbox("Centre Y", &flag_Centre_Y_Selected);

		if (flag_Centre_Y_Selected)
		{
			App->CL_Scene->B_Object[Message_Index]->S_Message[0]->PosYCentre_Flag = 1;
		}
		else
		{
			App->CL_Scene->B_Object[Message_Index]->S_Message[0]->PosYCentre_Flag = 0;
		}



		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Unindent();

		ImGuiColorEditFlags misc_flags = (ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Uint8);


		ImGui::ColorEdit3("Text##1", (float*)&Float_Colour, misc_flags);

		App->CL_Scene->B_Object[Message_Index]->S_Message[0]->Text_Colour.x = Float_Colour.x * 255.0f;
		App->CL_Scene->B_Object[Message_Index]->S_Message[0]->Text_Colour.y = Float_Colour.y * 255.0f;
		App->CL_Scene->B_Object[Message_Index]->S_Message[0]->Text_Colour.z = Float_Colour.z * 255.0f;

		ImGui::ColorEdit3("BG##1", (float*)&BackGround_color, misc_flags);

		App->CL_Scene->B_Object[Message_Index]->S_Message[0]->BackGround_Colour.x = BackGround_color.x * 255.0f;
		App->CL_Scene->B_Object[Message_Index]->S_Message[0]->BackGround_Colour.y = BackGround_color.y * 255.0f;
		App->CL_Scene->B_Object[Message_Index]->S_Message[0]->BackGround_Colour.z = BackGround_color.z * 255.0f;

		ImGui::Checkbox("Show Back Ground", &App->CL_Scene->B_Object[Message_Index]->S_Message[0]->Show_BackGround);

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Indent();
		ImGui::Indent();
		ImGui::Indent();

		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Spacing();

		if (ImGui::Button("Close"))
		{
			Float_Exit = 1;
			flag_Show_Dialog_MessageEditor = 0;
			flag_MessageEditor_Canceld = 0;
			ImGui::PopStyleColor();
			ImGui::End();
		}

		//ImGui::SameLine(0.0f, spacingX);

		/*if (ImGui::Button("Close"))
		{
			Float_Exit = 1;
			Show_Dialog_MessageEditor = 0;
			MessageEditor_Canceld = 1;
			ImGui::End();
		}*/

		if (Float_Exit == 0)
		{
			flag_MessageEditor_Canceld = 1;
			ImGui::PopStyleColor();
			ImGui::End();
		}
	}
}

// *************************************************************************
// *	 Start_Move_Entity_Editor:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_ImGui_Dialogs::Start_Move_Entity_Editor(int Index)
{
	Move_Ent_Index = Index;
	Float_Exit = 0;
	flag_Move_Ent_Canceld = 0;

	Float_Step = 1;

	App->CL_Panels->Disable_Panels(true);
	App->CL_Panels->Show_FileView(false);
	App->CL_Panels->Show_Properties(false);

	if (App->CL_Scene->B_Object[Move_Ent_Index]->S_MoveType[0]->WhatDirection == Enums::Axis_x)
	{
		flag_Move_Ent_PosX_Selected = 1;
		flag_Move_Ent_PosY_Selected = 0;
		flag_Move_Ent_PosZ_Selected = 0;
	}

	if (App->CL_Scene->B_Object[Move_Ent_Index]->S_MoveType[0]->WhatDirection == Enums::Axis_y)
	{
		flag_Move_Ent_PosX_Selected = 0;
		flag_Move_Ent_PosY_Selected = 1;
		flag_Move_Ent_PosZ_Selected = 0;
	}

	if (App->CL_Scene->B_Object[Move_Ent_Index]->S_MoveType[0]->WhatDirection == Enums::Axis_z)
	{
		flag_Move_Ent_PosX_Selected = 0;
		flag_Move_Ent_PosY_Selected = 0;
		flag_Move_Ent_PosZ_Selected = 1;
	}

	V_Move_Ent_Object_List.resize(0);
	V_Move_Ent_Object_List.reserve(50);

	int Count = 0;
	Move_Ent_ObjectCount = 0;
	int Total = App->CL_Scene->Object_Count;

	while (Count < Total)
	{

		if (App->CL_Scene->B_Object[Count]->Usage == Enums::Stage_Usage_Static)
		{
			bool test = strcmp(App->CL_Scene->B_Object[Index]->S_MoveType[0]->Object_Name, App->CL_Scene->B_Object[Count]->Object_Name);
			if (test == 0)
			{
				Move_Ent_item_current_idx = Move_Ent_ObjectCount;
			}

			V_Move_Ent_Object_List.push_back(App->CL_Scene->B_Object[Count]->Object_Name);
			Move_Ent_ObjectCount++;
		}

		Count++;
	}

	App->CL_Scene->B_Object[Index]->S_MoveType[0]->Object_Name;

	Message_Editor_PosX = 10;
	Message_Editor_PosY = 10;
	flag_Move_Ent_Editor_StartPos = 0;
	
	flag_Show_Move_Ent_Editor = 1;
}

// *************************************************************************
// *		Move_Entity_Editor:- Terry and Hazel Flanigan 2024  		   *
// *************************************************************************
void CL64_ImGui_Dialogs::Move_Entity_Editor(void)
{

	ImGui::SetNextWindowPos(ImVec2(Message_Editor_PosX, Message_Editor_PosY), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(250, 340), ImGuiCond_FirstUseEver);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(239, 239, 239, 255));

	if (!ImGui::Begin("Move Entity Editor", &flag_Show_Move_Ent_Editor, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize))
	{
		ImGui::End();
	}
	else
	{
		if (flag_Move_Ent_Editor_StartPos == 0)
		{
			ImVec2 Size = ImGui::GetWindowSize();
			Move_Ent_Editor_PosX = (((float)App->CL_Ogre->Ogre3D_Listener->View_Width - Size.x) - 10);
			Move_Ent_Editor_PosY = 10;

			ImGui::SetWindowPos("Move Entity Editor", ImVec2(Move_Ent_Editor_PosX, Move_Ent_Editor_PosY));

			flag_Move_Ent_Editor_StartPos = 1;
		}

		float spacingX = ImGui::GetStyle().ItemInnerSpacing.x;

		ImGui::Indent();

		static ImGuiComboFlags flags = 0;
		//const char* combo_preview_value = items[item_current_idx];  // Pass in the preview value visible before opening the combo (it could be anything)
		if (ImGui::BeginCombo("Object", V_Move_Ent_Object_List[Move_Ent_item_current_idx].c_str(), flags))
		{
			for (int n = 0; n < Move_Ent_ObjectCount; n++)
			{
				const bool is_selected = (Move_Ent_item_current_idx == n);
				if (ImGui::Selectable(V_Move_Ent_Object_List[n].c_str(), is_selected))
				{
					Move_Ent_item_current_idx = n;
					strcpy(App->CL_Scene->B_Object[Move_Ent_Index]->S_MoveType[0]->Object_Name, V_Move_Ent_Object_List[n].c_str());
					int MoveObjectIndex = App->CL_Com_Objects->GetIndex_By_Name(App->CL_Scene->B_Object[Move_Ent_Index]->S_MoveType[0]->Object_Name);
					App->CL_Scene->B_Object[Move_Ent_Index]->S_MoveType[0]->Object_To_Move_Index = MoveObjectIndex;
					App->Flash_Window();
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}

			ImGui::EndCombo();
		}

		ImGui::Unindent();
		ImGui::Separator();
		//ImGui::Indent();
		ImGui::Spacing();

		// ------------------------------------------------------------- Pos X
		ImGui::InputFloat("Distance", &App->CL_Scene->B_Object[Move_Ent_Index]->S_MoveType[0]->Move_Distance, Float_Step, 0,"%.3f");

		ImGui::Separator();
		// ------------------------------------------------------------ - Pos Y
		ImGui::InputFloat("Speed", &App->CL_Scene->B_Object[Move_Ent_Index]->S_MoveType[0]->Speed, Float_Step, 0, "%.3f");


		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Indent();
		ImGui::Indent();

		ImGui::Text("         Axis");

		ImGui::Checkbox("X", &flag_Move_Ent_PosX_Selected);
		
		if (flag_Move_Ent_PosX_Selected == 1)
		{
			flag_Move_Ent_PosY_Selected = 0;
			flag_Move_Ent_PosZ_Selected = 0;

			App->CL_Scene->B_Object[Move_Ent_Index]->S_MoveType[0]->WhatDirection = Enums::Axis_x;
		}

		ImGui::SameLine(0.0f, spacingX);

		ImGui::Checkbox("Y", &flag_Move_Ent_PosY_Selected);
		
		if (flag_Move_Ent_PosY_Selected == 1)
		{
			flag_Move_Ent_PosX_Selected = 0;
			flag_Move_Ent_PosZ_Selected = 0;

			App->CL_Scene->B_Object[Move_Ent_Index]->S_MoveType[0]->WhatDirection = Enums::Axis_y;
		}

		ImGui::SameLine(0.0f, spacingX);

		ImGui::Checkbox("Z", &flag_Move_Ent_PosZ_Selected);

		if (flag_Move_Ent_PosZ_Selected == 1)
		{
			flag_Move_Ent_PosX_Selected = 0;
			flag_Move_Ent_PosY_Selected = 0;

			App->CL_Scene->B_Object[Move_Ent_Index]->S_MoveType[0]->WhatDirection = Enums::Axis_z;
		}

		ImGui::Unindent();
		ImGui::Unindent();

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		/*ImGui::Indent();
		ImGui::Indent();
		ImGui::Indent();*/

		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Spacing();


		if (ImGui::Button("Test"))
		{
			App->CL_Com_MoveEntity->Test_Move_Entity(App->CL_Properties->Current_Selected_Object);
		}


		ImGui::SameLine(0.0f, spacingX);

		if (ImGui::Button("Reset"))
		{
			App->CL_Com_MoveEntity->Reset_Move_Entity(App->CL_Properties->Current_Selected_Object);
		}

		ImGui::SameLine(0.0f, spacingX);

		if (ImGui::Button("Goto"))
		{
			App->CL_Com_Cameras->Camera_Goto_Object(App->CL_Properties->Current_Selected_Object);
		}

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		if (ImGui::Button("Close"))
		{
			Float_Exit = 1;
			flag_Show_Move_Ent_Editor = 0;
			flag_Move_Ent_Canceld = 0;
			ImGui::PopStyleColor();
			ImGui::End();
		}

		if (Float_Exit == 0)
		{
			flag_Move_Ent_Canceld = 1;
			ImGui::PopStyleColor();
			ImGui::End();
		}
	}
}

// *************************************************************************
// *		Start_Dialog_List:- Terry and Hazel Flanigan 2024  			   *
// *************************************************************************
void CL64_ImGui_Dialogs::Start_Dialog_List()
{
	flag_List_Canceled = 1; // set to canceled 
	
	App->CL_Panels->Disable_Panels(true);

	List_PosX = ((float)App->CL_Ogre->Ogre3D_Listener->View_Width / 2) - (400 / 2);
	List_PosY = ((float)App->CL_Ogre->Ogre3D_Listener->View_Height / 2) - (330 / 2);

	flag_List_StartPos = 0;

	App->CL_ImGui_Dialogs->flag_Show_Dialog_list = 1;
}

// *************************************************************************
// *			Dialog_List_Gui:- Terry and Hazel Flanigan 2024  		   *
// *************************************************************************
void CL64_ImGui_Dialogs::Dialog_List_Gui(void)
{
	ImGui::SetNextWindowPos(ImVec2(List_PosX, List_PosY), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(400, 330), ImGuiCond_FirstUseEver);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(239, 239, 239, 255));

	if (!ImGui::Begin(List_Banner, &flag_Show_Dialog_list, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize))
	{
		ImGui::End();
	}
	else
	{
		if (flag_List_StartPos == 0)
		{
			List_PosX = ((float)App->CL_Ogre->Ogre3D_Listener->View_Width / 2) - (400 / 2);
			List_PosY = ((float)App->CL_Ogre->Ogre3D_Listener->View_Height / 2) - (330 / 2);
			ImGui::SetWindowPos(List_Banner, ImVec2(List_PosX, List_PosY));

			flag_List_StartPos = 1;
		}

		float spacingX = ImGui::GetStyle().ItemInnerSpacing.x;

		ImGui::Spacing();

		ImGui::Text("%s", List_Strings[List_Index].c_str());

		item_current_idx = List_Index;

		if (ImGui::BeginListBox("##listbox 2", ImVec2(-FLT_MIN, 8 * ImGui::GetTextLineHeightWithSpacing())))
		{
			for (int n = 0; n < List_Count; n++)
			{
				const bool is_selected = (item_current_idx == n);
				if (ImGui::Selectable(List_Strings[n].c_str(), is_selected))
				{
					item_current_idx = n;
					List_Index = n;
				}
			
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}

			}

			ImGui::EndListBox();
		}


		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Indent();

		ImGui::SameLine(0.0f, spacingX);

		if (ImGui::Button("Ok"))
		{
			flag_List_StartPos = 0;
			flag_List_Canceled = 0;
			flag_Show_Dialog_list = 0;

			ImGui::PopStyleColor();
			ImGui::End();
		}

		if (Float_Exit == 0)
		{
			Float_Canceld = 1;
			ImGui::PopStyleColor();
			ImGui::End();
		}
	}
}

// *************************************************************************
// *			Physics_Console:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_ImGui_Dialogs::Physics_Console_Gui(void)
{
	ImGui::SetNextWindowPos(ImVec2(Physics_PosX, Physics_PosY), ImGuiCond_FirstUseEver);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(239, 239, 239, 255));
	ImGuiStyle* style = &ImGui::GetStyle();
	
	if (!ImGui::Begin("Physics_Console", &Show_Physics_Console, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar))
	{
		ImGui::End();
	}
	else
	{
		if (flag_Disable_Physics_Console == 1)
		{
			ImGui::BeginDisabled(true);
		}

		ImGui::Text("Physics Console");

		ImGui::SameLine(0, 270);
		if (ImGui::Button("H"))
		{
			//App->Cl_Utilities->OpenHTML("Help\\Physics_Console.html");
		}

		ImGui::SameLine();
		if (ImGui::Button("X"))
		{
			CheckMenuItem(App->mMenu, ID_WINDOWS_PHYSICSCONSOLE, MF_BYCOMMAND | MF_UNCHECKED);
			Physics_Console_StartPos = 0;
			Show_Physics_Console = 0;
		}

		ImGui::Separator();

		if (App->CL_Ogre->Ogre3D_Listener->flag_Run_Physics == 1)
		{
			style->Colors[ImGuiCol_Button] = ImVec4(0.0f, 1.0f, 0.0f, 1.00f);
		}
		else
		{
			style->Colors[ImGuiCol_Button] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
		}

		if (ImGui::Button("Physics On"))
		{
			if (App->CL_Scene->flag_Scene_Loaded == 1)
			{
				if (App->CL_Ogre->Ogre3D_Listener->flag_Run_Physics == 1)
				{
					App->CL_Ogre->Ogre3D_Listener->flag_Run_Physics = 0;
				}
				else
				{
					App->CL_Ogre->Ogre3D_Listener->flag_Run_Physics = 1;
				}

			}
		}

		if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal | ImGuiHoveredFlags_NoSharedDelay))
		{
			ImGui::SetTooltip("Turn Physics \n On and Off");
		}

		style->Colors[ImGuiCol_Button] = ImVec4(1, 1, 0.58, 1); // Yellow


		ImGui::SameLine();
		if (ImGui::Button("Reset Physics"))
		{
			if (App->CL_Scene->flag_Scene_Loaded == 1)
			{
				App->CL_Physics->Reset_Physics();
			}
		}

		if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal | ImGuiHoveredFlags_NoSharedDelay))
		{
			ImGui::SetTooltip("Resets Phyics and Turns Physics Off \nTurn Physics back on with Physics On Button");
		}

		ImGui::SameLine();
		if (ImGui::Button("Reset Entities"))
		{
			if (App->CL_Scene->flag_Scene_Loaded == 1)
			{
				App->CL_Physics->Reset_Triggers();
			}
		}

		ImGui::SameLine();
		if (ImGui::Button("Reset Scene"))
		{
			if (App->CL_Scene->flag_Scene_Loaded == 1)
			{
				App->CL_Physics->Reset_Scene();
			}
		}

		if (Physics_Console_StartPos == 0)
		{
			ImVec2 Size = ImGui::GetWindowSize();
			Physics_PosX = 10;
			Physics_PosY = ((float)App->CL_Ogre->Ogre3D_Listener->View_Height) - (Size.y) - 10;
			ImGui::SetWindowPos("Physics_Console", ImVec2(Physics_PosX, Physics_PosY));

			Physics_Console_StartPos = 1;
		}

		style->Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
		ImGui::PopStyleColor();


		if (flag_Disable_Physics_Console == 1)
		{
			ImGui::EndDisabled();
		}

		ImGui::End();
	}
}

