#include "pch.h"
#include "CL64_App.h"
#include "Init_Libs.h"

Init_Libs::Init_Libs(void)
{
	CL_CreateBox = nullptr;
}

Init_Libs::~Init_Libs(void)
{
}

// *************************************************************************
// *			Start_Libraries:- Terry and Hazel Flanigan 2025		 	   *
// *************************************************************************
void Init_Libs::Start_Libraries()
{
	CL_CreateBox = new CreateBoxDialog();

	/*CL_CreateCylinder = new CreateCylDialog();
	CL_CreateArch =		new CreateArchDialog();
	CL_CreateCone =		new CreateConeDialog();*/
}

// *************************************************************************
// *			GetVersion:- Terry and Hazel Flanigan 2026			 	   *
// *************************************************************************
char* Init_Libs::GetVersion()
{
	return (LPSTR)" Cheryl Scene Builder :-- 11-01-26 Build 1 ";
}

