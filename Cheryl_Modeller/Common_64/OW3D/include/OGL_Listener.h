#pragma once

class OGL_Listener : public Ogre::RenderQueueListener
{
public:
	OGL_Listener(void);
	~OGL_Listener(void);

	float RX;
	float RZ;

private:

	virtual void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation,
		bool& skipThisInvocation);

	virtual void renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation,
		bool& repeatThisInvocation);

	void PreRender();
	void PostRender();

	void Render_Loop();
	void Translate(void);

	bool Render_Brush(const Brush* b);
	bool Render_Sub_Brushes(BrushList* BList);

	bool Render_Faces(const FaceList* pList);
	bool Render_Face(const Face* f);

	int	mBrushCount;
};

