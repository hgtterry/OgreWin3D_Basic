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

enum BrushFlags
{
	BRUSH_SOLID = 0x0001,
	BRUSH_WINDOW = 0x0002,
	BRUSH_WAVY = 0x0004,
	BRUSH_DETAIL = 0x0008,	//not included in vis calculations
	BRUSH_HOLLOWCUT = 0x0010,
	BRUSH_TRANSLUCENT = 0x0020,
	BRUSH_EMPTY = 0x0040,
	BRUSH_SUBTRACT = 0x0080,
	BRUSH_CLIP = 0x0100,
	BRUSH_FLOCKING = 0x0200,
	BRUSH_HOLLOW = 0x0400,
	BRUSH_SHEET = 0x0800,
	BRUSH_HIDDEN = 0x1000,
	BRUSH_LOCKED = 0x2000,
	BRUSH_HINT = 0x4000,
	BRUSH_AREA = 0x8000
};


typedef struct tag_FaceList FaceList;

#define Normal_Scaler 2

CL64_OGL_Listener::CL64_OGL_Listener(void)
{
	Selected_Face_Group = 0;

	RX = 0;
	RZ = 0;

	Light_Activated = 0;

	mBrushCount = 0;
	mSubBrushCount = 0;

	Flag_Show_Selected_Brush = 1;
	Flag_Show_Selected_Face = 1;
	Flag_Render_Brushes = 0;

	Render_Mode = Enums::Render_Nothing;

	memset(g_BrushTexture,0,399);
}

CL64_OGL_Listener::~CL64_OGL_Listener(void)
{
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

	if (Render_Mode == Enums::Render_Groups && Flag_Render_Brushes == 1)
	{
		glEnable(GL_DEPTH_TEST);
		glShadeModel(GL_SMOOTH);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		Groups_Render_Textures();
	}

	// ---------------------- Brush
	if (Flag_Show_Selected_Brush == 1)
	{
		glDisable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		Render_Selected_Brush();
	}

	// ---------------------- Face
	if (Flag_Show_Selected_Face == 1)
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
// *		Groups_Render_Textures:- Terry and Hazel Flanigan 2025	   	   *
// *************************************************************************
void CL64_OGL_Listener::Groups_Render_Textures(void)
{
	int Count = 0;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);

	int GroupCount = App->CL_Model->GroupCount;

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

	if (App->CL_Model->Group[Count]->MaterialIndex > -1)
	{
		glEnable(GL_TEXTURE_2D);
		glColor3f(1, 1, 1);

		glBindTexture(GL_TEXTURE_2D, g_BrushTexture[App->CL_Model->Group[Count]->MaterialIndex]);

	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}

	while (VertCount < App->CL_Model->Group[Count]->GroupFaceCount)
	{
		A = App->CL_Model->Group[Count]->Face_Data[VertCount].a;
		B = App->CL_Model->Group[Count]->Face_Data[VertCount].b;
		C = App->CL_Model->Group[Count]->Face_Data[VertCount].c;

		glBegin(GL_POLYGON);

		//-----------------------------------------------
		glTexCoord2f(App->CL_Model->Group[Count]->MapCord_Data[A].u, App->CL_Model->Group[Count]->MapCord_Data[A].v);
		glNormal3fv(&App->CL_Model->Group[Count]->Normal_Data[A].x);
		glVertex3fv(&App->CL_Model->Group[Count]->vertex_Data[A].x);

		//-----------------------------------------------
		glTexCoord2f(App->CL_Model->Group[Count]->MapCord_Data[B].u, App->CL_Model->Group[Count]->MapCord_Data[B].v);
		glNormal3fv(&App->CL_Model->Group[Count]->Normal_Data[B].x);
		glVertex3fv(&App->CL_Model->Group[Count]->vertex_Data[B].x);

		//-----------------------------------------------
		glTexCoord2f(App->CL_Model->Group[Count]->MapCord_Data[C].u, App->CL_Model->Group[Count]->MapCord_Data[C].v);
		glNormal3fv(&App->CL_Model->Group[Count]->Normal_Data[C].x);
		glVertex3fv(&App->CL_Model->Group[Count]->vertex_Data[C].x);
		VertCount++;
		//-----------------------------------------------

		glEnd();

	}
}

// *************************************************************************
// *		Render_Selected_Face:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void CL64_OGL_Listener::Render_Selected_Face()
{
	int FC = App->CL_SelFaceList->SelFaceList_GetSize(App->CL_Doc->pSelFaces);
	if (FC > 0)
	{
		int j = 0;
		int curnum_verts = 0;

		Brush* pBrush;
		Face* pFace;

		int Actual_Brush_Index = 0;
		pBrush = App->CL_Doc->CurBrush;

		pFace = App->CL_SelFaceList->SelFaceList_GetFace(App->CL_Doc->pSelFaces, 0);

		const T_Vec3* verts;
		verts = App->CL_Face->Face_GetPoints(pFace);
		curnum_verts = App->CL_Face->Face_GetNumPoints(pFace);

		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);

		for (j = 0; j < curnum_verts; j++)
		{
			glVertex3f(verts[j].x, verts[j].y, verts[j].z);
		}

		glEnd();
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
		if (!Get_Brush(pBrush, Actual_Brush_Index))
		{

		}
	}
}

// *************************************************************************
// *				Get_Brush:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
bool CL64_OGL_Listener::Get_Brush(const Brush* b, int Actual_Brush_Index)
{
	switch (b->Type)
	{
	case BRUSH_MULTI:
	{
		return Brush_Decode_List(b->BList, GE_TRUE); // Recursive
	}

	case BRUSH_LEAF:
		if (b->BList)
		{
			return Brush_Decode_List(b->BList, GE_TRUE); // Recursive
		}
		else
		{
			if (!(b->Flags & (BRUSH_HOLLOW | BRUSH_HOLLOWCUT | BRUSH_SUBTRACT)))
			{
				return Brush_FaceList_Create(b, b->Faces, mBrushCount, mSubBrushCount, Actual_Brush_Index);

			}
			else if ((b->Flags & BRUSH_SUBTRACT) && !(b->Flags & (BRUSH_HOLLOW | BRUSH_HOLLOWCUT)))
			{
				mBrushCount--;
			}
		}
		break;

	case BRUSH_CSG:
		if (!(b->Flags & (BRUSH_HOLLOW | BRUSH_HOLLOWCUT | BRUSH_SUBTRACT)))
		{
			return Brush_FaceList_Create(b, b->Faces, mBrushCount, mSubBrushCount, Actual_Brush_Index);
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
bool CL64_OGL_Listener::Brush_Decode_List(BrushList* BList, signed int SubBrush)
{
	Brush* pBrush;
	BrushIterator bi;

	pBrush = App->CL_Brush->BrushList_GetFirst(BList, &bi);

	while (pBrush != NULL)
	{
		if (mSubBrushCount == 0 && pBrush->Flags & 1 || pBrush->Flags & 1024)
		{
			if (SubBrush == 0)
			{
				//strcpy(mBrush_Name, pBrush->Name);
				//mBrush_Index = mBrushCount;
			}
		}

		if (!Get_Brush(pBrush, 0))
		{
			return GE_FALSE;
		}

		pBrush = App->CL_Brush->BrushList_GetNext(&bi);

		if (SubBrush)
		{
			mSubBrushCount++;
		}
		else
		{
			//mBrushCount++;
			//Actual_Brush_Index++;
		}
	}

	mSubBrushCount = 0;

	if (!SubBrush)
	{
		mBrushCount = 0;
	}

	return GE_TRUE;
}

// *************************************************************************
// *		Brush_FaceList_Create:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
bool CL64_OGL_Listener::Brush_FaceList_Create(const Brush* b, const FaceList* pList, int BrushCount, int SubBrushCount, int Actual_Brush_Index)
{
	int i, j, num_faces, num_verts, num_mats, num_chars, curnum_verts;

	num_faces = num_verts = num_mats = num_chars = 0;
	
	// -----------------------------------  Vertices
	glColor3f(0.0f, 1.0f, 0.0f);
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

