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

CL64_Doc::CL64_Doc(void)
{
    LastTemplateTypeName[0] = 0;
	pLevel = NULL;

	SelectLock = FALSE;
	TempEnt = FALSE;
    mCurrentTool = CURTOOL_NONE;
    mCurrentGroup = 0;
    flag_Is_Modified = 0;
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

