/*
Copyright (c) OgreWin3D_Stage 2024 W.T.Flanigan H.C.Flanigan Inflanite_HGT

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
class CL64_FileView
{
public:

	CL64_FileView(void);
	~CL64_FileView(void);

	void Start_FileView(void);
	void Init_FileView(void);
	HTREEITEM Add_Item(HTREEITEM Folder, char* SFileName, int Index, bool NewItem);
	void Set_FolderActive(HTREEITEM Folder);
	void SelectItem(HTREEITEM TreeItem);

	void Get_Selection(LPNMHDR lParam);



	bool Flag_FileView_Active;

	HTREEITEM FV_Players_Folder;	// Players Folder FileFView
	HTREEITEM FV_Areas_Folder;		// Areas/Rooms Folder FileFView
	HTREEITEM FV_LevelFolder;
	HTREEITEM FV_Cameras_Folder;
	HTREEITEM FV_Objects_Folder;
	HTREEITEM FV_Message_Trigger_Folder;
	HTREEITEM FV_Sounds_Folder;
	HTREEITEM FV_Move_Folder;
	HTREEITEM FV_Teleporters_Folder;
	HTREEITEM FV_Collectables_Folder;
	HTREEITEM FV_Counters_Folder;
	HTREEITEM FV_Counters2_Folder;
	HTREEITEM FV_EntitiesFolder;
	HTREEITEM FV_Evirons_Folder;
	HTREEITEM FV_Particles_Folder;
	HTREEITEM FV_Lights_Folder;
	HTREEITEM FV_UserObjects_Folder;

private:
	static LRESULT CALLBACK Proc_ListPanel(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void AddRootFolder(void);
	void MoreFolders(void);
	void ExpandRoot(void);

	TV_INSERTSTRUCT tvinsert;

	HTREEITEM Root;
	HTREEITEM GD_ProjectFolder;
	HTREEITEM GD_TriggerFolder;
	HTREEITEM GD_Environment_Folder;
	HTREEITEM GD_Area_Change_Folder;
	HTREEITEM GD_Level_Change_Folder;

	HIMAGELIST hImageList;
	HBITMAP hBitMap;

	char FileView_Folder[MAX_PATH];
	char FileView_File[MAX_PATH];
};

