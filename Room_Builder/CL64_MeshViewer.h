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

#include "CL64_MeshView_Listener.h"


class CL64_MeshViewer
{
public:
	CL64_MeshViewer(void);
	~CL64_MeshViewer(void);

	void Start_MeshViewer_Dlg();

	Ogre::String MV_Resource_Group;

	HWND MainDlgHwnd;
	HWND MeshViewer_3D_hWnd;

	Ogre::RenderWindow* Ogre_MV_Window;
	Ogre::SceneManager* Ogre_MV_SceneMgr;
	Ogre::Camera* Ogre_MV_Camera;
	Ogre::SceneNode* Ogre_MV_CamNode;
	Ogre::Entity* Ogre_MvEnt;
	Ogre::SceneNode* Ogre_MvNode;

	Ogre::ManualObject* MV_btDebug_Manual;
	Ogre::SceneNode* MV_btDebug_Node;

	bool flag_MV_Resource_Path_Loaded;
	bool flag_MeshViewer_Running;

	bool flag_MV_Render_Debug;
	// Used for Creating Objects ----------------
	char Object_Name[MAX_PATH];
	char Selected_MeshFile[MAX_PATH];
	char m_Resource_Folder_Full[MAX_PATH];
	// ------------------------------------------

	btRigidBody* Ogre_MV_Phys_Body;
	int Physics_Shape;
	int Physics_Type;

	int Mesh_Viewer_Mode;

private:
	static LRESULT CALLBACK Proc_MeshViewer_Dlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_MeshViewer_3D(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Proc_Mesh_Properties(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void Reset_Data();

	void Close_OgreWindow(void);
	bool Set_OgreWindow(void);

	bool Add_Resources();
	bool Delete_Resources_Group();

	void Get_Stock_Folders(HWND DropHwnd);
	void Get_Mesh_Files();
	void Show_Mesh(char* MeshFile);

	void Clear_Shape_Buttons();
	void Clear_Type_Buttons();

	void Clear_Debug_Shape();

	void Show_Physics_Trimesh();

	void Show_Physics_None();
	void Show_Physics_Box();
	void Show_Physics_Sphere();
	void Show_Physics_Capsule();
	void Show_Physics_Cylinder();
	void Show_Physics_Cone();

	void Set_For_Areas(HWND hDlg);

	void Set_Physics();

	void Enable_ShapeButtons(bool state);

	bool Check_if_Files_Exsit(char* File);
	void Copy_Assets();

	void Get_Ogre_Mesh_Data(Ogre::Entity* Ogre_Entity);

	bool Add_Resource_Location_Project(char* Resource_Location);

	void Show_Mesh_Properties();

	bool Create_Mesh(char* File);
	bool Create_Material(char* File);
	bool Create_Texture(char* Texture_Path, char* Texture_Name);

	HWND ListHwnd;
	HWND CB_hWnd;

	char m_Just_Folder[MAX_PATH];
	
	
	// Old Copy System
	char m_Material_File[MAX_PATH];

	std::vector<std::string> v_Texture_Names;
	std::vector<std::string> v_Scrip_Names;

	int Texure_Count;
	int Material_Scripts_Count;
	int NumSub_Meshes;

	char DestinationFile[MAX_PATH];
	char SourceFile[MAX_PATH];
	
	bool flag_SelectDynamic;
	bool flag_SelectStatic;
	bool flag_SelectTriMesh;

	bool flag_Selected_Shape_Box;
	bool flag_Selected_Shape_Sphere;
	bool flag_Selected_Shape_Capsule;
	bool flag_Selected_Shape_Cylinder;
	bool flag_Selected_Shape_Cone;

	CL64_MeshView_Listener* RenderListener;

	btCollisionShape* Ogre_MV_Phys_Shape;
};





