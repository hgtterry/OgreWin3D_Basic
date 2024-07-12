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

#include "cimport.h"
#include "PostProcess.h"
#include "Scene.h"
#include "DefaultLogger.hpp"
#include "LogStream.hpp"
#include "matrix4x4.h"


class CL64_Assimp
{
public:

	CL64_Assimp(void);
	~CL64_Assimp(void);

	bool LoadFile(const char* pFile);


	void logInfo(std::string logString);
	void logDebug(const char* logString);

	void GetBasicInfo(const aiScene* pScene);
	void Create_MeshGroups(const aiScene* pScene);
	void Get_Group_VertCount(const aiScene* pScene);
	void StoreMeshData(const aiScene* pScene);
	void LoadTextures();

	long int SelectedPreset;	// Switchs 
	int mTotalVertices;			// Total Vertice Count of Model

};

