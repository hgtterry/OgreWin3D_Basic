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
#include "CL64_3D_TR_OGL_Listener.h"

CL64_3D_TR_OGL_Listener::CL64_3D_TR_OGL_Listener()
{
	
}

CL64_3D_TR_OGL_Listener::~CL64_3D_TR_OGL_Listener()
{
}

// *************************************************************************
// *			renderQueueStarted:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_3D_TR_OGL_Listener::renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation)
{
}

// *************************************************************************
// *			renderQueueEnded:- Terry and Hazel Flanigan 2026		   *
// *************************************************************************
void CL64_3D_TR_OGL_Listener::renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation)
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
void CL64_3D_TR_OGL_Listener::PreRender()
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
	renderSystem->_setViewMatrix(App->CL_3D_TR_View->Ogre_MV_Camera->getViewMatrix());
	renderSystem->_setProjectionMatrix(App->CL_3D_TR_View->Ogre_MV_Camera->getProjectionMatrixWithRSDepth());

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
void CL64_3D_TR_OGL_Listener::PostRender()
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
void CL64_3D_TR_OGL_Listener::Render_Loop()
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
void CL64_3D_TR_OGL_Listener::MeshData_Render_Faces(void)
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
void CL64_3D_TR_OGL_Listener::MeshData_Face_Groups(int Count)
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
