/*
Copyright (c) 2024 - 2025 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OW3D_Mesh_Builder

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
#include "CL64_Bullet_Debug.h"

#pragma warning( disable : 4305)

CL64_Bullet_Debug::CL64_Bullet_Debug(void)
{
	flag_Render_Debug_Flag = 1;
	m_debugMode = (DebugDrawModes)DBG_DrawWireframe;

	ColourMain = Ogre::ColourValue(1, 0, 0, 1);
	mLines = nullptr;

	btDebug_Manual = App->CL_Ogre->mSceneMgr->createManualObject("btManual");
	btDebug_Manual->setRenderQueueGroup(Ogre::RENDER_QUEUE_MAX);

	btDebug_Manual->setDynamic(true);
	btDebug_Manual->estimateVertexCount(2000);

	btDebug_Manual->begin("Template/White_Alpha_GD64", Ogre::RenderOperation::OT_LINE_LIST,App->CL_Ogre->App_Resource_Group);
	btDebug_Manual->position(0, 0, 0);
	btDebug_Manual->colour(ColourMain);
	btDebug_Manual->position(0, 0, 0);
	btDebug_Manual->colour(ColourMain);
	btDebug_Manual->end();

	btDebug_Node = App->CL_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	btDebug_Node->attachObject(btDebug_Manual);
	btDebug_Node->setVisible(true);

	vertex_From.resize(100000);
	vertex_To.resize(100000);
	vertex_Colour.resize(100000);

	V_Count = 0;
}

CL64_Bullet_Debug::~CL64_Bullet_Debug(void)
{
}

void CL64_Bullet_Debug::drawSphere(const btVector3& p, btScalar radius, const btVector3& color)
{
	//FlashWindow(App->MainHwnd,true);
}

void CL64_Bullet_Debug::drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor)
{

}

void CL64_Bullet_Debug::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	
	vertex_Colour[V_Count].x = color.getX();
	vertex_Colour[V_Count].y = color.getY();
	vertex_Colour[V_Count].z = color.getZ();

	vertex_From[V_Count].x = from.getX();
	vertex_From[V_Count].y = from.getY();
	vertex_From[V_Count].z = from.getZ();

	vertex_To[V_Count].x = to.getX();
	vertex_To[V_Count].y = to.getY();
	vertex_To[V_Count].z = to.getZ();
	V_Count++;

}

void CL64_Bullet_Debug::drawTriangle(const btVector3& v0, const btVector3& v1, const btVector3& v2, const btVector3& color, btScalar)
{

}

void CL64_Bullet_Debug::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	
}

void CL64_Bullet_Debug::reportErrorWarning(const char* warningString)
{
	
}

void CL64_Bullet_Debug::draw3dText(const btVector3& location, const char* textString)
{
	
}

void CL64_Bullet_Debug::setDebugMode(int debugMode)
{
	m_debugMode = (DebugDrawModes)DBG_DrawWireframe;
}

int CL64_Bullet_Debug::getDebugMode() const {

	return DBG_DrawWireframe;
}

// *************************************************************************
// *		renderQueueStarted:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Bullet_Debug::renderQueueStarted(Ogre::uint8 queueGroupId, const String& invocation, bool& skipThisInvocation)
{
	Render_Debug();
}
// *************************************************************************
// *			renderQueueEnded:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Bullet_Debug::renderQueueEnded(Ogre::uint8 queueGroupId, const String& invocation, bool& repeatThisInvocation)
{
	if (queueGroupId != RENDER_QUEUE_MAIN)
	{
		return;
	}
}

// *************************************************************************
// *				PreRender:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Bullet_Debug::PreRender()
{

}

// *************************************************************************
// *				PostRender:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Bullet_Debug::PostRender()
{

}

// *************************************************************************
// *		Render_Debug_Bullet:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Bullet_Debug::Render_Debug_Bullet()
{
	Render_Debug();
}

// *************************************************************************
// *				Render_Debug:- Terry and Hazel Flanigan 2024	  	   *
// *************************************************************************
bool CL64_Bullet_Debug::Render_Debug(void)
{
	if (flag_Render_Debug_Flag == 1)
	{
		if (V_Count > 0)
		{

			btDebug_Manual->beginUpdate(0);

			int Count = 0;
			while (Count < V_Count)
			{
				btDebug_Manual->position(vertex_From[Count].x, vertex_From[Count].y, vertex_From[Count].z);
				btDebug_Manual->colour(ColourMain);
				btDebug_Manual->position(vertex_To[Count].x, vertex_To[Count].y, vertex_To[Count].z);
				btDebug_Manual->colour(ColourMain);
				Count++;
			}

			btDebug_Manual->end();

			V_Count = 0;
		}

	}
	else
	{
		Clear_Debug_Render();
	}

	V_Count = 0;
	return 1;
}

// *************************************************************************
// *			Clear_Debug_Render:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Bullet_Debug::Clear_Debug_Render()
{
	btDebug_Manual->beginUpdate(0);
	btDebug_Manual->position(0, 0, 0);
	btDebug_Manual->colour(ColourMain);
	btDebug_Manual->position(0, 0, 0);
	btDebug_Manual->colour(ColourMain);
	btDebug_Manual->end();
}
