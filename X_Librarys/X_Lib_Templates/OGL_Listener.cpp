#include "pch.h"
#include "CL64_App.h"
#include "OGL_Listener.h"
#include "Structures.cpp"

OGL_Listener::OGL_Listener(void)
{
	mBrushCount = 0;

	RX = 0;
	RZ = 0;
}

OGL_Listener::~OGL_Listener(void)
{
}

// *************************************************************************
// *			renderQueueStarted:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void OGL_Listener::renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation)
{
}

// *************************************************************************
// *			renderQueueEnded:- Terry and Hazel Flanigan 2025		   *
// *************************************************************************
void OGL_Listener::renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation)
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
void OGL_Listener::PreRender()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity(); //Texture addressing should start out as direct.

	Ogre::RenderSystem* renderSystem = App->CL_X_Shapes_3D->OGL_Obj->_getManager()->getDestinationRenderSystem();
	Ogre::Node* parentNode = App->CL_X_Shapes_3D->OGL_Obj->getParentNode();

#pragma warning(disable : 4996) // Nightmare why

	//renderSystem->_setWorldMatrix(parentNode->_getFullTransform());
	renderSystem->_setWorldMatrix(parentNode->_getFullTransform());
	renderSystem->_setViewMatrix(App->CL_X_Shapes_3D->Ogre_MV_Camera->getViewMatrix());
	renderSystem->_setProjectionMatrix(App->CL_X_Shapes_3D->Ogre_MV_Camera->getProjectionMatrixWithRSDepth());

	static Ogre::Pass* clearPass = NULL;
	if (!clearPass)
	{
		Ogre::MaterialPtr clearMat = Ogre::MaterialManager::getSingleton().getByName("BaseWhite");
		clearPass = clearMat->getTechnique(0)->getPass(0);
	}
	//Set a clear pass to give the renderer a clear renderstate
	App->CL_X_Shapes_3D->Ogre_MV_SceneMgr->_setPass(clearPass, true);
	//clearPass->
	// save attribs
	glPushAttrib(GL_ALL_ATTRIB_BITS);
}

// *************************************************************************
// *				PostRender:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void OGL_Listener::PostRender()
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
void OGL_Listener::Render_Loop()
{
	GLboolean depthTestEnabled = glIsEnabled(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);
	GLboolean stencilTestEnabled = glIsEnabled(GL_STENCIL_TEST);
	glDisable(GL_STENCIL_TEST);

	glColor3f(0.8f, 0.8f, 0.8f);
	Translate();

	//if (flag_Show_Selected_Brush == 1)
	{
		/*int BC = App->CL_X_Brush->Get_Brush_Count();
		if (BC > 0)*/
		{
			glDisable(GL_CULL_FACE);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			//Render_Selected_Brush();
			Render_Brush(App->CL_Doc->CurBrush);
		}
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
void OGL_Listener::Translate(void)
{
	glRotatef(RX, 1.0, 0.0, 0.0); // Rotations of the object 

	glRotatef(RZ, 0.0, 1.0, 0.0);
	glRotatef(0.0, 0.0, 0.0, 1.0);
}

// *************************************************************************
// *	  	Render_Brush:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
bool OGL_Listener::Render_Brush(const Brush* b)
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
bool OGL_Listener::Render_Sub_Brushes(BrushList* BList)
{
	Brush* pBrush;
	BrushIterator bi;

	pBrush = App->CL_X_Brush->BrushList_GetFirst(BList, &bi);
	while (pBrush != NULL)
	{
		Render_Brush(pBrush);  // Recursive

		pBrush = App->CL_X_Brush->BrushList_GetNext(&bi);
	}

	return 1;
}

// *************************************************************************
// *			Render_Faces:- Terry and Hazel Flanigan 2025			   *
// *************************************************************************
bool OGL_Listener::Render_Faces(const FaceList* pList)
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
bool OGL_Listener::Render_Face(const Face* f)
{
	int j, num_verts, curnum_verts;

	num_verts = 0;
	curnum_verts = 0;

	// -----------------------------------  Vertices
	glColor3f(0.0f, 1.0f, 1.0f);

	const T_Vec3* verts;
	verts = App->CL_X_Face->Face_GetPoints(f);
	curnum_verts = App->CL_X_Face->Face_GetNumPoints(f);

	glBegin(GL_POLYGON);

	for (j = 0; j < curnum_verts; j++)
	{
		glVertex3f(verts[j].x, verts[j].y, verts[j].z);
	}

	glEnd();

	return GE_TRUE;
}
