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
class CL64_FileView
{
public:

	CL64_FileView(void);
	~CL64_FileView(void);

	void Reset_Class();

	void Start_FileView(void);
	void Init_FileView(void);
	HTREEITEM Add_Item(HTREEITEM Folder, char* SFileName, int Index, bool NewItem);
	void Set_FolderActive(HTREEITEM Folder);
	void SelectItem(HTREEITEM TreeItem);

	void Get_Selection(LPNMHDR lParam);
	void Redraw_FileView();
	void Change_Level_Name(void);
	void Change_Project_Name(void);
	void Mark_Altered(HTREEITEM Item);
	void Show_FileView(bool Enable);
	void Change_Item_Name(HTREEITEM Folder, char* FolderName);
	void DeleteItem();
	void Mark_Altered_Folder(HTREEITEM Item);
	void Mark_Clear(HTREEITEM Item);
	void Mark_Clear_Folder(HTREEITEM Item);

	void Context_Rename() const;
	void Context_Delete();

	bool flag_FileView_Active;

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

	HMENU hMenu;

private:
	static LRESULT CALLBACK Proc_ListPanel(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void AddRootFolder(void);
	void MoreFolders(void);
	void ExpandRoot(void);

	void Context_Menu(HWND hDlg);
	void Context_New(HWND hDlg);
	
	void HideRightPanes(void);

	TV_INSERTSTRUCT tvinsert;

	HTREEITEM Root;
	HTREEITEM GD_ProjectFolder;
	HTREEITEM GD_TriggerFolder;
	HTREEITEM GD_Environment_Folder;
	HTREEITEM GD_Area_Change_Folder;
	HTREEITEM GD_Level_Change_Folder;

	HIMAGELIST hImageList;
	HBITMAP hBitMap;

	int Context_Selection;

	char FileView_Folder[MAX_PATH];
	char FileView_File[MAX_PATH];
};

