#pragma once
class Test_Lib_X
{
public:

	Test_Lib_X(void);
	~Test_Lib_X(void);

	void Try(void);
	void Message(char* pString, char* pString2);

private:

	static LRESULT CALLBACK Proc_Message2(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	char Message_Text_Header[MAX_PATH];
	char Message_Text_Message[MAX_PATH];

};

