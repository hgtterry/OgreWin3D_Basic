#pragma once
class CL64_Brushes_Control
{
public:
	CL64_Brushes_Control(void);
	~CL64_Brushes_Control(void);

	void Start_Brushes_Control_Dlg();

	HWND Brush_Control_Dlg_hWnd;

private:

	static LRESULT CALLBACK Proc_Top_Tabs_Brushes(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
};


