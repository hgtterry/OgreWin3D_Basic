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
	Chr_Type[0] = 0;
	Chr_Shape[0] = 0;
	Chr_Usage[0] = 0;
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

// *************************************************************************
// *   Player_Location_GetIndex_ByName:- Terry and Hazel Flanigan 2024	   *
// *************************************************************************
int CL64_LookUps::Player_Location_GetIndex_ByName(char* Name)
{
	int Count = 0;
	int Total = App->CL_Scene->Player_Location_Count;
	while (Count < Total)
	{
		if (App->CL_Scene->B_Locations[Count]->Deleted == 0)
		{
			int Result = 1;
			Result = strcmp(App->CL_Scene->B_Locations[Count]->Name, Name);

			if (Result == 0)
			{
				return Count;
				break;
			}
		}
		Count++;
	}
	return -1;
}

// *************************************************************************
// *			Update_Types:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_LookUps::Update_Types()
{
	int Index = App->CL_Properties->Current_Selected_Object;

	Get_Type(Index);
	Get_Shape(Index);
	Get_Usage(Index);

	return 0;
}

// *************************************************************************
// *			 Get_Type:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_LookUps::Get_Type(int Index)
{
	int Type = 0;

	if (App->CL_Properties->Edit_Category == Enums::Edit_Area)
	{
		Type = App->CL_Scene->B_Area[Index]->Type;
	}
	else
	{
		Type = App->CL_Scene->B_Object[Index]->Type;
	}


	switch (Type)
	{

	case Enums::Bullet_Type_Dynamic:
	{
		strcpy(Chr_Type, "Dynamic");
		return;
	}
	case Enums::Bullet_Type_Static:
	{
		strcpy(Chr_Type, "Static");
		return;
	}

	break;

	}

	strcpy(Chr_Type, "None");
	
}

// *************************************************************************
// *				Get_Shape:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_LookUps::Get_Shape(int Index)
{
	int Shape = 0;

	if (App->CL_Properties->Edit_Category == Enums::Edit_Area)
	{
		Shape = App->CL_Scene->B_Area[Index]->Shape;
	}
	else
	{
		Shape = App->CL_Scene->B_Object[Index]->Shape;
	}

	switch (Shape)
	{

	case Enums::Shape_None:
	{
		strcpy(Chr_Shape, "No Shape");
		return;
	}

	case Enums::Shape_Box:
	{
		strcpy(Chr_Shape, "Box");
		return;
	}
	case Enums::Shape_Sphere:
	{
		strcpy(Chr_Shape, "Sphere");
		return;
	}

	case Enums::Shape_Capsule:
	{
		strcpy(Chr_Shape, "Capsule");
		return;
	}

	case Enums::Shape_Cylinder:
	{
		strcpy(Chr_Shape, "Cylinder");
		return;
	}

	case Enums::Shape_Cone:
	{
		strcpy(Chr_Shape, "Cone");
		return;
	}

	case Enums::Shape_TriMesh:
	{
		strcpy(Chr_Shape, "Triangle Mesh");
		return;
	}

	case Enums::Shape_VolumeBox:
	{
		strcpy(Chr_Shape, "Volume Box");
		return;
	}

	break;

	}

	strcpy(Chr_Shape, "Not Defined");

}

// *************************************************************************
// *				Get_Usage:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_LookUps::Get_Usage(int Index)
{
	strcpy(Chr_Usage, "None");

	int Usage = 0;

	
		if (App->CL_Properties->Edit_Category == Enums::Edit_Area)
		{
			Usage = App->CL_Scene->B_Area[Index]->Usage;
		}
		else
		{
			Usage = App->CL_Scene->B_Object[Index]->Usage;
		}
	

	switch (Usage)
	{

	case Enums::Stage_Usage_Room:
	{
		strcpy(Chr_Usage, "Usage_Room");
		return;
	}
	case Enums::Stage_Usage_Static:
	{
		strcpy(Chr_Usage, "Usage_Static");
		return;
	}

	case Enums::Stage_Usage_Dynamic:
	{
		strcpy(Chr_Usage, "Usage_Dynamic");
		return;
	}

	case Enums::Stage_Usage_Object:
	{
		strcpy(Chr_Usage, "Usage_Object");
		return;
	}

	case Enums::Stage_Usage_Teleport:
	{
		strcpy(Chr_Usage, "Usage_Teleport");
		return;
	}

	case Enums::Stage_Usage_Colectable:
	{
		strcpy(Chr_Usage, "Usage_Colectable");
		return;
	}

	case Enums::Stage_Usage_Message:
	{
		strcpy(Chr_Usage, "Usage_Message");
		return;
	}

	case Enums::Stage_Usage_Move:
	{
		strcpy(Chr_Usage, "Usage_Move");
		return;
	}

	case Enums::Stage_Usage_Sound:
	{
		strcpy(Chr_Usage, "Usage_Sound");
		return;
	}

	case Enums::Stage_Usage_EnvironEntity:
	{
		strcpy(Chr_Usage, "Environment Entity");
		return;
	}
	
	break;

	}

	strcpy(Chr_Usage, "None");
	
}
