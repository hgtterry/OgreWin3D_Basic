#pragma once
class CL64_Importers
{
public:
	CL64_Importers(void);
	~CL64_Importers(void);

	void Set_Editor();

	bool Load_Ogre_Model(bool Use_File_Dialog);
	void Scan_Material_Files(void);
	bool Load_Ogre_Resource_CFG(bool Use_File_Dialog);
	void Reload_Ogre_Model(Ogre::Quaternion Rotation);

	bool Flag_Reload_Ogre_Model;
};

