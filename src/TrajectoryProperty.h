#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "WinTILDDoc.h"
#include "ColorStatic.h"
#include "BrushStatic.h"
#include "PenStatic.h"


class CTrajectoryProperty : public CPropertyPage
{
	DECLARE_DYNAMIC(CTrajectoryProperty)

public:
	CTrajectoryProperty();
	virtual ~CTrajectoryProperty();

	enum { IDD = IDD_TRAJECTORY_PROPERTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
protected:
	BOOL m_CheckSquare;
	BOOL m_Lock;
	CBrushStatic m_bsPlotArea;
	CBrushStatic m_bsMargin;
	CPenStatic   m_psFrame;
	CColorStatic m_csFrom;
	CColorStatic m_csTo;
	CColorStatic m_csBoundary;
	CColorStatic m_csBackground;
	CColorStatic m_csBackgroundText;
	CPenStatic   m_psLine1;
	CPenStatic   m_psLine2;
	CPenStatic   m_psLine3;
	CPenStatic   m_psLine4;
	CPenStatic   m_psLine5;

	unsigned long m_Status;
	BOOL m_drawStartEnd;

	CWinTILDDoc* GetDocument();
	void OnColorDlg( CColorStatic& cs );
	void OnBrushDlg( CBrushStatic& bs );
	void OnPenDlg( CPenStatic& ps );
	void UpdateControlBG();
	void UpdateControlTextBG();
	void UpdateControls( const CPlotterOption* pO );
protected:

	void DDX_Width(CDataExchange *pDX,int nIDC, CRect &rc);
	void DDX_Height(CDataExchange *pDX,int nIDC, CRect &rc);

	virtual void OnOK();
	afx_msg void OnEnChangeEditTrajectoryWidth();
	afx_msg void OnEnChangeEditTrajectoryHeight();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedButtonTrajPlotArea();
	afx_msg void OnBnClickedButtonTrajMargin();
	afx_msg void OnBnClickedButtonTrajFrame();
	afx_msg void OnBnClickedCheckTrajectorySquare();
	afx_msg void OnBnClickedButtonTrajFrom();
	afx_msg void OnBnClickedButtonTrajTo();
	afx_msg void OnBnClickedButtonTrajBoundary();
	afx_msg void OnBnClickedCheckTrajDrawstartend();
	afx_msg void OnBnClickedCheckTrajDrawcoordinates();
	afx_msg void OnBnClickedCheckTrajDrawfromto();
	afx_msg void OnBnClickedCheckTrajDrawboundary();
	afx_msg void OnBnClickedCheckTrajDrawboundarytext();
	afx_msg void OnBnClickedButtonTrajBg();
	afx_msg void OnBnClickedCheckTrajBackground();
	afx_msg void OnBnClickedButtonTrajTextbg();
	afx_msg void OnBnClickedCheckTrajTextbg();
public:
	afx_msg void OnBnClickedButtonTrajLine1();
	afx_msg void OnBnClickedButtonTrajLine2();
	afx_msg void OnBnClickedButtonTrajLine3();
	afx_msg void OnBnClickedButtonTrajLine4();
	afx_msg void OnBnClickedButtonTrajLine5();
};
