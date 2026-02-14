#include "pch.h"
#include "CL64_App.h"
#include "Ogre_Win_Render_Listener.h"

Ogre_Win_Render_Listener::Ogre_Win_Render_Listener(void)
{
}

Ogre_Win_Render_Listener::~Ogre_Win_Render_Listener(void)
{
}

// *************************************************************************
// *				frameStarted   Terry Bernie							   *
// *************************************************************************
bool Ogre_Win_Render_Listener::frameStarted(const Ogre::FrameEvent& evt)
{
	return true;
}


// *************************************************************************
// *			frameRenderingQueued   Terry Bernie						   *
// *************************************************************************
bool Ogre_Win_Render_Listener::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	//Ogre::ImGuiOverlay::NewFrame();
	//App->CL_ImGui->ImGui_Render_Loop();
	//ModelMode(evt.timeSinceLastFrame);

	return 1;
}
