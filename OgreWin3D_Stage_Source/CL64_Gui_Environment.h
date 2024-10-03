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
class CL64_Gui_Environment
{
public:
	CL64_Gui_Environment(void);
	~CL64_Gui_Environment(void);

	void Reset_Class();

	void Environ_PropertyEditor();
	void Start_Environment_Editor(int Index,bool IsTeleport);
	void Close_Environment_Editor();

	void SetSky(bool Enable);
	bool EnableFog(bool SetFog);

	bool flag_Show_PropertyEditor;
	int PropertyEditor_Page;

protected:

	void Set_To_PlayerView();

	// Ambient
	int Ambient_Int_Red;
	int Ambient_Int_Green;
	int Ambient_Int_Blue;
	ImVec4 Ambient_Colour_Copy;
	ImVec4 Ambient_Colour;

	// Fog Colour
	int Fog_Colour_Int_Red;
	int Fog_Colour_Int_Green;
	int Fog_Colour_Int_Blue;
	ImVec4 Fog_Colour_Copy;
	ImVec4 Fog_Colour;

	int Eviron_Index;

	bool flag_Float_Exit;

	bool flag_Is_Teleport;

	float Float_PosX;
	float Float_PosY;
	bool flag_Float_StartPos;

	bool flag_ClickOnTrack;
	bool flag_ClickOnVolume;
	bool flag_ClickOnPlay;
	bool flag_ClickOnLoop;

	bool flag_ClickOnFogVisible;
	bool flag_ClickOnFogMode;
	bool flag_ClickOnFogColour;
	bool flag_ClickOnFogStart;
	bool flag_ClickOnFogEnd;

	bool flag_ClickOnSkyEnabled;
	bool flag_ClickOnSkyTiling;
	bool flag_ClickOnSkyCurve;
};

