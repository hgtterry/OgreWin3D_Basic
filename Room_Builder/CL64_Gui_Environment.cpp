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
#include "CL64_Gui_Environment.h"

CL64_Gui_Environment::CL64_Gui_Environment(void)
{
	PropertyEditor_Page = 0;

	Ambient_Int_Red = 0;
	Ambient_Int_Green = 0;
	Ambient_Int_Blue = 0;

	Fog_Colour_Int_Red = 0;
	Fog_Colour_Int_Green = 0;
	Fog_Colour_Int_Blue = 0;

	Float_PosX = 0;
	Float_PosY = 0;
	
	Eviron_Index = 0;

	flag_Show_PropertyEditor = 0;

	flag_Float_StartPos = 0;
	flag_Float_Exit = 0;

	flag_Is_Teleport = 0;

	flag_ClickOnTrack = 0;
	flag_ClickOnVolume = 0;
	flag_ClickOnPlay = 0;
	flag_ClickOnLoop = 0;

	flag_ClickOnFogVisible = 0;
	flag_ClickOnFogMode = 0;
	flag_ClickOnFogColour = 0;
	flag_ClickOnFogStart = 0;
	flag_ClickOnFogEnd = 0;

	flag_ClickOnSkyEnabled = 0;
	flag_ClickOnSkyTiling = 0;
	flag_ClickOnSkyCurve = 0;

}

CL64_Gui_Environment::~CL64_Gui_Environment(void)
{
}

// *************************************************************************
// *		 Reset_Evironment:- Terry and Hazel Flanigan 2023  			   *
// *************************************************************************
void CL64_Gui_Environment::Reset_Class()
{
	//if (App->CL_Scene->flag_Scene_Loaded == 1)
	{
		App->CL_Ogre->mSceneMgr->setSkyDome(false, "Examples/CloudySky"); // Look At Terry

		PropertyEditor_Page = 0;

		Ambient_Int_Red = 0;
		Ambient_Int_Green = 0;
		Ambient_Int_Blue = 0;

		Fog_Colour_Int_Red = 0;
		Fog_Colour_Int_Green = 0;
		Fog_Colour_Int_Blue = 0;

		Float_PosX = 0;
		Float_PosY = 0;
		
		Eviron_Index = 0;

		flag_Show_PropertyEditor = 0;
		flag_Float_StartPos = 0;

		flag_Is_Teleport = 0;

		flag_ClickOnTrack = 0;
		flag_ClickOnVolume = 0;
		flag_ClickOnPlay = 0;
		flag_ClickOnLoop = 0;

		flag_ClickOnFogVisible = 0;
		flag_ClickOnFogMode = 0;
		flag_ClickOnFogColour = 0;
		flag_ClickOnFogStart = 0;
		flag_ClickOnFogEnd = 0;

		flag_ClickOnSkyEnabled = 0;
		flag_ClickOnSkyTiling = 0;
		flag_ClickOnSkyCurve = 0;
	}
}

// *************************************************************************
// *	 Start_Environment_Editor:- Terry and Hazel Flanigan 2023  		   *
// *************************************************************************
void CL64_Gui_Environment::Start_Environment_Editor(int index, bool isTeleport) {
	Eviron_Index = index;
	flag_Is_Teleport = isTeleport;
	flag_Float_Exit = 0;

	auto& environment = App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0];

	Ambient_Colour_Copy.x = environment->AmbientColour.x;
	Ambient_Colour_Copy.y = environment->AmbientColour.y;
	Ambient_Colour_Copy.z = environment->AmbientColour.z;

	Ambient_Colour.x = environment->AmbientColour.x;
	Ambient_Colour.y = environment->AmbientColour.y;
	Ambient_Colour.z = environment->AmbientColour.z;

	Ambient_Int_Red = static_cast<int>(Ambient_Colour.x * 255);
	Ambient_Int_Green = static_cast<int>(Ambient_Colour.y * 255);
	Ambient_Int_Blue = static_cast<int>(Ambient_Colour.z * 255);

	Fog_Colour_Copy.x = environment->Fog_Colour.x;
	Fog_Colour_Copy.y = environment->Fog_Colour.y;
	Fog_Colour_Copy.z = environment->Fog_Colour.z;

	Fog_Colour.x = environment->Fog_Colour.x;
	Fog_Colour.y = environment->Fog_Colour.x;
	Fog_Colour.z = environment->Fog_Colour.z;

	Fog_Colour_Int_Red = static_cast<int>(Fog_Colour.x * 255);
	Fog_Colour_Int_Green = static_cast<int>(Fog_Colour.y * 255);
	Fog_Colour_Int_Blue = static_cast<int>(Fog_Colour.z * 255);

	flag_ClickOnFogVisible = environment->Fog_On;
	flag_ClickOnSkyEnabled = environment->flag_Enabled;
	flag_ClickOnPlay = environment->flag_Play;
	flag_ClickOnLoop = environment->flag_Loop;

	App->CL_FileView->Show_FileView(false);
	App->CL_Panels->Enable_Scene_Editor_Dialogs(false);

	App->CL_SoundMgr->SoundEngine->stopAllSounds();

	App->CL_Com_Environments->Set_Environment_By_Index(true, Eviron_Index);

	flag_Show_PropertyEditor = 1;
}

// *************************************************************************
// *		Environ_PropertyEditor:- Terry and Hazel Flanigan 2022		   *
// *************************************************************************
void CL64_Gui_Environment::Environ_PropertyEditor()
{

	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(350, 220), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Environment Editor", &flag_Show_PropertyEditor, ImGuiWindowFlags_NoResize| ImGuiWindowFlags_NoTitleBar))
	{
		ImGui::End();
		return;
	}

	ImGuiColorEditFlags misc_flags2 = (ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Uint8);

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(213, 222, 242, 255));
	
	ImGui::Text("Environment:  %s", App->CL_Scene->B_Object[Eviron_Index]->Object_Name);
	ImGui::Separator();

	ImGui::Columns(2);
	ImGui::SetColumnWidth(-1, 120);

	if (ImGui::Button(" Main Light ",ImVec2(100, 0)))
	{
		PropertyEditor_Page = 0;
	}

	if (ImGui::Button("Sound   ", ImVec2(100, 0)))
	{
		PropertyEditor_Page = 1;
	}

	if (ImGui::Button("Fog   ", ImVec2(100, 0)))
	{
		PropertyEditor_Page = 2;
	}

	if (ImGui::Button("Sky   ", ImVec2(100, 0)))
	{
		PropertyEditor_Page = 3;
	}

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	
	// ---------------------------------------------------------------- Main Light
	if (PropertyEditor_Page == 0)
	{
		ImGui::NextColumn();
		ImGui::AlignTextToFramePadding();

		ImGui::Text("Ambient Colour:");
		ImGui::SameLine();

		if (ImGui::ColorEdit3("", (float*)&Ambient_Colour, ImGuiColorEditFlags_NoInputs | misc_flags2))
		{
			Ambient_Int_Red = Ambient_Colour.x * 255;
			Ambient_Int_Green = Ambient_Colour.y * 255;
			Ambient_Int_Blue = Ambient_Colour.z * 255;

			App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->AmbientColour = Ogre::Vector3(Ambient_Colour.x, Ambient_Colour.y, Ambient_Colour.z);
			App->CL_Ogre->mSceneMgr->setAmbientLight(ColourValue(Ambient_Colour.x, Ambient_Colour.y, Ambient_Colour.z));

			App->CL_Com_Environments->Mark_As_Altered_Environ(Eviron_Index);
		}

	}

	// ---------------------------------------------------------------- Sound
	if (PropertyEditor_Page == 1)
	{
		ImGui::NextColumn();
		ImGui::AlignTextToFramePadding();
		
		ImGui::Selectable("Track:- ", &flag_ClickOnTrack);
		ImGui::SameLine();
		ImGui::Text("%s", App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->Sound_File);

		if (flag_ClickOnTrack)
		{
			ImGui::TextColored(ImVec4(0.f, 1.f, 0.24f, 1.f), "ON");

			App->CL_SoundMgr->flag_Accessed = 1;
			strcpy(App->CL_SoundMgr->Access_File, App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->Sound_File);

			App->CL_Com_Environments->Set_Environment_By_Index(0, Eviron_Index);
			App->CL_SoundMgr->Show_Sound_Player();

			if (App->CL_SoundMgr->flag_IsCancelled == 0)
			{

				strcpy(App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->Sound_File, App->CL_SoundMgr->Access_File);
				App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->SndVolume = App->CL_SoundMgr->SndVolume;

				App->CL_Com_Environments->Set_Environment_By_Index(1, Eviron_Index);

				App->CL_Com_Environments->Mark_As_Altered_Environ(Eviron_Index);
			}
			else
			{
				App->CL_Com_Environments->Set_Environment_By_Index(1, Eviron_Index);
			}

			flag_ClickOnTrack = 0;
		}
		
		// ----------------- Volume
		ImGui::Selectable("Volume:- ", &flag_ClickOnVolume);
		ImGui::SameLine();
		ImGui::Text("%f", App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->SndVolume);
		if (flag_ClickOnVolume)
		{
			App->CL_Com_Environments->Mark_As_Altered_Environ(Eviron_Index);
			flag_ClickOnVolume = 0;
		}

		// ----------------- Play
		ImGui::Text("Play:");
		ImGui::SameLine();
		int test = ImGui::Checkbox("##Play", &flag_ClickOnPlay);
		if (test == 1)
		{
			if (flag_ClickOnPlay == 1)
			{
				App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->flag_Play = 1;
				App->CL_Com_Environments->Set_Environment_By_Index(1, Eviron_Index);
			}
			else
			{
				App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->flag_Play = 0;
				App->CL_Com_Environments->Set_Environment_By_Index(0, Eviron_Index);
			}

			App->CL_Com_Environments->Mark_As_Altered_Environ(Eviron_Index);
		}

		// ----------------- Loop
		ImGui::Text("Loop:");
		ImGui::SameLine();
		test = ImGui::Checkbox("##Loop", &flag_ClickOnLoop);
		if (test == 1)
		{
			if (flag_ClickOnLoop == 1)
			{
				App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->flag_Loop = 1;
				App->CL_Com_Environments->Set_Environment_By_Index(0, Eviron_Index);
				App->CL_Com_Environments->Set_Environment_By_Index(1, Eviron_Index);
			}
			else
			{
				App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->flag_Loop = 0;
				App->CL_Com_Environments->Set_Environment_By_Index(0, Eviron_Index);
				App->CL_Com_Environments->Set_Environment_By_Index(1, Eviron_Index);
			}

			App->CL_Com_Environments->Mark_As_Altered_Environ(Eviron_Index);
		}
	}

	// ---------------------------------------------------------------- Fog
	if (PropertyEditor_Page == 2)
	{
		ImGui::NextColumn();
		ImGui::AlignTextToFramePadding();

		// ----------------- Visible
		ImGui::Text("Visible");
		ImGui::SameLine();
		int test = ImGui::Checkbox("##Visible", &flag_ClickOnFogVisible);
		if (test == 1)
		{
			if (flag_ClickOnFogVisible == 1)
			{
				App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->Fog_On = 1;
				EnableFog(true);
			}
			else
			{
				App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->Fog_On = 0;
				EnableFog(false);
			}

			App->CL_Com_Environments->Mark_As_Altered_Environ(Eviron_Index);
		}
			
		// ----------------- Mode
		ImGui::Selectable("Mode:- ", &flag_ClickOnFogMode);
		ImGui::SameLine();
		ImGui::Text("%i", App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->Fog_Mode);
		if (flag_ClickOnFogMode)
		{
			flag_ClickOnFogMode = 0;
		}

		// ----------------- Fog Colour
		ImGui::Text("Fog Colour:");
		ImGui::SameLine();

		if (ImGui::ColorEdit3("", (float*)&Fog_Colour, ImGuiColorEditFlags_NoInputs | misc_flags2))
		{
			Ambient_Int_Red = Fog_Colour.x * 255;
			Ambient_Int_Green = Fog_Colour.y * 255;
			Ambient_Int_Blue = Fog_Colour.z * 255;

			App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->Fog_Colour = Ogre::Vector3(Fog_Colour.x, Fog_Colour.y, Fog_Colour.z);

			if (App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->Fog_On == 1)
			{
				EnableFog(true);
			}

			App->CL_Com_Environments->Mark_As_Altered_Environ(Eviron_Index);
		}



		// ----------------- Start
		ImGui::Text("Start:- ", App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->Fog_Start);
		ImGui::SameLine();
		
		if (ImGui::InputFloat("##1", &App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->Fog_Start, 1, 0, "%.3f"))
		{
			if (App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->Fog_On == 1)
			{
				EnableFog(true);
			}

			App->CL_Com_Environments->Mark_As_Altered_Environ(Eviron_Index);
		}

		// ----------------- End
		ImGui::Text("End:-   ", App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->Fog_End);
		ImGui::SameLine();

		if (ImGui::InputFloat("##2", &App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->Fog_End, 1, 0, "%.3f"))
		{
			if (App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->Fog_On == 1)
			{
				EnableFog(true);
			}

			App->CL_Com_Environments->Mark_As_Altered_Environ(Eviron_Index);
		}

	}

	// ---------------------------------------------------------------- Sky
	if (PropertyEditor_Page == 3)
	{
		ImGui::NextColumn();
		ImGui::AlignTextToFramePadding();

		// ----------------- Visible
		ImGui::Text("Enable");
		ImGui::SameLine();
		int test = ImGui::Checkbox("##Enable", &flag_ClickOnSkyEnabled);
		if (test == 1)
		{
			if (flag_ClickOnSkyEnabled == 1)
			{
				App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->flag_Enabled = 1;
				SetSky(true);
			}
			else
			{
				App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->flag_Enabled = 0;
				SetSky(false);
			}

			App->CL_Com_Environments->Mark_As_Altered_Environ(Eviron_Index);
		}

		// ----------------- Tiling
		ImGui::Text("Tiling:- ", App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->Tiling);
		ImGui::SameLine();

		if (ImGui::InputFloat("##5", &App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->Tiling, 0.5, 0, "%.3f"))
		{
			if (App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->flag_Enabled == 1)
			{
				SetSky(true);
			}

			App->CL_Com_Environments->Mark_As_Altered_Environ(Eviron_Index);
		}

		// ----------------- Curvature
		ImGui::Text("Curve:- ", App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->Curvature);
		ImGui::SameLine();

		if (ImGui::InputFloat("##6", &App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->Curvature, 0.5, 0, "%.3f"))
		{
			if (App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->flag_Enabled == 1)
			{
				SetSky(true);
			}

			App->CL_Com_Environments->Mark_As_Altered_Environ(Eviron_Index);
		}

	}
	
	ImGui::PopStyleVar();
	ImGui::Columns(0);
	
	ImGui::Spacing();
	ImGui::Spacing();

	if (ImGui::Button("Close", ImVec2(100, 0)))
	{
		Close_Environment_Editor();

		ImGui::PopStyleColor();
		PropertyEditor_Page = 0;
		flag_Show_PropertyEditor = 0;
	}

	if (flag_Is_Teleport == 1)
	{
		ImGui::SameLine();
		if (ImGui::Button("Goto Location", ImVec2(120, 0)))
		{
			App->CL_Scene->B_Player[0]->Phys_Body->getWorldTransform().setOrigin(App->CL_Scene->B_Object[Eviron_Index]->S_Teleport[0]->Physics_Position);
			App->CL_Scene->B_Player[0]->Phys_Body->getWorldTransform().setRotation(App->CL_Scene->B_Object[Eviron_Index]->S_Teleport[0]->Physics_Rotation);
			Set_To_PlayerView();
		}

		ImGui::SameLine();
		if (ImGui::Checkbox("Enabled", &App->CL_Scene->B_Object[Eviron_Index]->S_Environ[0]->flag_Environ_Enabled))
		{
			//App->CL_Properties->Update_ListView_Teleport();
		}
	}

	
	ImGui::End();
}

// *************************************************************************
// *	 Close_Environment_Editor:- Terry and Hazel Flanigan 2022  		   *
// *************************************************************************
void CL64_Gui_Environment::Close_Environment_Editor()
{
	App->CL_Panels->Enable_Scene_Editor_Dialogs(true);

	/*App->Disable_Panels(false);
	App->Show_Panels(true);*/

	App->CL_FileView->Show_FileView(true);
	/*int Index = App->CL_Properties->Current_Selected_Object;
	App->CL_Com_Environments->Set_Environment_By_Index(0, Index);*/

	int Index = App->CL_Com_Environments->Get_First_Environ();
	App->CL_Com_Environments->Set_Environment_By_Index(0, Index);
}

// *************************************************************************
// *		 Set_To_PlayerView:- Terry and Hazel Flanigan 2022  		   *
// *************************************************************************
void CL64_Gui_Environment::Set_To_PlayerView()
{
	/*App->CL_Ogre->OgreListener->GD_CameraMode = Enums::CamFirst;
	App->SBC_TopTabs->Toggle_FirstCam_Flag = 1;
	App->SBC_TopTabs->Toggle_FreeCam_Flag = 0;

	App->CL_Scene->B_Player[0]->Player_Node->setVisible(false);

	int f = App->CL_Scene->B_Player[0]->Phys_Body->getCollisionFlags();
	App->CL_Scene->B_Player[0]->Phys_Body->setCollisionFlags(f | (1 << 5));

	App->CL_Ogre->BulletListener->Render_Debug_Flag = 0;
	App->CL_Ogre->BulletListener->Render_Debug_Flag = 1;

	App->CL_Ogre->OgreListener->GD_Run_Physics = 1;

	RedrawWindow(App->SBC_TopTabs->Camera_TB_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);*/
}

// *************************************************************************
// *	  		SetSky:- Terry and Hazel Flanigan 2023					   *
// *************************************************************************
void CL64_Gui_Environment::SetSky(bool Enable)
{
	int Index = App->CL_Properties_Scene->Current_Selected_Object;

	App->CL_Ogre->mSceneMgr->setSkyDome(Enable, "OW3D/CloudySky", App->CL_Scene->B_Object[Index]->S_Environ[0]->Curvature, App->CL_Scene->B_Object[Index]->S_Environ[0]->Tiling, App->CL_Scene->B_Object[Index]->S_Environ[0]->Distance);

	/*App->CL_Ogre->mSceneMgr->setSkyDome(Enable,
		App->CL_Scene->B_Object[Index]->S_Environ[0]->Material,
		App->CL_Scene->B_Object[Index]->S_Environ[0]->Curvature,
		App->CL_Scene->B_Object[Index]->S_Environ[0]->Tiling,
		App->CL_Scene->B_Object[Index]->S_Environ[0]->Distance);*/
}

// *************************************************************************
// *	  		EnableFog:- Terry and Hazel Flanigan 2023				   *
// *************************************************************************
bool CL64_Gui_Environment::EnableFog(bool SetFog)
{
	int Index = App->CL_Properties_Scene->Current_Selected_Object;

	if (SetFog)
	{
		const auto& fogProperties = App->CL_Scene->B_Object[Index]->S_Environ[0];
		App->CL_Ogre->mSceneMgr->setFog(FOG_LINEAR,
			ColourValue(fogProperties->Fog_Colour.x,
				fogProperties->Fog_Colour.y,
				fogProperties->Fog_Colour.z),
			fogProperties->Fog_Density,
			static_cast<Ogre::Real>(fogProperties->Fog_Start),
			static_cast<Ogre::Real>(fogProperties->Fog_End));
	}
	else
	{
		App->CL_Ogre->mSceneMgr->setFog(FOG_NONE, ColourValue(0.7f, 0.7f, 0.8f), 0, 100, 1000);
	}

	return true;
}
