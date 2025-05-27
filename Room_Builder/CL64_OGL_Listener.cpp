/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

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
#include "CL64_App.h"
#include "CL64_OGL_Listener.h"
#include "Structures.cpp"

typedef struct tag_FaceList FaceList;

#define Normal_Scaler 2

CL64_OGL_Listener::CL64_OGL_Listener(void)
{
	Selected_Face_Group = 0;

	RX = 0;
	RZ = 0;

	Light_Activated = 0;

	flag_Render_Just_Brush = 0;
	Selected_Brush_Index = 0;
	
	mBrushCount = 0;
	mSubBrushCount = 0;

	flag_Show_Selected_Brush =0;
	flag_Show_Selected_Face = 0;

	flag_Render_Ogre = 1;
	flag_Render_Groups = 0;
	flag_Render_Brushes = 0;

	flag_Just_Face = 0;

	Render_Mode = Enums::Render_Nothing;

	memset(g_BrushTexture,0,399);
}

CL64_OGL_Listener::~CL64_OGL_Listener(void)
{
}

// *************************************************************************
// *				Show_Visuals:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_OGL_Listener::Show_Visuals(bool Show)
{
	if (Show == true)
	{
		flag_Show_Selected_Brush = 1;
		flag_Show_Selected_Face = 1;
	}
	else
	{
		flag_Show_Selected_Brush = 0;
		flag_Show_Selected_Face = 0;
	}
}

// *************************************************************************
// *			renderQueueStarted:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_OGL_Listener::renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation)
{
}

// *************************************************************************
// *			renderQueueEnded:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_OGL_Listener::renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation)
{
	if (queueGroupId != Ogre::RENDER_QUEUE_MAIN)
	{
		return;
	}

	PreRender();

	Render_Loop();

	PostRender();
}

// *************************************************************************
// *				PreRender:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_OGL_Listener::PreRender()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity(); //Texture addressing should start out as direct.

	Ogre::RenderSystem* renderSystem = App->CL_Ogre->manObj->_getManager()->getDestinationRenderSystem();
	Ogre::Node* parentNode = App->CL_Ogre->manObj->getParentNode();
	
#pragma warning(disable : 4996) // Nightmare why

	renderSystem->_setWorldMatrix(parentNode->_getFullTransform());
	renderSystem->_setWorldMatrix(parentNode->_getFullTransform());
	renderSystem->_setViewMatrix(App->CL_Ogre->mCamera->getViewMatrix());
	renderSystem->_setProjectionMatrix(App->CL_Ogre->mCamera->getProjectionMatrixWithRSDepth());

	static Ogre::Pass* clearPass = NULL;
	if (!clearPass)
	{
		Ogre::MaterialPtr clearMat = Ogre::MaterialManager::getSingleton().getByName("BaseWhite");
		clearPass = clearMat->getTechnique(0)->getPass(0);
	}
	//Set a clear pass to give the renderer a clear renderstate
	App->CL_Ogre->mSceneMgr->_setPass(clearPass, true);
	//clearPass->
	// save attribs
	glPushAttrib(GL_ALL_ATTRIB_BITS);
}

// *************************************************************************
// *				PostRender:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_OGL_Listener::PostRender()
{
	// restore original state
	glPopAttrib();

	// restore matrices
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

// *************************************************************************
// *				Render_Loop:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_OGL_Listener::Render_Loop()
{
	GLboolean depthTestEnabled = glIsEnabled(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);
	GLboolean stencilTestEnabled = glIsEnabled(GL_STENCIL_TEST);
	glDisable(GL_STENCIL_TEST);

	//glDisable(GL_CULL_FACE);

	if (Light_Activated == 0)
	{
		glDisable(GL_LIGHTING);
	}
	else
	{
		glEnable(GL_LIGHTING);

		GLfloat ambientLightFull[] = { 0.2f, 0.2f, 0.2f, 1.0f };

		glLightModelfv(GL_LIGHT_MODEL_AMBIENT,
			ambientLightFull);
		glEnable(GL_LIGHT0);
	}

	glColor3f(0.8f, 0.8f, 0.8f);
	Translate();

	if (Render_Mode == Enums::Render_Groups && flag_Render_Groups == 1)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		Groups_Render_Faces();
	}

	if (Render_Mode == Enums::Render_Brushes && flag_Render_Brushes == 1)
	{
		glColor4f(0.0f, 1.0f, 0.0f, 0.8f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		Brushes_Render_Faces();
	}

	// ---------------------- Selected Brush
	if (flag_Show_Selected_Brush == 1)
	{
		int BC = App->CL_Brush->Get_Brush_Count();
		if (BC > 0)
		{
			glDisable(GL_CULL_FACE);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			//Render_Selected_Brush();
			Render_Brush(App->CL_Doc->CurBrush);
		}
	}

	// ---------------------- Selected Face
	if (flag_Show_Selected_Face == 1)
	{
		glDisable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(2);
		Render_Selected_Face();
	}
	

	if (depthTestEnabled)
	{
		glEnable(GL_DEPTH_TEST);
	}
	if (stencilTestEnabled)
	{
		glEnable(GL_STENCIL_TEST);
	}
}

// *************************************************************************
// *				Translate:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_OGL_Listener::Translate(void)
{
	glRotatef(RX, 1.0, 0.0, 0.0); // Rotations of the object 

	glRotatef(RZ, 0.0, 1.0, 0.0);
	glRotatef(0.0, 0.0, 0.0, 1.0);
}

// *************************************************************************
// *		Bruses_Render_Faces:- Terry and Hazel Flanigan 2025	   		   *
// *************************************************************************
void CL64_OGL_Listener::Brushes_Render_Faces(void)
{
	int Count = 0;

	int BrushCount = App->CL_Scene->BrushCount;

	while (Count < BrushCount)
	{
		/*if (flag_Render_Just_Brush == 0)
		{
			glLineWidth(1);

			Brushes_Face_Parts(Count);
		}
		else*/
		{
			if (Count == Selected_Brush_Index)
			{
				glDisable(GL_CULL_FACE);
				glColor3f(1, 0, 0);
				glLineWidth(1);

				Brushes_Face_Parts(Count);
			}
			else
			{
				glEnable(GL_CULL_FACE);
				glColor3f(0, 1, 0);
				glLineWidth(1);

				Brushes_Face_Parts(Count);
			}
		}

		Count++;
	}
}

// *************************************************************************
// *		Bruses_Face_Parts:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
void CL64_OGL_Listener::Brushes_Face_Parts(int Count)
{
	int FaceCount = 0;
	int A = 0;
	int B = 0;
	int C = 0;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (FaceCount < App->CL_Scene->B_Brush[Count]->Face_Count)
	{
		A = App->CL_Scene->B_Brush[Count]->Face_Data[FaceCount].a;
		B = App->CL_Scene->B_Brush[Count]->Face_Data[FaceCount].b;
		C = App->CL_Scene->B_Brush[Count]->Face_Data[FaceCount].c;

		glBegin(GL_POLYGON);

		//-----------------------------------------------
		glVertex3fv(&App->CL_Scene->B_Brush[Count]->vertex_Data[A].x);

		//-----------------------------------------------
		glVertex3fv(&App->CL_Scene->B_Brush[Count]->vertex_Data[B].x);

		//-----------------------------------------------
		glVertex3fv(&App->CL_Scene->B_Brush[Count]->vertex_Data[C].x);

		FaceCount++;

		glEnd();
	}
}

// *************************************************************************
// *		Groups_Render_Textures:- Terry and Hazel Flanigan 2025	   	   *
// *************************************************************************
void CL64_OGL_Listener::Groups_Render_Textures(void)
{
	int Count = 0;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);

	int GroupCount = App->CL_Scene->GroupCount;

	//if (ShowOnlySubMesh == 1) // Show Only Selected SubMesh
	//{
	//	Groups_Textured_Parts(Selected_Group);
	//	glDisable(GL_TEXTURE_2D);
	//	return;
	//}

	//Count = 0;
	//if (Show_HideGroup == 1) // Hide Selected SubMesh
	//{
	//	while (Count < GroupCount)
	//	{
	//		if (Selected_Group == Count)
	//		{
	//		}
	//		else
	//		{
	//			Groups_Textured_Parts(Count);
	//		}
	//		Count++;
	//	}

	//	glDisable(GL_TEXTURE_2D);
	//	return;
	//}

	Count = 0;
	while (Count < GroupCount)
	{
		Groups_Textured_Parts(Count);
		Count++;
	}

	glDisable(GL_TEXTURE_2D);

}

// *************************************************************************
// *		Groups_Textured_Part:- Terry and Hazel Flanigan 2025	 	   *
// *************************************************************************
void CL64_OGL_Listener::Groups_Textured_Parts(int Count)
{
	int VertCount = 0;
	int A = 0;
	int B = 0;
	int C = 0;

	if (App->CL_Scene->Group[Count]->MaterialIndex > -1)
	{
		glEnable(GL_TEXTURE_2D);
		glColor3f(1, 1, 1);

		glBindTexture(GL_TEXTURE_2D, g_BrushTexture[App->CL_Scene->Group[Count]->MaterialIndex]);

	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}

	while (VertCount < App->CL_Scene->Group[Count]->GroupFaceCount)
	{
		A = App->CL_Scene->Group[Count]->Face_Data[VertCount].a;
		B = App->CL_Scene->Group[Count]->Face_Data[VertCount].b;
		C = App->CL_Scene->Group[Count]->Face_Data[VertCount].c;

		glBegin(GL_POLYGON);

		//-----------------------------------------------
		glTexCoord2f(App->CL_Scene->Group[Count]->MapCord_Data[A].u, App->CL_Scene->Group[Count]->MapCord_Data[A].v);
		glNormal3fv(&App->CL_Scene->Group[Count]->Normal_Data[A].x);
		glVertex3fv(&App->CL_Scene->Group[Count]->vertex_Data[A].x);

		//-----------------------------------------------
		glTexCoord2f(App->CL_Scene->Group[Count]->MapCord_Data[B].u, App->CL_Scene->Group[Count]->MapCord_Data[B].v);
		glNormal3fv(&App->CL_Scene->Group[Count]->Normal_Data[B].x);
		glVertex3fv(&App->CL_Scene->Group[Count]->vertex_Data[B].x);

		//-----------------------------------------------
		glTexCoord2f(App->CL_Scene->Group[Count]->MapCord_Data[C].u, App->CL_Scene->Group[Count]->MapCord_Data[C].v);
		glNormal3fv(&App->CL_Scene->Group[Count]->Normal_Data[C].x);
		glVertex3fv(&App->CL_Scene->Group[Count]->vertex_Data[C].x);
		VertCount++;
		//-----------------------------------------------

		glEnd();

	}
}

// *************************************************************************
// *		Groups_Render_Faces:- Terry and Hazel Flanigan 2025		 	   *
// *************************************************************************
void CL64_OGL_Listener::Groups_Render_Faces(void)
{
	int Count = 0;
	int GroupCount = App->CL_Scene->GroupCount;

	if (flag_Just_Face == 1)
	{
		Groups_Faces_Parts(App->CL_Picking->m_SubMesh);
	}
	else
	{
		Count = 0;
		while (Count < GroupCount)
		{
			Groups_Faces_Parts(Count);
			Count++;
		}
	}

}

// *************************************************************************
// *		Groups_Face_Part:- Terry and Hazel Flanigan 2025		 	   *
// *************************************************************************
void CL64_OGL_Listener::Groups_Faces_Parts(int Count)
{
	int FaceCount = 0;
	int A = 0;
	int B = 0;
	int C = 0;

	while (FaceCount < App->CL_Scene->Group[Count]->GroupFaceCount)
	{
		A = App->CL_Scene->Group[Count]->Face_Data[FaceCount].a;
		B = App->CL_Scene->Group[Count]->Face_Data[FaceCount].b;
		C = App->CL_Scene->Group[Count]->Face_Data[FaceCount].c;

		glBegin(GL_POLYGON);

		//-----------------------------------------------
		
		glVertex3fv(&App->CL_Scene->Group[Count]->vertex_Data[A].x);
		glVertex3fv(&App->CL_Scene->Group[Count]->vertex_Data[B].x);
		glVertex3fv(&App->CL_Scene->Group[Count]->vertex_Data[C].x);

		FaceCount++;

		//-----------------------------------------------

		glEnd();
	}
}

// *************************************************************************
// *		Render_Selected_Face:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_OGL_Listener::Render_Selected_Face()
{
	Brush* pBrush = App->CL_Doc->CurBrush;
	const T_Vec3* verts;
	int j = 0;
	int curnum_verts = 0;

	int Count = 0;
	int Face_Count = App->CL_SelFaceList->SelFaceList_GetSize(App->CL_Doc->pSelFaces);
	
	if (Face_Count > 0)
	{
		while (Count < Face_Count)
		{
			j = 0;
			curnum_verts = 0;

			Face* pFace;

			pFace = App->CL_SelFaceList->SelFaceList_GetFace(App->CL_Doc->pSelFaces, Count);

			verts = App->CL_Face->Face_GetPoints(pFace);
			curnum_verts = App->CL_Face->Face_GetNumPoints(pFace);

			glColor3f(1.0f, 0.0f, 1.0f);
			glBegin(GL_POLYGON);

			for (j = 0; j < curnum_verts; j++)
			{
				glVertex3f(verts[j].x, verts[j].y, verts[j].z);
			}

			glEnd();

			Count++;
		}
	}
}

// *************************************************************************
// *		Render_Selected_Brush:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_OGL_Listener::Render_Selected_Brush()
{
	Brush* pBrush;
	int Actual_Brush_Index = 0;
	pBrush = App->CL_Doc->CurBrush;

	if (App->CL_SelBrushList->SelBrushList_GetSize(App->CL_Doc->pSelBrushes) > 0)
	{
		if (!Get_Brush(pBrush))
		{

		}
	}
}

// *************************************************************************
// *				Get_Brush:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
bool CL64_OGL_Listener::Get_Brush(const Brush* b)
{
	switch (b->Type)
	{
	case BRUSH_MULTI:
	{
		return Brush_Decode_List(b->BList); // Recursive
	}

	case BRUSH_LEAF:
		if (b->BList)
		{
			return Brush_Decode_List(b->BList); // Recursive
		}
		else
		{
			if (!(b->Flags & (BRUSH_HOLLOW | BRUSH_HOLLOWCUT | BRUSH_SUBTRACT)))
			{
				return Brush_FaceList_Render(b, b->Faces);

			}
			else if ((b->Flags & BRUSH_SUBTRACT) && !(b->Flags & (BRUSH_HOLLOW | BRUSH_HOLLOWCUT)))
			{
				return Brush_FaceList_Render(b, b->Faces);
				mBrushCount--;
			}
		}
		break;

	case BRUSH_CSG:
		if (!(b->Flags & (BRUSH_HOLLOW | BRUSH_HOLLOWCUT | BRUSH_SUBTRACT)))
		{
			return Brush_FaceList_Render(b, b->Faces);
		}

		break;

	default:
		assert(0);
		break;
	}

	return GE_TRUE;
}

// *************************************************************************
// *			Brush_Decode_List:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
bool CL64_OGL_Listener::Brush_Decode_List(BrushList* BList)
{
	Brush* pBrush;
	BrushIterator bi;

	pBrush = App->CL_Brush->BrushList_GetFirst(BList, &bi);

	while (pBrush != NULL)
	{
		if (!Get_Brush(pBrush)) // Recursive
		{
			return GE_FALSE;
		}

		pBrush = App->CL_Brush->BrushList_GetNext(&bi);
	}

	return GE_TRUE;
}

// *************************************************************************
// *		Brush_FaceList_Render:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
bool CL64_OGL_Listener::Brush_FaceList_Render(const Brush* b, const FaceList* pList)
{
	int i, j, num_verts, curnum_verts;

	num_verts = 0;
	curnum_verts = 0;

	// -----------------------------------  Vertices
	glColor3f(0.0f, 1.0f, 1.0f);

	for (i = 0; i < pList->NumFaces; i++)
	{
		const T_Vec3* verts;
		verts = App->CL_Face->Face_GetPoints(pList->Faces[i]);
		curnum_verts = App->CL_Face->Face_GetNumPoints(pList->Faces[i]);

		glBegin(GL_POLYGON);

		for (j = 0; j < curnum_verts; j++)
		{
			glVertex3f(verts[j].x, verts[j].y, verts[j].z);
		}

		glEnd();
	}

	return GE_TRUE;
}

// *************************************************************************
// *	  	Render_Brush:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
bool CL64_OGL_Listener::Render_Brush(const Brush* b)
{

	if (b->Type == BRUSH_MULTI)
	{
		return Render_Sub_Brushes(b->BList); // Recursive
	}
	if (b->Type == BRUSH_LEAF)
	{
		return Render_Faces(b->Faces);
	}
	if (b->Type == BRUSH_CSG)
	{
		//App->Say("BRUSH_CSG");
	}

	return 1;
}

// *************************************************************************
// *	  	Render_Sub_Brushes:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
bool CL64_OGL_Listener::Render_Sub_Brushes(BrushList* BList)
{
	Brush* pBrush;
	BrushIterator bi;

	pBrush = App->CL_Brush->BrushList_GetFirst(BList, &bi);
	while (pBrush != NULL)
	{
		Render_Brush(pBrush);  // Recursive

		pBrush = App->CL_Brush->BrushList_GetNext(&bi);
	}

	return 1;
}

// *************************************************************************
// *			Render_Faces:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
bool CL64_OGL_Listener::Render_Faces(const FaceList* pList)
{
	int i;

	if (pList->NumFaces < 0)
	{
	}
	else
	{
		for (i = 0; i < pList->NumFaces; i++)
		{
			if (!Render_Face(pList->Faces[i])) return 0;
		}
	}

	return 1;
}

// *************************************************************************
// *				Render_Face:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
bool CL64_OGL_Listener::Render_Face(const Face* f)
{
	int j, num_verts, curnum_verts;

	num_verts = 0;
	curnum_verts = 0;

	// -----------------------------------  Vertices
	glColor3f(0.0f, 1.0f, 1.0f);

	const T_Vec3* verts;
	verts = App->CL_Face->Face_GetPoints(f);
	curnum_verts = App->CL_Face->Face_GetNumPoints(f);

	glBegin(GL_POLYGON);

	for (j = 0; j < curnum_verts; j++)
	{
		glVertex3f(verts[j].x, verts[j].y, verts[j].z);
	}

	glEnd();

	return GE_TRUE;
}

