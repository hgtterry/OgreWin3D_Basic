#include "pch.h"
#include "CL64_App.h"
#include "CL64_Render.h"

CL64_Render::CL64_Render(void)
{
}

CL64_Render::~CL64_Render(void)
{
}

static const T_Vec3	VecOrigin = { 0.0f, 0.0f, 0.0f };

#define PI2				((geFloat)(2.0f * (M_PI)))
#define ONE_OVER_2PI	((geFloat)(1.0f/(PI2)))
#define Units_Round(n) ((int)Units_FRound((n)))
#define Units_Trunc(n) ((int)(n))
#define Units_FRound(n)	((float)floor((n)+0.5f))
#define	VectorToSUB(a, b) (*((((float *)(&a))) + (b)))

// *************************************************************************
// *	  					Render_GetWidth								   *
// *************************************************************************
int	CL64_Render::Render_GetWidth(const ViewVars* v)
{
	return	v->Width;
}

// *************************************************************************
// *	  					Render_GetHeight							   *
// *************************************************************************
int	CL64_Render::Render_GetHeight(const ViewVars* v)
{
	return	v->Height;
}

// *************************************************************************
// *	  					Render_GetInidx							   *
// *************************************************************************
int	CL64_Render::Render_GetInidx(const ViewVars* v)
{
	return	(v->ViewType >> 3) & 0x3;
}

// *************************************************************************
// *	  				Render_RenderOrthoGridFromSize					   *
// *************************************************************************
bool CL64_Render::Render_RenderOrthoGridFromSize(ViewVars* cv, int Interval, HDC hDC, RECT Rect)
{
	cv->Width = Rect.right;
	cv->Height = Rect.bottom;

	T_Vec3 ystep, xstep, Delt, Delt2;
	int			i, cnt, xaxis, yaxis, inidx;
	static int axidx[3][2] = { 2, 1, 0, 2, 0, 1 };
	float	gsinv;
	Box3d ViewBox;
	POINT		sp;

	inidx = (cv->ViewType >> 3) & 0x3;

	xaxis = axidx[inidx][0];
	yaxis = axidx[inidx][1];

	App->CL_Render->Render_ViewToWorld(cv, Units_Round(-Interval), Units_Round(-Interval), &Delt);
	App->CL_Render->Render_ViewToWorld(cv, Units_Round(cv->Width + Interval), Units_Round(cv->Height + Interval), &Delt2);

	App->CL_Box->Box3d_Set(&ViewBox, Delt.x, Delt.y, Delt.z, Delt2.x, Delt2.y, Delt2.z);

	VectorToSUB(ViewBox.Min, inidx) = -FLT_MAX;
	VectorToSUB(ViewBox.Max, inidx) = FLT_MAX;

	gsinv = 1.0f / (float)Interval;
	for (i = 0; i < 3; i++)
	{
		VectorToSUB(ViewBox.Min, i) = (float)((int)(VectorToSUB(ViewBox.Min, i) * gsinv)) * Interval;
		VectorToSUB(ViewBox.Max, i) = (float)((int)(VectorToSUB(ViewBox.Max, i) * gsinv)) * Interval;
	}

	App->CL_Maths->Vector3_Copy(&VecOrigin, &xstep);
	App->CL_Maths->Vector3_Copy(&VecOrigin, &ystep);
	VectorToSUB(ystep, yaxis) = (float)Interval;
	VectorToSUB(xstep, xaxis) = (float)Interval;

	cnt = Rect.bottom / Interval; // TODO hgtterry Debug Odd

	// horizontal lines
	int Count = 0;
	App->CL_Maths->Vector3_Copy(&ViewBox.Min, &Delt);
	App->CL_Maths->Vector3_Copy(&ViewBox.Min, &Delt2);
	VectorToSUB(Delt2, xaxis) = VectorToSUB(ViewBox.Max, xaxis);
	cnt = Units_Round((VectorToSUB(ViewBox.Max, yaxis) - VectorToSUB(ViewBox.Min, yaxis)) * gsinv);

	while (Count < cnt)
	{
		sp = App->CL_Render->Render_OrthoWorldToView(cv, &Delt);
		MoveToEx(hDC, 0, sp.y, NULL);
		sp = App->CL_Render->Render_OrthoWorldToView(cv, &Delt2);
		LineTo(hDC, cv->Width, sp.y);
		App->CL_Maths->Vector3_Add(&Delt, &ystep, &Delt);
		App->CL_Maths->Vector3_Add(&Delt2, &ystep, &Delt2);
		Count++;
	}

	// vertical lines
	Count = 0;
	App->CL_Maths->Vector3_Copy(&ViewBox.Min, &Delt);
	App->CL_Maths->Vector3_Copy(&ViewBox.Min, &Delt2);
	VectorToSUB(Delt2, yaxis) = VectorToSUB(ViewBox.Max, yaxis);
	cnt = Units_Round((VectorToSUB(ViewBox.Max, xaxis) - VectorToSUB(ViewBox.Min, xaxis)) * gsinv);

	while (Count < cnt)
	{
		sp = App->CL_Render->Render_OrthoWorldToView(cv, &Delt);
		MoveToEx(hDC, sp.x, 0, NULL);
		sp = App->CL_Render->Render_OrthoWorldToView(cv, &Delt2);
		LineTo(hDC, sp.x, cv->Height);
		App->CL_Maths->Vector3_Add(&Delt, &xstep, &Delt);
		App->CL_Maths->Vector3_Add(&Delt2, &xstep, &Delt2);
		Count++;
	}

	return 1;
}

// *************************************************************************
// *						Render_ViewToWorld							   *
// *************************************************************************
void CL64_Render::Render_ViewToWorld(const ViewVars* cv, const int x, const int y, T_Vec3* wp)
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
// *	  			Render_OrthoWorldToView								   *
// *************************************************************************
POINT CL64_Render::Render_OrthoWorldToView(const ViewVars* cv, T_Vec3 const* wp)
{
	POINT	sc = { 0, 0 };
	T_Vec3 ptView;
	
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

// *************************************************************************
// *	  				Render_GetXScreenScale							   *
// *************************************************************************
float CL64_Render::Render_GetXScreenScale(const ViewVars* v)
{
	return	v->XScreenScale;
}

// *************************************************************************
// *	  				Render_ViewDeltaToRotation						   *
// *************************************************************************
void CL64_Render::Render_ViewDeltaToRotation(const ViewVars* v,const float dx, T_Vec3* VecRotate)
{
	float RotationRads;

	RotationRads = (dx) * 0.002;// (ONE_OVER_2PI / Render_GetXScreenScale(v));
	switch (v->ViewType)
	{
	case VIEWTOP:	// +dx = negative rotation about Y
		App->CL_Maths->Vector3_Set(VecRotate, 0.0f, -RotationRads, 0.0f);
		break;
	case VIEWFRONT:  // +dx = negative rotation about Z
		//disable roll
		App->CL_Maths->Vector3_Set(VecRotate, 0.0f, 0.0f, -RotationRads);
		break;
	case VIEWSIDE:	// +dx = positive rotation about X
		App->CL_Maths->Vector3_Set(VecRotate, RotationRads, 0.0f, 0.0f);
		break;
	default:
		assert(0);  // can't happen!
	}
}

// *************************************************************************
// *	  			Render_MoveCamPosOrtho								   *
// *************************************************************************
void CL64_Render::Render_MoveCamPosOrtho(ViewVars* v, const T_Vec3* dv)
{
	App->CL_Maths->Vector3_Add(&v->CamPos, dv, &v->CamPos);
}
