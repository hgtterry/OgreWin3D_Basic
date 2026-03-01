#pragma once
class CL64_Properties_Motions
{
public:
	CL64_Properties_Motions();
	~CL64_Properties_Motions();

	void Show_Motions_Dialog(bool Show);
	void Start_Motions_Dialog();

	HWND Motions_Dlg_Hwnd;

private:
	static LRESULT CALLBACK Proc_Motions_Dialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

};

