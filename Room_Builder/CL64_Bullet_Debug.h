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

#pragma once

#include "Ogre.h"

typedef struct Mvertex_type
{
	float x, y, z;
}Mvertex_type;

class CL64_Bullet_Debug : public btIDebugDraw, public RenderQueueListener
{
public:
	CL64_Bullet_Debug(void);
	~CL64_Bullet_Debug(void);

	virtual void   drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor);

	virtual void   drawLine(const btVector3& from, const btVector3& to, const btVector3& color);

	virtual void   drawSphere(const btVector3& p, btScalar radius, const btVector3& color);

	virtual void   drawTriangle(const btVector3& a, const btVector3& b, const btVector3& c, const btVector3& color, btScalar alpha);

	virtual void   drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);

	virtual void   reportErrorWarning(const char* warningString);

	virtual void   draw3dText(const btVector3& location, const char* textString);

	virtual void   setDebugMode(int debugMode);

	virtual int  getDebugMode() const;

	inline btVector3 cvt(const Ogre::Vector3& V) {
		return btVector3(V.x, V.y, V.z);
	}

	inline Ogre::Vector3 cvt(const btVector3& V) {
		return Ogre::Vector3(V.x(), V.y(), V.z());
	}

	inline btQuaternion cvt(const Ogre::Quaternion& Q)
	{
		return btQuaternion(Q.x, Q.y, Q.z, Q.w);
	};

	inline Ogre::Quaternion cvt(const btQuaternion& Q)
	{
		return Ogre::Quaternion(Q.w(), Q.x(), Q.y(), Q.z());
	};

	void Clear_Debug_Render();

	Ogre::Vector3 mfrom;
	Ogre::Vector3 mto;
	Ogre::Vector3 mCfrom;
	Ogre::Vector3 mCto;

	Ogre::ManualObject* mLines;

	Ogre::ManualObject* btDebug_Manual;
	Ogre::SceneNode* btDebug_Node;

	Ogre::ColourValue ColourMain;

	bool flag_Render_Debug_Flag; // Stop/Start Render Physics debug shapes [021221]

	std::vector<Mvertex_type> vertex_Colour;
	std::vector<Mvertex_type> vertex_From;
	std::vector<Mvertex_type> vertex_To;

	int V_Count;
	int m_debugMode;

protected:

	virtual void renderQueueStarted(Ogre::uint8 queueGroupId, const String& invocation,
		bool& skipThisInvocation);

	virtual void renderQueueEnded(Ogre::uint8 queueGroupId, const String& invocation,
		bool& repeatThisInvocation);

	void PreRender();
	void PostRender();

	void Render_Debug_Bullet();

	bool Render_Debug(void);
};

