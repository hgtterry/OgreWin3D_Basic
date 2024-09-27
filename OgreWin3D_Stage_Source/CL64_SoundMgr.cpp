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
#include "CL64_SoundMgr.h"

CL64_SoundMgr::CL64_SoundMgr(void)
{
	SoundEngine = irrklang::createIrrKlangDevice();
	SndFile = nullptr;
	SndVolume = 0.5;

	strcpy(Default_Folder, App->GD_Directory_FullPath);
}

CL64_SoundMgr::~CL64_SoundMgr(void)
{
}

// *************************************************************************
// *		Play_StartUp_Sound:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_SoundMgr::Play_StartUp_Sound()
{
	char Sound[MAX_PATH];
	strcpy(Sound, Default_Folder);
	strcat(Sound, "\\Media\\Sounds\\");
	strcat(Sound, "welcome.ogg");

	SndFile = SoundEngine->play2D(Sound, false, true, true);
	SndFile->setVolume(0.5);
	SndFile->setIsPaused(false);

	return 1;
}
