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

#define AXIS_X	0x1
#define AXIS_Y	0x2
#define AXIS_Z	0x4

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

    mLastOp = 0;

    ScaleNum = 1;
    sides = 1;
    NumSelEntities = 0;
    mAdjustMode = ADJUST_MODE_FACE;

	SelectLock = FALSE;
	TempEnt = FALSE;
    mCurrentTool = CURTOOL_NONE;
    mCurrentGroup = 0;

    flag_Is_Modified = 0;
    flag_IsNewDocument = 0;

    SelState = NOSELECTIONS;

    strcpy(mCurrent_MTF_PathAndFile, App->RB_Directory_FullPath);
    strcat(mCurrent_MTF_PathAndFile, "\\Data\\Room_Builder\\Test.mtf");
}

CL64_Doc::~CL64_Doc(void)
{
}

// *************************************************************************
// *		    Init_Doc:- Terry and Hazel Flanigan 2025 	    		   *
// *************************************************************************
void CL64_Doc::Init_Doc()
{
    const char* DefaultWadName;
    strcpy(LastTemplateTypeName, "Box");

    DefaultWadName = App->CL_Prefs->Wad_File_Name;

    //strcpy(LastPath, Prefs_GetProjectDir(pPrefs));

    const char* WadPath = FindTextureLibrary(DefaultWadName);

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
		OnBrushSubtractfromworld();
        //App->Say("AddBrushToWorld - Subtract");
	}

	//SetModifiedFlag();
}

struct fdocFaceScales
{
    float DrawScale;
    float LightmapScale;
};

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
    }

    //App->CLSB_Doc->UpdateSelected();
    App->CL_Doc->UpdateAllViews(Enums::UpdateViews_All);
}

// *************************************************************************
// *			       ( Static ) fdocSetFaceScales	                   	   *
// *************************************************************************
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
		App->CL_Doc->UpdateAllViews(Enums::UpdateViews_All);
	}
	else
	{
		App->CL_Doc->UpdateAllViews(Enums::UpdateViews_All);
	}

	Placed = true;

	if (Placed)
	{
		App->CL_Doc->DoGeneralSelect();
		App->CL_Doc->flag_Is_Modified = 1;

	}



   // App->CL_Doc->CurBrush->Faces[0]->
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
// *         UpdateAllViews:- Terry and Hazel Flanigan 2025                *
// *************************************************************************
void CL64_Doc::UpdateAllViews(int Update_Mode)
{
    if (Update_Mode == Enums::UpdateViews_Grids)
    {
        RedrawWindow(App->CL_MapEditor->Left_Window_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
        RedrawWindow(App->CL_MapEditor->Right_Window_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
        RedrawWindow(App->CL_MapEditor->Bottom_Left_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
    }

    if (Update_Mode == Enums::UpdateViews_3D)
    {
        App->CL_Doc->RebuildTrees();
        App->CL_Mesh_Mgr->Update_World();
    }

    if (Update_Mode == Enums::UpdateViews_All)
    {
        RedrawWindow(App->CL_MapEditor->Left_Window_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
        RedrawWindow(App->CL_MapEditor->Right_Window_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
        RedrawWindow(App->CL_MapEditor->Bottom_Left_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

        App->CL_Doc->RebuildTrees();
        App->CL_Mesh_Mgr->Update_World();
    }
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
        WadFileEntry* pbmp;
        // 
        App->CL_Face->Face_SetTextureName(f, pData->TexName);
        App->CL_Face->Face_SetTextureDibId(f, App->CL_Level->Level_GetDibId(App->CL_Doc->pLevel, pData->TexName));
        pbmp = App->CL_Level->Level_GetWadBitmap(App->CL_Doc->pLevel, pData->TexName);
        if (pbmp != NULL)
        {
            App->CL_Face->Face_SetTextureSize(f, pbmp->Width, pbmp->Height);
        }
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

#pragma warning (disable:4100)
static geBoolean ResetSelectedFacesCB(Brush* b, void* pVoid)
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
    geFloat Dist;
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
            DoBrushSelection(pMinBrush, brushSelToggle);    

            int Bnum = App->CL_Brush->Get_Brush_Count();
            if (Bnum > 0)
            {
                UpdateSelected();
                App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);

                App->CL_Properties_Tabs->Select_Brushes_Tab(0);
                App->CL_Properties_Brushes->Get_Index(CurBrush);

                App->CL_Top_Tabs->Enable_Brush_Options_Buttons(true, false);
     
            }
        } 
    }
    else
    {
        App->CL_Top_Tabs->Enable_Brush_Options_Buttons(false, false);
        App->CL_Doc->UpdateAllViews(Enums::UpdateViews_Grids);
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
    CurBrush = BTemplate;
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
// *            FindClosestBrush:- Terry and Hazel Flanigan 2025           *
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
   // App->Say("RebuildTrees");
   
    int	CurId = 0;
   /* GNode* n;
    AddBrushCBData	BspCallbackData;
    ModelInfo_Type* ModelInfo;*/
    BrushList* BList;
    //Model* pMod;

    BList = App->CL_Level->Level_GetBrushes(App->CL_Doc->pLevel);
    //SetModifiedFlag();


    //do the world csg list and tree first
   /* Node_ClearBsp(App->CLSB_Doc->mWorldBsp);
    App->CL_Doc->mWorldBsp = NULL;*/

    App->CL_Brush->BrushList_ClearAllCSG(BList);

    App->CL_Brush->BrushList_DoCSG(BList, CurId, ::fdocBrushCSGCallback, this);

    /*BspCallbackData.pDoc = this;
    BspCallbackData.CurId = CurId;
    BspCallbackData.pTree = &App->CL_Doc->mWorldBsp;
    BspCallbackData.bAddFlocking = GE_FALSE;

    App->CL_Brush->BrushList_EnumCSGBrushes(BList, &BspCallbackData, ::AddBrushToBspCB);
    BspCallbackData.bAddFlocking = GE_TRUE;
    App->CL_Brush->BrushList_EnumCSGBrushes(BList, &BspCallbackData, ::AddBrushToBspCB);*/

    //build individual model mini trees
    //ModelInfo = Level_GetModelInfo(App->CLSB_Doc->pLevel);
    //pMod = ModelList_GetFirst(ModelInfo->Models, &mi);
    //n = NULL;
    //for (i = 0; i < ModelList_GetCount(ModelInfo->Models); i++, n = NULL)
    //{
    //    CurId = Model_GetId(pMod);

    //    BrushList_DoCSG(BList, CurId, ::fdocBrushCSGCallback, this);

    //    //change pvoid from this to null to skip the BrushIsVisible check
    //    BspCallbackData.CurId = CurId;
    //    BspCallbackData.pTree = &n;
    //    BspCallbackData.bAddFlocking = GE_FALSE;
    //    BrushList_EnumCSGBrushes(BList, &BspCallbackData, ::AddBrushToBspCB);

    //    BspCallbackData.bAddFlocking = GE_TRUE;
    //    BrushList_EnumCSGBrushes(BList, &BspCallbackData, ::AddBrushToBspCB);

    //    Node_ClearBsp(Model_GetModelTree(pMod));
    //    Model_SetModelTree(pMod, n);
    //    pMod = ModelList_GetNext(ModelInfo->Models, &mi);
    //}
    //if (App->CLSB_Doc->mAdjustMode == ADJUST_MODE_FACE)
    //{
    //    UpdateFaceAttributesDlg();
    //}

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
void CL64_Doc::LockAxis(Ogre::Vector3* pWP)
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
void CL64_Doc::MoveSelectedBrushes(Ogre::Vector3 const* v)
{
    MoveSelectedBrushList(pTempSelBrushes, v);
}

// *************************************************************************
// *         MoveSelectedBrushList:- Terry and Hazel Flanigan 2025         *
// *************************************************************************
void CL64_Doc::MoveSelectedBrushList(SelBrushList* pList, Ogre::Vector3 const* v)
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
   // CFusionDoc* pDoc = GetDocument();
   // int ModeTool = GetModeTool();

   // pDoc->SetModifiedFlag();

    if (App->CL_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes) > 0)// || ModeTool == ID_TOOLS_TEMPLATE)
    {
        float fSnapSize;
        const Ogre::Vector3* vMin, * vMax;
        const Box3d* pBox;
        Ogre::Vector3 SnapDelta;
        geBoolean SnapX, SnapY, SnapZ;

        fSnapSize = 1.0f;
       /* if (Level_UseGrid(App->CLSB_Doc->pLevel))
        {
            fSnapSize = Level_GetGridSnapSize(App->CLSB_Doc->pLevel);
        }*/
        // do the snap thing...
        pBox = App->CL_Brush->Brush_GetBoundingBox(App->CL_Doc->CurBrush);
        vMin = App->CL_Box->Box3d_GetMin(pBox);
        vMax = App->CL_Box->Box3d_GetMax(pBox);
        App->CL_Maths->Vector3_Clear(&SnapDelta);
        /*
          In template mode, the brush is moved directly, so we have to snap to
          the current position, not current position plus delta.  Since we
          clear the delta before computing the snap, we have to save these
          flags.
        */
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

// *************************************************************************
// *			                 DoneMove                           	   *
// *************************************************************************
void CL64_Doc::DoneMove(void)
{
    int	i;
    //	BrushList *BList = Level_GetBrushes (pLevel);

    mLastOp = BRUSH_MOVE;

    TempDeleteSelected();

    if (mModeTool == ID_TOOLS_TEMPLATE)
    {
        /*if (TempEnt)
        {
            DoneMoveEntity();
        }
        else
        {
            Brush_Move(CurBrush, &FinalPos);
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
        }

        if (GetSelState() & ANYENTITY)
        {
            //DoneMoveEntity();
        }

        UpdateSelected();

       // App->m_pDoc->UpdateSelectedModel(BRUSH_MOVE, &FinalPos);
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
    float	ZoomInv = App->CL_MapEditor->Current_View->ZoomFactor;// Render_GetZoom(VCam);

    ZoomInv = (ZoomInv > .5) ? 0.5f / ZoomInv : 1.0f;

    // negated here because Brush_Resize is still thinking weird
    App->CL_Doc->ResizeSelected(-(((float)dx) * ZoomInv), -(((float)dy) * ZoomInv), sides, App->CL_Render->Render_GetInidx(App->CL_MapEditor->Current_View));
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
// *      ( Static ) SelAllBrushFaces:- Terry and Hazel Flanigan 2025      *
// *************************************************************************
static signed int SelAllBrushFaces(Brush* pBrush, void* lParam)
{
    int iFace, nFaces;

    nFaces = App->CL_Brush->Brush_GetNumFaces(pBrush);
    for (iFace = 0; iFace < nFaces; ++iFace)
    {
        Face* pFace;

        pFace = App->CL_Brush->Brush_GetFace(pBrush, iFace);
        App->CL_Face->Face_SetSelected(pFace, GE_TRUE);
        App->CL_SelFaceList->SelFaceList_Add(App->CL_Doc->pSelFaces, pFace);
    }
    return GE_TRUE;
}

// *************************************************************************
// *        SelectAllFacesInBrushes:- Terry and Hazel Flanigan 2025        *
// *************************************************************************
void CL64_Doc::SelectAllFacesInBrushes(void)
{
    DoGeneralSelect(); // hgtterry check function

    // Select all faces on all selected brushes
    int iBrush;
    int NumSelBrushes = App->CL_SelBrushList->SelBrushList_GetSize(pSelBrushes);

    for (iBrush = 0; iBrush < NumSelBrushes; ++iBrush)
    {
        Brush* pBrush;

        pBrush = App->CL_SelBrushList->SelBrushList_GetBrush(pSelBrushes, iBrush);
        if (App->CL_Brush->Brush_IsMulti(pBrush))
        {
            App->CL_Brush->BrushList_EnumLeafBrushes(App->CL_Brush->Brush_GetBrushList(pBrush), this, ::SelAllBrushFaces);
        }
        else
        {
            SelAllBrushFaces(pBrush, this);
        }
    }

    UpdateSelected();
    //ConfigureCurrentTool();
}

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
    DoGeneralSelect();
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


    if (mModeTool == ID_GENERALSELECT && NumSelBrushes == 1)
    {
        //if (GetSelState() & ONEBRUSH)
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

    //if (mModeTool == ID_TOOLS_TEMPLATE)
    //{
    //    App->CL_Brush->Brush_Center(CurBrush, &SelectedGeoCenter);
    //}
    //else if (SelState != NOSELECTIONS)
    //{
    //    if (NumSelBrushes)
    //    {
    //        App->CL_SelBrushList->SelBrushList_Center(pSelBrushes, &SelectedGeoCenter);
    //    }
    //}

    // App->m_pDoc->UpdateFaceAttributesDlg();
    // App->m_pDoc->UpdateBrushAttributesDlg();
}



