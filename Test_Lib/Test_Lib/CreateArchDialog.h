#pragma once
class CreateArchDialog
{
public:
	CreateArchDialog(void);
	~CreateArchDialog(void);

	void Start_CreateArch_Dlg();

	BrushTemplate_Arch* pArchTemplate;

	int		m_NumSlits;
	float	m_Thickness;
	float	m_Width;
	float	m_Radius;
	float	m_WallSize;
	int		m_Style;
	float	m_EndAngle;
	float	m_StartAngle;
	bool	m_TCut;
	float	m_Height;
	float	m_Radius2;
	bool	m_Massive;
	int		m_Shape;
	int		m_Sides;
	bool	m_Steps;
	int		m_CW;

	bool	m_UseCamPos;

private:

	static LRESULT CALLBACK CreateArch_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void Set_Members();
	void Set_DLG_Members(HWND hDlg);
	void Get_DLG_Members(HWND hDlg);
	void Set_ArchTemplate();

	void Set_Defaults(HWND hDlg);

	void CreateArch();
	void CreateNewTemplateBrush(Brush* pBrush);
	void Zero_Dlg_Flags(HWND hDlg);

	bool flag_Solid_Flag;
	bool flag_Hollow_Flag;
	bool flag_Ring_Flag;
	bool flag_Cut_Flag;
	bool flag_Rectangle_Flag;
	bool flag_Round_Flag;

	char ArchName[MAX_PATH];
};

