/*
Copyright (c) OgreWin3D_Basic 2024 W.T.Flanigan H.C.Flanigan Inflanite_HGT

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely.

An acknowledgment in the product documentation would be
appreciated but is not required.
*/

#include "pch.h"
#include "CL64_App.h"
#include "CL64_Keyboard.h"

CL64_Keyboard::CL64_Keyboard(void)
{
	Rate = 0;
	OldPos = Ogre::Vector3::ZERO;
	flag_Block_Keyboard = 0;
}

CL64_Keyboard::~CL64_Keyboard(void)
{
}

// *************************************************************************
// *		Keyboard_Mode_First:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Keyboard::Keyboard_Mode_First(float deltaTime)
{
	if (GetAsyncKeyState(80) < 0) // p Key
	{
		App->CL_Ogre->ExitFullScreen();
	}

	if (flag_Block_Keyboard == 0)
	{
		//	------------------------------------------------ Move Forward
		if (GetAsyncKeyState(87) < 0) // W Key
		{

			if (App->CL_Scene->flag_Player_Added == 1)
			{
				App->CL_Player->mMoveDirection = (btVector3(0, 0, -1));
				App->CL_Player->Check_Collisions();
				App->CL_Scene->B_Player[0]->IsMOving = 1;
			}

		}
		else
		{
			if (App->CL_Scene->flag_Player_Added == 1 && App->CL_Scene->B_Player[0]->IsMOving == 1)
			{
				App->CL_Player->mMoveDirection = (btVector3(0, 0, 0));
				App->CL_Scene->B_Player[0]->IsMOving = 0;
			}
		}

		//	------------------------------------------------ Move Back
		if (GetAsyncKeyState(83) < 0) // S Key	
		{

			if (App->CL_Scene->flag_Player_Added == 1)
			{
				App->CL_Player->mMoveDirection = (btVector3(0, 0, 1));
				App->CL_Player->Check_Collisions();
				App->CL_Scene->B_Player[0]->IsMOving_Back = 1;
			}

		}
		else
		{
			if (App->CL_Scene->flag_Player_Added == 1 && App->CL_Scene->B_Player[0]->IsMOving_Back == 1)
			{
				App->CL_Player->mMoveDirection = (btVector3(0, 0, 0));
				App->CL_Scene->B_Player[0]->IsMOving_Back = 0;
			}
		}

		//	------------------------------------------------ Move Right
		if (GetAsyncKeyState(65) < 0)
		{

			if (App->CL_Scene->flag_Player_Added == 1)
			{
				App->CL_Player->mMoveDirection = (btVector3(1, 0, 0));// walkDirection
				App->CL_Scene->B_Player[0]->IsMOving_Right = 1;
			}
		}
		else
		{
			if (App->CL_Scene->flag_Player_Added == 1 && App->CL_Scene->B_Player[0]->IsMOving_Right == 1)
			{
				App->CL_Player->mMoveDirection = (btVector3(0, 0, 0));// walkDirection
				App->CL_Scene->B_Player[0]->IsMOving_Right = 0;
			}
		}

		//	------------------------------------------------ Move Left
		if (GetAsyncKeyState(68) < 0)
		{

			if (App->CL_Scene->flag_Player_Added == 1)
			{
				App->CL_Player->mMoveDirection = (btVector3(-1, 0, 0));// walkDirection
				//App->CL_Player->Check_Collisions_New();
				App->CL_Scene->B_Player[0]->IsMOving_Left = 1;
			}

		}
		else
		{
			if (App->CL_Scene->flag_Player_Added == 1 && App->CL_Scene->B_Player[0]->IsMOving_Left == 1)
			{
				App->CL_Player->mMoveDirection = (btVector3(0, 0, 0));// walkDirection
				App->CL_Scene->B_Player[0]->IsMOving_Left = 0;
			}
		}

		//------------------------------------------------ Escape 
		if (GetAsyncKeyState(VK_ESCAPE) < 0) // Back to Editor mode;
		{

			if (App->CL_Scene->flag_GameMode_Running == 1 || App->CL_Scene->flag_FullScreen_Mode == 1)
			{
				if (App->CL_Build_Game->flag_Use_Front_Dlg == 1)
				{
					GetCursorPos(&Mouse_point);
					App->CL_Ogre->Ogre3D_Listener->flag_LeftMouseDown = 0;
					App->CL_Ogre->Ogre3D_Listener->flag_Block_Mouse = 1;
					App->CL_Keyboard->flag_Block_Keyboard = 1;
					App->flag_Block_Mouse_Buttons = 1;

					ReleaseCapture();
					App->CL_Ogre->Ogre3D_Listener->flag_LeftMouseDown = 0;
					SetCursor(App->CUR);

					App->CL_Front_Dialog->Show_Front_Dlg_Flag = 1;
				}
				else
				{
					App->CL_Ogre->ExitFullScreen();
				}
			}

		}

		//------------------------------------------------ Space Key - Jump and Selection
		if (GetAsyncKeyState(VK_SPACE) < 0 && App->CL_Scene->flag_Player_Added == 1)
		{
			if (App->CL_Ogre->Ogre3D_Listener->flag_Selection_Mode == 1)
			{
				App->CL_Picking->Mouse_Pick_Entity();
				App->CL_TopDlg->flag_Toggle_Select_Flag = 0;
				App->CL_Gizmos->mPickSight->hide();
				App->CL_Ogre->Ogre3D_Listener->flag_Selection_Mode = 0;
				RedrawWindow(App->CL_TopDlg->Camera_TB_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			}
			else
			{
				
			}
		}
	}
}

// *************************************************************************
// *		Keyboard_Mode_Model:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Keyboard::Keyboard_Mode_Model(float deltaTime)
{
	//	Pan Up
	if (GetAsyncKeyState(69) < 0) // E key 
	{
		Rate = (App->CL_Ogre->Ogre3D_Listener->mMoveSensitivity / 1000) * 2;
		OldPos = App->CL_Ogre->Ogre3D_Listener->mCamNode->getPosition();

		OldPos.y += Rate;
		App->CL_Ogre->Ogre3D_Listener->mCamNode->setPosition(OldPos);
	}

	// Pan Down
	if (GetAsyncKeyState(81) < 0)  // Q Key
	{
		Rate = (App->CL_Ogre->Ogre3D_Listener->mMoveSensitivity / 1000) * 2;

		OldPos = App->CL_Ogre->Ogre3D_Listener->mCamNode->getPosition();

		OldPos.y -= Rate;
		App->CL_Ogre->Ogre3D_Listener->mCamNode->setPosition(OldPos);
	}

	// Forward
	if (App->CL_Ogre->Ogre3D_Listener->Wheel < 0) // Mouse Wheel Forward
	{
		App->CL_Ogre->Ogre3D_Listener->mTranslateVector.z = -App->CL_Ogre->Ogre3D_Listener->mMoveScale * 25;
	}
	if (GetAsyncKeyState(87) < 0) // W Key
	{
		App->CL_Ogre->Ogre3D_Listener->mTranslateVector.z = -App->CL_Ogre->Ogre3D_Listener->mMoveScale;
	}

	// Back
	if (App->CL_Ogre->Ogre3D_Listener->Wheel > 0) // Mouse Wheel Back
	{
		App->CL_Ogre->Ogre3D_Listener->mTranslateVector.z = App->CL_Ogre->Ogre3D_Listener->mMoveScale * 25;
	}
	if (GetAsyncKeyState(83) < 0) // S Key	
	{
		App->CL_Ogre->Ogre3D_Listener->mTranslateVector.z = App->CL_Ogre->Ogre3D_Listener->mMoveScale;
	}

	// Pan Left
	if (GetAsyncKeyState(65) < 0) // A Key
	{
		App->CL_Ogre->Ogre3D_Listener->mTranslateVector.x = App->CL_Ogre->Ogre3D_Listener->mMoveScale;
	}

	// Pan Right
	if (GetAsyncKeyState(68) < 0)  // D Key
	{
		App->CL_Ogre->Ogre3D_Listener->mTranslateVector.x = -App->CL_Ogre->Ogre3D_Listener->mMoveScale;
	}

	//------------------------------------------------ Escape 
	if (GetAsyncKeyState(VK_ESCAPE) < 0) // Back to Editor mode;
	{
		
	}
}

// *************************************************************************
// *		Keyboard_Mode_Free:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Keyboard::Keyboard_Mode_Free(float deltaTime)
{
	if (flag_Block_Keyboard == 0)
	{
		if (GetAsyncKeyState(VK_UP) < 0 && App->CL_Scene->flag_Player_Added == 1)
		{
			if (App->CL_Scene->flag_Player_Added == 1)
			{
				App->CL_Player->mMoveDirection = (btVector3(0, 0, -1));
				App->CL_Player->Check_Collisions();
				App->CL_Scene->B_Player[0]->IsMOving = 1;
				App->CL_Player->Update_Player(NULL, deltaTime);
			}
		}
		else
		{
			if (App->CL_Scene->flag_Player_Added == 1 && App->CL_Scene->B_Player[0]->IsMOving == 1)
			{
				App->CL_Player->mMoveDirection = (btVector3(0, 0, 0));
				App->CL_Scene->B_Player[0]->IsMOving = 0;
				App->CL_Player->Update_Player(NULL, deltaTime);
			}
		}

		//	------------------------------------------------ Move Back
		if (GetAsyncKeyState(VK_DOWN) < 0 && App->CL_Scene->flag_Player_Added == 1) // S Key	
		{

			if (App->CL_Scene->flag_Player_Added == 1)
			{
				App->CL_Player->mMoveDirection = (btVector3(0, 0, 1));
				App->CL_Player->Check_Collisions();
				App->CL_Scene->B_Player[0]->IsMOving_Back = 1;
				App->CL_Player->Update_Player(NULL, deltaTime);
			}

		}
		else
		{
			if (App->CL_Scene->flag_Player_Added == 1 && App->CL_Scene->B_Player[0]->IsMOving_Back == 1)
			{
				App->CL_Player->mMoveDirection = (btVector3(0, 0, 0));
				App->CL_Scene->B_Player[0]->IsMOving_Back = 0;
				App->CL_Player->Update_Player(NULL, deltaTime);
			}
		}

		//	------------------------------------------------ Move Right
		if (GetAsyncKeyState(VK_RIGHT) < 0 && App->CL_Scene->flag_Player_Added == 1)
		{

			if (App->CL_Scene->flag_Player_Added == 1)
			{
				Ogre::Vector3 Rotate;
				Rotate.x = 0;
				Rotate.y = 1;
				Rotate.z = 0;

				App->CL_Scene->B_Player[0]->Rotate_FromCam(Rotate, 0.001, false);
			}
		}

		if (GetAsyncKeyState(VK_LEFT) < 0 && App->CL_Scene->flag_Player_Added == 1)
		{

			Ogre::Vector3 Rotate;
			Rotate.x = 0;
			Rotate.y = -1;
			Rotate.z = 0;

			App->CL_Scene->B_Player[0]->Rotate_FromCam(Rotate, 0.001, false);
		}

		//-------------------------------------------------------------
		//-------------------------------------------------------------
		// 
		// Forward
		if (App->CL_Ogre->Ogre3D_Listener->Wheel < 0) // Mouse Wheel Forward
		{
			App->CL_Ogre->Ogre3D_Listener->mTranslateVector.z = -App->CL_Ogre->Ogre3D_Listener->mMoveScale * 30;
		}
		if (GetAsyncKeyState(87) < 0) // W Key
		{
			if (App->CL_Ogre->Ogre3D_Listener->CameraMode == Enums::Cam_Mode_Free)
			{
				App->CL_Ogre->Ogre3D_Listener->mTranslateVector.z = -App->CL_Ogre->Ogre3D_Listener->mMoveScale;
			}
		}

		// Back
		if (App->CL_Ogre->Ogre3D_Listener->Wheel > 0) // Mouse Wheel Back
		{
			App->CL_Ogre->Ogre3D_Listener->mTranslateVector.z = App->CL_Ogre->Ogre3D_Listener->mMoveScale * 30;
		}
		if (GetAsyncKeyState(83) < 0) // S Key	
		{
			if (App->CL_Ogre->Ogre3D_Listener->CameraMode == Enums::Cam_Mode_Free)
			{
				App->CL_Ogre->Ogre3D_Listener->mTranslateVector.z = App->CL_Ogre->Ogre3D_Listener->mMoveScale;
			}

		}

		// Pan Down
		if (GetAsyncKeyState(81) < 0) // Q Key	
		{
			if (App->CL_Ogre->Ogre3D_Listener->CameraMode == Enums::Cam_Mode_Free)
			{
				Rate = (App->CL_Ogre->Ogre3D_Listener->mMoveSensitivity / 1000) * 2; //FlyRate;

				OldPos = App->CL_Ogre->Ogre3D_Listener->mCamNode->getPosition();

				OldPos.y -= Rate;

				App->CL_Ogre->Ogre3D_Listener->mCamNode->setPosition(OldPos);
			}
		}

		//	Pan Up
		if (GetAsyncKeyState(69) < 0) // E Key
		{
			if (App->CL_Ogre->Ogre3D_Listener->CameraMode == Enums::Cam_Mode_Free)
			{
				Rate = (App->CL_Ogre->Ogre3D_Listener->mMoveSensitivity / 1000) * 2; //FlyRate;

				OldPos = App->CL_Ogre->Ogre3D_Listener->mCamNode->getPosition();

				OldPos.y += Rate;

				App->CL_Ogre->Ogre3D_Listener->mCamNode->setPosition(OldPos);
			}
		}

		// Pan Left
		if (GetAsyncKeyState(65) < 0) // A Key
		{
			if (App->CL_Ogre->Ogre3D_Listener->CameraMode == Enums::Cam_Mode_Free)
			{
				App->CL_Ogre->Ogre3D_Listener->mTranslateVector.x = -App->CL_Ogre->Ogre3D_Listener->mMoveScale;
			}
		}

		// Pan Right
		if (GetAsyncKeyState(68) < 0) // D Key
		{
			if (App->CL_Ogre->Ogre3D_Listener->CameraMode == Enums::Cam_Mode_Free)
			{
				App->CL_Ogre->Ogre3D_Listener->mTranslateVector.x = App->CL_Ogre->Ogre3D_Listener->mMoveScale;
			}
		}

		//------------------------------------------------ Escape 
		if (GetAsyncKeyState(VK_ESCAPE) < 0) // Back to Editor mode;
		{

		}

		//------------------------------------------------ Space Key - Jump and Selection
		if (GetAsyncKeyState(VK_SPACE) < 0 && App->CL_Scene->flag_Player_Added == 1)
		{
			if (App->CL_Ogre->Ogre3D_Listener->flag_Selection_Mode == 1)
			{
				App->CL_Picking->Mouse_Pick_Entity();
				App->CL_TopDlg->flag_Toggle_Select_Flag = 0;
				App->CL_Gizmos->mPickSight->hide();
				App->CL_Ogre->Ogre3D_Listener->flag_Selection_Mode = 0;
				RedrawWindow(App->CL_TopDlg->Camera_TB_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			}
			else
			{

			}
		}
	}
}
