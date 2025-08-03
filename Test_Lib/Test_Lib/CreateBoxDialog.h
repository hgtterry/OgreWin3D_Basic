#pragma once
class CreateBoxDialog
{
public:

	CreateBoxDialog(void);
	~CreateBoxDialog(void);

	void Start_CreateBox_Dlg();

	BrushTemplate_Box* pBoxTemplate;

private:
	static LRESULT CALLBACK Proc_CreateBox(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

};

