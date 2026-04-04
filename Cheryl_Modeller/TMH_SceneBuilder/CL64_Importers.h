#pragma once
class CL64_Importers
{
public:
	CL64_Importers(void);
	~CL64_Importers(void);

	void Set_Editor();

	bool Assimp_Loader(bool UseDialog, const LPCWSTR Filetype = NULL, const LPCWSTR Extension = NULL);

	bool Load_Ogre_Model(bool Use_File_Dialog, bool Check_Resource_File);
	void Scan_Material_Files(void);
	bool Load_Ogre_Resource_CFG(bool Use_File_Dialog);
	void Reload_Ogre_Model(Ogre::Quaternion Rotation);
	void Load_Recent_File(char* FileAndPath);
	void Create_Brush();

	bool Flag_Reload_Ogre_Model;
};

