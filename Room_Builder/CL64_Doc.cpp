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

#define MAX_PIXEL_SELECT_DIST (50)
#define MIN_ENTITY_SELECT_DIST (8.0f)
#define MAX_PIXEL_SELECT_THINGNAME (20)

typedef struct FindClosestInfoTag
{
    CL64_Doc* pDoc;
    ViewVars* v;
    Brush** ppFoundBrush;
    geFloat* pMinEdgeDist;
    const POINT* ptFrom;
} FindClosestInfo;

CL64_Doc::CL64_Doc(void)
{
    LastTemplateTypeName[0] = 0;
	pLevel = NULL;

	SelectLock = FALSE;
	TempEnt = FALSE;
    mCurrentTool = CURTOOL_NONE;
    mCurrentGroup = 0;
    flag_Is_Modified = 0;
    SelState = NOSELECTIONS;
}

CL64_Doc::~CL64_Doc(void)
{
}

// *************************************************************************
// *		    Init_Doc:- Terry and Hazel Flanigan 2024 	    		   *
// *************************************************************************
void CL64_Doc::Init_Doc()
{
	
    const char* DefaultWadName;
    strcpy(LastTemplateTypeName, "Box");

    DefaultWadName = App->CL_Prefs->Wad_File_Name;

    //strcpy(LastPath, Prefs_GetProjectDir(pPrefs));

    const char* WadPath = FindTextureLibrary(DefaultWadName);

	/*pLevel = Level_Create(WadPath, Prefs_GetHeadersList(pPrefs),
		Prefs_GetActorsList(pPrefs), Prefs_GetPawnIni(pPrefs));*/
   
	pLevel = App->CL_Level->Level_Create(WadPath, NULL,NULL, NULL);

	if (!App->CL_Level->Level_LoadWad(pLevel))
	{
		App->Say_Win("Can not load Wad File");
	}

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
	
	//App->Say("Done",(LPSTR)"");
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
		return Path_And_File;
	}
	else
	{
		App->Say_Win("File Does not Exist");

		strcpy(Path_And_File, "");
		return Path_And_File;
	}
	
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
		//OnBrushSubtractfromworld();
	}

	//SetModifiedFlag();
}

struct fdocFaceScales
{
    float DrawScale;
    float LightmapScale;
};

static signed int fdocSetFaceScales(Face* pFace, void* lParam)
{
    fdocFaceScales* pScales = (fdocFaceScales*)lParam;

    App->CL_Face->Face_SetTextureScale(pFace, pScales->DrawScale, pScales->DrawScale);
    App->CL_Face->Face_SetLightScale(pFace, pScales->LightmapScale, pScales->LightmapScale);
    
    return false;
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
	Ogre::Vector3* pTemplatePos;

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
	App->CL_Brush->Brush_EnumFaces(nb, &Scales, ::fdocSetFaceScales);

	App->CL_Level->Level_AppendBrush(pLevel, nb);

	if (!App->CL_Brush->Brush_IsHollow(nb) && !App->CL_Brush->Brush_IsMulti(nb))
	{
		App->CL_Doc->UpdateAllViews(UAV_ALL3DVIEWS, NULL);
	}
	else
	{
		App->CL_Doc->UpdateAllViews(UAV_ALL3DVIEWS | REBUILD_QUICK, NULL);
	}

	Placed = true;

	if (Placed)
	{
		App->CL_Doc->DoGeneralSelect();
		App->CL_Doc->flag_Is_Modified = 1;

	}
}

// *************************************************************************
// *           DoGeneralSelect:- Terry and Hazel Flanigan 2023             *
// *************************************************************************
void CL64_Doc::DoGeneralSelect(void)
{
    mCurrentTool = CURTOOL_NONE;
    mModeTool = ID_GENERALSELECT;
    //ConfigureCurrentTool();
}

// *************************************************************************
// *         UpdateAllViews:- Terry and Hazel Flanigan 2023                *
// *************************************************************************
void CL64_Doc::UpdateAllViews(int Mode, BOOL Override)
{
    RedrawWindow(App->CL_MapEditor->Main_Dlg_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
    //App->Get_Current_Document();

    //if (App->m_pDoc->IsModified() && ((Mode & REBUILD_QUICK) && (Level_RebuildBspAlways(App->CLSB_Doc->pLevel))) || (Override))
    //{
    //    App->m_pDoc->RebuildTrees();
    //}
    //else if ((Mode & REBUILD_QUICK) && (!Level_RebuildBspAlways(App->CLSB_Doc->pLevel)))
    //{
    //    App->m_pDoc->InvalidateDrawTreeOriginalFaces();
    //}

    //if (Mode & REBUILD_QUICK)
    //    Mode &= ~REBUILD_QUICK;

    ////	Do we want to redraw everything?
    //if (Mode & UAV_ALLVIEWS)
    //{
    //    App->m_pDoc->CDocument::UpdateAllViews(pSender);
    //    return;
    //}

    //POSITION pos = App->m_pDoc->GetFirstViewPosition();

    //while (pos != NULL)
    //{
    //    CView* pView = App->m_pDoc->GetNextView(pos);

    //    if (pView->IsKindOf(RUNTIME_CLASS(CFusionView)))
    //    {
    //        CFusionView* pFusionView = (CFusionView*)pView;
    //        CDC* pDC = pFusionView->GetDC();

    //        switch (Mode)
    //        {
    //        case UAV_ACTIVE3DVIEW_ONLY:

    //            if (pFusionView->GetParentFrame() == App->m_pDoc->mpActiveViewFrame)
    //                pFusionView->Invalidate(TRUE);
    //            break;

    //        case UAV_NONACTIVE3DVIEWS_ONLY:

    //            if (pFusionView->GetParentFrame() != App->m_pDoc->mpActiveViewFrame)
    //                pFusionView->Invalidate(TRUE);
    //            break;

    //        case UAV_TEXTUREVIEW_ONLY:

    //            if (pFusionView->mViewType == ID_VIEW_TEXTUREVIEW)
    //                pFusionView->Invalidate(TRUE);
    //            break;

    //        case UAV_RENDER_ONLY:

    //            switch (pFusionView->mViewType)
    //            {
    //            case ID_VIEW_3DWIREFRAME:
    //            case ID_VIEW_TEXTUREVIEW:

    //                pFusionView->Invalidate(TRUE);
    //                break;

    //            default:
    //                break;
    //            }
    //            break;

    //        case UAV_GRID_ONLY:

    //            switch (pFusionView->mViewType)
    //            {
    //            case ID_VIEW_TOPVIEW:
    //            case ID_VIEW_SIDEVIEW:
    //            case ID_VIEW_FRONTVIEW:

    //                pFusionView->Invalidate(TRUE);
    //                break;
    //            }
    //            break;


    //        case UAV_THIS_GRID_ONLY:
    //            if (pFusionView == pSender)
    //            {
    //                switch (pFusionView->mViewType)
    //                {
    //                case ID_VIEW_TOPVIEW:
    //                case ID_VIEW_SIDEVIEW:
    //                case ID_VIEW_FRONTVIEW:
    //                    pFusionView->Invalidate(TRUE);
    //                    break;
    //                }
    //            }
    //            break;

    //        case UAV_ALL3DVIEWS:

    //            pFusionView->Invalidate(TRUE);
    //            break;

    //        default:
    //            break;
    //        }

    //        pFusionView->ReleaseDC(pDC);
    //    }
    //}
}

typedef struct
{
    CL64_Doc* pDoc;
    const char* TexName;
} BrushTexSetData;

// *************************************************************************
// *			               BrushTexSetCB	                       	   *
// *************************************************************************
static signed int BrushTexSetCB(Brush* b, void* lParam)
{
    int			i;
    BrushTexSetData* pData;

    pData = (BrushTexSetData*)lParam;

    //	Brush_SetName(b, pData->TexName);
    App->CL_Brush->Brush_SetName(b, pData->pDoc->LastTemplateTypeName);
    //	char const * const BrushName = Brush_GetName (b);
    const int NumFaces = App->CL_Brush->Brush_GetNumFaces(b);

    //copy face TexInfos
    for (i = 0; i < NumFaces; i++) // hgtterry Debug
    {
        Face* f = App->CL_Brush->Brush_GetFace(b, i);
        //WadFileEntry* pbmp;

        ////		Face_SetTextureName(f, BrushName);
        ////		Face_SetTextureDibId(f, Level_GetDibId (pData->pDoc->pLevel, BrushName));
        ////		pbmp = Level_GetWadBitmap (pData->pDoc->pLevel, BrushName);
        //Face_SetTextureName(f, pData->TexName);
        //Face_SetTextureDibId(f, Level_GetDibId(App->CLSB_Doc->pLevel, pData->TexName));
        //pbmp = Level_GetWadBitmap(App->CLSB_Doc->pLevel, pData->TexName);
        //if (pbmp != NULL)
        //{
        //    Face_SetTextureSize(f, pbmp->Width, pbmp->Height);
        //}
    }

    App->CL_Brush->Brush_SetFaceListDirty(b);

    return true;
}

// *************************************************************************
// *			        SetDefaultBrushTexInfo	                       	   *
// *************************************************************************
void CL64_Doc::SetDefaultBrushTexInfo(Brush* b)
{
    const char* TexName;
    BrushTexSetData CallbackData;

    TexName = "";// App->CLSB_TextureDialog->m_CurrentTexture; hgtterry Debug
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
        ::BrushTexSetCB(b, &CallbackData);
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
static geFloat PointToLineDist
(
    POINT const* ptFrom,
    POINT const* ptLine1,
    POINT const* ptLine2
)
{
    geFloat xkj, ykj;
    geFloat xlk, ylk;
    geFloat denom;
    geFloat dist;

    xkj = (geFloat)(ptLine1->x - ptFrom->x);
    ykj = (geFloat)(ptLine1->y - ptFrom->y);
    xlk = (geFloat)(ptLine2->x - ptLine1->x);
    ylk = (geFloat)(ptLine2->y - ptLine1->y);
    denom = (xlk * xlk) + (ylk * ylk);
    if (denom < .0005f)
    {
        // segment ends coincide
        dist = xkj * xkj + ykj * ykj;
    }
    else
    {
        geFloat t;
        geFloat xfac, yfac;

        t = -(xkj * xlk + ykj * ylk) / denom;
        t = std::max(t, 0.0f);
        t = std::min(t, 1.0f);
        xfac = xkj + t * xlk;
        yfac = ykj + t * ylk;
        dist = xfac * xfac + yfac * yfac;
    }

    return (geFloat)sqrt(dist);
}

// *************************************************************************
// *			            FindClosestBrushCB	                       	   *
// *************************************************************************
static geBoolean FindClosestBrushCB(Brush* pBrush, void* pVoid)
{
    FindClosestInfo* fci = (FindClosestInfo*)pVoid;

   // if (fci->pDoc->BrushIsVisible(pBrush))
    {
        // for each face...
        for (int iFace = 0; iFace < App->CL_Brush->Brush_GetNumFaces(pBrush); ++iFace)
        {
            POINT pt1, pt2;
            Face* pFace = App->CL_Brush->Brush_GetFace(pBrush, iFace);
            const Ogre::Vector3* FacePoints = App->CL_Face->Face_GetPoints(pFace);
            int				NumPoints = App->CL_Face->Face_GetNumPoints(pFace);

            // Starting with the edge formed by the last point and the first point,
            // determine distance from mouse cursor pos to the edge.
            pt1 = App->CL_Render->Render_OrthoWorldToView(fci->v, &FacePoints[NumPoints - 1]);
            for (int iPoint = 0; iPoint < NumPoints; ++iPoint)
            {
                geFloat Dist;

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

// *************************************************************************
// *			        	   SelectOrtho                             	   *
// *************************************************************************
void CL64_Doc::SelectOrtho(POINT point, ViewVars* v)
{
    Brush* pMinBrush;
    geFloat Dist;
    int FoundThingType;

    /*if (IsSelectionLocked())
    {
        return;
    }*/

    // if Control key isn't pressed, then clear all current selections
   /* if ((GetAsyncKeyState(VK_SHIFT) & 0x8000) == 0)
    {
        ResetAllSelections();
    }*/

    FoundThingType = FindClosestThing(&point, v, &pMinBrush,&Dist);

    if ((FoundThingType != fctNOTHING) && (Dist <= MAX_PIXEL_SELECT_DIST))
    {
        if (FoundThingType == fctBRUSH)
        {
           App->Say_Win(pMinBrush->Name);

                 DoBrushSelection(pMinBrush, brushSelToggle);
                // if (App->CLSB_Brushes->Dimensions_Dlg_Running == 1)
                //  {
                //     App->CLSB_Brushes->Update_Pos_Dlg(App->CLSB_Brushes->Dimensions_Dlg_hWnd);
                //  }
        }
        
    }

    /*UpdateSelected();

    App->CLSB_TabsControl->Select_Brushes_Tab(0);
    App->CL_TabsGroups_Dlg->Get_Index(CurBrush);

    App->CL_TabsGroups_Dlg->Update_Dlg_Controls();
    App->CLSB_TopTabs->Update_Dlg_Controls();*/
}

// *************************************************************************
// *             UpdateSelected:- Terry and Hazel Flanigan 2025            *
// *************************************************************************
void CL64_Doc::UpdateSelected(void)
{
    int		i;
    int NumSelFaces = App->CL_SelFaceList->SelFaceList_GetSize(pSelFaces);
    int NumSelBrushes = App->CL_SelBrushList->SelBrushList_GetSize(pSelBrushes);

    SelState = (NumSelBrushes > 1) ? MULTIBRUSH : NumSelBrushes;
    SelState |= (NumSelFaces > 1) ? MULTIFACE : (NumSelFaces + 1) << 3;
   

    if (mModeTool == ID_GENERALSELECT)
    {
        if (GetSelState() & ONEBRUSH)
        {
            CurBrush = App->CL_SelBrushList->SelBrushList_GetBrush(pSelBrushes, 0);
        }
        else
        {
            CurBrush = BTemplate;
        }
    }

    App->CL_Maths->Vector3_Clear(&SelectedGeoCenter);

    if (mModeTool == ID_TOOLS_TEMPLATE)
    {
        App->CL_Brush->Brush_Center(CurBrush, &SelectedGeoCenter);
    }
    else if (SelState != NOSELECTIONS)
    {
        //Model* pModel;
        //ModelInfo_Type* ModelInfo = Level_GetModelInfo(pLevel);

        //pModel = ModelList_GetAnimatingModel(ModelInfo->Models);
        //if (pModel != NULL)
        //{
        //    // we're animating a model, so use its current position
        //    Model_GetCurrentPos(pModel, &SelectedGeoCenter);
        //}
        //else
        {
            if (NumSelBrushes)
            {
                App->CL_SelBrushList->SelBrushList_Center(pSelBrushes, &SelectedGeoCenter);
            }
            /*else if (NumSelEntities)
            {
                Ogre::Vector3 EntitySelectionCenter = { 0.0f,0.0f,0.0f };

                CEntityArray* Entities;
                Entities = Level_GetEntities(pLevel);
                if (Entities)
                {
                    int NumEntities = Entities->GetSize();

                    for (int i = 0; i < NumEntities; i++)
                    {
                        if ((*Entities)[i].IsSelected())
                        {
                            App->CL_Maths->Vector3_Add(&EntitySelectionCenter, &(*Entities)[i].mOrigin, &EntitySelectionCenter);
                        }
                    }
                }

                App->CL_Maths->Vector3_Scale(&EntitySelectionCenter, 1 / (float)(NumSelEntities), &SelectedGeoCenter);
            }*/
        }
    }

   /* if (SelState & ONEENTITY)
    {
        CEntityArray* Entities = Level_GetEntities(pLevel);

        for (i = 0; i < Entities->GetSize() && !((*Entities)[i].IsSelected()); i++);
        mCurrentEntity = i;
    }
    else
    {
        mCurrentEntity = -1;
    }*/

   // App->m_pDoc->UpdateFaceAttributesDlg();
   // App->m_pDoc->UpdateBrushAttributesDlg();

    //assert( mpMainFrame->m_wndTabControls ) ;
    //assert( mpMainFrame->m_wndTabControls->GrpTab ) ;
    //mpMainFrame->m_wndTabControls->GrpTab->UpdateGroupSelection( ) ;

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

   /* ModelInfo = App->CL_Level->Level_GetModelInfo(pLevel);
    Groups = App->CL_Level->Level_GetGroups(pLevel);*/
    BList = App->CL_Level->Level_GetBrushes(pLevel);

    if (App->CL_Brush->Brush_GetParent(BList, pBrush, &pBParent))
    {
        pBrush = pBParent;
    }

    ModelLocked = GE_FALSE;
    GroupLocked = FALSE;
    //	if(mAdjustMode != ADJUST_MODE_FACE)
    {
        // don't do this stuff if we're in face mode...
       /* ModelId = Brush_GetModelId(pBrush);
        if (ModelId != 0)
        {
            Model* pModel;

            pModel = ModelList_FindById(ModelInfo->Models, ModelId);
            if (pModel != NULL)
            {
                ModelLocked = Model_IsLocked(pModel);
            }
        }*/

       /* if (!ModelLocked)
        {
            GroupId = Brush_GetGroupId(pBrush);
            if (GroupId != 0)
            {
                GroupLocked = Group_IsLocked(Groups, GroupId);
            }
        }*/
    }

    if (nSelType == brushSelToggle && BrushIsSelected(pBrush))
    {
        if (ModelLocked)
        {
            // model is locked, so deselect everything in the model
            //SelectModelBrushes(FALSE, ModelId);
        }
        else if (GroupLocked)
        {
            // group is locked, so deselect entire group
            // SelectGroupBrushes(FALSE, GroupId);
        }
        else
        {
            // SelBrushList_Remove(pSelBrushes, pBrush);

        }
    }
    else
    {
        //if (ModelLocked)
        //{
        //    // model is locked, so select everything in the model
        //    SelectModelBrushes(TRUE, ModelId);
        //}
        //else if (GroupLocked)
        //{
        //    // group is locked.  Select everything in the group
        //    SelectGroupBrushes(TRUE, GroupId);
        //}
        //else
        {
            // SelBrushList_Add(pSelBrushes, pBrush);

            /*if (strstr(App->CL_Brush->Brush_GetName(pBrush), ".act") != NULL)
            {
                CEntityArray* Entities = Level_GetEntities(pLevel);

                for (int i = 0; i < Entities->GetSize(); i++)
                {
                    Brush* b = (*Entities)[i].GetActorBrush();
                    if (b != NULL)
                        if (SelBrushList_Find(pSelBrushes, b))
                            if (!(*Entities)[i].IsSelected())
                            {
                                (*Entities)[i].Select();
                                ++NumSelEntities;
                            }
                }
            }*/
        }
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
int CL64_Doc::FindClosestThing(POINT const* ptFrom, ViewVars* v, Brush** ppMinBrush, geFloat* pDist)
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
// *            FindClosestBrush:- Terry and Hazel Flanigan 2023           *
// *************************************************************************
signed int CL64_Doc::FindClosestBrush(POINT const* ptFrom, ViewVars* v, Brush** ppFoundBrush, geFloat* pMinEdgeDist)
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

