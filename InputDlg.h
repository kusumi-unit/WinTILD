#pragma once
#include "afxcmn.h"
#include "afxwin.h"

class CInputDlg : public CDialog
{
	DECLARE_DYNAMIC(CInputDlg)

public:
	CInputDlg(CWnd* pParent = NULL);
	virtual ~CInputDlg();

	enum { IDD = IDD_INPUT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedButtonRemove();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonUp();
	afx_msg void OnBnClickedButtonDown();
	afx_msg void OnLbnSelcancelListInput();
	afx_msg void OnLbnSelchangeListInput();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnEnChangeEditInputFrom();
	afx_msg void OnEnChangeEditInputTo();
    afx_msg void OnEnChangeEditSmoothing();

protected:
	BOOL m_Lock;

	CWinTILDDoc* GetDocument();
	void UpdateListWidth();
	void UpdateDocList();
    void SwapListString(int index1, int index2);

public:
	virtual BOOL Create( CWnd* pParentWnd = NULL );
	CListBox* GetListBox()const{ return (CListBox*)GetDlgItem(IDC_LIST_INPUT); };
	CSliderCtrl m_SliderFrom;
	CSliderCtrl m_SliderTo;
    CSpinButtonCtrl m_SpinFrom;
	CSpinButtonCtrl m_SpinTo;
    CEdit m_EditFrom;
	CEdit m_EditTo;
    virtual BOOL OnInitDialog();
	void TextUpdate(void);
	void UpdateControlRange(int min, int max);
	void Lock();
	void Unlock();
	BOOL Locking()const{ return m_Lock; };
	void GetTargetFrame(int& start, int& end);

	afx_msg void OnBnClickedCheckDeleteafter();
	afx_msg void OnBnClickedCheckDontshow();

    void UpdateDlgList(void);
    void ResetSmoothing();
};
