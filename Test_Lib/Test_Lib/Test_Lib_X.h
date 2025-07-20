#pragma once
class Test_Lib_X
{
public:

	Test_Lib_X(void);
	~Test_Lib_X(void);

	void Try(void);
	void Message(char* pString, char* pString2);

private:

	LRESULT CALLBACK Proc_Message(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);


};

