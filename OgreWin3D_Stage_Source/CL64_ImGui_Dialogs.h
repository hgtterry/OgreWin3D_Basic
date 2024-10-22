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

	void BackGround_Render_Loop(void);

	void Start_Dialog_Float(float Step, float StartValue, char* Banner);
	void Dialog_Float(void);

	void Start_Dialog_MessageEditor(int Index);
	void Dialog_MessageEditor(void);

	// -------------- Float Dialog
	bool Show_Dialog_Float;
	float Float_Step;
	bool Float_Canceld;
	bool Float_StartPos;
	float Float_PosX;
	float Float_PosY;
	float m_Dialog_Float;
	float m_Dialog_Float_Copy;
	char Float_Banner[MAX_PATH];
	bool Float_Exit;

	// -------------- General
	ImVec4 Float_Colour;
	
	// -------------- Message Editor Dialog
	bool Centre_X_Selected;
	bool Centre_Y_Selected;
	bool Show_Dialog_MessageEditor;
	bool MessageEditor_Canceld;
	float Message_Editor_PosX;
	float Message_Editor_PosY;
	bool Message_Editor_StartPos;
	int Message_Index;
	ImVec4 BackGround_color;

private:


};

