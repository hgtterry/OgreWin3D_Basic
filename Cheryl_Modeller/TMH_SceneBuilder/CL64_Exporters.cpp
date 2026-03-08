#include "pch.h"
#include "CL64_App.h"
#include "CL64_Exporters.h"

CL64_Exporters::CL64_Exporters(void)
{
}

CL64_Exporters::~CL64_Exporters(void)
{
}

// *************************************************************************
// *	Export_Wavefront_Object_Model:- Terry and Hazel Flanigan 2026	   *
// *************************************************************************
void CL64_Exporters::Export_Wavefront_Object_Model(void)
{
	if (App->CL_Model->flag_Model_Loaded == false)
	{
		App->Say("No Model Loaded to Export");
		return;
	}

	bool test = App->CL_Exp_Obj->Create_ObjectFile();

	if (test == 1)
	{
		App->Say("Wavefront Object file Created successfully");
	}
}
