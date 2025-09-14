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
class CL64_Gizmos
{
public:

	CL64_Gizmos(void);
	~CL64_Gizmos(void);

	void Init_Gizmos(Ogre::SceneManager* m_SceneMgr);

	// Grid and Hair
	void Grid_Update(bool Create);
	void Hair_Update(bool Create);
	void Enable_Grid_And_Hair(bool Enable);
	void Reset_Grid_And_Hair();

	// Face
	void Face_Update(bool Create);
	void Face_Update2();

	// Marker Box
	void MarkerBox_Setup(void);
	void MarkerBox_Update(float Depth, float Height, float Width);
	void MarkerBox_Adjust(int index);
	void Show_MarkerBox(bool Show);

	void Load_PickSight(void);
	void Load_Crosshair();

	void Load_All_Axis();
	void Load_Red_Axis();
	void Load_Green_Axis();
	void Load_Blue_Axis();

	void Update_Red_Axis_Marker(int Index);
	void Update_Green_Axis_Marker(int Index);
	void Update_Blue_Axis_Marker(int Index);

	void Hide_Axis_Marker();

	void highlight(Ogre::Entity* entity);
	void unhighlight(Ogre::Entity* entity);

	int Last_Selected_Object;

	Ogre::SceneManager* mSceneMgr;

	Ogre::ManualObject* Grid_Manual;
	Ogre::SceneNode* Grid_Node;

	Ogre::ManualObject* Hair_Manual;
	Ogre::SceneNode* Hair_Node;

	// Face
	Ogre::ManualObject* Face_Manual;
	Ogre::SceneNode* Face_Node;
	Ogre::ColourValue Face_Colour;
	Ogre::Vector3 HitVertices[3];


	// Grid
	Ogre::ColourValue Grid_ColourMain;
	Ogre::ColourValue Grid_ColourDivision;

	int Hair_Extend;
	Ogre::ColourValue Colour_HairX;
	Ogre::ColourValue Colour_HairZ;
	Ogre::ColourValue Colour_HairY;

	int Grid_XAxis_min;
	int	Grid_XAxis_max;
	int Grid_YAxis_min;
	int	Grid_YAxis_max;
	int Grid_ZAxis_min;
	int	Grid_ZAxis_max;

	Ogre::Vector2 Grid_Division;
	Ogre::Vector3 Grid_Scale;

	bool flag_ShowGrid;
	bool flag_ShowDivisions;

	Ogre::ManualObject* BoxManual;
	Ogre::SceneNode* BoxNode;

	Ogre::Entity* Crosshair_Ent;
	Ogre::SceneNode* Crosshair_Node;

	Ogre::Overlay* mPickSight;

	Ogre::Entity* RedAxis_Ent;
	Ogre::SceneNode* RedAxis_Node;

	Ogre::Entity* GreenAxis_Ent;
	Ogre::SceneNode* GreenAxis_Node;

	Ogre::Entity* BlueAxis_Ent;
	Ogre::SceneNode* BlueAxis_Node;

};



