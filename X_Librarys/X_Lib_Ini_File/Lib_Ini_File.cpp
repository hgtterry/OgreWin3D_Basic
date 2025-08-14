#include "pch.h"
#include "Lib_Ini_File.h"

#pragma warning(disable:4996 4800)
#pragma warning( disable : 4244)

#define min(a,b)            (((a) < (b)) ? (a) : (b))

#define DEF_PROFILE_NUM_LEN		64 // numeric string length, could be quite long for binary format
#define DEF_PROFILE_THRESHOLD	512 // temporary string length
#define DEF_PROFILE_DELIMITER	_T(",") // default string delimiter

Lib_Ini_File::Lib_Ini_File(void)
{
	m_pszPathName = NULL;
}

Lib_Ini_File::~Lib_Ini_File(void)
{
}

// *************************************************************************
// *	  		Clean_Up:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void Lib_Ini_File::Clean_Up(void)
{

}

// *************************************************************************
// *	  		SetPathName:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void Lib_Ini_File::SetPathName(LPCTSTR lpPathName)
{
	if (lpPathName == NULL)
	{
		if (m_pszPathName != NULL)
			*m_pszPathName = _T('\0');
	}
	else
	{
		if (m_pszPathName != NULL)
			delete[] m_pszPathName;

		m_pszPathName = _tcsdup(lpPathName);
	}
}

// *************************************************************************
// *	  			GetInt:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
int Lib_Ini_File::GetInt(LPCTSTR lpSection, LPCTSTR lpKey, int nDefault, int nBase) const
{
	TCHAR sz[DEF_PROFILE_NUM_LEN + 1] = _T("");
	GetString(lpSection, lpKey, sz, DEF_PROFILE_NUM_LEN);
	return *sz == _T('\0') ? nDefault : int(_tcstoul(sz, NULL, __ValidateBase(nBase)));
}

// *************************************************************************
// *	  			GetString:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
DWORD Lib_Ini_File::GetString(LPCTSTR lpSection, LPCTSTR lpKey, LPTSTR lpBuffer, DWORD dwBufSize, LPCTSTR lpDefault) const
{
	if (lpBuffer != NULL)
		*lpBuffer = _T('\0');

	LPTSTR psz = __GetStringDynamic(lpSection, lpKey, lpDefault);
	DWORD dwLen = _tcslen(psz);

	if (lpBuffer != NULL)
	{
		_tcsncpy(lpBuffer, psz, dwBufSize);
		dwLen = min(dwLen, dwBufSize);
	}

	delete[] psz;
	return dwLen;
}

// *************************************************************************
// *	  		__ValidateBase:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
int Lib_Ini_File::__ValidateBase(int nBase)
{
	switch (nBase)
	{
	case BASE_BINARY:
	case BASE_OCTAL:
	case BASE_HEXADECIMAL:
		break;

	default:
		nBase = BASE_DECIMAL;
	}

	return nBase;
}

// *************************************************************************
// *	  	__GetStringDynamic:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
LPTSTR Lib_Ini_File::__GetStringDynamic(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpDefault) const
{
	TCHAR* psz = NULL;
	if (lpSection == NULL || lpKey == NULL)
	{
		// Invalid section or key name, just return the default string
		if (lpDefault == NULL)
		{
			// Empty string
			psz = new TCHAR[1];
			*psz = _T('\0');
		}
		else
		{
			psz = new TCHAR[_tcslen(lpDefault) + 1];
			_tcscpy(psz, lpDefault);
		}

		return psz;
	}

	// Keep enlarging the buffer size until being certain on that the string we
	// retrieved was original(not truncated).
	DWORD dwLen = DEF_PROFILE_THRESHOLD;
	psz = new TCHAR[dwLen + 1];
	DWORD dwCopied = ::GetPrivateProfileString(lpSection, lpKey, lpDefault == NULL ? _T("") : lpDefault, psz, dwLen, m_pszPathName);
	while (dwCopied + 1 >= dwLen)
	{
		dwLen += DEF_PROFILE_THRESHOLD;
		delete[] psz;
		psz = new TCHAR[dwLen + 1];
		dwCopied = ::GetPrivateProfileString(lpSection, lpKey, lpDefault == NULL ? _T("") : lpDefault, psz, dwLen, m_pszPathName);
	}

	return psz; // !!! Requires the caller to free this memory !!!
}
