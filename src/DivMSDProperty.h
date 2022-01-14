#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "WinTILDDoc.h"
#include "ColorStatic.h"
#include "BrushStatic.h"
#include "PenStatic.h"


class CDivMSDProperty : public CPropertyPage
{
	DECLARE_DYNAMIC(CDivMSDProperty)

public:
	CDivMSDProperty();
	virtual ~CDivMSDProperty();

	enum { IDD = IDD_DIVMSD_PROPERTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
public:
protected:
	BOOL m_Lock;
	int m_Size;
	CColorStatic m_csBase;
	CColorStatic m_csThreshold;
	CPenStatic   m_psStartLine;
	CPenStatic   m_psEndLine;
	CPenStatic   m_psBoundary;

	CWinTILDDoc* GetDocument();
	void SetDlgItemDouble(int id,double d);
	double GetDlgItemDouble(int id);
 
	void UpdateControl(int nIDParent, int nIDChile);

	void OnColorDlg( CColorStatic& cs );
	void OnBrushDlg( CBrushStatic& bs );
	void OnPenDlg( CPenStatic& ps );

	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnEnChangeEditDivmsdSize();
	afx_msg void OnBnClickedRadioUsesize();
	afx_msg void OnBnClickedRadioUsemagnify();
	afx_msg void OnDeltaposSpinDivmsdMagnify(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditDivmsdMagnify();
	
	void DDX_Size(CDataExchange *pDX,int nIDC, CRect &rc);
	void DDX_Check(CDataExchange *pDX, int nIDC, BOOL &value);

public:
	BOOL OnInitDialog();
	void Update();
	virtual void OnOK();
	afx_msg void OnBnClickedButtonDmsdBasecolor();
	afx_msg void OnBnClickedButtonDmsdThreshold();
	afx_msg void OnBnClickedButtonDmsdBoundary();
	afx_msg void OnBnClickedButtonDmsdEndline();
	afx_msg void OnBnClickedButtonDmsdStartline();
	afx_msg void OnBnClickedCheckDmsdDrawfromto();
	afx_msg void OnBnClickedCheckDmsdDrawlinemethod();
	afx_msg void OnBnClickedCheckDmsdDrawfromtoframe();
	afx_msg void OnBnClickedCheckDmsdDrawboundary();
	afx_msg void OnBnClickedCheckDmsdDrawboundaryframe();
	afx_msg void OnBnClickedCheckDmsdDrawmagnify();
	afx_msg void OnBnClickedCheckDmsdDrawscale();
	CColorStatic m_csBackground;
	CColorStatic m_csBackgroundText;
	afx_msg void OnBnClickedButtonDmsdBg();
	afx_msg void OnBnClickedButtonDmsdTextbg();
	afx_msg void OnBnClickedCheckDmsdBackground();
	afx_msg void OnBnClickedCheckDmsdTextbg();
	afx_msg void OnBnClickedButtonDmsdMargin();
	CBrushStatic m_bsMargin;
	CPenStatic m_psFrame;
	afx_msg void OnBnClickedButtonDmsdFrame();
private:
	// Slider for setting the color of the threshold value
	CSliderCtrl m_threshold_color_control;
	CString m_threshold_color_string;
public:
	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

private:
	//	Set a certain color of the m_picture_threshold_colo 
	void SetPictureThresholdColor(int threshole_color_value);
	CColorStatic m_picture_threshold_color;
};
