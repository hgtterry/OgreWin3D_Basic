#pragma once
class App_Templates
{
public:
	App_Templates(void);
	~App_Templates(void);

	void Enable_Map_Editor_Dialogs(bool Enable);
	void Enable_Shape_Dialog(bool Enable);

	HWND Shape_Dlg_hWnd;
};

