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

	float PosX;
	float PosY;

	// -----------------------------------------

	void ImGui_Render_Loop(void);
	void ImGui_FPS(void);
	void Camera_Data_GUI(void);
	void ImGui_Collision_Debug(void);

	void App_Debug(void);

	void Demo_Options_Gui(void);

	ImFont* font0;
	ImFont* font1;
	ImFont* font2;
	ImFont* font3;
	ImFont* fontDroid;

	bool flag_Imgui_Initialized;
	bool StartPos;
	bool flag_Show_FPS;
	bool flag_Show_ImGui_Demo;
	bool flag_Show_Camera_Data;
	bool flag_Show_Demo_Options;
	bool flag_Show_App_Debug;
	bool Show_Collision_Debug;

	bool flag_Show_Ogre_Data;
	bool flag_Open_Textures_List;
	// Demo 1
	bool flag_Show_Physics_Debug;

	float Camera_Data_PosX;
	float Camera_Data_Posy;

	float Model_Data_PosX;
	float Model_Data_PosY;

	float Float_Step;

};

