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

#include "pch.h"
#include "CL64_App.h"
#include "CL64_LookUps.h"

CL64_LookUps::CL64_LookUps()
{
}

CL64_LookUps::~CL64_LookUps()
{
}

// *************************************************************************
// *	  Get_Adjusted_Counters_Count:- Terry and Hazel Flanigan 2024	   *
// *************************************************************************
int CL64_LookUps::Get_Adjusted_Counters_Count(void)
{
	int New_Count = 0;
	int Count = 0;
	int Total = App->CL_Scene->Counters_Count;

	while (Count < Total)
	{
		if (App->CL_Scene->B_Counter[Count]->Deleted == 0)
		{
			New_Count++;
		}

		Count++;
	}

	return New_Count;
}

// *************************************************************************
// *	  	Player_Location_GetCount:- Terry and Hazel Flanigan 2024	   *
// *************************************************************************
int CL64_LookUps::Player_Location_GetCount(void)
{
	int RealCount = 0;
	int Count = 0;
	int Total = App->CL_Scene->Player_Location_Count;

	while (Count < Total)
	{
		if (App->CL_Scene->B_Locations[Count]->Deleted == 0)
		{
			RealCount++;
		}
		Count++;
	}
	return RealCount;
}
