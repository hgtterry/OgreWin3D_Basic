/*
Copyright (c) 2024 - 2025 TMH_Software W.T.Flanigan M.Habib H.C.Flanigan

TMH_SceneBuilder

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
#include "CL64_Keyboard.h"

CL64_Keyboard::CL64_Keyboard(void)
{
	Rate = 0;
	OldPos = Ogre::Vector3::ZERO;
	flag_Block_Keyboard = false;
	flag_Update_Views = false;
	Mouse_point = { 0,0 };
}

CL64_Keyboard::~CL64_Keyboard(void)
{
}

// *************************************************************************
// *			Keyboard_Mode_First:- Terry Mo and Hazel 2025			   *
// *************************************************************************
void CL64_Keyboard::Keyboard_Mode_First(float deltaTime)
{
	//if (App->CL_Scene->flag_Player_Added == false != flag_Block_Keyboard == true)
	//{
	//	return;
	//}

	//auto& m_Player = App->CL_Scene->B_Player[0]; // Local Pointer to Player Object

	////	------------------------------------------------ Move Forward
	//if (KEYDOWN(87)) // W Key
	//{
	//	App->CL_Com_Player->mMoveDirection = (btVector3(0, 0, -1));
	//	App->CL_Com_Player->Update_Player(NULL, deltaTime);
	//	App->CL_Com_Player->Check_Collisions();
	//	m_Player->flag_IsMOving = true;
	//}
	//else
	//{
	//	if (m_Player->flag_IsMOving == true)
	//	{
	//		App->CL_Com_Player->mMoveDirection = (btVector3(0, 0, 0));
	//		App->CL_Com_Player->Update_Player(NULL, deltaTime);
	//		m_Player->flag_IsMOving = false;
	//	}
	//}

	////	------------------------------------------------ Move Back
	//if (KEYDOWN(83)) // S Key	
	//{
	//	App->CL_Com_Player->mMoveDirection = (btVector3(0, 0, 1));
	//	App->CL_Com_Player->Update_Player(NULL, deltaTime);
	//	App->CL_Com_Player->Check_Collisions();
	//	m_Player->flag_IsMOving_Back = true;

	//}
	//else
	//{
	//	if (m_Player->flag_IsMOving_Back == true)
	//	{
	//		App->CL_Com_Player->mMoveDirection = (btVector3(0, 0, 0));
	//		App->CL_Com_Player->Update_Player(NULL, deltaTime);
	//		m_Player->flag_IsMOving_Back = false;
	//	}
	//}

	////	------------------------------------------------ Move Right
	//if (GetAsyncKeyState(65) < 0)
	//{

	//	if (App->CL_Scene->flag_Player_Added == true)
	//	{
	//		App->CL_Com_Player->mMoveDirection = (btVector3(1, 0, 0));// walkDirection
	//		App->CL_Com_Player->Update_Player(NULL, deltaTime);
	//		m_Player->flag_IsMOving_Right = true;
	//	}
	//}
	//else
	//{
	//	if (App->CL_Scene->flag_Player_Added == true && m_Player->flag_IsMOving_Right == true)
	//	{
	//		App->CL_Com_Player->mMoveDirection = (btVector3(0, 0, 0));// walkDirection
	//		App->CL_Com_Player->Update_Player(NULL, deltaTime);
	//		m_Player->flag_IsMOving_Right = false;
	//	}
	//}

	////	------------------------------------------------ Move Left
	//if (GetAsyncKeyState(68) < 0)
	//{
	//	if (App->CL_Scene->flag_Player_Added == true)
	//	{
	//		App->CL_Com_Player->mMoveDirection = (btVector3(-1, 0, 0));// walkDirection
	//		App->CL_Com_Player->Update_Player(NULL, deltaTime);
	//		//App->CL_Player->Check_Collisions_New();
	//		m_Player->flag_IsMOving_Left = true;
	//	}
	//}
	//else
	//{
	//	if (App->CL_Scene->flag_Player_Added == true && m_Player->flag_IsMOving_Left == true)
	//	{
	//		App->CL_Com_Player->mMoveDirection = (btVector3(0, 0, 0));// walkDirection
	//		App->CL_Com_Player->Update_Player(NULL, deltaTime);
	//		m_Player->flag_IsMOving_Left = false;
	//	}
	//}

	////------------------------------------------------ Escape 
	//if (GetAsyncKeyState(VK_ESCAPE) < 0 && App->CL_Editor_Control->flag_PreviewMode_Active == true)
	//{
	//	App->CL_Editor_Control->flag_Scene_Game_Running = false;
	//	App->CL_Ogre->Listener_3D->flag_LeftMouseDown = false;
	//	App->flag_Block_Mouse_Buttons = false;
	//	ReleaseCapture();
	//	SetCursor(App->CUR);
	//}

	//if (KEYDOWN(80)) // P Key if all else fails
	//{
	//	App->CL_Editor_Control->flag_Scene_Game_Running = false;
	//	App->CL_Ogre->Listener_3D->flag_LeftMouseDown = false;
	//	App->flag_Block_Mouse_Buttons = false;
	//	ReleaseCapture();
	//	SetCursor(App->CUR);
	//	App->CL_Editor_Control->Start_Editor_MapBrush_Mode();
	//}

	////------------------------------------------------ Space Key - Jump and Selection
	//if (GetAsyncKeyState(VK_SPACE) < 0 && App->CL_Scene->flag_Player_Added == true)
	//{
	//	
	//}

}

// *************************************************************************
// *		Keyboard_Mode_Model:- Terry Mo and Hazel 2025				   *
// *************************************************************************
void CL64_Keyboard::Keyboard_Mode_Model(float deltaTime)
{
	auto& m_Listener = App->CL_Ogre->Listener_3D; // Local Pointer to the 3D Frame Listener

	// Define movement rate based on sensitivity
	float rate = (m_Listener->mMoveSensitivity / 1000) * 2;
	Ogre::Vector3 oldPos = m_Listener->mCamNode->getPosition();

	// Pan Up E key 
	if (GetAsyncKeyState(69) < 0)
	{
		oldPos.y += rate;
	}

	// Pan Down Q Key
	if (GetAsyncKeyState(81) < 0)
	{
		oldPos.y -= rate;
	}

	// Update camera position if panning occurred
	m_Listener->mCamNode->setPosition(oldPos);

	// Forward and Backward movement
	if (GetAsyncKeyState(87) < 0 || m_Listener->Wheel < 0) // W Key or Mouse Wheel Forward
	{
		m_Listener->mTranslateVector.z = - m_Listener->mMoveScale * (GetAsyncKeyState(87) < 0 ? 1 : 25);
	}
	else if (GetAsyncKeyState(83) < 0 || m_Listener->Wheel > 0) // S Key or Mouse Wheel Back
	{
		m_Listener->mTranslateVector.z = m_Listener->mMoveScale * (GetAsyncKeyState(83) < 0 ? 1 : 25);
	}

	// Pan Left A Key
	if (GetAsyncKeyState(65) < 0)
	{
		m_Listener->mTranslateVector.x = -m_Listener->mMoveScale;
	}

	// Pan Right  D Key
	if (GetAsyncKeyState(68) < 0)
	{
		m_Listener->mTranslateVector.x = m_Listener->mMoveScale;
	}

	// -------------------------- Escape 
	if (KEYDOWN(VK_ESCAPE) && App->CL_Editor_Control->flag_Map_Editor_Active == true)
	{
		App->CL_Panels->Deselect_All_Brushes_Update_Dlgs();
		App->CL_Top_Tabs->Redraw_TopTabs_Dlg();
		App->CL_Ogre->OGL_Listener->Show_Visuals(false);
	}
}

// *************************************************************************
// *		Keyboard_Mode_Free:- Terry Mo and Hazel 2025				   *
// *************************************************************************
void CL64_Keyboard::Keyboard_Mode_Free(float deltaTime)
{
	auto& m_Listener = App->CL_Ogre->Listener_3D; // Local Pointer to the 3D Frame Listener

	if (flag_Block_Keyboard == false && m_Listener->CameraMode == Enums::Cam_Mode_Free)
	{
		// -------------------------- Forward W Key
		if (KEYDOWN(87)) // W Key
		{
			m_Listener->mTranslateVector.z = -m_Listener->mMoveScale;
			flag_Update_Views = true;
		}
		if (m_Listener->Wheel < 0) // Mouse Wheel Forward
		{
			m_Listener->mTranslateVector.z = - m_Listener->mMoveScale * 30;
			flag_Update_Views = true;
		}
		
		// --------------------------Back S Key
		if (KEYDOWN(83))
		{
			m_Listener->mTranslateVector.z = m_Listener->mMoveScale;
			flag_Update_Views = true;
		}
		if (m_Listener->Wheel > 0) // Mouse Wheel Back
		{
			m_Listener->mTranslateVector.z = m_Listener->mMoveScale * 30;
			flag_Update_Views = true;
		}
	
		// -------------------------- Pan Down Q Key
		if (KEYDOWN(81))
		{
			Rate = (m_Listener->mMoveSensitivity / 1000) * 8; //FlyRate;
			OldPos = m_Listener->mCamNode->getPosition();
			OldPos.y -= Rate;

			m_Listener->mCamNode->setPosition(OldPos);

			flag_Update_Views = true;
		}

		// -------------------------- Pan Up E Key
		if (KEYDOWN(69))
		{
			Rate = (m_Listener->mMoveSensitivity / 1000) * 8; //FlyRate;
			OldPos = m_Listener->mCamNode->getPosition();
			OldPos.y += Rate;

			m_Listener->mCamNode->setPosition(OldPos);

			flag_Update_Views = true;
		}

		// -------------------------- Pan Left A Key
		if (KEYDOWN(65))
		{
			m_Listener->mTranslateVector.x = - App->CL_Ogre->Listener_3D->mMoveScale;
			flag_Update_Views = true;	
		}

		// -------------------------- Pan Right D Key
		if (KEYDOWN(68))
		{
			m_Listener->mTranslateVector.x = m_Listener->mMoveScale;
			flag_Update_Views = true;
		}

		// -------------------------- Escape 
		if (KEYDOWN(VK_ESCAPE) && App->CL_Editor_Control->flag_Map_Editor_Active == true)
		{
			App->CL_Panels->Deselect_All_Brushes_Update_Dlgs();
			App->CL_Top_Tabs->Redraw_TopTabs_Dlg();
			App->CL_Ogre->OGL_Listener->Show_Visuals(false);
		}
	}
}
