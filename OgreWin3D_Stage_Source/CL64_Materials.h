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

#pragma once
class CL64_Materials
{
public:
	CL64_Materials(void);
	~CL64_Materials(void);

	void Start_Material_Editor();
	void Close_Material_Editor();

	void Material_Editor_Gui();

	void Get_Material_Name(Ogre::Entity* mEntity);
	void Update_MaterialFile(Ogre::Entity* mBaseEntity);

	bool flag_Show_Material_Editor;


protected:

	void Copy_Texture();

	void Scroll_Gui();

	bool flag_Show_Scroll_Editor;
	int item_current_idx;
	int NumSubMesh;

	char Material_FileName[MAX_PATH];
	Ogre::Entity* BaseEntity;
	Ogre::SceneNode* BaseNode;

	Ogre::MaterialPtr MatCurent;

	std::vector<MaterialPtr> MatClone;
};

