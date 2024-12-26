/*
Copyright (c) 2024 Inflanite_HGT W.T.Flanigan H.C.Flanigan

OgreWin3D_Stage

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
class CL64_Utilities
{
public:

	CL64_Utilities(void);
	~CL64_Utilities(void);

	void Vector3_Set(Ogre::Vector3* V, float X, float Y, float Z);
	void Vector3_Add(const Ogre::Vector3* V1, const Ogre::Vector3* V2, Ogre::Vector3* V1PlusV2);
	void Vector3_Subtract(const Ogre::Vector3* V1, const Ogre::Vector3* V2, Ogre::Vector3* V1MinusV2);
	void Vector3_Scale(const Ogre::Vector3* VSrc, float Scale, Ogre::Vector3* VDst);
	float Vector3_Normalize(Ogre::Vector3* V1);
	float Vector3_DotProduct(const Ogre::Vector3* V1, const Ogre::Vector3* V2);

	std::string Get_FileName_From_Path(char* pString);
};

