#include "pch.h"
#include "CL64_App.h"
#include "CL64_Render.h"

CL64_Render::CL64_Render(void)
{
}

CL64_Render::~CL64_Render(void)
{
}

// *************************************************************************
// *	  	Render_ViewToWorld:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Render::Render_ViewToWorld(const ViewVars* cv, const int x, const int y, Ogre::Vector3* wp)
{
	float	ZoomInv = 1.0f / cv->ZoomFactor;

	switch (cv->ViewType)
	{
	case VIEWTOP:
	{
		App->CL_Maths->Vector3_Set(wp, (x - cv->XCenter), 0.0f, (y - cv->YCenter));
		App->CL_Maths->Vector3_Scale(wp, ZoomInv, wp);
		App->CL_Maths->Vector3_Add(wp, &cv->CamPos, wp);
		break;
	}
	case VIEWFRONT:
	{

		App->CL_Maths->Vector3_Set(wp, (x - cv->XCenter), -(y - cv->YCenter), 0.0f);
		App->CL_Maths->Vector3_Scale(wp, ZoomInv, wp);
		App->CL_Maths->Vector3_Add(wp, &cv->CamPos, wp);
		break;
	}
	case VIEWSIDE:
	{
		App->CL_Maths->Vector3_Set(wp, 0.0f, -(y - cv->YCenter), (x - cv->XCenter));
		App->CL_Maths->Vector3_Scale(wp, ZoomInv, wp);
		App->CL_Maths->Vector3_Add(wp, &cv->CamPos, wp);
		break;
	}
	default:
	{
		App->CL_Maths->Vector3_Set
		(
			wp,
			-(x - cv->XCenter) * (cv->MaxScreenScaleInv),
			-(y - cv->YCenter) * (cv->MaxScreenScaleInv),
			1.0f
		);

		App->CL_Maths->Vector3_Normalize(wp);

		break;
	}
	}
}

// *************************************************************************
// *	  			Render_OrthoWorldToView							   *
// *************************************************************************
POINT CL64_Render::Render_OrthoWorldToView(const ViewVars* cv, Ogre::Vector3 const* wp)
{
	POINT	sc = { 0, 0 };
	Ogre::Vector3 ptView;
	Ogre::Vector3 Campos;

	switch (cv->ViewType)
	{
	case VIEWTOP:
	{
		App->CL_Maths->Vector3_Subtract(wp, &cv->CamPos, &ptView);
		App->CL_Maths->Vector3_Scale(&ptView, cv->ZoomFactor, &ptView);

		sc.x = (int)(cv->XCenter + ptView.x);
		sc.y = (int)(cv->YCenter + ptView.z);
		break;
	}
	case VIEWFRONT:
	{
		App->CL_Maths->Vector3_Subtract(wp, &cv->CamPos, &ptView);
		App->CL_Maths->Vector3_Scale(&ptView, cv->ZoomFactor, &ptView);

		sc.x = (int)(cv->XCenter + ptView.x);
		sc.y = (int)(cv->YCenter - ptView.y);
		break;
	}
	case VIEWSIDE:
	{
		App->CL_Maths->Vector3_Subtract(wp, &cv->CamPos, &ptView);
		App->CL_Maths->Vector3_Scale(&ptView, cv->ZoomFactor, &ptView);

		sc.x = (int)(cv->XCenter + ptView.z);
		sc.y = (int)(cv->YCenter - ptView.y);
		break;
	}
	default:

		break;
	}

	return sc;
}
