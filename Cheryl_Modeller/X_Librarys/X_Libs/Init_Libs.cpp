#include "pch.h"
#include "CL64_App.h"
#include "Init_Libs.h"

Init_Libs::Init_Libs(void)
{
	CL_CreateBox =		nullptr;
	CL_CreateCylinder = nullptr;
	CL_CreateCone =		nullptr;
	CL_CreateArch =		nullptr;

	CL_Preference =		nullptr;
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

	//CL_Preference =		new Lib_Preference();
}

// *************************************************************************
// *			GetVersion:- Terry and Hazel Flanigan 2026			 	   *
// *************************************************************************
char* Init_Libs::GetVersion()
{
	return (LPSTR)" Init_Libs [ 22-04-26 ] Build 1T ";
}

// *************************************************************************
// *			Get_Time_Stamp:- Terry and Hazel Flanigan 2026			   *
// *************************************************************************
char* Init_Libs::Get_Time_Stamp()
{
	static char buf[MAX_PATH];
	sprintf(buf, "Build Time %s   Date  %s", __TIME__, __DATE__);

	return (LPSTR)buf;
}
