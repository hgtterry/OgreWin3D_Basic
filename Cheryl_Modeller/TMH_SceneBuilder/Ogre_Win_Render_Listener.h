#pragma once

using namespace Ogre;

class Ogre_Win_Render_Listener : public Ogre::FrameListener
{
public:
	Ogre_Win_Render_Listener(void);
	~Ogre_Win_Render_Listener(void);

	bool frameStarted(const Ogre::FrameEvent& evt);
	bool frameRenderingQueued(const Ogre::FrameEvent& evt);
};

