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
class CL64_Com_Environments
{
public:
	CL64_Com_Environments();
	~CL64_Com_Environments();

	void Create_Test_Environment();

	bool Add_New_Environ_Entity(bool FirstOne);
	void V_Set_Environ_Defaults(int Index);
	bool Create_Environ_Entity(int Index);
	int Set_Environment_By_Index(bool PlayMusic, int Index);
	void Set_First_Environment(int Index);
	void Set_Environment_GameMode();

	void Rename_Environ_Entity(int Index);
	void Mark_As_Altered_Environ(int Index);
	int Get_First_Environ();
	void Stop_All_Sounds(int Index);
	void Reset_Triggers();

};

