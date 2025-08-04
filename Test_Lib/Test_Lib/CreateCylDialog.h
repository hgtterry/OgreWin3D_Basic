#pragma once
class CreateCylDialog
{
public:
	CreateCylDialog(void);
	~CreateCylDialog(void);

	void Start_CreateCyl_Dlg();

	BrushTemplate_Cylinder* pCylinderTemplate;

	float	m_BotXOffset;
	float	m_BotXSize;
	float	m_BotZOffset;
	float	m_BotZSize;
	int		m_Solid;
	float	m_TopXOffset;
	float	m_TopXSize;
	float	m_TopZOffset;
	float	m_TopZSize;
	float	m_YSize;
	float	m_RingLength;
	BOOL	m_TCut;
	int		m_VerticalStripes;
	float	m_Thickness;

	bool	m_UseCamPos;

private:

	static LRESULT CALLBACK Proc_Create_Cylinder(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void Set_Members();
	void Set_DLG_Members(HWND hDlg);
	void Get_DLG_Members(HWND hDlg);
	void Set_CylinderTemplate();

	void Set_Defaults(HWND hDlg);

	void CreateCylinder_New();
	void CreateNewTemplateBrush(Brush* pBrush);

	char CylinderName[MAX_PATH];

	bool flag_Solid_Flag_Dlg;
	bool flag_Hollow_Flag_Dlg;
	bool flag_Ring_Flag_Dlg;
};

