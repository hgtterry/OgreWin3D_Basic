/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

Room Builder

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
	CL64_ImGui();
	~CL64_ImGui();

	void Init_ImGui(void);
	void ImGui_Render_Loop(void);

	bool flag_Imgui_Initialized;
	bool flag_Show_Paths;
	bool flag_Show_Render_Reports;
	bool flag_Show_Press_Excape;
	bool flag_Show_Preview_Options;
	bool flag_Show_App_Stats;
	bool flag_Show_Player_Data;
	bool flag_PlayerData_Start_Pos;

private:

	void Load_Font(void);
	void ImGui_Set_Colours(void);

	void ImGui_FPS(void);
	void Paths_GUI(void);
	void Render_Report_GUI(void);

	void Press_Excape_GUI(void);
	void Preview_Options_GUI(void);

	void App_Stats_GUI(void);
	void Player_Data_GUI(void);

	std::vector<std::pair<bool, std::function<void()>>> guiFunctions;

	ImFont* font0;
	ImFont* font1;
	ImFont* font2;
	ImFont* font3;
	ImFont* fontDroid;

	float PosX;
	float PosY;

	float Cam_Mode_PosX;
	float Cam_Mode_PosY;

	float Player_Data_PosX;
	float Player_Data_PosY;

	bool flag_Show_FPS;
	bool flag_StartPos;
};

