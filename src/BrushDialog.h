#pragma once

#include "ColorStatic.h"
#include "BrushHatchComboBox.h"
#include "BrushStyleComboBox.h"
#include "afxwin.h"


class CBrushDialog : public CDialog
{
	DECLARE_DYNAMIC(CBrushDialog)

protected:
	LOGBRUSH m_Brush;
	CColorStatic m_csColor;
	CBrushStyleComboBox m_bsComboBox;
	CBrushHatchComboBox m_bhComboBox;
public:
	CBrushDialog(const LOGBRUSH& brush, CWnd* pParent = NULL);
	virtual ~CBrushDialog();

	enum { IDD = IDD_BRUSH_DIALOG };

	LOGBRUSH GetBrush()const{return m_Brush;};
protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
public:
	void UpdateControls(UINT style);
protected:
	afx_msg void OnBnClickedButtonBrushColor();
	afx_msg void OnCbnSelchangeComboBrushStyle();
	afx_msg void OnCbnSelchangeComboBrushHatch();
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
};
