/*
Copyright (c) 2022 - 2023 GameDirectorSB and EquityME -- HGTInflanite Software W.T.Flanigan H.C.Flanigan

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/

#include "pch.h"
#include "CL64_App.h"
#include "Bass_Front_Dialog.h"

Bass_Front_Dialog::Bass_Front_Dialog()
{
	flag_Use_Front_Dlg_Flag = 1;
	flag_Show_Front_Dlg_Flag = 0;
	flag_Game_Running_Flag = 0;

	PosX = 500;
	PosY = 500;
}

Bass_Front_Dialog::~Bass_Front_Dialog()
{
}

// *************************************************************************
// *			Render_Front_Dlg:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void Bass_Front_Dialog::Render_Front_Dlg(void)
{

	ImGui::SetNextWindowPos(ImVec2(PosX, PosY));

	ImGuiWindowFlags window_flags = 0;

	//if (S_Message[0]->Show_BackGround == 1)
	{
		window_flags |= ImGuiWindowFlags_NoSavedSettings;
		window_flags |= ImGuiWindowFlags_NoResize;
		window_flags |= ImGuiWindowFlags_NoTitleBar;
		window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
	}
	/*else
	{
		window_flags |= ImGuiWindowFlags_NoSavedSettings;
		window_flags |= ImGuiWindowFlags_NoResize;
		window_flags |= ImGuiWindowFlags_NoTitleBar;
		window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
		window_flags |= ImGuiWindowFlags_NoBackground;
	}*/

	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(239, 239, 239, 255));

	if (!ImGui::Begin("Front_Dialog", &flag_Show_Front_Dlg_Flag, window_flags))
	{
		ImGui::End();
	}
	else
	{

		ImGui::PushFont(App->CL_ImGui->font2);

		ImGui::Spacing();
		ImGui::Spacing();

		if (flag_Game_Running_Flag == 0)
		{
			if (ImGui::Button("Start Game",ImVec2(220, 0)))
			{
				SetCapture(App->ViewGLhWnd);
				App->CL_Ogre->Ogre3D_Listener->flag_LeftMouseDown = 1;
				App->CUR = SetCursor(NULL);
				App->CL_Ogre->Ogre3D_Listener->flag_Block_Mouse = 0;
				App->CL_Keyboard->flag_Block_Keyboard = 0;

				App->CL_Physics->Reset_Physics();
				App->CL_Scene->Game_Restart();

				flag_Show_Front_Dlg_Flag = 0;

				flag_Game_Running_Flag = 1;
			}
		}

		if (flag_Game_Running_Flag == 1)
		{
			ImGui::Spacing();
			ImGui::Spacing();

			if (ImGui::Button("   Resume   ", ImVec2(220, 0)))
			{
				SetCursorPos(App->CL_Keyboard->Mouse_point.x, App->CL_Keyboard->Mouse_point.y);
				SetCapture(App->ViewGLhWnd);
				App->CL_Ogre->Ogre3D_Listener->flag_LeftMouseDown = 1;
				App->CUR = SetCursor(NULL);
				App->CL_Ogre->Ogre3D_Listener->flag_Block_Mouse = 0;
				App->CL_Keyboard->flag_Block_Keyboard = 0;
				flag_Show_Front_Dlg_Flag = 0;
				
			}
		}

		if (flag_Game_Running_Flag == 1)
		{
			ImGui::Spacing();
			ImGui::Spacing();

			if (ImGui::Button("    Restart    ",ImVec2(220, 0)))
			{
				SetCapture(App->ViewGLhWnd);
				App->CL_Ogre->Ogre3D_Listener->flag_LeftMouseDown = 1;
				App->CUR = SetCursor(NULL);
				App->CL_Ogre->Ogre3D_Listener->flag_Block_Mouse = 0;
				App->CL_Keyboard->flag_Block_Keyboard = 0;

				App->CL_Physics->Reset_Physics();
				App->CL_Scene->Game_Restart();

				flag_Show_Front_Dlg_Flag = 0;
			}
		}

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		
		if (ImGui::Button("      Quit      ",ImVec2(220,0)))
		{
			App->CL_Ogre->Ogre3D_Listener->flag_LeftMouseDown = 0;
			App->CL_Ogre->Ogre3D_Listener->flag_Block_Mouse = 0;
			App->CL_Keyboard->flag_Block_Keyboard = 0;
			App->flag_Block_Mouse_Buttons = 0;
			App->CL_Ogre->ExitFullScreen();
			flag_Game_Running_Flag = 0;
			flag_Show_Front_Dlg_Flag = 0;

		}

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::PopFont();

		ImVec2 Size = ImGui::GetWindowSize();
		PosX = ((float)App->CL_Ogre->Ogre3D_Listener->View_Width / 2) - (Size.x / 2);
		PosY = ((float)App->CL_Ogre->Ogre3D_Listener->View_Height / 2) - (Size.y / 2);

		ImGui::PopStyleColor();
		ImGui::End();

	}
}
