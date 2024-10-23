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
#include "CL64_ImGui_Dialogs.h"

CL64_ImGui_Dialogs::CL64_ImGui_Dialogs(void)
{
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
	Move_Ent_Editor_PosX = 10;
	Move_Ent_Editor_PosY = 10;
	Move_Ent_Index = 0;
	
}

CL64_ImGui_Dialogs::~CL64_ImGui_Dialogs(void)
{
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
void CL64_ImGui_Dialogs::Start_Dialog_Float(float Step, float StartValue, char* Banner)
{
	Float_Exit = 0;
	App->CL_ImGui_Dialogs->Float_Canceld = 0;
	App->CL_ImGui_Dialogs->Float_Step = Step;
	App->CL_ImGui_Dialogs->m_Dialog_Float = StartValue;
	strcpy(App->CL_ImGui_Dialogs->Float_Banner, Banner);

	m_Dialog_Float_Copy = StartValue;

	App->CL_Panels->Disable_Panels(true);

	Float_PosX = ((float)App->CL_Ogre->Ogre3D_Listener->View_Width / 2) - (200 / 2);
	Float_PosY = ((float)App->CL_Ogre->Ogre3D_Listener->View_Height / 2) - (130 / 2);

	Float_StartPos = 0;

	App->CL_ImGui_Dialogs->Show_Dialog_Float = 1;
}

// *************************************************************************
// *			Dialog_Float:- Terry and Hazel Flanigan 2024  			   *
// *************************************************************************
void CL64_ImGui_Dialogs::Dialog_Float(void)
{
	ImGui::SetNextWindowPos(ImVec2(Float_PosX, Float_PosY), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(200, 130), ImGuiCond_FirstUseEver);

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

		if (ImGui::Button("Close"))
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

	flag_Centre_X_Selected = App->CL_Scene->V_Object[Message_Index]->S_Message[0]->PosXCentre_Flag;
	flag_Centre_Y_Selected = App->CL_Scene->V_Object[Message_Index]->S_Message[0]->PosYCentre_Flag;

	Float_Colour = ImVec4(App->CL_Scene->V_Object[Message_Index]->S_Message[0]->Text_Colour.x / 255.0f,
		App->CL_Scene->V_Object[Message_Index]->S_Message[0]->Text_Colour.y / 255.0f,
		App->CL_Scene->V_Object[Message_Index]->S_Message[0]->Text_Colour.z / 255.0f,
		255);

	BackGround_color = ImVec4(App->CL_Scene->V_Object[Message_Index]->S_Message[0]->BackGround_Colour.x / 255.0f,
		App->CL_Scene->V_Object[Message_Index]->S_Message[0]->BackGround_Colour.y / 255.0f,
		App->CL_Scene->V_Object[Message_Index]->S_Message[0]->BackGround_Colour.z / 255.0f,
		255);

	App->CL_Scene->V_Object[Index]->Show_Message_Flag = 1;

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
		ImGui::InputFloat("X", &App->CL_Scene->V_Object[Message_Index]->S_Message[0]->Message_PosX, Float_Step, 0, "%.3f");

		ImGui::Checkbox("Centre X", &flag_Centre_X_Selected);

		if (flag_Centre_X_Selected)
		{
			App->CL_Scene->V_Object[Message_Index]->S_Message[0]->PosXCentre_Flag = 1;
		}
		else
		{
			App->CL_Scene->V_Object[Message_Index]->S_Message[0]->PosXCentre_Flag = 0;
		}

		ImGui::Separator();
		// ------------------------------------------------------------ - Pos Y
		ImGui::InputFloat("Y", &App->CL_Scene->V_Object[Message_Index]->S_Message[0]->Message_PosY, Float_Step, 0, "%.3f");

		ImGui::Checkbox("Centre Y", &flag_Centre_Y_Selected);

		if (flag_Centre_Y_Selected)
		{
			App->CL_Scene->V_Object[Message_Index]->S_Message[0]->PosYCentre_Flag = 1;
		}
		else
		{
			App->CL_Scene->V_Object[Message_Index]->S_Message[0]->PosYCentre_Flag = 0;
		}



		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Unindent();

		ImGuiColorEditFlags misc_flags = (ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Uint8);


		ImGui::ColorEdit3("Text##1", (float*)&Float_Colour, misc_flags);

		App->CL_Scene->V_Object[Message_Index]->S_Message[0]->Text_Colour.x = Float_Colour.x * 255.0f;
		App->CL_Scene->V_Object[Message_Index]->S_Message[0]->Text_Colour.y = Float_Colour.y * 255.0f;
		App->CL_Scene->V_Object[Message_Index]->S_Message[0]->Text_Colour.z = Float_Colour.z * 255.0f;

		ImGui::ColorEdit3("BG##1", (float*)&BackGround_color, misc_flags);

		App->CL_Scene->V_Object[Message_Index]->S_Message[0]->BackGround_Colour.x = BackGround_color.x * 255.0f;
		App->CL_Scene->V_Object[Message_Index]->S_Message[0]->BackGround_Colour.y = BackGround_color.y * 255.0f;
		App->CL_Scene->V_Object[Message_Index]->S_Message[0]->BackGround_Colour.z = BackGround_color.z * 255.0f;

		ImGui::Checkbox("Show Back Ground", &App->CL_Scene->V_Object[Message_Index]->S_Message[0]->Show_BackGround);

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

	Float_Exit = 0;
	flag_Move_Ent_Canceld = 0;

	Float_Step = 1;

	App->CL_Panels->Disable_Panels(true);
	App->CL_Panels->Show_FileView(false);
	App->CL_Panels->Show_Properties(false);

	Message_Editor_PosX = 10;
	Message_Editor_PosY = 10;
	flag_Move_Ent_Editor_StartPos = 0;
	Move_Ent_Index = Index;

	/*Centre_X_Selected = App->CL_Scene->V_Object[Message_Index]->S_Message[0]->PosXCentre_Flag;
	Centre_Y_Selected = App->CL_Scene->V_Object[Message_Index]->S_Message[0]->PosYCentre_Flag;

	Float_Colour = ImVec4(App->CL_Scene->V_Object[Message_Index]->S_Message[0]->Text_Colour.x / 255.0f,
		App->CL_Scene->V_Object[Message_Index]->S_Message[0]->Text_Colour.y / 255.0f,
		App->CL_Scene->V_Object[Message_Index]->S_Message[0]->Text_Colour.z / 255.0f,
		255);

	BackGround_color = ImVec4(App->CL_Scene->V_Object[Message_Index]->S_Message[0]->BackGround_Colour.x / 255.0f,
		App->CL_Scene->V_Object[Message_Index]->S_Message[0]->BackGround_Colour.y / 255.0f,
		App->CL_Scene->V_Object[Message_Index]->S_Message[0]->BackGround_Colour.z / 255.0f,
		255);*/

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

		//ImGui::Indent();
		ImGui::Spacing();

		// ------------------------------------------------------------- Pos X
		ImGui::InputFloat("Distance", &App->CL_Scene->V_Object[Move_Ent_Index]->S_MoveType[0]->Move_Distance, Float_Step, 0,"%.3f");

		/*ImGui::Checkbox("Centre X", &Centre_X_Selected);

		if (Centre_X_Selected)
		{
			App->CL_Scene->V_Object[Message_Index]->S_Message[0]->PosXCentre_Flag = 1;
		}
		else
		{
			App->CL_Scene->V_Object[Message_Index]->S_Message[0]->PosXCentre_Flag = 0;
		}*/

		ImGui::Separator();
		// ------------------------------------------------------------ - Pos Y
		ImGui::InputFloat("Speed", &App->CL_Scene->V_Object[Move_Ent_Index]->S_MoveType[0]->Speed, Float_Step, 0, "%.3f");

		/*ImGui::Checkbox("Centre Y", &Centre_Y_Selected);

		if (Centre_Y_Selected)
		{
			App->CL_Scene->V_Object[Message_Index]->S_Message[0]->PosYCentre_Flag = 1;
		}
		else
		{
			App->CL_Scene->V_Object[Message_Index]->S_Message[0]->PosYCentre_Flag = 0;
		}*/



		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		//ImGui::Unindent();

		//ImGuiColorEditFlags misc_flags = (ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Uint8);


		//ImGui::ColorEdit3("Text##1", (float*)&Float_Colour, misc_flags);

		/*App->CL_Scene->V_Object[Message_Index]->S_Message[0]->Text_Colour.x = Float_Colour.x * 255.0f;
		App->CL_Scene->V_Object[Message_Index]->S_Message[0]->Text_Colour.y = Float_Colour.y * 255.0f;
		App->CL_Scene->V_Object[Message_Index]->S_Message[0]->Text_Colour.z = Float_Colour.z * 255.0f;*/

		//ImGui::ColorEdit3("BG##1", (float*)&BackGround_color, misc_flags);

		/*App->CL_Scene->V_Object[Message_Index]->S_Message[0]->BackGround_Colour.x = BackGround_color.x * 255.0f;
		App->CL_Scene->V_Object[Message_Index]->S_Message[0]->BackGround_Colour.y = BackGround_color.y * 255.0f;
		App->CL_Scene->V_Object[Message_Index]->S_Message[0]->BackGround_Colour.z = BackGround_color.z * 255.0f;*/

		//ImGui::Checkbox("Show Back Ground", &App->CL_Scene->V_Object[Message_Index]->S_Message[0]->Show_BackGround);

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
			App->CL_Camera->Camera_Goto_Object(App->CL_Properties->Current_Selected_Object);
		}

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
