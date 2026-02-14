#pragma once

#include "CreateBoxDialog.h"
#include "CreateCylDialog.h"
#include "CreateArchDialog.h"
#include "CreateConeDialog.h"

class App_Templates
{
public:
	App_Templates(void);
	~App_Templates(void);

	void Init_Templates(void);

	void Enable_Map_Editor_Dialogs(bool Enable);
	void Enable_Shape_Dialog(bool Enable);

	HWND Shape_Dlg_hWnd;

	//CreateBoxDialog*	CL_CreateBox;
	//CreateCylDialog*	CL_CreateCylinder;
	//CreateArchDialog*	CL_CreateArch;
	//CreateConeDialog*	CL_CreateCone;
};

