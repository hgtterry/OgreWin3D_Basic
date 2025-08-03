#pragma once
class CreateConeDialog
{
public:
	CreateConeDialog(void);
	~CreateConeDialog(void);

	void Start_CreateCone_Dlg();

	BrushTemplate_Cone* pConeTemplate;

	int		m_Style;
	float	m_Width;
	float	m_Height;
	int		m_VerticalStrips;
	float	m_Thickness;
	bool	m_TCut;

	bool	m_UseCamPos;

private:

	static LRESULT CALLBACK Proc_CreateCone(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void Set_Members();
	void Set_DLG_Members(HWND hDlg);
	void Get_DLG_Members(HWND hDlg);
	void Set_ConeTemplate();

	void Set_Defaults(HWND hDlg);

	void CreateCone();
	void CreateNewTemplateBrush(Brush* pBrush);

	char ConeName[MAX_PATH];

	bool flag_Solid_Flag_Dlg;
	bool flag_Hollow_Flag_Dlg;
	bool flag_Funnel_Flag_Dlg;
};

