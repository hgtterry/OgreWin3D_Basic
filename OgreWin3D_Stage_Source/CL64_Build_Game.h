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

// ------------------------ Game Options
typedef struct Game_Options
{
	bool flag_Show_FPS;
	bool flag_FullScreen;
	bool flag_Zipped_Assets_Flag;
	bool flag_Front_Dialog_Flag;

}Game_Options;

class CL64_Build_Game
{
public:

	CL64_Build_Game(void);
	~CL64_Build_Game(void);

	void Init_Build_Game_Class();

	char GameName[MAX_PATH];
	Game_Options* GameOptions;

	bool flag_Use_Front_Dlg;
	bool flag_Show_FPS;
	bool flag_Saved_Show_FPS;

};

