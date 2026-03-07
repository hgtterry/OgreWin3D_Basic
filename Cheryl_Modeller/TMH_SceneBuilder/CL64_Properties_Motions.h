#pragma once
class CL64_Properties_Motions
{
public:
	CL64_Properties_Motions();
	~CL64_Properties_Motions();

	void Show_Motions_Dialog(bool Show);
	void Start_Motions_Dialog();

	void Update_Motions_Combo(void);
	void Update_Motions_By_Name(const char* Name, bool Play_Motion);

	HWND Motions_Dlg_Hwnd;

private:
	static LRESULT CALLBACK Proc_Motions_Dialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void Update_Speed_Combo(void);
	
};

