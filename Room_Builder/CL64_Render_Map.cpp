#include "pch.h"
#include "CL64_App.h"
#include "CL64_Render_Map.h"

CL64_Render_Map::CL64_Render_Map(void)
{
}

CL64_Render_Map::~CL64_Render_Map(void)
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
int	CL64_Render_Map::Render_GetWidth(const ViewVars* v)
{
	return	v->Width;
}

// *************************************************************************
// *	  					Render_GetHeight							   *
// *************************************************************************
int	CL64_Render_Map::Render_GetHeight(const ViewVars* v)
{
	return	v->Height;
}

// *************************************************************************
// *	  					Render_GetInidx							   *
// *************************************************************************
int	CL64_Render_Map::Render_GetInidx(const ViewVars* v)
{
	return	(v->ViewType >> 3) & 0x3;
}

// *************************************************************************
// *	  				Render_RenderOrthoGridFromSize					   *
// *************************************************************************
bool CL64_Render_Map::Render_RenderOrthoGridFromSize(ViewVars* cv, int Interval, HDC hDC, RECT Rect)
{
	auto& m_Maths = App->CL_X_Maths; // Maths Library

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

	App->CL_X_Box->Box3d_Set(&ViewBox, Delt.x, Delt.y, Delt.z, Delt2.x, Delt2.y, Delt2.z);

	VectorToSUB(ViewBox.Min, inidx) = -FLT_MAX;
	VectorToSUB(ViewBox.Max, inidx) = FLT_MAX;

	gsinv = 1.0f / (float)Interval;
	for (i = 0; i < 3; i++)
	{
		VectorToSUB(ViewBox.Min, i) = (float)((int)(VectorToSUB(ViewBox.Min, i) * gsinv)) * Interval;
		VectorToSUB(ViewBox.Max, i) = (float)((int)(VectorToSUB(ViewBox.Max, i) * gsinv)) * Interval;
	}

	m_Maths->Vector3_Copy(&VecOrigin, &xstep);
	m_Maths->Vector3_Copy(&VecOrigin, &ystep);
	VectorToSUB(ystep, yaxis) = (float)Interval;
	VectorToSUB(xstep, xaxis) = (float)Interval;

	cnt = Rect.bottom / Interval; // TODO hgtterry Debug Odd

	// horizontal lines
	int Count = 0;
	m_Maths->Vector3_Copy(&ViewBox.Min, &Delt);
	m_Maths->Vector3_Copy(&ViewBox.Min, &Delt2);
	VectorToSUB(Delt2, xaxis) = VectorToSUB(ViewBox.Max, xaxis);
	cnt = Units_Round((VectorToSUB(ViewBox.Max, yaxis) - VectorToSUB(ViewBox.Min, yaxis)) * gsinv);

	while (Count < cnt)
	{
		sp = App->CL_Render->Render_OrthoWorldToView(cv, &Delt);
		MoveToEx(hDC, 0, sp.y, NULL);
		sp = App->CL_Render->Render_OrthoWorldToView(cv, &Delt2);
		LineTo(hDC, cv->Width, sp.y);
		m_Maths->Vector3_Add(&Delt, &ystep, &Delt);
		m_Maths->Vector3_Add(&Delt2, &ystep, &Delt2);
		Count++;
	}

	// vertical lines
	Count = 0;
	m_Maths->Vector3_Copy(&ViewBox.Min, &Delt);
	m_Maths->Vector3_Copy(&ViewBox.Min, &Delt2);
	VectorToSUB(Delt2, yaxis) = VectorToSUB(ViewBox.Max, yaxis);
	cnt = Units_Round((VectorToSUB(ViewBox.Max, xaxis) - VectorToSUB(ViewBox.Min, xaxis)) * gsinv);

	while (Count < cnt)
	{
		sp = App->CL_Render->Render_OrthoWorldToView(cv, &Delt);
		MoveToEx(hDC, sp.x, 0, NULL);
		sp = App->CL_Render->Render_OrthoWorldToView(cv, &Delt2);
		LineTo(hDC, sp.x, cv->Height);
		m_Maths->Vector3_Add(&Delt, &xstep, &Delt);
		m_Maths->Vector3_Add(&Delt2, &xstep, &Delt2);
		Count++;
	}

	return 1;
}

// *************************************************************************
// *						Render_ViewToWorld							   *
// *************************************************************************
void CL64_Render_Map::Render_ViewToWorld(const ViewVars* cv, const int x, const int y, T_Vec3* wp)
{
	float	ZoomInv = 1.0f / cv->ZoomFactor;

	switch (cv->ViewType)
	{
	case TOP_LEFT_VIEW:
	{
		App->CL_X_Maths->Vector3_Set(wp, (x - cv->XCenter), 0.0f, (y - cv->YCenter));
		App->CL_X_Maths->Vector3_Scale(wp, ZoomInv, wp);
		App->CL_X_Maths->Vector3_Add(wp, &cv->CamPos, wp);
		break;
	}
	case BOTTOM_LEFT_VIEW:
	{

		App->CL_X_Maths->Vector3_Set(wp, (x - cv->XCenter), -(y - cv->YCenter), 0.0f);
		App->CL_X_Maths->Vector3_Scale(wp, ZoomInv, wp);
		App->CL_X_Maths->Vector3_Add(wp, &cv->CamPos, wp);
		break;
	}
	case TOP_RIGHT_VIEW:
	{
		App->CL_X_Maths->Vector3_Set(wp, 0.0f, -(y - cv->YCenter), (x - cv->XCenter));
		App->CL_X_Maths->Vector3_Scale(wp, ZoomInv, wp);
		App->CL_X_Maths->Vector3_Add(wp, &cv->CamPos, wp);
		break;
	}
	default:
	{
		App->CL_X_Maths->Vector3_Set
		(
			wp,
			-(x - cv->XCenter) * (cv->MaxScreenScaleInv),
			-(y - cv->YCenter) * (cv->MaxScreenScaleInv),
			1.0f
		);

		App->CL_X_Maths->Vector3_Normalize(wp);

		break;
	}
	}
}

// *************************************************************************
// *	  			Render_OrthoWorldToView								   *
// *************************************************************************
POINT CL64_Render_Map::Render_OrthoWorldToView(const ViewVars* cv, T_Vec3 const* wp)
{
	POINT	sc = { 0, 0 };
	T_Vec3 ptView;

	switch (cv->ViewType)
	{
	case TOP_LEFT_VIEW:
	{
		App->CL_X_Maths->Vector3_Subtract(wp, &cv->CamPos, &ptView);
		App->CL_X_Maths->Vector3_Scale(&ptView, cv->ZoomFactor, &ptView);

		sc.x = (int)(cv->XCenter + ptView.x);
		sc.y = (int)(cv->YCenter + ptView.z);
		break;
	}
	case BOTTOM_LEFT_VIEW:
	{
		App->CL_X_Maths->Vector3_Subtract(wp, &cv->CamPos, &ptView);
		App->CL_X_Maths->Vector3_Scale(&ptView, cv->ZoomFactor, &ptView);

		sc.x = (int)(cv->XCenter + ptView.x);
		sc.y = (int)(cv->YCenter - ptView.y);

		//App->Flash_Window();

		break;
	}
	case TOP_RIGHT_VIEW:
	{
		App->CL_X_Maths->Vector3_Subtract(wp, &cv->CamPos, &ptView);
		App->CL_X_Maths->Vector3_Scale(&ptView, cv->ZoomFactor, &ptView);

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
float CL64_Render_Map::Render_GetXScreenScale(const ViewVars* v)
{
	return	v->XScreenScale;
}

// *************************************************************************
// *	  				Render_ViewDeltaToRotation						   *
// *************************************************************************
void CL64_Render_Map::Render_ViewDeltaToRotation(const ViewVars* v,const float dx, T_Vec3* VecRotate)
{
	float RotationRads;

	RotationRads = (dx) * 0.002;// (ONE_OVER_2PI / Render_GetXScreenScale(v));
	switch (v->ViewType)
	{
	case TOP_LEFT_VIEW:	// +dx = negative rotation about Y
		App->CL_X_Maths->Vector3_Set(VecRotate, 0.0f, -RotationRads, 0.0f);
		break;
	case BOTTOM_LEFT_VIEW:  // +dx = negative rotation about Z
		//disable roll
		App->CL_X_Maths->Vector3_Set(VecRotate, 0.0f, 0.0f, -RotationRads);
		break;
	case TOP_RIGHT_VIEW:	// +dx = positive rotation about X
		App->CL_X_Maths->Vector3_Set(VecRotate, RotationRads, 0.0f, 0.0f);
		break;
	default:
		assert(0);  // can't happen!
	}
}

// *************************************************************************
// *	  			Render_MoveCamPosOrtho								   *
// *************************************************************************
void CL64_Render_Map::Render_MoveCamPosOrtho(ViewVars* v, const T_Vec3* dv)
{
	App->CL_X_Maths->Vector3_Add(&v->CamPos, dv, &v->CamPos);
}

// *************************************************************************
// *			Pan_View:- Terry and Hazel Flanigan 2025				   *
// *************************************************************************
void CL64_Render_Map::Pan_View(ViewVars* currentView, int startPosX, int startPosY)
{
	T_Vec3 startPoint, worldPoint, deltaVector, cameraVector;

	// Get the current cursor position
	POINT realCursorPosition;
	GetCursorPos(&realCursorPosition);
	ScreenToClient(currentView->hDlg, &realCursorPosition);

	// Calculate the difference in position
	int deltaX = realCursorPosition.x - startPosX;
	int deltaY = realCursorPosition.y - startPosY;

	// If there is no movement, exit the function
	if (deltaX == 0 && deltaY == 0)
	{
		return;
	}

	// Convert screen coordinates to world coordinates
	Render_ViewToWorld(currentView, startPosX, startPosY, &startPoint);
	Render_ViewToWorld(currentView, realCursorPosition.x, realCursorPosition.y, &worldPoint);

	// Calculate the delta in world space
	App->CL_X_Maths->Vector3_Subtract(&worldPoint, &startPoint, &deltaVector);
	App->CL_X_Maths->Vector3_Scale(&deltaVector, -1.0f, &cameraVector);

	// Move the camera position
	Render_MoveCamPosOrtho(currentView, &cameraVector);

	// Set Cursor back to Original Position
	POINT screenPoint = { startPosX, startPosY };
	ClientToScreen(currentView->hDlg, &screenPoint);
	SetCursorPos(screenPoint.x, screenPoint.y);

	if (currentView->ViewType == TOP_LEFT_VIEW)
	{
		App->CL_View_Top_Left->Redraw_Window_TL();
		return;
	}

	if (currentView->ViewType == TOP_RIGHT_VIEW)
	{
		App->CL_View_Top_Right->Redraw_Window_TR();
		return;
	}

	if (currentView->ViewType == BOTTOM_LEFT_VIEW)
	{
		App->CL_View_Bottom_Left->Redraw_Window_BL();
		return;
	}
}

// *************************************************************************
// *			Render_Camera:- Terry and Hazel Flanigan 2026	 		   *
// *************************************************************************
void CL64_Render_Map::Render_Camera(HDC ViewDC)
{
	bool All = false;

	constexpr float ENTITY_SIZE = 32.0f;  // 16" across

	T_Vec3 VecOrigin{};
	T_Vec3 EntSizeWorld{};
	T_Vec3 DummyPos{ 0, 0, 0 };
	T_Vec3 OgrePos{};
	/*Ogre::Vector3 OgreRot{};
	T_Vec3 Cam_Angles{};*/

	POINT EntPosView{};
	POINT EntSizeView{};
	POINT EntWidthHeight{};
	POINT OriginView{};

	POINT TopLeft{}, BottomRight{}, TopRight{}, BottomLeft{};


	/*static const float COS45 = static_cast<float>(cos(M_PI / 4.0f));
	static const float SIN45 = static_cast<float>(sin(M_PI / 4.0f));
	static const float MCOS45 = static_cast<float>(cos(-M_PI / 4.0f));
	static const float MSIN45 = static_cast<float>(sin(-M_PI / 4.0f));*/


	// Compute entity size in view coordinates
	App->CL_X_Maths->Vector3_Set(&EntSizeWorld, ENTITY_SIZE, ENTITY_SIZE, ENTITY_SIZE);
	EntSizeView = App->CL_Render->Render_OrthoWorldToView(App->CL_Views_Com->Current_View, &EntSizeWorld);
	App->CL_X_Maths->Vector3_Clear(&VecOrigin);
	OriginView = App->CL_Render->Render_OrthoWorldToView(App->CL_Views_Com->Current_View, &VecOrigin);

	// Calculate width and height of the entity
	EntWidthHeight.x = std::abs(OriginView.x - EntSizeView.x);
	EntWidthHeight.y = std::abs(OriginView.y - EntSizeView.y);

	// Adjust entity size view
	EntSizeView.x -= OriginView.x;
	EntSizeView.y -= OriginView.y;

	// Entity's position in the view
	if (App->flag_3D_Started == 1)
	{
		/*if (All == true)
		{
			OgreRot.x = App->CL_Ogre->camNode->getOrientation().getPitch().valueRadians();
			OgreRot.y = App->CL_Ogre->camNode->getOrientation().getYaw().valueRadians();
			Cam_Angles = { M_PI - OgreRot.x, -OgreRot.y, 0 };
		}*/

		OgrePos.x = App->CL_Ogre->camNode->getPosition().x;
		OgrePos.y = App->CL_Ogre->camNode->getPosition().y;
		OgrePos.z = App->CL_Ogre->camNode->getPosition().z;

		EntPosView = App->CL_Render->Render_OrthoWorldToView(App->CL_Views_Com->Current_View, &OgrePos);
	}
	else
	{
		EntPosView = App->CL_Render->Render_OrthoWorldToView(App->CL_Views_Com->Current_View, &DummyPos);
	}

	// Draw an X at the Camera position
	TopLeft = { EntPosView.x - EntSizeView.x, EntPosView.y - EntSizeView.y };
	BottomRight = { EntPosView.x + EntSizeView.x, EntPosView.y + EntSizeView.y };
	TopRight = { BottomRight.x, TopLeft.y };
	BottomLeft = { TopLeft.x, BottomRight.y };

	MoveToEx(ViewDC, TopLeft.x, TopLeft.y, NULL);
	LineTo(ViewDC, BottomRight.x, BottomRight.y);
	MoveToEx(ViewDC, TopRight.x, TopRight.y, NULL);
	LineTo(ViewDC, BottomLeft.x, BottomLeft.y);

	if (All == true)
	{
		//// Prepare for drawing the direction line
		//POINT ptDirSlope{};
		//POINT ptRotationPoint{};
		//POINT ptRelRotatePoint{};
		//POINT ptPlus45{}, ptMinus45{};

		//float fRadius = 100.0f;
		//bool bUIAvailable = true;

		//// Set camera angles

		//App->CL_X_Maths->Vector3_Set(&Cam_Angles, Cam_Angles.z, (-Cam_Angles.y - M_PI / 2.0f), Cam_Angles.x);


		//Matrix3d Xfm{};
		//T_Vec3 VecTarg{};
		//App->CL_X_Maths->XForm3d_SetEulerAngles(&Xfm, &Cam_Angles);
		//App->CL_X_Maths->Vector3_Set(&VecTarg, fRadius, 0.0f, 0.0f);
		//App->CL_X_Maths->XForm3d_Transform(&Xfm, &VecTarg, &VecTarg);
		//App->CL_X_Maths->Vector3_Add(&OgrePos, &VecTarg, &VecTarg);

		//POINT LineEndView = App->CL_Render->Render_OrthoWorldToView(Current_View, &VecTarg);

		//// Draw to the end point
		//MoveToEx(ViewDC, EntPosView.x, EntPosView.y, NULL);
		//LineTo(ViewDC, LineEndView.x, LineEndView.y);
		////Ellipse(ViewDC, LineEndView.x, LineEndView.x, LineEndView.y+50, LineEndView.y+50);

		//ptDirSlope = { LineEndView.x - EntPosView.x, LineEndView.y - EntPosView.y };
		//float fDirLength = sqrt(ptDirSlope.x * ptDirSlope.x + ptDirSlope.y * ptDirSlope.y);
		//float fEntityLength = sqrt(EntSizeView.x * EntSizeView.x + EntSizeView.y * EntSizeView.y) * 1; // Arrow 2x entity size
		//float fPercentIntoLine = 1.0f - (fEntityLength / fDirLength);

		//ptRotationPoint = { static_cast<long>(ptDirSlope.x * fPercentIntoLine + EntPosView.x),
		//				   static_cast<long>(ptDirSlope.y * fPercentIntoLine + EntPosView.y) };

		//ptRelRotatePoint = { ptRotationPoint.x - LineEndView.x, ptRotationPoint.y - LineEndView.y };

		//ptPlus45 = { static_cast<long>(ptRelRotatePoint.x * COS45 - ptRelRotatePoint.y * SIN45 + LineEndView.x),
		//			static_cast<long>(ptRelRotatePoint.y * COS45 + ptRelRotatePoint.x * SIN45 + LineEndView.y) };

		//ptMinus45 = { static_cast<long>(ptRelRotatePoint.x * MCOS45 - ptRelRotatePoint.y * MSIN45 + LineEndView.x),
		//			 static_cast<long>(ptRelRotatePoint.y * MCOS45 + ptRelRotatePoint.x * MSIN45 + LineEndView.y) };

		//LineTo(ViewDC, ptPlus45.x, ptPlus45.y);
		//LineTo(ViewDC, ptMinus45.x, ptMinus45.y);
		//LineTo(ViewDC, LineEndView.x, LineEndView.y);
	}

}

