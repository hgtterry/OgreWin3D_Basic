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

#include "pch.h"
#include "CL64_App.h"
#include "CL64_Materials.h"

CL64_Materials::CL64_Materials(void)
{
}

CL64_Materials::~CL64_Materials(void)
{
}

// *************************************************************************
// *		 Start_Material_Editor:- Terry and Hazel Flanigan 2023  	   *
// *************************************************************************
void CL64_Materials::Start_Material_Editor()
{
	NumSubMesh = 0;
	MatClone.resize(0);

	int Index = App->CL_Properties->Current_Selected_Object;
	item_current_idx = 0;

	if (App->CL_Properties->Edit_Category == Enums::Edit_Area)
	{
		BaseEntity = App->CL_Scene->B_Area[Index]->Area_Ent;
	}
	else
	{
		BaseEntity = App->CL_Scene->V_Object[Index]->Object_Ent;
		BaseNode = App->CL_Scene->V_Object[Index]->Object_Node;
	}

	// ---------------- Get Material Name
	Get_Material_Name(BaseEntity);

	// ---------------- Copy Scripts
	NumSubMesh = BaseEntity->getMesh()->getNumSubMeshes();

	int Count = 0;
	while (Count < NumSubMesh)
	{
		Ogre::String text = BaseEntity->getMesh()->getSubMesh(Count)->getMaterialName().c_str();
		Ogre::MaterialPtr  Mat = static_cast<Ogre::MaterialPtr> (Ogre::MaterialManager::getSingleton().getByName(text));

		char Name[MAX_PATH] = { 0 };
		char Num[MAX_PATH] = { 0 };
		_itoa(Count, Num, 10);

		strcpy(Name, "Clone_");
		strcat(Name, Num);

		MatClone.push_back(Mat->clone(Name));

		Count++;
	}


	App->CL_FileView->Show_FileView(false);

	//App->CL_Panels->Close_Gui_Panels();
	//App->CL_Panels->Enable_All_Panels(false);

	Show_Material_Editor = 1;
}

// *************************************************************************
// *			Material_Editor_Gui:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void CL64_Materials::Material_Editor_Gui()
{
	int Index = App->CL_Properties->Current_Selected_Object;

	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(320, 350), ImGuiCond_FirstUseEver);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(239, 239, 239, 255));

	if (!ImGui::Begin("Material_Editor", &Show_Material_Editor, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar))
	{
		ImGui::End();
	}
	else
	{

		ImGui::Spacing();
		ImGui::Spacing();

		// ---------------------------------------------------------------- Material Editor

		ImGui::Text("Material Editor");
		ImGui::Separator();

		ImGui::Text(Material_FileName);
		ImGui::Text("Sub Materials %i", NumSubMesh);

		if (ImGui::BeginCombo("###MATS", BaseEntity->getMesh()->getSubMesh(item_current_idx)->getMaterialName().c_str()))
		{
			for (int n = 0; n < NumSubMesh; n++)
			{
				Ogre::String text = BaseEntity->getMesh()->getSubMesh(n)->getMaterialName().c_str();
				const bool is_selected = (item_current_idx == n);
				if (ImGui::Selectable(text.c_str(), is_selected))
					item_current_idx = n;

				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}

			ImGui::EndCombo();
		}

		Ogre::String text = BaseEntity->getMesh()->getSubMesh(item_current_idx)->getMaterialName().c_str();
		MatCurent = static_cast<Ogre::MaterialPtr> (Ogre::MaterialManager::getSingleton().getByName(text));

		//ImGui::Text("Texture:-  %s", Mat->getTechnique(0)->getPass(0)->getTextureUnitState(0)->getTextureName().c_str());

		char Texture[256];

		strcpy(Texture, MatCurent->getTechnique(0)->getPass(0)->getTextureUnitState(0)->getTextureName().c_str());

		const char* listbox_items[] = { Texture,"Scroll" };
		static int listbox_item_current = -1;

		if (ImGui::ListBox("##L1", &listbox_item_current, listbox_items, IM_ARRAYSIZE(listbox_items), 4))
		{
			if (listbox_item_current == 0) // Change Texture
			{
				bool test = App->CL_File_IO->OpenTextureFile((LPSTR)"Texture", NULL, NULL);
				if (test == 1)
				{
					Copy_Texture();
					MatCurent->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureName(App->CL_File_IO->Texture_FileName);
					MatCurent->getTechnique(0)->getPass(0)->setAmbient(1, 1, 1);
					//MatCurent->getTechnique(0)->getPass(0)->setPolygonMode(Ogre::PolygonMode::PM_POINTS);
					Update_MaterialFile();
					Update_MaterialFile();
					//Debug
				}
			}


			if (listbox_item_current == 1) // Scroll
			{
				MatCurent->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setScrollAnimation(0.01, 0.71);
				//Mat->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureRotate(Ogre::Radian(90));
				//Update_MaterialFile();
			}
		}

		if (listbox_item_current == 1)
		{
			Show_Scroll_Editor = 1;
			Scroll_Gui();
		}


		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		if (ImGui::Button("Close", ImVec2(100, 0)))
		{
			Update_MaterialFile();
			Show_Material_Editor = 0;
			Close_Material_Editor();
			Show_Material_Editor = 0;
		}

		ImGui::SameLine();

		if (ImGui::Button("Undo All", ImVec2(100, 0)))
		{
			int Count = 0;
			while (Count < NumSubMesh)
			{
				Ogre::String text = BaseEntity->getMesh()->getSubMesh(Count)->getMaterialName().c_str();
				Ogre::MaterialPtr  Mat = static_cast<Ogre::MaterialPtr> (Ogre::MaterialManager::getSingleton().getByName(text));

				MatClone[Count]->copyDetailsTo(Mat);

				Count++;
			}

			Update_MaterialFile();
		}

		ImGui::PopStyleColor();
		ImGui::End();
	}
}

// *************************************************************************
// *				Scroll_Gui:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void CL64_Materials::Scroll_Gui()
{
	int Index = App->CL_Properties->Current_Selected_Object;

	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(320, 350), ImGuiCond_FirstUseEver);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(239, 239, 239, 255));

	if (!ImGui::Begin("Scroll_Editor", &Show_Scroll_Editor, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar))
	{
		ImGui::End();
	}
	else
	{
		ImGui::Text("Scroll Editor");
		ImGui::Separator();

		ImGui::Text("Poop %f", MatCurent->getTechnique(0)->getPass(0)->getTextureUnitState(0)->getTextureUScroll());

		ImGui::PopStyleColor();
		ImGui::End();
	}
}

// *************************************************************************
// *		 Close_Material_Editor:- Terry and Hazel Flanigan 2023  	   *
// *************************************************************************
void CL64_Materials::Close_Material_Editor()
{
	Show_Material_Editor = 0;
	BaseEntity = nullptr;
	item_current_idx = 0;

	//App->CL_Panels->Enable_All_Panels(true);
	//App->CL_FileView->Show_FileView(true);
}

// *************************************************************************
// *			 Copy_Texture:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void CL64_Materials::Copy_Texture()
{
	char SourceFile[MAX_PATH] = { 0 };
	char DestinationFile[MAX_PATH] = { 0 };

	strcpy(SourceFile, App->CL_File_IO->Texture_Path_FileName);

	strcpy(DestinationFile, App->CL_Project->m_Main_Assets_Path);
	strcat(DestinationFile, App->CL_File_IO->Texture_FileName);


	//App->Say_Win(SourceFile);
	//App->Say_Win(DestinationFile);

	CopyFile(SourceFile, DestinationFile, false);
}


// *************************************************************************
// *			Update_MaterialFile:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void CL64_Materials::Update_MaterialFile()
{

	_chdir(App->CL_Project->m_Main_Assets_Path);

	int NumSubMesh = BaseEntity->getMesh()->getNumSubMeshes();

	Ogre::MaterialSerializer matSer;

	for (int n = 0; n < NumSubMesh; n++)
	{
		Ogre::MaterialPtr  Mat;

		Ogre::String text = BaseEntity->getMesh()->getSubMesh(n)->getMaterialName().c_str();
		Mat = static_cast<Ogre::MaterialPtr> (Ogre::MaterialManager::getSingleton().getByName(text));

		//BaseEntity->getMesh()->getSubMesh(n)->setMaterialName(text);
		matSer.queueForExport(Mat);
	}

	matSer.exportQueued(Material_FileName);

	int Index = App->CL_Properties->Current_Selected_Object;

	MaterialManager* mm = MaterialManager::getSingletonPtr();
	ResourceGroupManager* rgm = ResourceGroupManager::getSingletonPtr();

	int NumSubMesh2 = BaseEntity->getMesh()->getNumSubMeshes();

	Ogre::String text2 = BaseEntity->getMesh()->getSubMesh(0)->getMaterialName().c_str();
	ResourcePtr r = mm->getResourceByName(text2);
	String origin = r->getOrigin();
	String group = r->getGroup();
	Ogre::DataStreamPtr ds = NULL;
	

	if (!origin.empty())
	{
		ds = rgm->openResource(origin, group);

		mm->remove(r);
		mm->parseScript(ds, group);
		mm->load(r->getName(), group);
	}

	for (int k = 0; k < App->CL_Scene->V_Object[Index]->Object_Ent->getNumSubEntities(); k++)
	{
		SubEntity* se = App->CL_Scene->V_Object[Index]->Object_Ent->getSubEntity(k);
		se->setMaterial(se->getMaterial());
	}
}

// *************************************************************************
// *			Get_Material_Name:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void CL64_Materials::Get_Material_Name(Ogre::Entity* mEntity)
{
	Ogre::String text = mEntity->getMesh()->getSubMesh(0)->getMaterialName().c_str();
	Ogre::MaterialPtr  Mat = static_cast<Ogre::MaterialPtr> (Ogre::MaterialManager::getSingleton().getByName(text));

	strcpy(Material_FileName, Mat->getOrigin().c_str());
}
