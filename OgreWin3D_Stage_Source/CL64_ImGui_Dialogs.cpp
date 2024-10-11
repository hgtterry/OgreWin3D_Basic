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

	if (App->CL_Ogre->FPStimer.getMilliseconds() > 3)
	{
		App->CL_Ogre->mRoot->_fireFrameStarted();
		App->CL_Ogre->mRoot->_updateAllRenderTargets();
		App->CL_Ogre->mRoot->_fireFrameEnded();
		App->CL_Ogre->FPStimer.reset();
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
