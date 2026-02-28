#include "pch.h"
#include "CL64_App.h"
#include "Init_Libs.h"

Init_Libs::Init_Libs(void)
{
	CL_CreateBox =		nullptr;
	CL_CreateCylinder = nullptr;
	CL_CreateCone =		nullptr;
	CL_CreateArch =		nullptr;
}

Init_Libs::~Init_Libs(void)
{
}

// *************************************************************************
// *			Start_Libraries:- Terry and Hazel Flanigan 2025		 	   *
// *************************************************************************
void Init_Libs::Start_Libraries()
{
	CL_CreateBox =		new CreateBoxDialog();
	CL_CreateCylinder = new CreateCylDialog();
	CL_CreateCone =		new CreateConeDialog();
	CL_CreateArch =		new CreateArchDialog();
}

// *************************************************************************
// *			GetVersion:- Terry and Hazel Flanigan 2025			 	   *
// *************************************************************************
char* Init_Libs::GetVersion()
{
	return (LPSTR)" Cheryl 3D Modeller (Init_Libs):-- 28-02-26 Build 2 ";
}
