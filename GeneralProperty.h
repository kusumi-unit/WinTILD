#pragma once
#include "WinTILDDoc.h"
#include "colorstatic.h"


class CGeneralProperty : public CPropertyPage
{
	DECLARE_DYNAMIC(CGeneralProperty)

public:
	CGeneralProperty();
	virtual ~CGeneralProperty();

	enum { IDD = IDD_GENERAL_PROPERTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	CWinTILDDoc* GetDocument();
	void OnColorDlg( CColorStatic& cs );

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonGeneralBackground();
	CColorStatic m_csBackground;
	virtual void OnOK();
};
