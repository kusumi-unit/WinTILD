#pragma once


class CDetailSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CDetailSheet)

public:
	CDetailSheet();
	CDetailSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CDetailSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CDetailSheet();

protected:
	DECLARE_MESSAGE_MAP()
};


