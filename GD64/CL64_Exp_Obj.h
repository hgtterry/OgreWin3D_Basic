/*
Copyright (c) OgreWin3D_Basic 2024 W.T.Flanigan H.C.Flanigan Inflanite_HGT

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely.

An acknowledgment in the product documentation would be
appreciated but is not required.
*/

#pragma once
class CL64_Exp_Obj
{
public:

	CL64_Exp_Obj(void);
	~CL64_Exp_Obj(void);

	bool Create_ObjectFile(void);
	void Write_ObjectFile(void);
	void Write_ObjectFile_Commit(void);
	bool WriteMTLFile(void);

	char OutputFolder[1024];

	char Object_FileName[255];
	char mtl_FileName[255];
	char Just_mtl_FileName[255];

	FILE* Write_OBJECTFILE;
	FILE* Write_MTLFile;

};

