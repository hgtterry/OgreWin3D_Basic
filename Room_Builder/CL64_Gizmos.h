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

	void Set_Gizmos();

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

	ManualObject* BoxManual;
	SceneNode* BoxNode;

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



