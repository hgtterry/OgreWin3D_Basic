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
	void Reset_Class(void);
	void Init_ImGui(void);
	void ImGui_Set_Colours(void);
	void Load_Font(void);
	void Reset_Material_Index(void);

	float PosX;
	float PosY;

	// -----------------------------------------

	void ImGui_Render_Loop(void);
	void ImGui_FPS(void);
	void Camera_Data_GUI(void);

	void Model_Data_GUI(void);
	void Show_Ogre_Model_Data_GUI(void);
	void Show_Assimp_Model_Data_GUI(void);

	void App_Debug(void);

	bool flag_Imgui_Initialized;
	bool StartPos;
	bool flag_Show_FPS;
	bool flag_Show_ImGui_Demo;
	bool flag_Show_Camera_Data;
	bool flag_Show_Model_Data;
	
	bool flag_Show_App_Debug;

	bool flag_Show_Ogre_Data;
	bool flag_Open_Textures_List;
	// Demo 1
	bool flag_Show_Physics_Debug;

	float Camera_Data_PosX;
	float Camera_Data_Posy;

	float Model_Data_PosX;
	float Model_Data_PosY;

	float Float_Step;

	bool listSubMeshItems[50] = { 0 };
	int PreviouseSubMesh;

	bool listMaterialItems_Ogre[50] = { 0 };
	int PreviouseMaterial_Ogre;

	bool listMaterialItems_Assimp[50] = { 0 };
	int PreviouseMaterial_Assimp;
};

