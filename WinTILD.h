#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"


class CWinTILDApp : public CWinApp
{
public:
	CWinTILDApp();

public:
	virtual BOOL InitInstance();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	TCHAR* GetIniFileName()const;
	
};

extern CWinTILDApp theApp;

void DeleteExtension(CString& path);
CString ExtractFileName(const CString& path);
