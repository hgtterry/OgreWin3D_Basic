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
#include "CL64_Motions.h"

CL64_Motions::CL64_Motions(void)
{
	Animate_State = nullptr;
	AnimationScale = 1;
}

CL64_Motions::~CL64_Motions(void)
{
}

// *************************************************************************
// *		Set_Player_Motion:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Motions::Set_Player_Motion(void)
{
	Animate_State = nullptr;

	if (App->CL_Scene->flag_Player_Added == 1)
	{
		if (App->CL_Scene->B_Player[0]->Player_Ent)
		{
			Animate_State = App->CL_Scene->B_Player[0]->Player_Ent->getAnimationState("IdleTop");

			if (Animate_State)
			{
				Animate_State->setEnabled(true);
				App->CL_Ogre->Ogre3D_Listener->flag_Animate_Ogre = 1;

			}
		}
	}
}

// *************************************************************************
// *		Update_Player_Motion:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Motions::Update_Player_Motion(float deltaTime)
{
	if (App->CL_Scene->flag_Player_Added == 1)
	{
		if (Animate_State)
		{
			Animate_State->addTime(deltaTime * AnimationScale);
			App->CL_Scene->B_Player[0]->Player_Ent->_updateAnimation();
			//App->Flash_Window();
		}
	}
}
