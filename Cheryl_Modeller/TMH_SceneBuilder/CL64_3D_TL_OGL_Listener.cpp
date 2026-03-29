/*
Copyright (c) 2024 - 2026 HGT_Software W.T.Flanigan H.C.Flanigan

Cheryl 3D Modeller

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
#include "CL64_3D_TL_OGL_Listener.h"

CL64_3D_TL_OGL_Listener::CL64_3D_TL_OGL_Listener()
{
}

CL64_3D_TL_OGL_Listener::~CL64_3D_TL_OGL_Listener()
{
}

// *************************************************************************
// *			renderQueueStarted:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_3D_TL_OGL_Listener::renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation)
{
}

// *************************************************************************
// *			renderQueueEnded:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_3D_TL_OGL_Listener::renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation)
{
	/*if (App->CL_Editor_Control->flag_Scene_Game_Running == true)
	{
		return;
	}*/

	if (queueGroupId != Ogre::RENDER_QUEUE_MAIN)
	{
		return;
	}

	PreRender();

	Render_Loop();

	PostRender();
}

// *************************************************************************
// *				PreRender:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_3D_TL_OGL_Listener::PreRender()
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
	renderSystem->_setViewMatrix(App->CL_3D_TL_View->Ogre_TL_Camera->getViewMatrix());
	renderSystem->_setProjectionMatrix(App->CL_3D_TL_View->Ogre_TL_Camera->getProjectionMatrixWithRSDepth());

	static Ogre::Pass* clearPass = NULL;
	if (!clearPass)
	{
		Ogre::MaterialPtr clearMat = Ogre::MaterialManager::getSingleton().getByName("BaseWhite");
		clearPass = clearMat->getTechnique(0)->getPass(0);
	}

	//Set a clear pass to give the renderer a clear renderstate
	//App->CL_Ogre->mSceneMgr->_setPass(clearPass, true);

	// save attribs
	glPushAttrib(GL_ALL_ATTRIB_BITS);
}

// *************************************************************************
// *				PostRender:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_3D_TL_OGL_Listener::PostRender()
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
// *				Render_Loop:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_3D_TL_OGL_Listener::Render_Loop()
{

	GLboolean depthTestEnabled = glIsEnabled(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);
	GLboolean stencilTestEnabled = glIsEnabled(GL_STENCIL_TEST);
	glDisable(GL_STENCIL_TEST);

	// ---------------------- Mesh
	//if (App->CL_Model->flag_Model_Loaded && flag_ShowFaces == true)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		MeshData_Render_Faces();
	}

	RECT Test;
	Test.right = 300;
	Test.bottom = 300;

	HDC     hdc;
	hdc = GetDC(App->CL_3D_TL_View->ViewGLhWnd_TL);

	//Render_RenderOrthoGridFromSize(App->CL_Editor_Map->Current_View, 1.5, hdc, Test);

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
// *		MeshData_Render_Faces:- Terry and Hazel Flanigan 2024	 	   *
// *************************************************************************
void CL64_3D_TL_OGL_Listener::MeshData_Render_Faces(void)
{
	int Count = 0;

	glColor3f(1, 1, 1);

	int GroupCount = App->CL_Model->GroupCount;


	//if (flag_ShowOnlySubFaces == 1) // Show Only Selected SubMesh
	//{
	//	MeshData_Face_Groups(Selected_Face_Group);
	//	return;
	//}

	while (Count < GroupCount)
	{
		/*if (flag_Show_Material_Faces == true && Count == App->CL_Properties_Textures_Com->Selected_Group)
		{

		}
		else*/
		{
			MeshData_Face_Groups(Count);
		}

		Count++;
	}

}

// *************************************************************************
// *		MeshData_Face_Groups:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_3D_TL_OGL_Listener::MeshData_Face_Groups(int Count)
{
	// Set the polygon mode to draw lines for both front and back faces
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Retrieve the group and its face count
	auto& group = App->CL_Mesh->Group[Count];
	int faceCount = group->GroupFaceCount;

	// Iterate through each face in the group
	for (int i = 0; i < faceCount; ++i)
	{
		// Retrieve vertex indices for the current face
		int A = group->Face_Data[i].a;
		int B = group->Face_Data[i].b;
		int C = group->Face_Data[i].c;

		// Begin drawing the polygon
		glBegin(GL_POLYGON);

		// Specify the vertices of the polygon
		glVertex3fv(&group->vertex_Data[A].x);
		glVertex3fv(&group->vertex_Data[B].x);
		glVertex3fv(&group->vertex_Data[C].x);

		// End drawing the polygon
		glEnd();
	}
}

static const T_Vec3	VecOrigin = { 0.0f, 0.0f, 0.0f };

#define	VectorToSUB(a, b) (*((((float *)(&a))) + (b)))
#define Units_FRound(n)	((float)floor((n)+0.5f))
#define Units_Round(n) ((int)Units_FRound((n)))

// *************************************************************************
// *	  				Render_RenderOrthoGridFromSize					   *
// *************************************************************************
bool CL64_3D_TL_OGL_Listener::Render_RenderOrthoGridFromSize(ViewVars* cv, int Interval, HDC hDC, RECT Rect)
{
	auto& m_Maths = App->CL_X_Maths; // Maths Library

	cv->Width = App->CL_3D_TL_View->vp->getActualWidth();
	cv->Height = App->CL_3D_TL_View->vp->getActualHeight();

	T_Vec3 ystep, xstep, Delt, Delt2;
	int			i, cnt, xaxis, yaxis, inidx;
	static int axidx[3][2] = { 2, 1, 0, 2, 0, 1 };
	float	gsinv;
	Box3d ViewBox;
	POINT		sp;

	inidx = (cv->ViewType >> 3) & 0x3;

	xaxis = axidx[inidx][0];
	yaxis = axidx[inidx][1];

	App->CL_Render->Render_ViewToWorld(cv, Units_Round(-Interval), Units_Round(-Interval), &Delt);
	App->CL_Render->Render_ViewToWorld(cv, Units_Round(cv->Width + Interval), Units_Round(cv->Height + Interval), &Delt2);

	App->CL_X_Box->Box3d_Set(&ViewBox, Delt.x, Delt.y, Delt.z, Delt2.x, Delt2.y, Delt2.z);

	VectorToSUB(ViewBox.Min, inidx) = -FLT_MAX;
	VectorToSUB(ViewBox.Max, inidx) = FLT_MAX;

	gsinv = 1.0f / (float)Interval;
	for (i = 0; i < 3; i++)
	{
		VectorToSUB(ViewBox.Min, i) = (float)((int)(VectorToSUB(ViewBox.Min, i) * gsinv)) * Interval;
		VectorToSUB(ViewBox.Max, i) = (float)((int)(VectorToSUB(ViewBox.Max, i) * gsinv)) * Interval;
	}

	m_Maths->Vector3_Copy(&VecOrigin, &xstep);
	m_Maths->Vector3_Copy(&VecOrigin, &ystep);
	VectorToSUB(ystep, yaxis) = (float)Interval;
	VectorToSUB(xstep, xaxis) = (float)Interval;

	cnt = Rect.bottom / Interval; // TODO hgtterry Debug Odd

	// horizontal lines
	int Count = 0;
	m_Maths->Vector3_Copy(&ViewBox.Min, &Delt);
	m_Maths->Vector3_Copy(&ViewBox.Min, &Delt2);
	VectorToSUB(Delt2, xaxis) = VectorToSUB(ViewBox.Max, xaxis);
	cnt = Units_Round((VectorToSUB(ViewBox.Max, yaxis) - VectorToSUB(ViewBox.Min, yaxis)) * gsinv);

	while (Count < cnt)
	{
		sp = App->CL_Render->Render_OrthoWorldToView(cv, &Delt);
		//MoveToEx(hDC, 0, sp.y, NULL);
		sp = App->CL_Render->Render_OrthoWorldToView(cv, &Delt2);
		//LineTo(hDC, cv->Width, sp.y);
		m_Maths->Vector3_Add(&Delt, &ystep, &Delt);
		m_Maths->Vector3_Add(&Delt2, &ystep, &Delt2);
		Count++;
	}

	// vertical lines
	Count = 0;
	m_Maths->Vector3_Copy(&ViewBox.Min, &Delt);
	m_Maths->Vector3_Copy(&ViewBox.Min, &Delt2);
	VectorToSUB(Delt2, yaxis) = VectorToSUB(ViewBox.Max, yaxis);
	cnt = Units_Round((VectorToSUB(ViewBox.Max, xaxis) - VectorToSUB(ViewBox.Min, xaxis)) * gsinv);

	while (Count < cnt)
	{
		sp = App->CL_Render->Render_OrthoWorldToView(cv, &Delt);
		float stp = sp.x;

		//MoveToEx(hDC, sp.x, 0, NULL);
		sp = App->CL_Render->Render_OrthoWorldToView(cv, &Delt2);

		float epp = sp.x;

		//LineTo(hDC, sp.x, cv->Height);
		m_Maths->Vector3_Add(&Delt, &xstep, &Delt);
		m_Maths->Vector3_Add(&Delt2, &xstep, &Delt2);

		//Render_Line(stp, epp);
		Count++;
	}

	return 1;
}

// *************************************************************************
// *				Render_Line:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
void CL64_3D_TL_OGL_Listener::Render_Line(float	sp, float ep)
{
	glColor3f(1, 0, 0);

	// Set the polygon mode to draw lines for both front and back faces
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBegin(GL_LINES);

	float Top = App->CL_3D_TL_View->Ogre_TL_Window->getHeight();
	//Top = Top / 2;

	glVertex3f(sp, -Top, 0);
	glVertex3f(ep, -Top, 1000);

	glEnd();

}
