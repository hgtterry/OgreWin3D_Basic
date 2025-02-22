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

#include "pch.h"
#include "CL64_App.h"
#include "CL64_Panels.h"

CL64_Panels::CL64_Panels()
{
}

CL64_Panels::~CL64_Panels()
{
}

// ************************************************************************
// *			Resize_OgreWin:- Terry and Hazel Flanigan 2025			*
// ************************************************************************
void CL64_Panels::Resize_OgreWin(void)
{
	RECT rcl;

	GetClientRect(App->CL_MapEditor->Bottom_Right_Hwnd, &rcl);

	//-----------------Ogre Window
	//SetWindowPos(App->CL_MapEditor->Bottom_Right_Hwnd, NULL, 4, 2, NewWidth + 380, NewHeight + 58, SWP_NOZORDER);

	if (App->flag_OgreStarted == 1)
	{
		RECT rect;
		GetClientRect(App->CL_MapEditor->Bottom_Right_Hwnd, &rect);

		if ((rect.bottom - rect.top) != 0 && App->CL_Ogre->mCamera != 0)
		{
			App->CL_Ogre->mWindow->windowMovedOrResized();
			App->CL_Ogre->mCamera->setAspectRatio((Ogre::Real)App->CL_Ogre->mWindow->getWidth() / (Ogre::Real)App->CL_Ogre->mWindow->getHeight());
			App->CL_Ogre->camNode->yaw(Radian(0));
		}

	}
}

// *************************************************************************
// *  Deselect_All_Brushes_Update_Dlgs:- Terry and Hazel Flanigan 2025   *
// *************************************************************************
void CL64_Panels::Deselect_All_Brushes_Update_Dlgs(void)
{
	App->CL_Doc->DoGeneralSelect(false);
	App->CL_Doc->ResetAllSelections();
	App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);

	//App->CL_Doc->UpdateSelected();

	App->CL_Properties_Brushes->Update_SelectedBrushesCount_Dlg();
	App->CL_Top_Tabs->Enable_Brush_Options_Buttons(false, false);
	App->CL_Top_Tabs->Deselect_Faces_Dlg_Buttons();

	App->CL_Properties_Textures->Enable_FaceProps_Button(false);

	App->CL_Top_Tabs->Reset_Brush_Buttons();
	App->CL_Top_Tabs->flag_Brush_Select = 1;

	App->CL_Properties_Brushes->Enable_Options_Buttons(false);

	App->CL_Top_Tabs->Deselect_Faces_Dlg_Buttons();

}
