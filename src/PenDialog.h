#pragma once
#include "colorstatic.h"
#include "PenStyleComboBox.h"


class CPenDialog : public CDialog
{
	DECLARE_DYNAMIC(CPenDialog)

protected:
	LOGPEN m_Pen;
	CColorStatic m_csColor;
	CPenStyleComboBox m_psComboBox;
	long m_nWidth;
public:
	CPenDialog(const LOGPEN& pen, CWnd* pParent = NULL);
	virtual ~CPenDialog();

	enum { IDD = IDD_PEN_DIALOG };

	LOGPEN GetPen()const{return m_Pen;};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
public:
	void UpdateControls( long width );
	void UpdateControls( UINT style );
protected:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedButtonPenColor();
	afx_msg void OnEnChangeEditPenWidth();
public:
	afx_msg void OnCbnSelchangeComboPenStyle();
};
