#pragma once

#define BASE_BINARY			2
#define BASE_OCTAL			8
#define BASE_DECIMAL		10
#define BASE_HEXADECIMAL	16

class Lib_Ini_File
{
public:
	Lib_Ini_File(void);
	~Lib_Ini_File(void);

	void Clean_Up(void);
	void SetPathName(LPCTSTR lpPathName);

	int GetInt(LPCTSTR lpSection, LPCTSTR lpKey, int nDefault, int nBase) const;
	DWORD GetString(LPCTSTR lpSection, LPCTSTR lpKey, LPTSTR lpBuffer, DWORD dwBufSize, LPCTSTR lpDefault = NULL) const;

	static int __ValidateBase(int nBase);
	LPTSTR __GetStringDynamic(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpDefault = NULL) const;

	LPTSTR m_pszPathName;
};

