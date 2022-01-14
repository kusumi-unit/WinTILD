#include "stdafx.h"
#include "WinTILD.h"
#include "GeneralOption.h"


CGeneralOption::CGeneralOption()
{
	m_lpszSection = "General";
}

CGeneralOption::~CGeneralOption()
{
}

void CGeneralOption::LoadProfiles(CWinApp *pApp)
{
	m_clBackGround = pApp->GetProfileInt( m_lpszSection, "Background", GetSysColor(COLOR_WINDOW) );
}

void CGeneralOption::SaveProfiles(CWinApp *pApp)
{
	pApp->WriteProfileInt( m_lpszSection, "Background", m_clBackGround );
}
