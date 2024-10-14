#include "pch.h"
#include "CL64_App.h"
#include "CL64_ResourceListener.h"

CL64_ResourceListener::CL64_ResourceListener(void)
{
	flag_Monitor_Resource = 0;
}

CL64_ResourceListener::~CL64_ResourceListener(void)
{
}

// *************************************************************************
// *			resourceCreated:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_ResourceListener::resourceCreated(const ResourcePtr& resource)
{
	if (flag_Monitor_Resource == 1)
	{
		//Debug
	}
}

// *************************************************************************
// *	  	resourceGroupLoadEnded:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_ResourceListener::resourceGroupLoadEnded(const String& groupName)
{
	if (flag_Monitor_Resource == 1)
	{
		App->Say("resourceGroupLoadEnded", (LPSTR)groupName.c_str());
	}
}

void CL64_ResourceListener::resourcePrepareEnded()
{
	/*if (flag_Monitor_Resource == 1)
	{
		Debug
	}*/
}

void CL64_ResourceListener::resourceLoadEnded(void)
{
	if (flag_Monitor_Resource == 1)
	{
		//App->Say("resourceLoadEnded");
	}
}

void CL64_ResourceListener::resourceRemove(const ResourcePtr& resource)
{
	if (flag_Monitor_Resource == 1)
	{
		//App->Say("resourceRemove");
	}
}
