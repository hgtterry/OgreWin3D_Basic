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

    strcpy(mDoc_MTF_PathAndFile, App->RB_Directory_FullPath);
    strcat(mDoc_MTF_PathAndFile, "\\Data\\Room_Builder\\Room_1.mtf");

    strcpy(mDoc_MTF_Just_FileName,"Room_1.mtf");
    strcpy(mDoc_MTF_JustName_NoExt, "Room_1");

    mDoc_TXL_Path_And_File[0] = 0;
    mDoc_TXL_Just_FileName[0] = 0;

	pLevel = NULL;
   
    BTemplate = NULL;
    CurBrush = NULL;
    TempShearTemplate = NULL;
    pSelBrushes = NULL;
    pTempSelBrushes = NULL;
    pSelFaces = NULL;
    Temp_SelFaces = NULL;

    App->CL_Maths->Vector3_Set(&SelectedGeoCenter,0, 0, 0);
    App->CL_Maths->Vector3_Set(&FinalPos, 0, 0, 0);
    App->CL_Maths->Vector3_Set(&FinalRot, 0, 0, 0);
    App->CL_Maths->Vector3_Set(&FinalScale, 0, 0, 0);
   
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

    flag_Is_Modified = 0;
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
   
	pLevel = App->CL_Level->Level_Create(); // TODO Creating Twice one in Win Main
  
	pSelBrushes = App->CL_SelBrushList->SelBrushList_Create();
	pTempSelBrushes = App->CL_SelBrushList->SelBrushList_Create();
	pSelFaces = App->CL_SelFaceList->SelFaceList_Create();

	SetLockAxis(0);	// Start with no axis locked

	// create our default box
	BrushTemplate_Box* pBoxTemplate;
   
	pBoxTemplate = App->CL_Level->Level_GetBoxTemplate(pLevel);

	BTemplate = App->CL_BrushTemplate->BrushTemplate_CreateBox(pBoxTemplate);
    
	App->CL_Brush->Brush_Bound(BTemplate);
	CurBrush = BTemplate;

	mModeTool = ID_TOOLS_TEMPLATE;

	App->CL_Maths->Vector3_Clear(&SelectedGeoCenter);

    strcpy(App->CL_Export->mJustName, mDoc_MTF_JustName_NoExt);

    CheckMenuItem(App->Menu_Map, ID_CAMERA_TRACKCAMERA, MF_BYCOMMAND | MF_CHECKED);

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
        strcpy(App->CL_Doc->mDoc_TXL_Path_And_File, Path_And_File);
		return Path_And_File;
	}
	else
	{
		App->Say_Win("File Does not Exist");

        strcpy(App->CL_Doc->mDoc_TXL_Path_And_File, "No_File");
		strcpy(Path_And_File, "");
		return Path_And_File;
	}
	
    strcpy(App->CL_Doc->mDoc_TXL_Path_And_File, "No_File");
	strcpy(Path_And_File, "");
	return Path_And_File;
}

// *************************************************************************
// *			        	AddBrushToWorld	                           	   *
// *************************************************************************
void CL64_Doc::AddBrushToWorld()
{
	if (TempEnt || !App->CL_Brush->Brush_IsSubtract(CurBrush))
	{
        Brush_Add_To_world();
	}
	else
	{
		OnBrushSubtractfromworld();
	}

    flag_Is_Modified = 1;
}

// *************************************************************************
// *			        OnBrushSubtractfromworld	                   	   *
// *************************************************************************
void CL64_Doc::OnBrushSubtractfromworld()
{
    Brush* nb;
    BrushList* BList = App->CL_Level->Level_GetBrushes(App->CL_Doc->pLevel);

    if ((App->CL_Doc->mModeTool == ID_GENERALSELECT) && (App->CL_Brush->BrushList_Count(BList, BRUSH_COUNT_MULTI | BRUSH_COUNT_LEAF) < 2))
    {
        // cuts shouldn't start the list
        return;
    }

    //SetModifiedFlag();

    if (App->CL_Doc->mModeTool == ID_GENERALSELECT)
    {
        // put the brush at the very end of the list
        App->CL_Brush->BrushList_Remove(BList, App->CL_Doc->CurBrush);
        Brush_SetSubtract(App->CL_Doc->CurBrush, GE_TRUE);

        App->CL_SelBrushList->SelBrushList_RemoveAll(App->CL_Doc->pSelBrushes);
        App->CL_Brush->BrushList_Append(BList, App->CL_Doc->CurBrush);
    }
    else
    {
        nb = App->CL_Brush->Brush_Clone(App->CL_Doc->CurBrush);

        SetDefaultBrushTexInfo(nb);
        App->CL_Brush->Brush_Bound(nb);

        // add to current group
        Brush_SetGroupId(nb, App->CL_Doc->mCurrentGroup);

        App->CL_Brush->BrushList_Append(BList, nb);

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

	Brush* nb;
    T_Vec3* pTemplatePos;

    
	nb = App->CL_Brush->Brush_Clone(App->CL_Doc->CurBrush);
   
	SetDefaultBrushTexInfo(nb);
   
	App->CL_Brush->Brush_Bound(nb);
	pTemplatePos = App->CL_Level->Level_GetTemplatePos(pLevel);
    
	App->CL_Brush->Brush_Center(nb, pTemplatePos);

	// add to current group
	Brush_SetGroupId(nb, mCurrentGroup);
   
	fdocFaceScales Scales;
   
	Scales.DrawScale = App->CL_Level->Level_GetDrawScale(pLevel);
	Scales.LightmapScale = App->CL_Level->Level_GetLightmapScale(pLevel);
	App->CL_Brush->Brush_EnumFaces(nb, &Scales, fdocSetFaceScales);
   
	App->CL_Level->Level_AppendBrush(pLevel, nb);
   

	if (!App->CL_Brush->Brush_IsHollow(nb) && !App->CL_Brush->Brush_IsMulti(nb))
	{
		//App->CL_Doc->UpdateAllViews(Enums::UpdateViews_All);
	}
	else
	{
		//App->CL_Doc->UpdateAllViews(Enums::UpdateViews_All);
	}

    App->CL_Doc->CurBrush = nb;
    App->CL_Brush_X->Set_Brush_Faces_Name(App->CL_Doc->CurBrush);

	Placed = true;

	if (Placed)
	{
		App->CL_Doc->flag_Is_Modified = 1;
	}
}

// *************************************************************************
// *           DoGeneralSelect:- Terry and Hazel Flanigan 2023             *
// *************************************************************************
void CL64_Doc::Set_GeneralSelect()
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

    if (from_Insert == true)
    {
        if (App->CL_Brush->Get_Brush_Count() > 0)
        {
            Set_GeneralSelect();

            App->CL_Top_Tabs->Enable_Brush_Options_Buttons(false, false);
            App->CL_Top_Tabs->Enable_Select_Button(true, true);

            RedrawWindow(App->CL_Top_Tabs->Headers_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

            return;
        }
    }

    if (App->CL_Brush->Get_Brush_Count() > 0)
    {
        Set_GeneralSelect();

        App->CL_Top_Tabs->Enable_Brush_Options_Buttons(true, false);
        App->CL_Top_Tabs->Enable_Select_Button(true, true);

        RedrawWindow(App->CL_Top_Tabs->Headers_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
    }
}

// *************************************************************************
// *         UpdateAllViews:- Terry and Hazel Flanigan 2025                *
// *************************************************************************
void CL64_Doc::UpdateAllViews(int Update_Mode)
{
    if (Update_Mode == Enums::UpdateViews_Grids)
    {
        RedrawWindow(App->CL_Editor_Map->Left_Window_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
        RedrawWindow(App->CL_Editor_Map->Right_Window_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
        RedrawWindow(App->CL_Editor_Map->Bottom_Left_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
    }

    if (Update_Mode == Enums::UpdateViews_3D)
    {
        int BC = App->CL_Brush->Get_Brush_Count();
        if (BC > 0)
        {
            App->CL_Doc->RebuildTrees();
            App->CL_Mesh_Mgr->Update_World(0); // Will Set Node Visable
        }
        else
        {
            App->CL_Mesh_Mgr->World_Node->setVisible(false);
        }
    }

    if (Update_Mode == Enums::UpdateViews_All)
    {
        RedrawWindow(App->CL_Editor_Map->Left_Window_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
        RedrawWindow(App->CL_Editor_Map->Right_Window_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
        RedrawWindow(App->CL_Editor_Map->Bottom_Left_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

        int BC = App->CL_Brush->Get_Brush_Count();
        if (BC > 0)
        {
            App->CL_Doc->RebuildTrees();
            App->CL_Mesh_Mgr->Update_World(0); // Will Set Node Visable
        }
        else
        {
            App->CL_Mesh_Mgr->World_Node->setVisible(false);
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

    //	Brush_SetName(b, TexName);
    App->CL_Brush->Brush_SetName(b, LastTemplateTypeName);

    if (App->CL_Brush->Brush_IsMulti(b))
    {
        App->CL_Brush->BrushList_EnumLeafBrushes(App->CL_Brush->Brush_GetBrushList(b), &CallbackData, ::BrushTexSetCB);
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
    return App->CL_Level->Level_GetWadBitmap(pLevel, Name);
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
        for (int iFace = 0; iFace < App->CL_Brush->Brush_GetNumFaces(pBrush); ++iFace)
        {
            POINT pt1, pt2;
            Face* pFace = App->CL_Brush->Brush_GetFace(pBrush, iFace);
            const T_Vec3* FacePoints = App->CL_Face->Face_GetPoints(pFace);
            int	NumPoints = App->CL_Face->Face_GetNumPoints(pFace);

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

    for (i = 0; i < App->CL_Brush->Brush_GetNumFaces(b); i++)
    {
        Face* pFace;

        pFace = App->CL_Brush->Brush_GetFace(b, i);
        App->CL_Face->Face_SetSelected(pFace, false);
    }
    return GE_TRUE;
}
#pragma warning (default:4100)

// *************************************************************************
// *			        	   SelectOrtho                             	   *
// *************************************************************************
void CL64_Doc::SelectOrtho(POINT point, ViewVars* v)
{
    Brush* pMinBrush;
    float Dist;
    int FoundThingType;

   if ((GetAsyncKeyState(VK_SHIFT) & 0x8000) == 0)
    {
        ResetAllSelections();
    }


    FoundThingType = FindClosestThing(&point, v, &pMinBrush,&Dist);

    if ((FoundThingType != fctNOTHING) && (Dist <= MAX_PIXEL_SELECT_DIST))
    {
        if (FoundThingType == fctBRUSH)
        {
            //Do_Timer

            bool locked = App->CL_Brush->Brush_IsLocked(pMinBrush);
            if (locked == 1)
            {
                // Dont Select if Locked
            }
            else
            {
                DoBrushSelection(pMinBrush, brushSelToggle);

                int Bnum = App->CL_Brush->Get_Brush_Count();
                if (Bnum > 0)
                {
                    UpdateSelected();
                    App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);

                    App->CL_Properties_Tabs->Select_Brushes_Tab();
                    App->CL_Properties_Brushes->Get_Index(CurBrush);

                    App->CL_Top_Tabs->Enable_Brush_Options_Buttons(true, false);
                    App->CL_Properties_Brushes->Set_Dlg_Brush_Options_Buttons(true);

                    App->CL_Properties_Brushes->Update_SelectedBrushesCount_Dlg();

                    App->CL_Top_Tabs->Update_Faces_Combo();
                }
            }

            //Get_Timer
        } 
    }
    else
    {
        if (App->CL_Properties_Faces->flag_FaceDlg_Active == 1)
        {
            App->CL_Properties_Faces->Close_Faces_Dialog();
        }

        App->CL_Panels->Deselect_All_Brushes_Update_Dlgs();
        App->CL_Top_Tabs->Update_Faces_Combo();
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
    App->CL_Brush->BrushList_EnumLeafBrushes(App->CL_Level->Level_GetBrushes(pLevel), NULL, ResetSelectedFacesCB);
    App->CL_SelFaceList->SelFaceList_RemoveAll(pSelFaces);
}

// *************************************************************************
// *         ResetAllSelectedBrushes:- Terry and Hazel Flanigan 2025       *
// *************************************************************************
void CL64_Doc::ResetAllSelectedBrushes(void)
{
    App->CL_SelBrushList->SelBrushList_RemoveAll(pSelBrushes);
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

    BList = App->CL_Level->Level_GetBrushes(pLevel);

    if (App->CL_Brush->Brush_GetParent(BList, pBrush, &pBParent))
    {
        pBrush = pBParent;
    }

    ModelLocked = false;
    GroupLocked = false;

    if (nSelType == brushSelToggle && BrushIsSelected(pBrush))
    {
       
    }
    else
    {
       //Debug
       App->CL_SelBrushList->SelBrushList_Add(pSelBrushes, pBrush);    
    }
}

// *************************************************************************
// *            FindClosestThing:- Terry and Hazel Flanigan 2025           *
// *************************************************************************
signed int CL64_Doc::BrushIsSelected(Brush const* pBrush)
{
    assert(pBrush != NULL);

    return App->CL_SelBrushList->SelBrushList_Find(pSelBrushes, pBrush);
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

    App->CL_Brush->BrushList_EnumLeafBrushes(App->CL_Level->Level_GetBrushes(pLevel), &fci, ::FindClosestBrushCB);

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
   
    BList = App->CL_Level->Level_GetBrushes(App->CL_Doc->pLevel);
    //SetModifiedFlag();

    App->CL_Brush->BrushList_ClearAllCSG(BList);

    App->CL_Brush->BrushList_DoCSG(BList, CurId, ::fdocBrushCSGCallback, this);

}

// *************************************************************************
// *         TempCopySelectedBrushes:- Terry and Hazel Flanigan 2025       *
// *************************************************************************
void CL64_Doc::TempCopySelectedBrushes(void)
{
    int		i;
    int NumSelBrushes;

    NumSelBrushes = App->CL_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes);

    App->CL_SelBrushList->SelBrushList_RemoveAll(App->CL_Doc->pTempSelBrushes);

    // make copies of selected brushes
    for (i = 0; i < NumSelBrushes; i++)
    {
        Brush* pBrush, * pClone;

        pBrush = App->CL_SelBrushList->SelBrushList_GetBrush(App->CL_Doc->pSelBrushes, i);
        pClone = App->CL_Brush->Brush_Clone(pBrush);
        App->CL_Level->Level_AppendBrush(App->CL_Doc->pLevel, pClone);
        App->CL_SelBrushList->SelBrushList_Add(App->CL_Doc->pTempSelBrushes, pClone);
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

    App->CL_Maths->Vector3_Add(&SelectedGeoCenter, v, &SelectedGeoCenter);
    App->CL_Maths->Vector3_Add(v, &FinalPos, &FinalPos);

    NumBrushes = App->CL_SelBrushList->SelBrushList_GetSize(pList);
    for (i = 0; i < NumBrushes; i++)
    {
        Brush* pBrush;

        pBrush = App->CL_SelBrushList->SelBrushList_GetBrush(pList, i);
        App->CL_Brush->Brush_Move(pBrush, v);

    }
}

// *************************************************************************
// *         RotateSelectedBrushes:- Terry and Hazel Flanigan 2025         *
// *************************************************************************
void CL64_Doc::RotateSelectedBrushes(T_Vec3 const* v)
{
    RotateSelectedBrushList(App->CL_Doc->pTempSelBrushes, v);
}

// *************************************************************************
// *         RotateSelectedBrushList:- Terry and Hazel Flanigan 2025       *
// *************************************************************************
void CL64_Doc::RotateSelectedBrushList(SelBrushList* pList,T_Vec3 const* v)
{
    int i;
    Matrix3d rm;
    T_Vec3 RotationPoint;
    int NumBrushes = App->CL_SelBrushList->SelBrushList_GetSize(pList);

   // App->CLSB_Doc->mLastOp = BRUSH_ROTATE;

    GetRotationPoint(&RotationPoint);


    App->CL_Maths->Vector3_Add(v, &App->CL_Doc->FinalRot, &App->CL_Doc->FinalRot);
    App->CL_Maths->XForm3d_SetEulerAngles(&rm, v);

    for (i = 0; i < NumBrushes; i++)
    {
        Brush* pBrush = App->CL_SelBrushList->SelBrushList_GetBrush(pList, i);
        App->CL_Brush->Brush_Rotate(pBrush, &rm, &RotationPoint);
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
    App->CL_Maths->Vector3_Subtract(&VecOrigin, &App->CL_Doc->SelectedGeoCenter, &MoveTo);
    App->CL_Maths->Vector3_Subtract(&App->CL_Doc->SelectedGeoCenter, &VecOrigin, &MoveBack);

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
        int NumSelBrushes = App->CL_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes);

        for (i = 0; i < NumSelBrushes; ++i)
        {
            Brush* pBrush;

            pBrush = App->CL_SelBrushList->SelBrushList_GetBrush(App->CL_Doc->pSelBrushes, i);

            App->CL_Brush->Brush_Move(pBrush, &MoveTo);
            App->CL_Brush->Brush_Scale3d(pBrush, ScaleVector);
            App->CL_Brush->Brush_Move(pBrush, &MoveBack);

            if (App->CL_Brush->Brush_IsMulti(pBrush))
            {
                App->CL_Brush->BrushList_ClearCSGAndHollows((BrushList*)App->CL_Brush->Brush_GetBrushList(pBrush), App->CL_Brush->Brush_GetModelId(pBrush));
                App->CL_Brush->BrushList_RebuildHollowFaces((BrushList*)App->CL_Brush->Brush_GetBrushList(pBrush), App->CL_Brush->Brush_GetModelId(pBrush), ::fdocBrushCSGCallback, this);
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
 
    flag_Is_Modified = 1;

    if (App->CL_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes) > 0)// || ModeTool == ID_TOOLS_TEMPLATE)
    {
        float fSnapSize;
        const T_Vec3* vMin, * vMax;
        const Box3d* pBox;
        T_Vec3 SnapDelta;
        signed int SnapX, SnapY, SnapZ;

        fSnapSize = 1.0f;

        if (App->CL_Level->flag_UseGrid == 1)
        {
            fSnapSize = App->CL_Level->Level_GetGridSnapSize(App->CL_Doc->pLevel);
        }

        // do the snap thing...
        pBox = App->CL_Brush->Brush_GetBoundingBox(App->CL_Doc->CurBrush);
        vMin = App->CL_Box->Box3d_GetMin(pBox);
        vMax = App->CL_Box->Box3d_GetMax(pBox);
        App->CL_Maths->Vector3_Clear(&SnapDelta);
       
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
            App->CL_Maths->Vector3_Add(&App->CL_Doc->FinalPos, &SnapDelta, &App->CL_Doc->FinalPos);
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
        RSnap = Units_DegreesToRadians((float)App->CL_Level->Level_GetRotationSnap(pLevel));
        FinalRot.x = ((float)((int)(FinalRot.x / RSnap))) * RSnap;
        FinalRot.y = ((float)((int)(FinalRot.y / RSnap))) * RSnap;
        FinalRot.z = ((float)((int)(FinalRot.z / RSnap))) * RSnap;
    }

    
    if (mModeTool == ID_TOOLS_TEMPLATE)
    {
        App->CL_Maths->Vector3_Subtract(&FinalRot, &TemplateReversalRot, &FinalRot);
    }

    App->CL_Maths->XForm3d_SetEulerAngles(&rm, &FinalRot);

    if (mModeTool == ID_TOOLS_TEMPLATE)
    {
       
        App->CL_Brush->Brush_Rotate(CurBrush, &rm, &RotationPoint);
        return;
    }
   
    int NumSelBrushes = App->CL_SelBrushList->SelBrushList_GetSize(pSelBrushes);

    for (i = 0; i < NumSelBrushes; i++)
    {
        Brush* pBrush;

        pBrush = App->CL_SelBrushList->SelBrushList_GetBrush(pTempSelBrushes, i);
        App->CL_Brush->Brush_Rotate(pBrush, &rm, &RotationPoint);
    }

    if (i < NumSelBrushes)
    {
        App->CL_Doc->TempDeleteSelected();
    }
    else
    {
        BrushList* BList = App->CL_Level->Level_GetBrushes(pLevel);

        for (i = 0; i < NumSelBrushes; i++)
        {
            // Replace the sel list brushes with the TSelList brushes
            Brush* TempBrush, * OldBrush;

            TempBrush = App->CL_SelBrushList->SelBrushList_GetBrush(pTempSelBrushes, 0);
            OldBrush = App->CL_SelBrushList->SelBrushList_GetBrush(pSelBrushes, 0);
           
            App->CL_Brush->BrushList_Remove(BList, TempBrush);
            App->CL_Brush->BrushList_InsertAfter(BList, OldBrush, TempBrush);
            App->CL_Brush->BrushList_Remove(BList, OldBrush);

            App->CL_SelBrushList->SelBrushList_Remove(pSelBrushes, OldBrush);
            App->CL_SelBrushList->SelBrushList_Remove(pTempSelBrushes, TempBrush);

            App->CL_SelBrushList->SelBrushList_Add(pSelBrushes, TempBrush);

            //App->CL_Brush->BrushList_Remove(OldBrush->BList, OldBrush);
        }
    }
   
    UpdateSelected();
   
    App->CL_Maths->Vector3_Clear(&FinalRot);

}

// *************************************************************************
// *			                 DoneMove                           	   *
// *************************************************************************
void CL64_Doc::DoneMove(void)
{
    int	i;
   
    mLastOp = BRUSH_MOVE;

    TempDeleteSelected();

    if (mModeTool == ID_TOOLS_TEMPLATE)
    {
       /* if (TempEnt)
        {
            DoneMoveEntity();
        }
        else*/
      /*  {
            App->CL_Brush->Brush_Move(CurBrush, &FinalPos);
        }*/
        return;
    }
    else
    {
        int NumSelBrushes = App->CL_SelBrushList->SelBrushList_GetSize(pSelBrushes);
       
        for (i = 0; i < NumSelBrushes; i++)
        {
            Brush* pBrush;

            pBrush = App->CL_SelBrushList->SelBrushList_GetBrush(pSelBrushes, i);

            App->CL_Brush->Brush_Move(pBrush, &FinalPos);

            if (pBrush->GroupId == 1) // Player
            {
                App->CL_SelBrushList->SelBrushList_Center(App->CL_Doc->pSelBrushes, &App->CL_Doc->SelectedGeoCenter);
                T_Vec3 CenterOfSelection = App->CL_Doc->SelectedGeoCenter;

                App->CL_Editor_Com->B_Player[0]->StartPos.x = CenterOfSelection.x;
                App->CL_Editor_Com->B_Player[0]->StartPos.y = CenterOfSelection.y;
                App->CL_Editor_Com->B_Player[0]->StartPos.z = CenterOfSelection.z;

                App->CL_Physics->Reset_Physics();
            }
        }

        UpdateSelected();
    }

    App->CL_Maths->Vector3_Clear(&FinalPos);

}

// *************************************************************************
// *          TempDeleteSelected:- Terry and Hazel Flanigan 2025           *
// *************************************************************************
BOOL CL64_Doc::TempDeleteSelected(void)
{
    BOOL	ret;
    int		i;
    int		NumTSelBrushes = App->CL_SelBrushList->SelBrushList_GetSize(pTempSelBrushes);

    for (ret = FALSE, i = 0; i < NumTSelBrushes; i++)
    {
        Brush* pBrush;

        pBrush = App->CL_SelBrushList->SelBrushList_GetBrush(pTempSelBrushes, 0);

        App->CL_Level->Level_RemoveBrush(pLevel, pBrush);
        App->CL_SelBrushList->SelBrushList_Remove(pTempSelBrushes, pBrush);
        App->CL_Brush->Brush_Destroy(&pBrush);
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

        NumBrushes = App->CL_SelBrushList->SelBrushList_GetSize(pTempSelBrushes);

        for (i = 0; i < NumBrushes; ++i)
        {
            Brush* pBrush;

            pBrush = App->CL_SelBrushList->SelBrushList_GetBrush(pTempSelBrushes, i);

            App->CL_Brush->Brush_Resize(pBrush, dx, dy, sides, inidx, &FinalScale, &ScaleNum);
            
            if (App->CL_Brush->Brush_IsMulti(pBrush))
            {
                App->CL_Brush->BrushList_ClearCSGAndHollows((BrushList*)App->CL_Brush->Brush_GetBrushList(pBrush), App->CL_Brush->Brush_GetModelId(pBrush));
                App->CL_Brush->BrushList_RebuildHollowFaces((BrushList*)App->CL_Brush->Brush_GetBrushList(pBrush), App->CL_Brush->Brush_GetModelId(pBrush), ::fdocBrushCSGCallback, this);
            }
        }
    }
}

// *************************************************************************
// *			                     DoneResize                       	   *
// *************************************************************************
void CL64_Doc::DoneResize(int sides, int inidx)
{
    mLastOp = BRUSH_SCALE;

    TempDeleteSelected();

    if (mModeTool == ID_TOOLS_TEMPLATE)
    {
        if (App->CL_Brush->Brush_IsMulti(CurBrush))
        {
            App->CL_Brush->BrushList_ClearCSGAndHollows((BrushList*)App->CL_Brush->Brush_GetBrushList(CurBrush), App->CL_Brush->Brush_GetModelId(CurBrush));
            App->CL_Brush->BrushList_RebuildHollowFaces((BrushList*)App->CL_Brush->Brush_GetBrushList(CurBrush), App->CL_Brush->Brush_GetModelId(CurBrush), fdocBrushCSGCallback, NULL);
        }
        return;
    }

    int NumSelBrushes = App->CL_SelBrushList->SelBrushList_GetSize(pSelBrushes);
    for (int i = 0; i < NumSelBrushes; ++i)
    {
        Brush* pBrush;

        pBrush = App->CL_SelBrushList->SelBrushList_GetBrush(pSelBrushes, i);

 
        App->CL_Brush->Brush_ResizeFinal(pBrush, sides, inidx, &FinalScale);
        if (App->CL_Brush->Brush_IsMulti(pBrush))
        {
            App->CL_Brush->BrushList_ClearCSGAndHollows((BrushList*)App->CL_Brush->Brush_GetBrushList(pBrush), App->CL_Brush->Brush_GetModelId(pBrush));
            App->CL_Brush->BrushList_RebuildHollowFaces((BrushList*)App->CL_Brush->Brush_GetBrushList(pBrush), App->CL_Brush->Brush_GetModelId(pBrush), fdocBrushCSGCallback, NULL);
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
    int NumSelBrushes = App->CL_SelBrushList->SelBrushList_GetSize(pSelBrushes);

    for (iBrush = 0; iBrush < NumSelBrushes; ++iBrush)
    {
        Brush* pBrush;

        pBrush = App->CL_SelBrushList->SelBrushList_GetBrush(pSelBrushes, iBrush);
        if (App->CL_Brush->Brush_IsMulti(pBrush))
        {
            App->CL_Brush->BrushList_EnumLeafBrushes(App->CL_Brush->Brush_GetBrushList(pBrush), this, SelAllBrushFaces);
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
    BrushList* pList = App->CL_Level->Level_GetBrushes(App->CL_Doc->pLevel);

    int Count = 0;
    int BC = App->CL_Brush->Get_Brush_Count();
   
    while (Count < BC)
    {
        App->CL_Doc->Set_GeneralSelect();

        App->CL_Properties_Brushes->Selected_Index = Count;

        App->CL_Doc->ResetAllSelections();
        App->CL_Doc->UpdateSelected();
        App->CL_Properties_Brushes->Selected_Brush = App->CL_Brush->Get_Brush_ByIndex(Count);
        App->CL_SelBrushList->SelBrushList_Add(App->CL_Doc->pSelBrushes, App->CL_Properties_Brushes->Selected_Brush);
        App->CL_Doc->UpdateSelected();

        App->CL_Doc->SelectAllFacesInBrushes();

        Count++;
    }

    App->CL_Doc->Set_GeneralSelect();
    App->CL_Doc->ResetAllSelections();
    App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);

}

// *************************************************************************
// *	Set_Faces_To_Brush_Name_Selected:- Terry and Hazel Flanigan 2025   *
// *************************************************************************
void CL64_Doc::Set_Faces_To_Brush_Name_Selected()
{
    int NumSelBrushes = App->CL_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes);

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
    App->CL_SelBrushList->SelBrushList_Add(App->CL_Doc->pSelBrushes, pBrush);

    return GE_TRUE;
}

// *************************************************************************
// *              SelectAll:- Terry and Hazel Flanigan 2025           	   *
// *************************************************************************
void CL64_Doc::SelectAll(void)
{
    Do_General_Select_Dlg(false);
    App->CL_Level->Level_EnumBrushes(pLevel, this, fdocSelectBrush);

    // Select all faces on all selected brushes
    int iBrush;
    int NumSelBrushes = App->CL_SelBrushList->SelBrushList_GetSize(pSelBrushes);

    for (iBrush = 0; iBrush < NumSelBrushes; ++iBrush)
    {
        Brush* pBrush;

        pBrush = App->CL_SelBrushList->SelBrushList_GetBrush(pSelBrushes, iBrush);

        if (App->CL_Brush->Brush_IsMulti(pBrush))
        {
            App->CL_Brush->BrushList_EnumLeafBrushes(App->CL_Brush->Brush_GetBrushList(pBrush), this, SelAllBrushFaces);
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
    int NumSelFaces = App->CL_SelFaceList->SelFaceList_GetSize(pSelFaces);
    int NumSelBrushes = App->CL_SelBrushList->SelBrushList_GetSize(pSelBrushes);

    SelState = (NumSelBrushes > 1) ? MULTIBRUSH : NumSelBrushes;
    SelState |= (NumSelFaces > 1) ? MULTIFACE : (NumSelFaces + 1) << 3;
    SelState |= (NumSelEntities > 1) ? MULTIENTITY : (NumSelEntities + 1) << 7;


    if (mModeTool == ID_GENERALSELECT)
    {
        if (GetSelState() & ONEBRUSH)
        if(NumSelBrushes == 1)
        {
            CurBrush = App->CL_SelBrushList->SelBrushList_GetBrush(pSelBrushes, 0);
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

    App->CL_Maths->Vector3_Clear(&SelectedGeoCenter);

    if (mModeTool == ID_TOOLS_TEMPLATE)
    {
        App->CL_Brush->Brush_Center(CurBrush, &SelectedGeoCenter);
    }
    else if (SelState != NOSELECTIONS)
    {
        if (NumSelBrushes)
        {
            App->CL_SelBrushList->SelBrushList_Center(pSelBrushes, &SelectedGeoCenter);
        }
    }

    // App->m_pDoc->UpdateFaceAttributesDlg();
    // App->m_pDoc->UpdateBrushAttributesDlg();
}

static signed int fdocUpdateFaceTextures(Face* pFace, void* lParam)
{
    App->CL_Face->Face_SetTextureDibId(pFace, App->CL_Level->Level_GetDibId(App->CL_Doc->pLevel, App->CL_Face->Face_GetTextureName(pFace)));
    // changed QD 12/03
    const WadFileEntry* const pbmp = App->CL_Level->Level_GetWadBitmap(App->CL_Doc->pLevel, App->CL_Face->Face_GetTextureName(pFace));
    if (pbmp)
        App->CL_Face->Face_SetTextureSize(pFace, pbmp->Width, pbmp->Height);
    // end change
    return GE_TRUE;
}

static signed int fdocUpdateBrushFaceTextures(Brush* pBrush, void* pVoid)
{
    App->CL_Brush->Brush_EnumFaces(pBrush, pVoid, ::fdocUpdateFaceTextures);
    return GE_TRUE;
}

// *************************************************************************
// *        UpdateAfterWadChange:- Terry and Hazel Flanigan 2025           *
// *************************************************************************
void CL64_Doc::UpdateAfterWadChange()
{
   flag_Is_Modified = 1;

   if (!App->CL_Level->Level_LoadWad(pLevel))
    {
       App->Say("Cant Load TXL File");
    }

    // update textures tab
   // mCurTextureSelection = 0;
    //App->CLSB_TextureDialog->Fill_ListBox();

    // update all brush faces
    App->CL_Brush->BrushList_EnumLeafBrushes(App->CL_Level->Level_GetBrushes(pLevel), this, ::fdocUpdateBrushFaceTextures);
    {
        // find the rendered view and set the wad size infos for it
        //POSITION		pos;
        //CFusionView* pView;

        //pos = App->m_pDoc->GetFirstViewPosition();
        //while (pos != NULL)
        //{
        //    pView = (CFusionView*)App->m_pDoc->GetNextView(pos);
        //    if (Render_GetViewType(pView->VCam) & (VIEWSOLID | VIEWTEXTURE | VIEWWIRE))
        //    {
        //        Render_SetWadSizes(pView->VCam, Level_GetWadSizeInfos(pLevel));
        //        break;	// Only 1 rendered view for now
        //    }
        //}
    }

   /* if (Level_RebuildBspAlways(pLevel))
    {
        App->m_pDoc->RebuildTrees();
        UpdateAllViews(UAV_ALL3DVIEWS, NULL);
    }*/
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
    
        int NumSelBrushes = App->CL_SelBrushList->SelBrushList_GetSize(pSelBrushes);
        for (int i = 0; i < NumSelBrushes; i++)
        {
            Brush* pBrush;

            pBrush = App->CL_SelBrushList->SelBrushList_GetBrush(pSelBrushes, 0);
            
            App->CL_Level->Level_RemoveBrush(pLevel, pBrush);
            App->CL_SelBrushList->SelBrushList_Remove(pSelBrushes, pBrush);
            App->CL_Brush->Brush_Destroy(&pBrush);

            bAlteredCurrentGroup = true;
        }
        
        //turn off any operation tools
        mCurrentTool = CURTOOL_NONE;

        flag_Is_Modified = 1;
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
    Set_Current_TxlPath();
}

// *************************************************************************
// *		Set_Current_3DT_Paths:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_Doc::Set_Current_3DT_Paths(void)
{
    strcpy(App->CL_Doc->mDoc_MTF_PathAndFile, App->CL_File->PathFileName_3dt);
    strcpy(App->CL_Doc->mDoc_MTF_Just_FileName, App->CL_File->FileName_3dt);

    char buf[MAX_PATH];
    strcpy(buf, App->CL_Doc->mDoc_MTF_Just_FileName);
    int Len = strlen(buf);
    buf[Len - 4] = 0;
    strcpy(App->CL_Doc->mDoc_MTF_JustName_NoExt, buf);

    strcpy(App->CL_Export->mJustName, App->CL_Doc->mDoc_MTF_JustName_NoExt);

}

// *************************************************************************
// *		Set_Current_TxlPath:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Doc::Set_Current_TxlPath(void)
{
    const char* WadFilePath;
    WadFilePath = App->CL_Level->Level_GetWadPath(App->CL_Doc->pLevel);

    strcpy(mDoc_TXL_Path_And_File, WadFilePath);

    App->CL_Utilities->Get_FileName_FromPath(mDoc_TXL_Path_And_File, mDoc_TXL_Path_And_File);

    strcpy(mDoc_TXL_Just_FileName, App->CL_Utilities->JustFileName);
}

// *************************************************************************
// *		SnapScaleNearest:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_Doc::SnapScaleNearest(int sides, int inidx, ViewVars* v)
{
    float	bsnap;

    flag_Is_Modified = 1;

    //App->CLSB_Doc->mLastOp = BRUSH_SCALE;

    bsnap = 1.0f;
    if (App->CL_Level->flag_UseGrid == 1)
    {
        bsnap = App->CL_Level->Level_GetGridSnapSize(App->CL_Doc->pLevel);
    }

   /* if (App->CLSB_Doc->mModeTool == ID_TOOLS_TEMPLATE)
    {
        Brush_SnapScaleNearest(App->CLSB_Doc->CurBrush, bsnap, sides, inidx, &App->CLSB_Doc->FinalScale, &App->CLSB_Doc->ScaleNum);
    }
    else*/
    {
        int i;
        int NumBrushes = App->CL_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pTempSelBrushes);

        for (i = 0; i < NumBrushes; ++i)
        {
            Brush* pBrush = App->CL_SelBrushList->SelBrushList_GetBrush(App->CL_Doc->pTempSelBrushes, i);
            App->CL_Brush->Brush_SnapScaleNearest(pBrush, bsnap, sides, inidx, &App->CL_Doc->FinalScale, &App->CL_Doc->ScaleNum);
        }
    }
}




