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

#pragma once
class CL64_Importers
{
public:
	CL64_Importers();
	~CL64_Importers();

	bool Assimp_Loader(bool UseDialog,const char* Extension, const char* Extension2);

	bool Load_Ogre_Model(bool Use_File_Dialog);
	bool Load_Ogre_Resource_CFG(bool Use_File_Dialog);
	
	void Reload_Ogre_Model(void);
	void Scan_Material_Files(void);

	Ogre::String Ogre_CFG_Resource_Group;

	bool Flag_Reload_Ogre_Model;
}; 

