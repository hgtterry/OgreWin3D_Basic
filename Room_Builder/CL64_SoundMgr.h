/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OgreWin3D

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
class CL64_SoundMgr
{
public:

	CL64_SoundMgr(void);
	~CL64_SoundMgr(void);

	void Show_Sound_Player();
	bool Play_StartUp_Sound();

	irrklang::ISoundEngine* SoundEngine;
	irrklang::ISound* SndFile;
	Ogre::Real SndVolume;

	char Default_Folder[MAX_PATH];
	char mSoundFile[MAX_PATH];
	char m_Current_Sound_file[MAX_PATH];
	char Access_File[MAX_PATH];

	bool flag_IsCancelled;
	bool flag_Accessed;

	int SoundFile_Count;
	std::vector<std::string> Sound_File;

private:
	static LRESULT CALLBACK Proc_Dialog_SoundFile(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void GetSoundFiles(HWND hDlg, char* Extention);
	void Remeber_SoundFile(char* File);

};

