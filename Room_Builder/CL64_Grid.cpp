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
#include "CL64_Grid.h"

CL64_Grid::CL64_Grid()
{
	GridManual = nullptr;
	GridNode = nullptr;

	HairManual = nullptr;
	HairNode = nullptr;

	FaceManual = nullptr;
	FaceNode = nullptr;

	ColourMain = ColourValue(0.7, 0.7, 0, 0.6);
	ColourDivision = ColourValue(1, 1, 1, 0.4);

	HairExtend = 8;
	ColourHairZ = ColourValue(1, 0, 0, 1);
	ColourHairX = ColourValue(0, 0, 1, 1);
	ColourHairY = ColourValue(0, 1, 0, 1);

	FaceColour = ColourValue(1, 0, 0, 1);

	HitVertices[0] = Ogre::Vector3(0, 0, 0);
	HitVertices[1] = Ogre::Vector3(0, 0, 0);
	HitVertices[2] = Ogre::Vector3(0, 0, 0);

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
}

CL64_Grid::~CL64_Grid()
{
}

// *************************************************************************
// *	  		Grid_Update:- Terry and Hazel Flanigan 2025				   *
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
// *	  		Hair_Update:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
void CL64_Grid::Hair_Update(bool Create)
{
	if (Create == 1)
	{
		HairManual = App->CL_Ogre->mSceneMgr->createManualObject("HairManual");
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
// *	  		Face_Update:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
void CL64_Grid::Face_Update(bool Create)
{
	if (Create == 1)
	{
		FaceManual = App->CL_Ogre->mSceneMgr->createManualObject("Face_Manual");
		FaceManual->setDynamic(true);
		FaceManual->setRenderQueueGroup(RENDER_QUEUE_MAX);
	}

	FaceManual->clear();
	FaceManual->begin("Template/Alpha_Blend_GD64", RenderOperation::OT_LINE_STRIP);

	// X Axis
	FaceManual->position(HitVertices[0]);
	FaceManual->colour(FaceColour);

	FaceManual->position(HitVertices[1]);
	FaceManual->colour(FaceColour);

	FaceManual->position(HitVertices[2]);
	FaceManual->colour(FaceColour);

	FaceManual->position(HitVertices[0]);
	FaceManual->colour(FaceColour);

	FaceManual->end();

	if (Create == 1)
	{
		FaceNode = App->CL_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();
		FaceNode->attachObject(FaceManual);

		FaceNode->setPosition(0, 0, 0);
		FaceNode->setVisible(false);
	}
}

// *************************************************************************
// *	  		Face_Update2:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Grid::Face_Update2()
{
	FaceManual->beginUpdate(0);

	FaceManual->position(HitVertices[0]);

	FaceManual->position(HitVertices[1]);

	FaceManual->position(HitVertices[2]);

	FaceManual->position(HitVertices[0]);
	FaceManual->end();
}
