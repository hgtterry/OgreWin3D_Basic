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
#include "CL64_Grid.h"

CL64_Grid::CL64_Grid(void)
{
	GridManual = NULL;
	GridNode = NULL;

	HairManual = NULL;
	HairNode = NULL;

	ColourMain = ColourValue(0.7, 0.7, 0, 0.6);
	ColourDivision = ColourValue(1, 1, 1, 0.4);

	HairExtend = 8;
	ColourHairZ = ColourValue(1, 0, 0, 1);
	ColourHairX = ColourValue(0, 0, 1, 1);
	ColourHairY = ColourValue(0, 1, 0, 1);
	
	YAxis_min = -8;
	YAxis_max = 8;

	ZAxis_min = -8;
	ZAxis_max = 8;

	XAxis_min = -8;
	XAxis_max = 8;

	Division_X = 2;
	Division_Y = 2;
	Division_Z = 2;

	Scale_X = 4;
	Scale_Y = 4;
	Scale_Z = 4;

	flag_ShowGrid = 1;
	flag_ShowDivisions = 1;
	flag_ShowHair = 1;

}

CL64_Grid::~CL64_Grid(void)
{
	
}

// *************************************************************************
// *			Reset_Class:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Grid::Reset_Class(void)
{
	Grid_SetVisible(true);
	flag_ShowGrid = true;

	flag_ShowHair = true;
	Hair_SetVisible(true);

}

// *************************************************************************
// *	  		Grid_Update:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Grid::Grid_Update(bool Create)
{
	int i = 0;
	Real r;

	if (Create == 1)
	{
		GridManual = App->CL_Ogre->mSceneMgr->createManualObject("BoxManual");
	}

	GridManual->clear();
	GridManual->begin("Template/Alpha_Blend_GD64", Ogre::RenderOperation::OT_LINE_LIST, App->CL_Ogre->App_Resource_Group);
	
	if (flag_ShowGrid == 1)
	{
		for (int x = XAxis_min; x <= XAxis_max; ++x)
		{
			GridManual->position(x, 0, ZAxis_min);
			GridManual->colour(ColourMain);
			GridManual->position(x, 0, ZAxis_max);
			GridManual->colour(ColourMain);
			if (x < XAxis_max && flag_ShowDivisions == 1)
			{
				for (int d = 0; d < Division_X; ++d)
				{
					r = x + (1.0 / Real(Division_X)) * Real(d);
					GridManual->position(r, 0, ZAxis_min);
					GridManual->colour(ColourDivision);
					GridManual->position(r, 0, ZAxis_max);
					GridManual->colour(ColourDivision);
				}
			}
		}

		for (int z = ZAxis_min; z <= ZAxis_max; ++z)
		{
			GridManual->position(Ogre::Vector3(XAxis_min, 0, z));
			GridManual->colour(ColourMain);
			GridManual->position(Ogre::Vector3(XAxis_max, 0, z));
			GridManual->colour(ColourMain);
			if (z < ZAxis_max && flag_ShowDivisions == 1)
			{
				for (int d = 1; d < Division_Y; ++d)
				{
					r = z + (1.0 / Real(Division_Y)) * Real(d);
					GridManual->position(XAxis_min, 0, r);
					GridManual->colour(ColourDivision);
					GridManual->position(XAxis_max, 0, r);
					GridManual->colour(ColourDivision);
				}
			}
		}
	}


	GridManual->end();

	if (Create == 1)
	{
		GridNode = App->CL_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();
		GridNode->attachObject(GridManual);
	}

	GridNode->setPosition(0, 0, 0);
	GridNode->setVisible(true);
	GridNode->setScale(Scale_X, Scale_Y, Scale_Z);
}

// *************************************************************************
// *	  		Grid_SetVisiable:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Grid::Grid_SetVisible(bool Enable)
{
	GridNode->setVisible(Enable);
	flag_ShowGrid = Enable;
}

// *************************************************************************
// *	  		Hair_Update:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Grid::Hair_Update(bool Create)
{
	if (Create == 1)
	{
		HairManual = App->CL_Ogre->mSceneMgr->createManualObject("HairManual");
		//HairManual->setRenderQueueGroup(5);
	}

	HairManual->clear();

	
	HairManual->begin("Template/Alpha_Blend_GD64", RenderOperation::OT_LINE_LIST, "App_Resource_Group");

	// X Axis
	HairManual->position(Ogre::Vector3(YAxis_min - HairExtend, 0, 0));
	HairManual->colour(ColourHairX);
	HairManual->position(Ogre::Vector3(YAxis_max + HairExtend, 0, 0));
	HairManual->colour(ColourHairX);
	// Y Axis
	HairManual->position(Ogre::Vector3(0, YAxis_min - HairExtend, 0));
	HairManual->colour(ColourHairY);
	HairManual->position(Ogre::Vector3(0, YAxis_max + HairExtend, 0));
	HairManual->colour(ColourHairY);
	// Z Axis
	HairManual->position(Ogre::Vector3(0, 0, ZAxis_min - HairExtend));
	HairManual->colour(ColourHairZ);
	HairManual->position(Ogre::Vector3(0, 0, ZAxis_max + HairExtend));
	HairManual->colour(ColourHairZ);

	HairManual->end();

	if (Create == 1)
	{
		HairNode = App->CL_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();
		HairNode->attachObject(HairManual);
	}

	HairNode->setPosition(0, 0, 0);
	HairNode->setVisible(true);
	HairNode->setScale(Scale_X, Scale_Y, Scale_Z);
}

// *************************************************************************
// *	  		Hair_SetVisiable:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Grid::Hair_SetVisible(bool Enable)
{
	HairNode->setVisible(Enable);
	flag_ShowHair = Enable;
}

// *************************************************************************
// *	  	Enable_Grid_And_Hair:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Grid::Enable_Grid_And_Hair(bool Enable)
{
	GridNode->setVisible(Enable);
	HairNode->setVisible(Enable);

	flag_ShowGrid = Enable;
	flag_ShowHair = Enable;

	App->CL_TopDlg->Enable_Grid_Hair_Icons(Enable);
}
