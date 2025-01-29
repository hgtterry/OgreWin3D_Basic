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

struct tag_FaceList
{
	int NumFaces;
	int Limit;
	Face** Faces;
	geBoolean Dirty;
	Box3d Bounds;
};

typedef struct tag_FaceList FaceList;

#define Normal_Scaler 2

CL64_OGL_Listener::CL64_OGL_Listener(void)
{
	Selected_Face_Group = 0;

	RX = 0;
	RZ = 0;

	Hair_1PosX = 0;
	Hair_1PosY = 0;
	Hair_1PosZ = 0;

	Hair_1RotX = 0;
	Hair_1RotY = 0;
	Hair_1RotZ = 0;

	Flag_ShowFaces = 0;
	Flag_ShowBoundingBox = 0;
	Flag_ShowPoints = 0;
	Flag_ShowBones = 0;
	Flag_ShowNormals = 0;
	Flag_ShowTextured = 0;

	Light_Activated = 0;

	flag_ShowOnlySubFaces = 0;

	mBrushCount = 0;
	mSubBrushCount = 0;
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

	//---------------------- Textured
	/*if (App->CL_Scene->flag_Model_Loaded == 1 && Flag_ShowTextured == 1)
	{
		glEnable(GL_DEPTH_TEST);
		glShadeModel(GL_SMOOTH);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		MeshData_Render_Textures();
	}*/

	// ---------------------- Mesh
	if (Flag_ShowFaces == 1)
	{
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//if (Block == 0)
		{
			Block = 1;
			MeshData_Render_Faces();
			Block = 0;
		}
	}

	// ---------------------- Points
	/*if (App->CL_Scene->flag_Model_Loaded == 1 && Flag_ShowPoints == 1)
	{
		MeshData_Render_Points();
	}*/

	// ---------------------- Bounding Box
	/*if (App->CL_Scene->flag_Model_Loaded && Flag_ShowBoundingBox == 1)
	{
		MeshData_Render_BoundingBox();
	}*/

	// ---------------------- Normals
	/*if (App->CL_Scene->flag_Model_Loaded == 1 && Flag_ShowNormals == 1)
	{
		MeshData_Render_Normals();
	}*/

	// ---------------------- Bones
	/*if (App->CL_Scene->flag_Model_Loaded == 1 && Flag_ShowBones == 1)
	{
		MeshData_RenderBones();
	}*/

	// ---------------------- Crosshair
	//if (Show_Crosshair == 1)
	{
		//RenderCrossHair();
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
// *		MeshData_Render_Textures:- Terry and Hazel Flanigan 2024	   *
// *************************************************************************
bool CL64_OGL_Listener::MeshData_Render_Textures(void)
{
	/*int Count = 0;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);

	int GroupCount = App->CL_Scene->GroupCount;

	Count = 0;
	while (Count < GroupCount)
	{
		MeshData_Textured_Groups(Count);
		Count++;
	}

	glDisable(GL_TEXTURE_2D);*/

	return 1;
}
// *************************************************************************
// *		MeshData_Textured_Groups:- Terry and Hazel Flanigan 2024	   *
// *************************************************************************
bool CL64_OGL_Listener::MeshData_Textured_Groups(int Count)
{
	//int VertCount = 0;
	//int A = 0;
	//int B = 0;
	//int C = 0;

	//if (App->CL_Scene->Group[Count]->MaterialIndex > -1)
	//{
	//	glEnable(GL_TEXTURE_2D);
	//	glColor3f(1, 1, 1);

	//	glBindTexture(GL_TEXTURE_2D, App->CL_Textures->g_Texture[App->CL_Scene->Group[Count]->MaterialIndex]);

	//}
	//else
	//{
	//	glDisable(GL_TEXTURE_2D);
	//}

	//while (VertCount < App->CL_Scene->Group[Count]->GroupFaceCount)
	//{
	//	A = App->CL_Scene->Group[Count]->Face_Data[VertCount].a;
	//	B = App->CL_Scene->Group[Count]->Face_Data[VertCount].b;
	//	C = App->CL_Scene->Group[Count]->Face_Data[VertCount].c;

	//	glBegin(GL_POLYGON);

	//	//-----------------------------------------------
	//	glTexCoord2f(App->CL_Scene->Group[Count]->MapCord_Data[A].u, App->CL_Scene->Group[Count]->MapCord_Data[A].v);
	//	glNormal3fv(&App->CL_Scene->Group[Count]->Normal_Data[A].x);
	//	glVertex3fv(&App->CL_Scene->Group[Count]->vertex_Data[A].x);

	//	//-----------------------------------------------
	//	glTexCoord2f(App->CL_Scene->Group[Count]->MapCord_Data[B].u, App->CL_Scene->Group[Count]->MapCord_Data[B].v);
	//	glNormal3fv(&App->CL_Scene->Group[Count]->Normal_Data[B].x);
	//	glVertex3fv(&App->CL_Scene->Group[Count]->vertex_Data[B].x);

	//	//-----------------------------------------------
	//	glTexCoord2f(App->CL_Scene->Group[Count]->MapCord_Data[C].u, App->CL_Scene->Group[Count]->MapCord_Data[C].v);
	//	glNormal3fv(&App->CL_Scene->Group[Count]->Normal_Data[C].x);
	//	glVertex3fv(&App->CL_Scene->Group[Count]->vertex_Data[C].x);
	//	VertCount++;
	//	//-----------------------------------------------

	//	glEnd();

	//}

	return 1;
}

// *************************************************************************
// *		MeshData_Render_Faces:- Terry and Hazel Flanigan 2024	 	   *
// *************************************************************************
void CL64_OGL_Listener::MeshData_Render_Faces(void)
{
	if (App->CL_Model->BrushCount == 0)
	{
		return;
	}

	int Count = 0;

	glColor3f(1, 1, 1);

	int GroupCount = App->CL_Model->BrushCount;
	MeshData_Face_Groups(NULL);
}

// *************************************************************************
// *		MeshData_Face_Groups:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_OGL_Listener::MeshData_Face_Groups(int Count)
{
	Brush* pBrush;
	int Actual_Brush_Index = 0;
	pBrush = App->CL_Doc->CurBrush;

	if (!Brush_Create(pBrush, Actual_Brush_Index))
	{
		
	}
}

// *************************************************************************
// *			Brush_Create:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
bool CL64_OGL_Listener::Brush_Create(const Brush* b, int Actual_Brush_Index)
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

		if (!Brush_Create(pBrush, 0))
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

	assert(pList != NULL);
	assert(f != NULL);

	num_faces = num_verts = num_mats = num_chars = 0;
	
	// -----------------------------------  Vertices
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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

// *************************************************************************
// *		MeshData_Render_Points:- Terry and Hazel Flanigan 2024	  	   *
// *************************************************************************
void CL64_OGL_Listener::MeshData_Render_Points(void)
{
	/*int Count = 0;

	glColor3f(1.0f, 1.0f, 0.0f);

	int GroupCount = App->CL_Scene->GroupCount;

	while (Count < GroupCount)
	{
		MeshData_Points_Groups(Count);
		Count++;
	}*/
}

// *************************************************************************
// *		MeshData_Points_Groups:- Terry and Hazel Flanigan 2024	   	   *
// *************************************************************************
void CL64_OGL_Listener::MeshData_Points_Groups(int Count)
{
	/*glPointSize(5);

	int VertCount = 0;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	int GroupVertCount = App->CL_Scene->Group[Count]->GroupVertCount;

	while (VertCount < GroupVertCount)
	{
		glBegin(GL_POINTS);

		glVertex3fv(&App->CL_Scene->Group[Count]->vertex_Data[VertCount].x);

		glEnd();

		VertCount++;
	}*/
}

//**************************************************************************
// *		MeshData_Render_Normals:- Terry and Hazel Flanigan 2024   	   *
// *************************************************************************
void CL64_OGL_Listener::MeshData_Render_Normals(void)
{
	/*int Count = 0;

	glColor3f(1, 1, 1);

	int GroupCount = App->CL_Scene->GroupCount;

	while (Count < GroupCount)
	{
		MeshData_Normals_Groups(Count);
		Count++;
	}*/
}

// *************************************************************************
// *		MeshData_Normals_Groups:- Terry and Hazel Flanigan 2024	 	   *
// *************************************************************************
void CL64_OGL_Listener::MeshData_Normals_Groups(int Count)
{
	//int VertCount = 0;

	//glPointSize(3);
	//glBegin(GL_LINES);

	//while (VertCount < App->CL_Scene->Group[Count]->GroupVertCount)
	//{
	//	//-----------------------------------------------
	//	glVertex3fv(&App->CL_Scene->Group[Count]->vertex_Data[VertCount].x);

	//	glVertex3f(App->CL_Scene->Group[Count]->vertex_Data[VertCount].x + App->CL_Scene->Group[Count]->Normal_Data[VertCount].x * Normal_Scaler,
	//		App->CL_Scene->Group[Count]->vertex_Data[VertCount].y + App->CL_Scene->Group[Count]->Normal_Data[VertCount].y * Normal_Scaler,
	//		App->CL_Scene->Group[Count]->vertex_Data[VertCount].z + App->CL_Scene->Group[Count]->Normal_Data[VertCount].z * Normal_Scaler);

	//	VertCount++;
	//}

	//glEnd();
}

// *************************************************************************
// *		MeshData_RenderBones:- Terry and Hazel Flanigan 2024 	  	   *
// *************************************************************************
void CL64_OGL_Listener::MeshData_RenderBones()
{
	//glDisable(GL_TEXTURE_2D);
	//glDisable(GL_DEPTH_TEST);
	//int Start = 0;

	//glColor3f(1, 1, 0);
	//glPointSize(6); //PointSize
	//int Point = 0;

	//while (Start < App->CL_Scene->BoneCount)
	//{

	//	if (App->CL_Scene->S_Bones[Start]->Parent == -1)
	//	{
	//		glColor3f(1, 0, 0);			// Root Joint Colour
	//	}
	//	else { glColor3f(0, 0, 1); }		// Joint Colours

	//	glBegin(GL_POINTS);
	//	glVertex3f(App->CL_Scene->S_Bones[Start]->TranslationStart.X,
	//		App->CL_Scene->S_Bones[Start]->TranslationStart.Y,
	//		App->CL_Scene->S_Bones[Start]->TranslationStart.Z);

	//	glEnd();
	//	Start++;
	//}

	//Start = 0;

	//while (Start < App->CL_Scene->BoneCount)
	//{

	//	if (App->CL_Scene->S_Bones[Start]->Parent == -1)
	//	{
	//		glColor3f(1, 0, 0);			// Root Joint Color Again Both the same
	//		glBegin(GL_POINTS);
	//		glVertex3f(App->CL_Scene->S_Bones[Start]->TranslationStart.X,
	//			App->CL_Scene->S_Bones[Start]->TranslationStart.Y,
	//			App->CL_Scene->S_Bones[Start]->TranslationStart.Z);
	//		glEnd();
	//	}
	//	else
	//	{
	//		glLineWidth(3);
	//		glBegin(GL_LINES);
	//		glColor3f(1, 1, 0);			// Bone Colours Between Joints
	//		glVertex3f(App->CL_Scene->S_Bones[Start]->TranslationStart.X,
	//			App->CL_Scene->S_Bones[Start]->TranslationStart.Y,
	//			App->CL_Scene->S_Bones[Start]->TranslationStart.Z);

	//		glVertex3f(App->CL_Scene->S_Bones[App->CL_Scene->S_Bones[Start]->Parent]->TranslationStart.X,
	//			App->CL_Scene->S_Bones[App->CL_Scene->S_Bones[Start]->Parent]->TranslationStart.Y,
	//			App->CL_Scene->S_Bones[App->CL_Scene->S_Bones[Start]->Parent]->TranslationStart.Z);

	//		glEnd();
	//	}

	//	Start++;
	//}

}

// *************************************************************************
// *	MeshData_Render_BoundingBox:- Terry and Hazel Flanigan 2024	  	   *
// *************************************************************************
void CL64_OGL_Listener::MeshData_Render_BoundingBox(void)
{
	/*float m_xMin = App->CL_Scene->S_BoundingBox[0]->BB_Min[0].x;
	float m_yMin = App->CL_Scene->S_BoundingBox[0]->BB_Min[0].y;
	float m_zMin = App->CL_Scene->S_BoundingBox[0]->BB_Min[0].z;

	float m_xMax = App->CL_Scene->S_BoundingBox[0]->BB_Max[0].x;
	float m_yMax = App->CL_Scene->S_BoundingBox[0]->BB_Max[0].y;
	float m_zMax = App->CL_Scene->S_BoundingBox[0]->BB_Max[0].z;

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);

	glLineWidth(2);

	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);

	glVertex3f(m_xMin, m_yMin, m_zMin);
	glVertex3f(m_xMin, m_yMin, m_zMax);

	glVertex3f(m_xMax, m_yMin, m_zMin);
	glVertex3f(m_xMax, m_yMin, m_zMax);

	glVertex3f(m_xMin, m_yMax, m_zMin);
	glVertex3f(m_xMin, m_yMax, m_zMax);

	glVertex3f(m_xMax, m_yMax, m_zMin);
	glVertex3f(m_xMax, m_yMax, m_zMax);

	glVertex3f(m_xMin, m_yMin, m_zMin);
	glVertex3f(m_xMax, m_yMin, m_zMin);

	glVertex3f(m_xMin, m_yMin, m_zMin);
	glVertex3f(m_xMin, m_yMax, m_zMin);

	glVertex3f(m_xMax, m_yMin, m_zMin);
	glVertex3f(m_xMax, m_yMax, m_zMin);

	glVertex3f(m_xMin, m_yMax, m_zMin);
	glVertex3f(m_xMax, m_yMax, m_zMin);

	glVertex3f(m_xMin, m_yMin, m_zMax);
	glVertex3f(m_xMax, m_yMin, m_zMax);

	glVertex3f(m_xMin, m_yMin, m_zMax);
	glVertex3f(m_xMin, m_yMax, m_zMax);

	glVertex3f(m_xMax, m_yMin, m_zMax);
	glVertex3f(m_xMax, m_yMax, m_zMax);

	glVertex3f(m_xMin, m_yMax, m_zMax);
	glVertex3f(m_xMax, m_yMax, m_zMax);

	glEnd();
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);*/
}

// **************************************************************************
// *			RenderCrossHair:- Terry and Hazel Flanigan 2024				*
// **************************************************************************
void CL64_OGL_Listener::RenderCrossHair(void)
{
	glDisable(GL_TEXTURE_2D);
	
	float Length = 40;
	glLineWidth(3);

	glTranslatef(Hair_1PosX, Hair_1PosY, Hair_1PosZ);

	glRotatef(Hair_1RotX, 1.0, 0.0, 0.0); // Rotations of the object 
	glRotatef(Hair_1RotY, 0.0, 1.0, 0.0);
	glRotatef(Hair_1RotZ, 0.0, 0.0, 1.0);

	glScalef(1, 1, 1);

	//-------------------------------------------  x hair] Blue
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 1.0f); // Color
	glVertex3f(-Length, 0, 0);
	glVertex3f(0, 0, 0);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(Length, 0, 0);
	glEnd();

	//-------------------------------------------  z hair Red
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f); // Color
	glVertex3f(0, 0, -Length);
	glVertex3f(0, 0, 0);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, Length);
	glEnd();

	//-------------------------------------------  y hair Green
	glBegin(GL_LINES);
	glColor3f(0.0f, 1.0f, 0.0f); // Color
	glVertex3f(0, -Length, 0);
	glVertex3f(0, 0, 0);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, Length, 0);
	glEnd();
}