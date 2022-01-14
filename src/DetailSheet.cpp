#include "stdafx.h"
#include "WinTILD.h"
#include "DetailSheet.h"


CDetailSheet::CDetailSheet()
{
}

IMPLEMENT_DYNAMIC(CDetailSheet, CPropertySheet)
CDetailSheet::CDetailSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CDetailSheet::CDetailSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

CDetailSheet::~CDetailSheet()
{
}


BEGIN_MESSAGE_MAP(CDetailSheet, CPropertySheet)
END_MESSAGE_MAP()

