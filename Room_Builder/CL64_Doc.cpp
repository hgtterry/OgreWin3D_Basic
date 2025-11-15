/*
Copyright (c) 2024 Inflanite_HGT W.T.Flanigan H.C.Flanigan

Room Builder

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
#include "resource.h"
#include "CL64_App.h"
#include "CL64_Doc.h"
#include "Room Builder.h"
#include "Structures.cpp"

#define MAX_PIXEL_SELECT_DIST (50)
#define MIN_ENTITY_SELECT_DIST (8.0f)
#define MAX_PIXEL_SELECT_THINGNAME (20)

#define AXIS_X	0x1
#define AXIS_Y	0x2
#define AXIS_Z	0x4

CL64_Doc::CL64_Doc(void)
{
    LastTemplateTypeName[0] = 0;

    Current_Level = NULL;
   
    BTemplate = NULL;
    CurBrush = NULL;
    TempShearTemplate = NULL;
    pSelBrushes = NULL;
    pTempSelBrushes = NULL;
    pSelFaces = NULL;
    Temp_SelFaces = NULL;

    App->CL_X_Maths->Vector3_Set(&SelectedGeoCenter,0, 0, 0);
    App->CL_X_Maths->Vector3_Set(&FinalPos, 0, 0, 0);
    App->CL_X_Maths->Vector3_Set(&FinalRot, 0, 0, 0);
    App->CL_X_Maths->Vector3_Set(&FinalScale, 0, 0, 0);
   
    mLastOp = 0;

    ScaleNum = 1;
    sides = 1;
    NumSelEntities = 0;
    mAdjustMode = ADJUST_MODE_FACE;
    mLockAxis = 0;
    mModeTool = ID_TOOLS_TEMPLATE;

	SelectLock = FALSE;
	TempEnt = FALSE;
    mCurrentTool = CURTOOL_NONE;
    mCurrentGroup = 0;

    flag_IsNewDocument = 0;
    flag_Track_Camera = 1;

    SelState = NOSELECTIONS;

}

CL64_Doc::~CL64_Doc(void)
{
}

// *************************************************************************
// *		    Init_Doc:- Terry and Hazel Flanigan 2025 	    		   *
// *************************************************************************
void CL64_Doc::Init_Doc()
{
    strcpy(LastTemplateTypeName, "Box");
   
    Current_Level = App->CL_Level->Level_Create();
  
	pSelBrushes = App->CL_X_SelBrushList->SelBrushList_Create();
	pTempSelBrushes = App->CL_X_SelBrushList->SelBrushList_Create();
	pSelFaces = App->CL_X_SelFaceList->SelFaceList_Create();

	SetLockAxis(0);	// Start with no axis locked

	// create our default box
	BrushTemplate_Box* pBoxTemplate;
   
	pBoxTemplate = App->CL_Level->Level_GetBoxTemplate();

	BTemplate = App->CL_X_BrushTemplate->BrushTemplate_CreateBox(pBoxTemplate);
    
	App->CL_X_Brush->Brush_Bound(BTemplate);
	CurBrush = BTemplate;

	mModeTool = ID_TOOLS_TEMPLATE;

	App->CL_X_Maths->Vector3_Clear(&SelectedGeoCenter);

    strcpy(App->CL_Export->mJustName, App->CL_Level->MTF_JustName_NoExt);

    CheckMenuItem(App->Menu_Map, ID_CAMERA_TRACKCAMERA, MF_BYCOMMAND | MF_CHECKED);

}

// *************************************************************************
// *		    Load_Default_Wad:- Terry and Hazel Flanigan 2025 	  	   *
// *************************************************************************
void CL64_Doc::Load_Wad_File(char* TXL_File)
{
    // Set the Wad path and file
    strcpy(App->CL_Level->TXL_PathAndFile,TXL_File);

    // Extract the file name from the path
   
    App->CL_Utilities->Get_FileName_FromPath(TXL_File, TXL_File);
    strcpy(App->CL_Level->TXL_Just_File_Name,App->CL_Utilities->JustFileName);
  
    // Load texture resources and scan the texture resource group
    App->CL_Resources->Load_Texture_Resources();
    App->CL_TXL_Editor->Scan_Textures_Resource_Group();

    // Create the TXL class and handle failure
    if (!App->CL_Level->Level_Create_TXL_Class())
    {
        App->Say_Win("Cannot create class");
    }
}

// *************************************************************************
// *	    	Set_Editor:- Terry and Hazel Flanigan 2025	    		   *
// *************************************************************************
void CL64_Doc::Editor_Set_Dlgs(int Set_State)
{
    if (Set_State == Enums::Editor_Dlgs_First_Brush)
    {
        App->CL_Top_Tabs->Enable_Select_Button(true, true);
        App->CL_Properties_Tabs->Enable_Tabs(true);
    }
}

// *************************************************************************
// *		   FindTextureLibrary- Terry and Hazel Flanigan 2024 	   	   *
// *************************************************************************
const char* CL64_Doc::FindTextureLibrary(char const* WadName)
{
	static char Path_And_File[MAX_PATH];
	strcpy(Path_And_File, App->RB_Directory_FullPath);
	strcat(Path_And_File, "\\Data\\Room_Builder\\");
	strcat(Path_And_File, WadName);

	bool test = App->CL_Utilities->Check_File_Exist(Path_And_File);
	if (test == 1)
	{
        strcpy(App->CL_Level->TXL_PathAndFile, Path_And_File);
		return Path_And_File;
	}
	else
	{
		App->Say_Win("File Does not Exist");

        strcpy(App->CL_Level->TXL_PathAndFile, "No_File");
		strcpy(Path_And_File, "");
		return Path_And_File;
	}
	
    strcpy(App->CL_Level->TXL_PathAndFile, "No_File");
	strcpy(Path_And_File, "");
	return Path_And_File;
}

// *************************************************************************
// *			        	AddBrushToWorld	                           	   *
// *************************************************************************
void CL64_Doc::AddBrushToWorld()
{
	if (TempEnt || !App->CL_X_Brush->Brush_IsSubtract(CurBrush))
	{
        Brush_Add_To_world();
	}
	else
	{
		OnBrushSubtractfromworld();
	}

    App->CL_Level->flag_Level_is_Modified = true;
}

// *************************************************************************
// *			        OnBrushSubtractfromworld	                   	   *
// *************************************************************************
void CL64_Doc::OnBrushSubtractfromworld()
{
    Brush* nb;
    BrushList* BList = App->CL_Level->Level_Get_Main_Brushes();

    auto& CL_Brush = App->CL_X_Brush; // Pointer to Brush Class

    if ((App->CL_Doc->mModeTool == ID_GENERALSELECT) && (CL_Brush->BrushList_Count(BList, BRUSH_COUNT_MULTI | BRUSH_COUNT_LEAF) < 2))
    {
        // cuts shouldn't start the list
        return;
    }

    //SetModifiedFlag();

    if (App->CL_Doc->mModeTool == ID_GENERALSELECT)
    {
        // put the brush at the very end of the list
        CL_Brush->BrushList_Remove(BList, App->CL_Doc->CurBrush);
        Brush_SetSubtract(App->CL_Doc->CurBrush, GE_TRUE);

        App->CL_X_SelBrushList->SelBrushList_RemoveAll(App->CL_Doc->pSelBrushes);
        CL_Brush->BrushList_Append(BList, App->CL_Doc->CurBrush);
    }
    else
    {
        nb = App->CL_X_Brush->Brush_Clone(App->CL_Doc->CurBrush);

        SetDefaultBrushTexInfo(nb);
        CL_Brush->Brush_Bound(nb);

        // add to current group
        Brush_SetGroupId(nb, App->CL_Doc->mCurrentGroup);

        CL_Brush->BrushList_Append(BList, nb);

        App->CL_Doc->CurBrush = nb;
        App->CL_Brush_X->Set_Brush_Faces_Name(App->CL_Doc->CurBrush);
    }
}

// *************************************************************************
// *			        	Brush_Add_To_world	                       	   *
// *************************************************************************
void CL64_Doc::Brush_Add_To_world()
{
	bool Placed;

	if (mModeTool != ID_TOOLS_TEMPLATE)
	{
		return;
	}

	Placed = false;

	Brush* NewBrush;
    T_Vec3* pTemplatePos;
    auto& CL_Brush = App->CL_X_Brush; // Pointer to Brush Class
    
	NewBrush = CL_Brush->Brush_Clone(App->CL_Doc->CurBrush);
   
	SetDefaultBrushTexInfo(NewBrush);
   
    CL_Brush->Brush_Bound(NewBrush);
	pTemplatePos = App->CL_Level->Level_GetTemplatePos(Current_Level);
    
    CL_Brush->Brush_Get_Center(NewBrush, pTemplatePos);

	// add to current group
	Brush_SetGroupId(NewBrush, mCurrentGroup);
   
	fdocFaceScales Scales;
   
	Scales.DrawScale = App->CL_Level->Level_GetDrawScale(Current_Level);
	Scales.LightmapScale = App->CL_Level->Level_GetLightmapScale(Current_Level);
    CL_Brush->Brush_EnumFaces(NewBrush, &Scales, fdocSetFaceScales);
   
	App->CL_Level->Level_AppendBrush(NewBrush);
   

	if (!CL_Brush->Brush_IsHollow(NewBrush) && !CL_Brush->Brush_IsMulti(NewBrush))
	{
		//App->CL_Doc->UpdateAllViews(Enums::UpdateViews_All);
	}
	else
	{
		//App->CL_Doc->UpdateAllViews(Enums::UpdateViews_All);
	}

    App->CL_Doc->CurBrush = NewBrush;
    App->CL_Brush_X->Set_Brush_Faces_Name(App->CL_Doc->CurBrush);

	Placed = true;

	if (Placed)
	{
        App->CL_Level->flag_Level_is_Modified = true;
	}
}

// *************************************************************************
// *         Set_Tool_GeneralSelect:- Terry and Hazel Flanigan 2023        *
// *************************************************************************
void CL64_Doc::Set_Tool_GeneralSelect()
{
	mCurrentTool = CURTOOL_NONE;
	mModeTool = ID_GENERALSELECT;
}

// *************************************************************************
// *           Do_General_Select_Dlg:- Terry and Hazel Flanigan 2023       *
// *************************************************************************
void CL64_Doc::Do_General_Select_Dlg(bool from_Insert)
{
    // TODO Check All references to this
    
    auto& CL_Brush = App->CL_X_Brush; // Pointer to Brush Class

    if (from_Insert == true)
    {
        if (CL_Brush->Get_Brush_Count() > 0)
        {
            Set_Tool_GeneralSelect();

            App->CL_Top_Tabs->Enable_TopBar_Brush_Buttons(false, false);
            App->CL_Top_Tabs->Enable_TopBar_Face_Buttons(false);
            App->CL_Top_Tabs->Enable_Select_Button(true, true);

            App->CL_Top_Tabs->Redraw_TopTabs_Dlg();

            return;
        }
    }

    if (CL_Brush->Get_Brush_Count() > 0)
    {
        Set_Tool_GeneralSelect();

        App->CL_Top_Tabs->Enable_TopBar_Brush_Buttons(true, false);
        App->CL_Top_Tabs->Enable_TopBar_Face_Buttons(true);
        App->CL_Top_Tabs->Enable_Select_Button(true, true);

        App->CL_Top_Tabs->Redraw_TopTabs_Dlg();
    }
}

// *************************************************************************
// *         UpdateAllViews:- Terry and Hazel Flanigan 2025                *
// *************************************************************************
void CL64_Doc::UpdateAllViews(int Update_Mode)
{
    if (Update_Mode == Enums::UpdateViews_Grids)
    {
        ViewVars* Save_View = App->CL_Editor_Map->Current_View;

        RedrawWindow(App->CL_Editor_Map->Top_Left_Window_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
        RedrawWindow(App->CL_Editor_Map->Top_Right_Window_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
        RedrawWindow(App->CL_Editor_Map->Bottom_Left_Window_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

        App->CL_Editor_Map->Current_View = Save_View;
    }

    if (Update_Mode == Enums::UpdateViews_3D)
    {
        int BC = App->CL_X_Brush->Get_Brush_Count();
        if (BC > 0)
        {
            App->CL_Doc->RebuildTrees();
            App->CL_Mesh_Mgr->Update_World(0); // Will Set Node Visable
        }
        else
        {
            if (App->CL_Mesh_Mgr->World_Ent && App->CL_Mesh_Mgr->World_Node)
            {
                App->CL_Mesh_Mgr->World_Node->setVisible(false);
            }
        }
    }

    if (Update_Mode == Enums::UpdateViews_All)
    {
        RedrawWindow(App->CL_Editor_Map->Top_Left_Window_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
        RedrawWindow(App->CL_Editor_Map->Top_Right_Window_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
        RedrawWindow(App->CL_Editor_Map->Bottom_Left_Window_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

        int BC = App->CL_X_Brush->Get_Brush_Count();
        if (BC > 0)
        {
            App->CL_Doc->RebuildTrees();
            App->CL_Mesh_Mgr->Update_World(0); // Will Set Node Visable
        }
        else
        {
            if (App->CL_Mesh_Mgr->World_Ent && App->CL_Mesh_Mgr->World_Node)
            {
                App->CL_Mesh_Mgr->World_Node->setVisible(false);
            }
        }
    }
}

// *************************************************************************
// *			        SetDefaultBrushTexInfo	                       	   *
// *************************************************************************
void CL64_Doc::SetDefaultBrushTexInfo(Brush* b)
{
    const char* TexName;
    BrushTexSetData CallbackData;

    TexName = App->CL_Properties_Textures->m_CurrentTexture;
    CallbackData.pDoc = this;
    CallbackData.TexName = TexName;

    auto& CL_Brush = App->CL_X_Brush; // Pointer to Brush Class
    //	Brush_SetName(b, TexName);
    CL_Brush->Brush_SetName(b, LastTemplateTypeName);

    if (CL_Brush->Brush_IsMulti(b))
    {
        CL_Brush->BrushList_EnumLeafBrushes(CL_Brush->Brush_GetBrushList(b), &CallbackData, ::BrushTexSetCB);
    }
    else
    {
        BrushTexSetCB(b, &CallbackData);
    }
}

// *************************************************************************
// *			                 GetDibBitmap	                       	   *
// *************************************************************************
WadFileEntry* CL64_Doc::GetDibBitmap(const char* Name)
{
    return App->CL_Level->Level_GetWadBitmap(Name);
}

// *************************************************************************
// *			                 PointToLineDist	                   	   *
// *************************************************************************
static float PointToLineDist(POINT const* ptFrom,POINT const* ptLine1,POINT const* ptLine2)
{
    float xkj, ykj;
    float xlk, ylk;
    float denom;
    float dist;

    xkj = (float)(ptLine1->x - ptFrom->x);
    ykj = (float)(ptLine1->y - ptFrom->y);
    xlk = (float)(ptLine2->x - ptLine1->x);
    ylk = (float)(ptLine2->y - ptLine1->y);
    denom = (xlk * xlk) + (ylk * ylk);
    if (denom < .0005f)
    {
        // segment ends coincide
        dist = xkj * xkj + ykj * ykj;
    }
    else
    {
        float t;
        float xfac, yfac;

        t = -(xkj * xlk + ykj * ylk) / denom;
        t = std::max(t, 0.0f);
        t = std::min(t, 1.0f);
        xfac = xkj + t * xlk;
        yfac = ykj + t * ylk;
        dist = xfac * xfac + yfac * yfac;
    }

    return (float)sqrt(dist);
}

// *************************************************************************
// *			            FindClosestBrushCB	                       	   *
// *************************************************************************
static  signed int FindClosestBrushCB(Brush* pBrush, void* pVoid)
{
    FindClosestInfo* fci = (FindClosestInfo*)pVoid;

   // if (fci->pDoc->BrushIsVisible(pBrush))
    {
        // for each face...
        for (int iFace = 0; iFace < App->CL_X_Brush->Brush_GetNumFaces(pBrush); ++iFace)
        {
            POINT pt1, pt2;
            Face* pFace = App->CL_X_Brush->Brush_GetFace(pBrush, iFace);
            const T_Vec3* FacePoints = App->CL_X_Face->Face_GetPoints(pFace);
            int	NumPoints = App->CL_X_Face->Face_GetNumPoints(pFace);

            // Starting with the edge formed by the last point and the first point,
            // determine distance from mouse cursor pos to the edge.
            pt1 = App->CL_Render->Render_OrthoWorldToView(fci->v, &FacePoints[NumPoints - 1]);
            for (int iPoint = 0; iPoint < NumPoints; ++iPoint)
            {
                float Dist;

                pt2 = App->CL_Render->Render_OrthoWorldToView(fci->v, &FacePoints[iPoint]);
                Dist = PointToLineDist(fci->ptFrom, &pt1, &pt2);
                if (Dist < *fci->pMinEdgeDist)
                {
                    *fci->pMinEdgeDist = Dist;
                    *fci->ppFoundBrush = pBrush;
                }
                pt1 = pt2;	// next edge...
            }
        }
    }

    return GE_TRUE;
}

#pragma warning (disable:4100)
static signed int ResetSelectedFacesCB(Brush* b, void* pVoid)
{
    int	i;

    for (i = 0; i < App->CL_X_Brush->Brush_GetNumFaces(b); i++)
    {
        Face* pFace;

        pFace = App->CL_X_Brush->Brush_GetFace(b, i);
        App->CL_X_Face->Face_SetSelected(pFace, false);
    }
    return GE_TRUE;
}
#pragma warning (default:4100)

// *************************************************************************
// *			        	   SelectOrtho                             	   *
// *************************************************************************
void CL64_Doc::SelectOrtho(POINT point, ViewVars* v)
{
    point.y = point.y - 5;

    Brush* pMinBrush;
    float Dist;
    int FoundThingType;

   if ((GetAsyncKeyState(VK_SHIFT) & 0x8000) == 0)
    {
        ResetAllSelections();
    }

    FoundThingType = FindClosestThing(&point, v, &pMinBrush,&Dist);

    if ((FoundThingType != fctNOTHING) && (Dist <= 25)) //MAX_PIXEL_SELECT_DIST))
    {
        if (FoundThingType == fctBRUSH)
        {
            if (pMinBrush->GroupId == Enums::Brushs_ID_Players)
            {
               
            }
			else
			{
				App->CL_Brush_X->Select_Brush_Editor(pMinBrush);
                App->CL_Ogre->OGL_Listener->Show_Visuals(true);

                EnableMenuItem(App->Menu_Map, ID_EDIT_DELETE, MF_ENABLED);
                // Entity Selected in OnSelchangeBrushlist
			}
        } 
    }
    else
    {
        if (App->CL_X_Face_Editor->flag_FaceDlg_Active == true)
        {
            App->CL_X_Face_Editor->Close_Faces_Dialog();
        }

        App->CL_Panels->Deselect_All_Brushes_Update_Dlgs();
        App->CL_Top_Tabs->Update_Faces_Combo();
        App->CL_Ogre->OGL_Listener->Show_Visuals(false);
        EnableMenuItem(App->Menu_Map, ID_EDIT_DELETE, MF_DISABLED | MF_GRAYED);
    }

}

// *************************************************************************
// *           ResetAllSelections:- Terry and Hazel Flanigan 2025          *
// *************************************************************************
void CL64_Doc::ResetAllSelections(void)
{
    ResetAllSelectedFaces();
    ResetAllSelectedBrushes();
}

// *************************************************************************
// *          ResetAllSelectedFaces:- Terry and Hazel Flanigan 2025        *
// *************************************************************************
void CL64_Doc::ResetAllSelectedFaces(void)
{
    App->CL_X_Brush->BrushList_EnumLeafBrushes(App->CL_Level->Level_Get_Main_Brushes(), NULL, ResetSelectedFacesCB);
    App->CL_X_SelFaceList->SelFaceList_RemoveAll(pSelFaces);
}

// *************************************************************************
// *         ResetAllSelectedBrushes:- Terry and Hazel Flanigan 2025       *
// *************************************************************************
void CL64_Doc::ResetAllSelectedBrushes(void)
{
    App->CL_X_SelBrushList->SelBrushList_RemoveAll(pSelBrushes);

   // CurBrush = BTemplate; // hgtterry Check
}

// *************************************************************************
// *           DoBrushSelection:- Terry and Hazel Flanigan 2025            *
// *************************************************************************
void CL64_Doc::DoBrushSelection(Brush* pBrush, BrushSel	nSelType) //	brushSelToggle | brushSelAlways)
{
    int ModelId = 0;
    signed int ModelLocked;
   /* ModelInfo_Type* ModelInfo;
    GroupListType* Groups;*/
    int GroupId = 0;
    signed int GroupLocked;
    BrushList* BList;
    Brush* pBParent = NULL;

    BList = App->CL_Level->Level_Get_Main_Brushes();

   if (App->CL_X_Brush->Brush_GetParent(BList, pBrush, &pBParent))
    {
        pBrush = pBParent;
    }

    ModelLocked = false;
    GroupLocked = false;

    if (pBrush)
    {
        if (nSelType == brushSelToggle && BrushIsSelected(pBrush))
        {

        }
        else
        {
           App->CL_X_SelBrushList->SelBrushList_Add(pSelBrushes, pBrush);
        }
    }
}

// *************************************************************************
// *            BrushIsSelected:- Terry and Hazel Flanigan 2025            *
// *************************************************************************
signed int CL64_Doc::BrushIsSelected(Brush const* pBrush)
{
    return App->CL_X_SelBrushList->SelBrushList_Find(pSelBrushes, pBrush);
}

// *************************************************************************
// *            FindClosestThing:- Terry and Hazel Flanigan 2025           *
// *************************************************************************
int CL64_Doc::FindClosestThing(POINT const* ptFrom, ViewVars* v, Brush** ppMinBrush, float* pDist)
{
    int rslt;

    signed int FoundBrush;
    float MinEdgeDist;
    Brush* pMinBrush;

    rslt = fctNOTHING;

    FoundBrush = FindClosestBrush(ptFrom, v, &pMinBrush, &MinEdgeDist);
   
    
   if (FoundBrush)
    {
        *pDist = MinEdgeDist;
        if (ppMinBrush != NULL)
            *ppMinBrush = pMinBrush;
        rslt = fctBRUSH;
    }
    return rslt;
}

// *************************************************************************
// *            FindClosestBrush:- Terry and Hazel Flanigan 2025           *
// *************************************************************************
signed int CL64_Doc::FindClosestBrush(POINT const* ptFrom, ViewVars* v, Brush** ppFoundBrush, float* pMinEdgeDist)
{
    // determine the distance to the closest brush edge in the current view.
    FindClosestInfo	fci;

    *pMinEdgeDist = FLT_MAX;
    *ppFoundBrush = NULL;

    fci.pDoc = this;
    fci.v = v;
    fci.ppFoundBrush = ppFoundBrush;
    fci.pMinEdgeDist = pMinEdgeDist;
    fci.ptFrom = ptFrom;

    App->CL_X_Brush->BrushList_EnumLeafBrushes(App->CL_Level->Level_Get_Main_Brushes(), &fci, ::FindClosestBrushCB);

    return	(*ppFoundBrush) ? GE_TRUE : GE_FALSE;
}

enum
{
    Group_ShowAll,
    Group_ShowVisible,
    Group_ShowCurrent
};

// *************************************************************************
// *            BrushIsVisible:- Terry and Hazel Flanigan 2025           *
// *************************************************************************
signed int CL64_Doc::BrushIsVisible(const Brush* pBrush) const
{
    //int			GroupId;

    //if (!App->CL_Brush->Brush_IsVisible(pBrush))
    //{
    //    return GE_FALSE;
    //}
    //GroupId = 0;// App->CL_Brush->Brush_GetGroupId(pBrush);

    //switch (Level_GetGroupVisibility(App->CL_Doc->pLevel))
    //{
    //case Group_ShowAll:
    //    return GE_TRUE;

    //case Group_ShowCurrent:
    //    return (GroupId == App->CL_Doc->mCurrentGroup);

    //case Group_ShowVisible:
    //    return Group_IsVisible(App->CL_Level->Level_GetGroups(App->CL_Doc->pLevel), GroupId);

    //default:
    //    assert(0);
    //    return GE_FALSE;
    //}
}/* CFusionDoc::BrushIsVisible */

// *************************************************************************
// *         fdocBrushCSGCallback:- Terry and Hazel Flanigan 2025          *
// *************************************************************************
static signed int fdocBrushCSGCallback(const Brush* pBrush, void* lParam)
{
    CL64_Doc* pDoc = (CL64_Doc*)lParam;

    // hgtterry Finish
    return 1;// (pDoc->BrushIsVisible(pBrush) && (!Brush_IsHint(pBrush)) && (!Brush_IsClip(pBrush)));
}

// *************************************************************************
// *             RebuildTrees:- Terry and Hazel Flanigan 2025              *
// *************************************************************************
void CL64_Doc::RebuildTrees(void)
{
    int	CurId = 0;
  
    BrushList* BList;
   
    BList = App->CL_Level->Level_Get_Main_Brushes();
    //SetModifiedFlag();

    App->CL_X_Brush->BrushList_ClearAllCSG(BList);

    App->CL_X_Brush->BrushList_DoCSG(BList, CurId, ::fdocBrushCSGCallback, this);

}

// *************************************************************************
// *         TempCopySelectedBrushes:- Terry and Hazel Flanigan 2025       *
// *************************************************************************
void CL64_Doc::TempCopySelectedBrushes(void)
{
    int		i;
    int NumSelBrushes;

    NumSelBrushes = App->CL_X_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes);

    App->CL_X_SelBrushList->SelBrushList_RemoveAll(App->CL_Doc->pTempSelBrushes);

    // make copies of selected brushes
    for (i = 0; i < NumSelBrushes; i++)
    {
        Brush* pBrush, * pClone;

        pBrush = App->CL_X_SelBrushList->SelBrushList_GetBrush(App->CL_Doc->pSelBrushes, i);
        pClone = App->CL_X_Brush->Brush_Clone(pBrush);
        App->CL_Level->Level_AppendBrush(pClone);
        App->CL_X_SelBrushList->SelBrushList_Add(App->CL_Doc->pTempSelBrushes, pClone);
    }
}

// *************************************************************************
// *                 LockAxis:- Terry and Hazel Flanigan 2025              *
// *************************************************************************
void CL64_Doc::LockAxis(T_Vec3* pWP)
{
    int mLockAxis;
   
    mLockAxis = App->CL_Doc->GetLockAxis();

    if (mLockAxis & AXIS_X)	pWP->x = 0.0f;
    if (mLockAxis & AXIS_Y)	pWP->y = 0.0f;
    if (mLockAxis & AXIS_Z)	pWP->z = 0.0f;
}

// *************************************************************************
// *              GetLockAxis:- Terry and Hazel Flanigan 2025              *
// *************************************************************************
int	CL64_Doc::GetLockAxis(void) 
{ 
    return mLockAxis; 
}

enum LastBrushAction
{
    BRUSH_MOVE,
    BRUSH_ROTATE,
    BRUSH_SCALE,
    BRUSH_SHEAR,
    BRUSH_RESET,
    BRUSH_DIALOG
};

// *************************************************************************
// *         MoveSelectedBrushes:- Terry and Hazel Flanigan 2025           *
// *************************************************************************
void CL64_Doc::MoveSelectedBrushes(T_Vec3 const* v)
{
    MoveSelectedBrushList(pTempSelBrushes, v);
}

// *************************************************************************
// *         MoveSelectedBrushList:- Terry and Hazel Flanigan 2025         *
// *************************************************************************
void CL64_Doc::MoveSelectedBrushList(SelBrushList* pList, T_Vec3 const* v)
{
    int		i;
    int NumBrushes;
    mLastOp = BRUSH_MOVE;

    App->CL_X_Maths->Vector3_Add(&SelectedGeoCenter, v, &SelectedGeoCenter);
    App->CL_X_Maths->Vector3_Add(v, &FinalPos, &FinalPos);

    NumBrushes = App->CL_X_SelBrushList->SelBrushList_GetSize(pList);
    for (i = 0; i < NumBrushes; i++)
    {
        Brush* pBrush;

        pBrush = App->CL_X_SelBrushList->SelBrushList_GetBrush(pList, i);
        App->CL_X_Brush->Brush_Move(pBrush, v);

    }
}

// *************************************************************************
// *         RotateSelectedBrushes:- Terry and Hazel Flanigan 2025         *
// *************************************************************************
void CL64_Doc::RotateSelectedBrushes(const ViewVars* view, T_Vec3 const* v)
{
    RotateSelectedBrushList(view,App->CL_Doc->pTempSelBrushes, v);
}

// *************************************************************************
// *         RotateSelectedBrushList:- Terry and Hazel Flanigan 2025       *
// *************************************************************************
void CL64_Doc::RotateSelectedBrushList(const ViewVars* view, SelBrushList* pList,T_Vec3 const* v)
{
    int i;
    Matrix3d rm;
    T_Vec3 RotationPoint;
    int NumBrushes = App->CL_X_SelBrushList->SelBrushList_GetSize(pList);

    GetRotationPoint(&RotationPoint);
    App->CL_X_Maths->XForm3d_SetIdentity(&rm);

    App->CL_X_Maths->Vector3_Add(v, &App->CL_Doc->FinalRot, &App->CL_Doc->FinalRot);
    App->CL_X_Maths->XForm3d_SetEulerAngles(&rm, v);

    for (i = 0; i < NumBrushes; i++)
    {
        Brush* pBrush = App->CL_X_SelBrushList->SelBrushList_GetBrush(pList, i);
       
		if (pBrush->GroupId == Enums::Brushs_ID_Evirons)
		{
            int Delta = 1;
			int Index = App->CL_Entities->GetIndex_By_Name(pBrush->Name);

            if (Index > -1)
            {
                auto& m_object = App->CL_Scene->B_Object[Index];

                switch (view->ViewType)
                {
                case VIEWTOP:
                {
                    if (v->y > 0)
                    {
                        m_object->Mesh_Rot.y += Delta;
                        m_object->Object_Node->rotate(Ogre::Quaternion(Ogre::Degree(1.0f), Ogre::Vector3(0, 1, 0)), Ogre::Node::TransformSpace::TS_LOCAL);
                    }

                    if (v->y < 0)
                    {
                        m_object->Mesh_Rot.y -= Delta;
                        m_object->Object_Node->rotate(Ogre::Quaternion(Ogre::Degree(-1.0f), Ogre::Vector3(0, 1, 0)), Ogre::Node::TransformSpace::TS_LOCAL);
                    }
                    break;
                }
                case VIEWFRONT:
                {
                    if (v->z > 0)
                    {
                        m_object->Mesh_Rot.z += Delta;
                        m_object->Object_Node->rotate(Ogre::Quaternion(Ogre::Degree(1.0f), Ogre::Vector3(0, 0, 1)), Ogre::Node::TransformSpace::TS_WORLD);
                    }

                    if (v->z < 0)
                    {
                        m_object->Mesh_Rot.z -= Delta;
                        m_object->Object_Node->rotate(Ogre::Quaternion(Ogre::Degree(-1.0f), Ogre::Vector3(0, 0, 1)), Ogre::Node::TransformSpace::TS_WORLD);
                    }
                    break;
                }
                case VIEWSIDE:
                {
                    if (v->x > 0)
                    {
                        m_object->Mesh_Rot.x += Delta;
                        m_object->Object_Node->rotate(Ogre::Quaternion(Ogre::Degree(1.0f), Ogre::Vector3(1, 0, 0)), Ogre::Node::TransformSpace::TS_WORLD);
                    }

                    if (v->x < 0)
                    {
                        m_object->Mesh_Rot.x -= Delta;
                        m_object->Object_Node->rotate(Ogre::Quaternion(Ogre::Degree(-1.0f), Ogre::Vector3(1, 0, 0)), Ogre::Node::TransformSpace::TS_WORLD);
                    }
                    break;
                }
                default:
                {
                    App->Say("No View", "RotateSelectedBrushList");
                }
                }

                // TODO Test
                App->CL_Brush_X->Set_Brush_From_Entity_ByName(pBrush->Name, false);
            }
		}
        else
        {
            App->CL_X_Brush->Brush_Rotate(pBrush, &rm, &RotationPoint);
        }
      
    }
}

// *************************************************************************
// *            ScaleSelectedBrushes:- Terry and Hazel Flanigan 2025       *
// *************************************************************************
void CL64_Doc::ScaleSelectedBrushes(T_Vec3* ScaleVector)
{
    App->CL_Doc->mLastOp = BRUSH_SCALE;

    T_Vec3	VecOrigin = { 0.0f, 0.0f, 0.0f };

    T_Vec3 MoveTo;
    T_Vec3 MoveBack;

    App->CL_X_Maths->Vector3_Subtract(&VecOrigin, &App->CL_Doc->SelectedGeoCenter, &MoveTo);
    App->CL_X_Maths->Vector3_Subtract(&App->CL_Doc->SelectedGeoCenter, &VecOrigin, &MoveBack);

    if (App->CL_Doc->mModeTool == ID_TOOLS_TEMPLATE)
    {
       /* Brush_Scale3d(App->CLSB_Doc->CurBrush, ScaleVector);
        if (Brush_IsMulti(App->CLSB_Doc->CurBrush))
        {
            BrushList_ClearCSGAndHollows((BrushList*)App->CL_Brush->Brush_GetBrushList(App->CLSB_Doc->CurBrush), Brush_GetModelId(App->CLSB_Doc->CurBrush));
            BrushList_RebuildHollowFaces((BrushList*)App->CL_Brush->Brush_GetBrushList(App->CLSB_Doc->CurBrush), Brush_GetModelId(App->CLSB_Doc->CurBrush), ::fdocBrushCSGCallback, this);
        }*/
    }
    else
    {
        int i;
        int NumSelBrushes = App->CL_X_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes);

        for (i = 0; i < NumSelBrushes; ++i)
        {
            Brush* pBrush;

            pBrush = App->CL_X_SelBrushList->SelBrushList_GetBrush(App->CL_Doc->pSelBrushes, i);

            App->CL_X_Brush->Brush_Move(pBrush, &MoveTo);
            App->CL_X_Brush->Brush_Scale3d(pBrush, ScaleVector);
            App->CL_X_Brush->Brush_Move(pBrush, &MoveBack);

            if (App->CL_X_Brush->Brush_IsMulti(pBrush))
            {
                App->CL_X_Brush->BrushList_ClearCSGAndHollows((BrushList*)App->CL_X_Brush->Brush_GetBrushList(pBrush), App->CL_X_Brush->Brush_GetModelId(pBrush));
                App->CL_X_Brush->BrushList_RebuildHollowFaces((BrushList*)App->CL_X_Brush->Brush_GetBrushList(pBrush), App->CL_X_Brush->Brush_GetModelId(pBrush), ::fdocBrushCSGCallback, this);
            }
        }
    }
}

// *************************************************************************
// *         GetRotationPoint:- Terry and Hazel Flanigan 2025              *
// *************************************************************************
void CL64_Doc::GetRotationPoint(T_Vec3* pVec)
{
        *pVec = App->CL_Doc->SelectedGeoCenter;
}

static float ComputeSnap(float Cur, float Delta, float SnapSize)
{
    float Target;
    float SnapDelta;
    float Remainder;

    Target = Cur + Delta;
    Remainder = (float)fmod(Target, SnapSize);
    if (fabs(Remainder) < (SnapSize / 2.0f))
    {
        SnapDelta = -Remainder;
    }
    else
    {
        if (Target < 0.0f)
        {
            SnapDelta = -(SnapSize + Remainder);
        }
        else
        {
            SnapDelta = SnapSize - Remainder;
        }
    }
    return SnapDelta;
}

static float SnapSide(float CurMin, float CurMax, float Delta, float SnapSize)
{
    float MinDelta, MaxDelta;

    MinDelta = ComputeSnap(CurMin, Delta, SnapSize);
    MaxDelta = ComputeSnap(CurMax, Delta, SnapSize);

    return (fabs(MinDelta) < fabs(MaxDelta)) ? MinDelta : MaxDelta;
}

// *************************************************************************
// *            DoneMovingBrushes:- Terry and Hazel Flanigan 2025         *
// *************************************************************************
void CL64_Doc::DoneMovingBrushes()
{
 
    App->CL_Level->flag_Level_is_Modified = true;

    if (App->CL_X_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes) > 0)// || ModeTool == ID_TOOLS_TEMPLATE)
    {
        float fSnapSize;
        const T_Vec3* vMin, * vMax;
        const Box3d* pBox;
        T_Vec3 SnapDelta;
        signed int SnapX, SnapY, SnapZ;

        fSnapSize = 1.0f;

        if (App->CL_Level->flag_UseGrid == 1)
        {
            fSnapSize = App->CL_Level->Level_GetGridSnapSize(Current_Level);
        }

        // do the snap thing...
        pBox = App->CL_X_Brush->Brush_GetBoundingBox(App->CL_Doc->CurBrush);
        vMin = App->CL_X_Box->Box3d_GetMin(pBox);
        vMax = App->CL_X_Box->Box3d_GetMax(pBox);
        App->CL_X_Maths->Vector3_Clear(&SnapDelta);
       
        SnapX = (App->CL_Doc->FinalPos.x != 0.0f);
        SnapY = (App->CL_Doc->FinalPos.y != 0.0f);
        SnapZ = (App->CL_Doc->FinalPos.z != 0.0f);

        /*if ((ModeTool == ID_TOOLS_TEMPLATE) || IsCopying)
        {
            geVec3d_Clear(&App->CLSB_Doc->FinalPos);
        }*/

        if (SnapX)
        {
            SnapDelta.x = ::SnapSide(vMin->x, vMax->x, App->CL_Doc->FinalPos.x, fSnapSize);
        }
        if (SnapY)
        {
            SnapDelta.y = ::SnapSide(vMin->y, vMax->y, App->CL_Doc->FinalPos.y, fSnapSize);
        }
        if (SnapZ)
        {
            SnapDelta.z = ::SnapSide(vMin->z, vMax->z, App->CL_Doc->FinalPos.z, fSnapSize);
        }
       /* if (ModeTool == ID_TOOLS_TEMPLATE)
        {
            App->CL_Doc->FinalPos = SnapDelta;
        }
        else*/
        {
            App->CL_X_Maths->Vector3_Add(&App->CL_Doc->FinalPos, &SnapDelta, &App->CL_Doc->FinalPos);
        }
    }

	App->CL_Doc->DoneMove();
	App->CL_Doc->UpdateSelected();

    /*if ((ModeTool == ID_TOOLS_TEMPLATE) ||
        ((App->CLSB_Doc->GetSelState() & ANYENTITY) && (!(App->CLSB_Doc->GetSelState() & ANYBRUSH))))
    {
        App->CLSB_Doc->UpdateAllViews(UAV_ALL3DVIEWS, NULL);
    }
    else
    {
        App->CLSB_Doc->UpdateAllViews(UAV_ALL3DVIEWS | REBUILD_QUICK, NULL);
    }*/
}

#define	M_PI		((float)3.14159265358979323846f)
#define Units_DegreesToRadians(d) ((((float)(d)) * M_PI) / 180.0f)
// *************************************************************************
// *              DoneRotate:- Terry and Hazel Flanigan 2025              *
// *************************************************************************
void CL64_Doc::DoneRotate(void)
{
    int			i;
    float	RSnap = 1;
    Matrix3d		rm;
    T_Vec3 RotationPoint;
    T_Vec3 TemplateReversalRot;

    TemplateReversalRot = FinalRot;

    mLastOp = BRUSH_ROTATE;

    TempDeleteSelected();

    App->CL_Doc->TempCopySelectedBrushes();

    App->CL_Doc->GetRotationPoint(&RotationPoint);

    if (App->CL_Level->flag_UseGrid == 1)
    {
        RSnap = Units_DegreesToRadians((float)App->CL_Level->Level_GetRotationSnap(Current_Level));
        FinalRot.x = ((float)((int)(FinalRot.x / RSnap))) * RSnap;
        FinalRot.y = ((float)((int)(FinalRot.y / RSnap))) * RSnap;
        FinalRot.z = ((float)((int)(FinalRot.z / RSnap))) * RSnap;
    }

    
    if (mModeTool == ID_TOOLS_TEMPLATE)
    {
        App->CL_X_Maths->Vector3_Subtract(&FinalRot, &TemplateReversalRot, &FinalRot);
    }

    App->CL_X_Maths->XForm3d_SetEulerAngles(&rm, &FinalRot);

    if (mModeTool == ID_TOOLS_TEMPLATE)
    {
       
        App->CL_X_Brush->Brush_Rotate(CurBrush, &rm, &RotationPoint);
        return;
    }
   
    int NumSelBrushes = App->CL_X_SelBrushList->SelBrushList_GetSize(pSelBrushes);

    for (i = 0; i < NumSelBrushes; i++)
    {
        Brush* pBrush;

        pBrush = App->CL_X_SelBrushList->SelBrushList_GetBrush(pTempSelBrushes, i);
       
        //App->Say_Int(pBrush->GroupId);

        if (pBrush->GroupId == Enums::Brushs_ID_Evirons)
        {
            // TODO Test
            //App->BeepBeep();
            App->CL_Brush_X->Set_Brush_From_Entity_ByName(pBrush->Name, true);
            int Index = App->CL_Entities->GetIndex_By_Name(pBrush->Name);

            if (Index > -1)
            {
                App->CL_Physics->Update_Object_Physics(Index);
            }
        }
        else
        {
            App->CL_X_Brush->Brush_Rotate(pBrush, &rm, &RotationPoint);
        }

    }

    if (i < NumSelBrushes)
    {
        App->CL_Doc->TempDeleteSelected();
    }
    else
    {
        BrushList* BList = App->CL_Level->Level_Get_Main_Brushes();

        for (i = 0; i < NumSelBrushes; i++)
        {
            // Replace the sel list brushes with the TSelList brushes
            Brush* TempBrush, * OldBrush;

            TempBrush = App->CL_X_SelBrushList->SelBrushList_GetBrush(pTempSelBrushes, 0);
            OldBrush = App->CL_X_SelBrushList->SelBrushList_GetBrush(pSelBrushes, 0);
           
            App->CL_X_Brush->BrushList_Remove(BList, TempBrush);
            App->CL_X_Brush->BrushList_InsertAfter(BList, OldBrush, TempBrush);
            App->CL_X_Brush->BrushList_Remove(BList, OldBrush);

            App->CL_X_SelBrushList->SelBrushList_Remove(pSelBrushes, OldBrush);
            App->CL_X_SelBrushList->SelBrushList_Remove(pTempSelBrushes, TempBrush);

            App->CL_X_SelBrushList->SelBrushList_Add(pSelBrushes, TempBrush);

            //App->CL_Brush->BrushList_Remove(OldBrush->BList, OldBrush);
        }
    }
   
    UpdateSelected();
   
    App->CL_X_Maths->Vector3_Clear(&FinalRot);

}

// *************************************************************************
// *			   DoneMove:- Terry and Hazel Flanigan 2025                *
// *************************************************************************
void CL64_Doc::DoneMove(void)
{
	mLastOp = BRUSH_MOVE;

	TempDeleteSelected();

	if (mModeTool == ID_TOOLS_TEMPLATE)
	{
		return;
	}

	int	i = 0; // Can only select 1 item at the moment

	Brush* pBrush;
	pBrush = App->CL_X_SelBrushList->SelBrushList_GetBrush(pSelBrushes, i);
    if (!pBrush) return; // Exit if no brush is selected
    
    App->CL_X_Brush->Brush_Move(pBrush, &FinalPos);
    App->CL_X_SelBrushList->SelBrushList_Center(App->CL_Doc->pSelBrushes, &App->CL_Doc->SelectedGeoCenter);
    T_Vec3 CenterOfSelection = App->CL_Doc->SelectedGeoCenter;

    // Special Brushes 
    switch (pBrush->GroupId)
    {
    case Enums::Brushs_ID_Players: // Player Brush Selected
    {
        App->CL_Scene->B_Player[0]->StartPos.x = CenterOfSelection.x;
        App->CL_Scene->B_Player[0]->StartPos.y = CenterOfSelection.y;
        App->CL_Scene->B_Player[0]->StartPos.z = CenterOfSelection.z;

        App->CL_Physics->Reset_Physics();

        break;
    }

    case Enums::Brushs_ID_Evirons: // Environment Brush Selected
    {
        char Name[MAX_PATH]{ 0 };
        strcpy(Name, pBrush->Name);

        int Index = App->CL_Entities->GetIndex_By_Name(Name);

        if (Index > -1)
        {
            Ogre::Vector3 Ogre_Center = App->CL_Scene->B_Object[Index]->Object_Node->getPosition();
            Ogre::Vector3 Ogre_BBCenter = App->CL_Scene->B_Object[Index]->Object_Node->_getWorldAABB().getCenter();

            float True_Center = (App->CL_Scene->B_Object[Index]->Object_Node->_getWorldAABB().getSize().y) / 2;

            // Adjust the center of selection based on the bounding box center
            if (fabs(Ogre_BBCenter.y - Ogre_Center.y) > 1)
            {
                CenterOfSelection.y = CenterOfSelection.y - True_Center;
            }

            // Update the position of the object in the scene
            App->CL_Scene->B_Object[Index]->Object_Node->setPosition(CenterOfSelection.x, CenterOfSelection.y, CenterOfSelection.z);
            App->CL_Scene->B_Object[Index]->Mesh_Pos = { CenterOfSelection.x, CenterOfSelection.y, CenterOfSelection.z };

            App->CL_Brush_X->Set_Brush_From_Entity_ByName(pBrush->Name, true);

            if (App->CL_Scene->B_Object[Index]->Shape == Enums::Shape_TriMesh)
            {
                App->CL_Scene->B_Object[Index]->Phys_Body->getWorldTransform().setOrigin(btVector3(CenterOfSelection.x, CenterOfSelection.y, CenterOfSelection.z));
            }
            else
            {
                App->CL_Physics->Update_Object_Physics(Index);
            }
        }

        break;
    }

    default:
        break;
    }

	UpdateSelected();

	App->CL_X_Maths->Vector3_Clear(&FinalPos);
}

// *************************************************************************
// *          TempDeleteSelected:- Terry and Hazel Flanigan 2025           *
// *************************************************************************
BOOL CL64_Doc::TempDeleteSelected(void)
{
    BOOL	ret;
    int		i;
    int		NumTSelBrushes = App->CL_X_SelBrushList->SelBrushList_GetSize(pTempSelBrushes);

    for (ret = FALSE, i = 0; i < NumTSelBrushes; i++)
    {
        Brush* pBrush;

        pBrush = App->CL_X_SelBrushList->SelBrushList_GetBrush(pTempSelBrushes, 0);

        App->CL_Level->Level_RemoveBrush(pBrush);
        App->CL_X_SelBrushList->SelBrushList_Remove(pTempSelBrushes, pBrush);
        App->CL_X_Brush->Brush_Destroy(&pBrush);
        ret = TRUE;
    }

    return	ret;
}

// *************************************************************************
// *             ScaleSelected:- Terry and Hazel Flanigan 2025             *
// *************************************************************************
void CL64_Doc::ScaleSelected(int dx, int dy)
{
    //smooth out the zoom scale curve with a scalar
    float	ZoomInv = App->CL_Editor_Map->Current_View->ZoomFactor;// Render_GetZoom(VCam);

    ZoomInv = (ZoomInv > .5) ? 0.5f / ZoomInv : 1.0f;

    // negated here because Brush_Resize is still thinking weird
    App->CL_Doc->ResizeSelected(-(((float)dx) * ZoomInv), -(((float)dy) * ZoomInv), sides, App->CL_Render->Render_GetInidx(App->CL_Editor_Map->Current_View));
}

// *************************************************************************
// *            ResizeSelected:- Terry and Hazel Flanigan 2023             *
// *************************************************************************
void CL64_Doc::ResizeSelected(float dx, float dy, int sides, int inidx)
{
    mLastOp = BRUSH_SCALE;
    auto& CL_Brush = App->CL_X_Brush; // Pointer to Brush Library

    if (mModeTool == ID_TOOLS_TEMPLATE)
    {
        /*Brush_Resize(CurBrush, dx, dy, sides, inidx, &FinalScale, &ScaleNum);
        if (Brush_IsMulti(CurBrush))
        {
            BrushList_ClearCSGAndHollows((BrushList*)App->CL_Brush->Brush_GetBrushList(CurBrush), Brush_GetModelId(CurBrush));
            BrushList_RebuildHollowFaces((BrushList*)App->CL_Brush->Brush_GetBrushList(CurBrush), Brush_GetModelId(CurBrush), ::fdocBrushCSGCallback, this);
        }*/
    }
    else
    {
        int i;
        int NumBrushes;

        NumBrushes = App->CL_X_SelBrushList->SelBrushList_GetSize(pTempSelBrushes);

        for (i = 0; i < NumBrushes; ++i)
        {
            Brush* pBrush;

            pBrush = App->CL_X_SelBrushList->SelBrushList_GetBrush(pTempSelBrushes, i);

            CL_Brush->Brush_Resize(pBrush, dx, dy, sides, inidx, &FinalScale, &ScaleNum);
            
            if (CL_Brush->Brush_IsMulti(pBrush))
            {
                CL_Brush->BrushList_ClearCSGAndHollows((BrushList*)CL_Brush->Brush_GetBrushList(pBrush), CL_Brush->Brush_GetModelId(pBrush));
                CL_Brush->BrushList_RebuildHollowFaces((BrushList*)CL_Brush->Brush_GetBrushList(pBrush), CL_Brush->Brush_GetModelId(pBrush), ::fdocBrushCSGCallback, this);
            }
        }
    }
}

// *************************************************************************
// *			                     DoneResize                       	   *
// *************************************************************************
void CL64_Doc::DoneResize(int sides, int inidx)
{
   // App->Say_Int(sides);
    mLastOp = BRUSH_SCALE;

    TempDeleteSelected();

    auto& CL_Brush = App->CL_X_Brush; // Pointer to Brush Library

    if (mModeTool == ID_TOOLS_TEMPLATE)
    {
        if (CL_Brush->Brush_IsMulti(CurBrush))
        {
            CL_Brush->BrushList_ClearCSGAndHollows((BrushList*)CL_Brush->Brush_GetBrushList(CurBrush), CL_Brush->Brush_GetModelId(CurBrush));
            CL_Brush->BrushList_RebuildHollowFaces((BrushList*)CL_Brush->Brush_GetBrushList(CurBrush), CL_Brush->Brush_GetModelId(CurBrush), fdocBrushCSGCallback, NULL);
        }
        return;
    }

    int NumSelBrushes = App->CL_X_SelBrushList->SelBrushList_GetSize(pSelBrushes);
    for (int i = 0; i < NumSelBrushes; ++i)
    {
        Brush* pBrush = App->CL_X_SelBrushList->SelBrushList_GetBrush(pSelBrushes, i);;
        int Index = App->CL_Entities->GetIndex_By_Name(pBrush->Name);
       
        // Set Brush Scale Size
        CL_Brush->Brush_ResizeFinal(pBrush, sides, inidx, &FinalScale);
        if (CL_Brush->Brush_IsMulti(pBrush))
        {
            CL_Brush->BrushList_ClearCSGAndHollows((BrushList*)CL_Brush->Brush_GetBrushList(pBrush), CL_Brush->Brush_GetModelId(pBrush));
            CL_Brush->BrushList_RebuildHollowFaces((BrushList*)CL_Brush->Brush_GetBrushList(pBrush), CL_Brush->Brush_GetModelId(pBrush), fdocBrushCSGCallback, NULL);
        }

        if (pBrush->GroupId == Enums::Brushs_ID_Evirons)
        {
            if (Index > -1)
            {
                auto& pObject = App->CL_Scene->B_Object[Index];

                App->CL_X_SelBrushList->SelBrushList_Center(App->CL_Doc->pSelBrushes, &App->CL_Doc->SelectedGeoCenter);
                T_Vec3 CenterOfSelection = App->CL_Doc->SelectedGeoCenter;

                pObject->Object_Node->setVisible(false);

               // pObject->Object_Node->setOrientation(Ogre::Quaternion::IDENTITY);
                pObject->Object_Node->setScale(1, 1, 1);
                App->CL_Ogre->RenderFrame(2);

                // Get the new Scaled Brush Size
				T_Vec3 Brush_Size =
				{
			        fabs(pBrush->BoundingBox.Max.x - pBrush->BoundingBox.Min.x),
			        fabs(pBrush->BoundingBox.Max.y - pBrush->BoundingBox.Min.y),
			        fabs(pBrush->BoundingBox.Max.z - pBrush->BoundingBox.Min.z)
				};

                // Get Ogre BB Size at sacle 1,1,1
                T_Vec3 Ogre_Size = 
                {
                    pObject->Object_Node->_getWorldAABB().getSize().x,
                    pObject->Object_Node->_getWorldAABB().getSize().y,
                    pObject->Object_Node->_getWorldAABB().getSize().z
                };

                // Recalculate the new Scale for Ogre
                T_Vec3 Ogre_NewScale = 
                {
                    Brush_Size.x / Ogre_Size.x,
                    Brush_Size.y / Ogre_Size.y,
                    Brush_Size.z / Ogre_Size.z
                };

               /* if (sides == 10)
                {
                    App->BeepBeep();
                    T_Vec3 Ogre_NewScale =
                    {
                        Brush_Size.x / Ogre_Size.z,
                        Brush_Size.y / Ogre_Size.y,
                        Brush_Size.z / Ogre_Size.x
                    };

                    pObject->Object_Node->setScale(1, 1, Ogre_NewScale.x);
                    pObject->Mesh_Scale = { 1, 1 , Ogre_NewScale.x };
                }
                else*/
                {
                    pObject->Object_Node->setScale(Ogre_NewScale.x, Ogre_NewScale.y, Ogre_NewScale.z);
                    pObject->Mesh_Scale = { Ogre_NewScale.x, Ogre_NewScale.y , Ogre_NewScale.z };
                }
                
                // Set Position
                pObject->Object_Node->setPosition(CenterOfSelection.x, CenterOfSelection.y, CenterOfSelection.z);
                App->CL_Ogre->RenderFrame(2);

                float True_Center = (App->CL_Scene->B_Object[Index]->Object_Node->_getWorldAABB().getCenter().y);

                // check position are the same
                if (True_Center > CenterOfSelection.y || True_Center < CenterOfSelection.y)
                {
                    float Size = (pObject->Object_Node->_getWorldAABB().getSize().y);
                    pObject->Object_Node->setPosition(CenterOfSelection.x, CenterOfSelection.y - Size/2, CenterOfSelection.z);
                 
                    //App->BeepBeep();
                }
               
                pObject->Object_Node->setVisible(true);

                // Update Physics
                App->CL_Physics->Update_Object_Physics(Index);
            }
        }
    }

    UpdateSelected();
}

// *************************************************************************
// *            OnToolsTemplate:- Terry and Hazel Flanigan 2025           *
// *************************************************************************
void CL64_Doc::OnToolsTemplate()
{
    App->CL_Doc->ResetAllSelectedFaces();
    App->CL_Doc->ResetAllSelectedBrushes();

    //UpdateBrushAttributesDlg();
   // UpdateFaceAttributesDlg();

    App->CL_Doc->mModeTool = ID_TOOLS_TEMPLATE;

    App->CL_Doc->mCurrentTool = ID_TOOLS_BRUSH_MOVEROTATEBRUSH;

    //App->CL_Doc->SetAdjustmentMode(ADJUST_MODE_FACE);
    //ConfigureCurrentTool();
}

// *************************************************************************
// *        SelectAllFacesInBrushes:- Terry and Hazel Flanigan 2025        *
// *************************************************************************
void CL64_Doc::SelectAllFacesInBrushes(void)
{
    Do_General_Select_Dlg(false); // hgtterry check function

    // Select all faces on all selected brushes
    int iBrush;
    int NumSelBrushes = App->CL_X_SelBrushList->SelBrushList_GetSize(pSelBrushes);

    for (iBrush = 0; iBrush < NumSelBrushes; ++iBrush)
    {
        Brush* pBrush;

        pBrush = App->CL_X_SelBrushList->SelBrushList_GetBrush(pSelBrushes, iBrush);
        if (App->CL_X_Brush->Brush_IsMulti(pBrush))
        {
            App->CL_X_Brush->BrushList_EnumLeafBrushes(App->CL_X_Brush->Brush_GetBrushList(pBrush), this, SelAllBrushFaces);
        }
        else
        {
            SelAllBrushFaces(pBrush, this);
        }
    }

    UpdateSelected();
    //ConfigureCurrentTool();
}

// *************************************************************************
// *		Set_Faces_Brush_Name_All:- Terry and Hazel Flanigan 2025 	   *
// *************************************************************************
void CL64_Doc::Set_Faces_To_Brush_Name_All()
{
    BrushList* pList = App->CL_Level->Level_Get_Main_Brushes();

    int Count = 0;
    int BC = App->CL_X_Brush->Get_Brush_Count();
   
    while (Count < BC)
    {
        App->CL_Doc->Set_Tool_GeneralSelect();

        App->CL_Properties_Brushes->Selected_Index = Count;

        App->CL_Doc->ResetAllSelections();
        App->CL_Doc->UpdateSelected();
        App->CL_Properties_Brushes->Selected_Brush = App->CL_X_Brush->Get_Brush_ByIndex(Count);
        App->CL_X_SelBrushList->SelBrushList_Add(App->CL_Doc->pSelBrushes, App->CL_Properties_Brushes->Selected_Brush);
        App->CL_Doc->UpdateSelected();

        // TODO are these in the right Place;
        App->CL_Doc->SelectAllFacesInBrushes();
        App->CL_Brush_X->Set_Brush_Faces_Index(App->CL_Properties_Brushes->Selected_Brush);

        Count++;
    }

    App->CL_Doc->Set_Tool_GeneralSelect();
    App->CL_Doc->ResetAllSelections();
    App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);

}

// *************************************************************************
// *	Set_Faces_To_Brush_Name_Selected:- Terry and Hazel Flanigan 2025   *
// *************************************************************************
void CL64_Doc::Set_Faces_To_Brush_Name_Selected()
{
    int NumSelBrushes = App->CL_X_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes);

    if (NumSelBrushes > 0)
    {
        App->CL_Doc->Do_General_Select_Dlg(false);
        App->CL_Doc->SelectAllFacesInBrushes();
    }
}

// *************************************************************************
// *	( Static )  fdocSelectBrush:- Terry and Hazel Flanigan 2025        *
// *************************************************************************
static signed int fdocSelectBrush(Brush* pBrush, void* lParam)
{
    App->CL_X_SelBrushList->SelBrushList_Add(App->CL_Doc->pSelBrushes, pBrush);

    return GE_TRUE;
}

// *************************************************************************
// *              SelectAll:- Terry and Hazel Flanigan 2025           	   *
// *************************************************************************
void CL64_Doc::SelectAll(void)
{
    Do_General_Select_Dlg(false);

    App->CL_Level->Level_EnumBrushes(this, fdocSelectBrush);

    // Select all faces on all selected brushes
    int iBrush;
    int NumSelBrushes = App->CL_X_SelBrushList->SelBrushList_GetSize(pSelBrushes);

    for (iBrush = 0; iBrush < NumSelBrushes; ++iBrush)
    {
        Brush* pBrush;

        pBrush = App->CL_X_SelBrushList->SelBrushList_GetBrush(pSelBrushes, iBrush);

        if (App->CL_X_Brush->Brush_IsMulti(pBrush))
        {
            App->CL_X_Brush->BrushList_EnumLeafBrushes(App->CL_X_Brush->Brush_GetBrushList(pBrush), this, SelAllBrushFaces);
        }
        else
        {
            SelAllBrushFaces(pBrush, this);
        }

    }

    UpdateSelected();

    //ConfigureCurrentTool();
}

// *************************************************************************
// *             UpdateSelected:- Terry and Hazel Flanigan 2025            *
// *************************************************************************
void CL64_Doc::UpdateSelected(void)
{
    int NumSelFaces = App->CL_X_SelFaceList->SelFaceList_GetSize(pSelFaces);
    int NumSelBrushes = App->CL_X_SelBrushList->SelBrushList_GetSize(pSelBrushes);

    SelState = (NumSelBrushes > 1) ? MULTIBRUSH : NumSelBrushes;
    SelState |= (NumSelFaces > 1) ? MULTIFACE : (NumSelFaces + 1) << 3;
    SelState |= (NumSelEntities > 1) ? MULTIENTITY : (NumSelEntities + 1) << 7;


    if (mModeTool == ID_GENERALSELECT)
    {
        if (GetSelState() & ONEBRUSH)
        if(NumSelBrushes == 1)
        {
            CurBrush = App->CL_X_SelBrushList->SelBrushList_GetBrush(pSelBrushes, 0);
        }
        else
        {
           //CurBrush = BTemplate;
        }
    }
 
    //if (NumSelBrushes)
    {
        //SelBrushList_Center(pSelBrushes, &SelectedGeoCenter);
    }

    App->CL_X_Maths->Vector3_Clear(&SelectedGeoCenter);

    if (mModeTool == ID_TOOLS_TEMPLATE)
    {
        App->CL_X_Brush->Brush_Get_Center(CurBrush, &SelectedGeoCenter);
    }
    else if (SelState != NOSELECTIONS)
    {
        if (NumSelBrushes)
        {
            App->CL_X_SelBrushList->SelBrushList_Center(pSelBrushes, &SelectedGeoCenter);
        }
    }

}

// *************************************************************************
// *        fdocUpdateFaceTextures:- Terry and Hazel Flanigan 2025         *
// *************************************************************************
static signed int fdocUpdateFaceTextures(Face* pFace, void* lParam)
{
    Ogre::uint16 TestDibID = App->CL_Level->Level_GetDibId(App->CL_X_Face->Face_GetTextureName(pFace));
    if (TestDibID == 0xffff)
    {
        App->CL_X_Face->Face_SetTextureDibId(pFace, 0);
    }
    else
    {
        App->CL_X_Face->Face_SetTextureDibId(pFace, App->CL_Level->Level_GetDibId(App->CL_X_Face->Face_GetTextureName(pFace)));
    }
   
    const WadFileEntry* const pbmp = App->CL_Level->Level_GetWadBitmap(App->CL_X_Face->Face_GetTextureName(pFace));
    if (pbmp)
    {
        App->CL_X_Face->Face_SetTextureSize(pFace, pbmp->Width, pbmp->Height);
    }
  
    return GE_TRUE;
}

// *************************************************************************
// *      fdocUpdateBrushFaceTextures:- Terry and Hazel Flanigan 2025      *
// *************************************************************************
static signed int fdocUpdateBrushFaceTextures(Brush* pBrush, void* pVoid)
{
    App->CL_X_Brush->Brush_EnumFaces(pBrush, pVoid, fdocUpdateFaceTextures);
    return GE_TRUE;
}

// *************************************************************************
// *        UpdateAfterWadChange:- Terry and Hazel Flanigan 2025           *
// *************************************************************************
void CL64_Doc::UpdateAfterWadChange()
{
	//flag_Is_Modified = 1;

	// update all brush faces
	App->CL_X_Brush->BrushList_EnumLeafBrushes(App->CL_Level->Level_Get_Main_Brushes(), this, fdocUpdateBrushFaceTextures);

}

// *************************************************************************
// *			        	DeleteCurrentThing	                    	   *
// *************************************************************************
void CL64_Doc::DeleteCurrentThing()
{
    BOOL	ReBuild;

    if (mModeTool == ID_GENERALSELECT)
    {
        ResetAllSelectedFaces();
        ReBuild = (GetSelState() & ANYBRUSH);

        DeleteSelectedBrushes();
        //CurBrush = NULL;
       
        UpdateAllViews(Enums::UpdateViews_All);

    }
}

// *************************************************************************
// *			        	DeleteSelectedBrushes	                	   *
// *************************************************************************
bool CL64_Doc::DeleteSelectedBrushes()
{
    signed int	bAlteredCurrentGroup = GE_FALSE;
   
    if (GetSelState() & ANYBRUSH)
    {
    
        int NumSelBrushes = App->CL_X_SelBrushList->SelBrushList_GetSize(pSelBrushes);
        for (int i = 0; i < NumSelBrushes; i++)
        {
            Brush* pBrush;

            pBrush = App->CL_X_SelBrushList->SelBrushList_GetBrush(pSelBrushes, 0);
            
            App->CL_Level->Level_RemoveBrush(pBrush);
            App->CL_X_SelBrushList->SelBrushList_Remove(pSelBrushes, pBrush);
            App->CL_X_Brush->Brush_Destroy(&pBrush);

            bAlteredCurrentGroup = true;
        }
        
        //turn off any operation tools
        mCurrentTool = CURTOOL_NONE;

        App->CL_Level->flag_Level_is_Modified = true;
    }

    
    if (bAlteredCurrentGroup)
    {
        App->CL_Properties_Brushes->Fill_ListBox();
    }

    UpdateSelected();

    return FALSE;
}

// *************************************************************************
// *				Set_Paths:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Doc::Set_Paths(void)
{
    Set_Current_3DT_Paths();
    //Set_Current_TxlPath();
}

// *************************************************************************
// *		Set_Current_3DT_Paths:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Doc::Set_Current_3DT_Paths(void)
{
    strcpy(App->CL_Level->MTF_PathAndFile, App->CL_File->PathFileName_3dt);
    strcpy(App->CL_Level->MTF_Just_FileName, App->CL_File->FileName_3dt);

    char buf[MAX_PATH];
    strcpy(buf, App->CL_Level->MTF_Just_FileName);
    int Len = strlen(buf);
    buf[Len - 4] = 0;
    strcpy(App->CL_Level->MTF_JustName_NoExt, buf);

    strcpy(App->CL_Export->mJustName, App->CL_Level->MTF_JustName_NoExt);

    // Just Path
    char Just_Path[MAX_PATH];
    strcpy(Just_Path, App->CL_Level->MTF_PathAndFile);

    int Len1 = strlen(App->CL_Level->MTF_PathAndFile);
    int Len2 = strlen(App->CL_Level->MTF_Just_FileName);
    Just_Path[Len1 - Len2] = 0;

    strcpy(App->CL_Level->MTF_Just_Path, Just_Path);
   
}

// *************************************************************************
// *		Set_Current_TxlPath:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Doc::Set_Current_TxlPath(void)
{
    /*const char* WadFilePath;
    WadFilePath = App->CL_Level->Level_GetWadPath();

    strcpy(App->CL_Level->Wad_PathAndFile, WadFilePath);

    App->CL_Utilities->Get_FileName_FromPath(App->CL_Level->Wad_PathAndFile, App->CL_Level->Wad_PathAndFile);

    strcpy(App->CL_Level->Wad_Just_File_Name, App->CL_Utilities->JustFileName);*/
}

// *************************************************************************
// *		SnapScaleNearest:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Doc::SnapScaleNearest(int sides, int inidx, ViewVars* v)
{
    float	bsnap;

    App->CL_Level->flag_Level_is_Modified = true;

    //App->CLSB_Doc->mLastOp = BRUSH_SCALE;

    bsnap = 1.0f;
    if (App->CL_Level->flag_UseGrid == 1)
    {
        bsnap = App->CL_Level->Level_GetGridSnapSize(Current_Level);
    }

   /* if (App->CLSB_Doc->mModeTool == ID_TOOLS_TEMPLATE)
    {
        Brush_SnapScaleNearest(App->CLSB_Doc->CurBrush, bsnap, sides, inidx, &App->CLSB_Doc->FinalScale, &App->CLSB_Doc->ScaleNum);
    }
    else*/
    {
        int i;
        int NumBrushes = App->CL_X_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pTempSelBrushes);

        for (i = 0; i < NumBrushes; ++i)
        {
            Brush* pBrush = App->CL_X_SelBrushList->SelBrushList_GetBrush(App->CL_Doc->pTempSelBrushes, i);
            App->CL_X_Brush->Brush_SnapScaleNearest(pBrush, bsnap, sides, inidx, &App->CL_Doc->FinalScale, &App->CL_Doc->ScaleNum);
        }
    }
}




