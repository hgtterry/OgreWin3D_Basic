#pragma once
class Init_Libs
{
public:
	Init_Libs(void);
	~Init_Libs(void);

	void Start_Libraries();

	char* GetVersion();

	CreateBoxDialog* CL_CreateBox;

	///*CreateCylDialog* CL_CreateCylinder;
	//CreateArchDialog* CL_CreateArch;
	//CreateConeDialog* CL_CreateCone;*/

};

