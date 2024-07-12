/*
Copyright (c) GameDirector 2019 To 2024 HGT Software W.T.Flanigan H.C.Flanigan

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/

#pragma once
class CL64_Importers
{
public:
	CL64_Importers();
	~CL64_Importers();

	bool Assimp_Loader(bool UseDialog,const char* Extension, const char* Extension2);

	void Load_Ogre_Model(bool Use_File_Dialog);
	void Reload_Ogre_Model(void);

	void Ogre_Resource_CFG_Loader(char* Extension, char* Extension2);

	Ogre::String Ogre_Loader_Resource_Group;
	Ogre::String Ogre_CFG_Resource_Group;

	bool Flag_Reload_Ogre_Model;
}; 

