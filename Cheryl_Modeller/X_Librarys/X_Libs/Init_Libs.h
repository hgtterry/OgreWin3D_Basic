#pragma once

class Init_Libs
{
public:
	Init_Libs(void);
	~Init_Libs(void);

	void Start_Libraries();

	char* GetVersion();

	CreateBoxDialog*	CL_CreateBox;
	CreateCylDialog*	CL_CreateCylinder;
	CreateConeDialog*	CL_CreateCone;
	CreateArchDialog*	CL_CreateArch;

};

