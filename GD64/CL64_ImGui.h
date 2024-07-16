/*
Copyright (c) OgreWin3D_Basic 2024 W.T.Flanigan H.C.Flanigan Inflanite_HGT

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely.

An acknowledgment in the product documentation would be
appreciated but is not required.
*/

#pragma once
class CL64_ImGui
{
public:

	CL64_ImGui(void);
	~CL64_ImGui(void);

	// Setup -----------------------------------
	void Init_ImGui(void);
	void ImGui_Set_Colours(void);
	void Load_Font(void);

	float PosX;
	float PosY;

	// -----------------------------------------

	void ImGui_Render_Loop(void);
	void ImGui_FPS(void);
	void Camera_Data_GUI(void);
	void Model_Data_GUI(void);
	void App_Debug(void);

	void Demo_1_GUI(void);

	bool Imgui_Initialized;
	bool StartPos;
	bool Show_FPS;
	bool Show_ImGui_Demo;
	bool Show_Camera_Data_F;
	bool Show_Model_Data_F;
	bool flag_Show_Demo_1;
	bool flag_Show_App_Debug;

	// Demo 1
	bool Show_Physics_Debug_F;

	float Camera_Data_PosX;
	float Camera_Data_Posy;

	float Model_Data_PosX;
	float Model_Data_PosY;

	float Float_Step;
};

