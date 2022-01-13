#pragma once
#include "afxwin.h"


class CParameterDlg : public CDialog
{
	DECLARE_DYNAMIC(CParameterDlg)

public:
	CParameterDlg(CWnd* pParent = NULL);
	virtual ~CParameterDlg();

	enum { IDD = IDD_PARAMETER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	
	DECLARE_MESSAGE_MAP()

private:
    int smoothing_;

protected:
	BOOL m_Lock;
	CWinTILDDoc* GetDocument(void);
	void DeltaDeltaControl(LPNMUPDOWN pNMUpDown, int id, double delta, double upperlimit, double lowerlimit);

public:
	virtual BOOL OnInitDialog();
	virtual BOOL Create( CWnd* pParentWnd = NULL );
	double GetDlgItemDouble(int id);
	void SetDlgItemDouble(int id, double v);

	void SetSkip(int frames);
	void SetRatio(double ratio);
	void SetThreshold(double threshold);
	void SetMaximum(double maximum);
	void SetWindow(int start, int end);
	void SetWindowRange(int start, int end);
	void SetSmoothing(int smoothing);

	// If "Lock" is true, it do not update the CWinTILDDoc
	void Lock(){m_Lock = TRUE;};
	void Unlock(){m_Lock = FALSE;};
	BOOL Locking()const{ return m_Lock; };
	// Return the current parameter
	CParameter* GetParameter(void);
	afx_msg void OnDeltaposSpinThreshold(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinThresholdFine(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditThreshold();
	afx_msg void OnEnChangeEditWfrom();
	afx_msg void OnEnChangeEditWto();
	afx_msg void OnEnChangeEditRatio();
	afx_msg void OnEnChangeEditSkip();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDeltaposSpinWfrom(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinWto(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinRatio(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinRatioFine(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheckInteractive();
	void UpdateControlForCalcMax(BOOL calc);
	afx_msg void OnEnChangeEditCalcavefrom();
	afx_msg void OnEnChangeEditCalcaveto();
	void SetAverageFrom(int from);
	void SetAverageTo(int to);
	afx_msg void OnEnChangeEditMaximum();
	afx_msg void OnDeltaposSpinMaximum(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinMaximumFine(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnBnClickedCheckDrawBoundaries();
	void SetDrawBoundaries(BOOL drawboundaries);	
	CButton m_draw_boundaries;
};
