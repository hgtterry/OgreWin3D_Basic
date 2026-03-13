#pragma once

class Init_Libs
{
public:
	Init_Libs(void);
	~Init_Libs(void);

	void Start_Libraries();

	char* GetVersion();
	char* Get_Time_Stamp();

	CreateBoxDialog*	CL_CreateBox;
	CreateCylDialog*	CL_CreateCylinder;
	CreateConeDialog*	CL_CreateCone;
	CreateArchDialog*	CL_CreateArch;

	Lib_Preference*		CL_Preference;

};

