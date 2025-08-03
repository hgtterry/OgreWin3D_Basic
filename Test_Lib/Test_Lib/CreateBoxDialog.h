#pragma once
class CreateBoxDialog
{
public:

	CreateBoxDialog(void);
	~CreateBoxDialog(void);

	void Start_CreateBox_Dlg();
	void CreateDefault_TemplateCube();

	void CreateCube();

	void CreateNewTemplateBrush(Brush* pBrush);

	BrushTemplate_Box* pBoxTemplate;

	float	m_YSize;
	int		m_Solid;
	float	m_XSizeBot;
	float	m_XSizeTop;
	float	m_ZSizeBot;
	float	m_ZSizeTop;
	bool	m_TCut;
	float	m_Thickness;
	bool	m_TSheet;

	bool	m_UseCamPos;

private:
	static LRESULT CALLBACK Proc_CreateBox(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void SetMembers();
	void Get_Dialog_Members(HWND hDlg);
	void Set_Dialog_Members(HWND hDlg);

	void Set_BoxTemplate();

	void SetDefaults(HWND hDlg);
	void SetRoom(HWND hDlg);

	void Zero_Dlg_Flags(HWND hDlg);

	bool Solid_Flag;
	bool Hollow_Flag;
	bool Cut_Flag;

	bool flag_Default;
	bool flag_Room;

	char BoxName[MAX_PATH];
};

