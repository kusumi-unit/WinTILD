#pragma once
#include "afxcmn.h"


class CProgressDlg : public CDialog
{
	DECLARE_DYNAMIC(CProgressDlg)

public:
	CProgressDlg(CWnd* pParent = NULL);
	virtual ~CProgressDlg();

	enum { IDD = IDD_PROGRESS_DIALOG };

	BOOL Create(CWnd *pParentWnd = NULL);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
public:
	void SetMessage(const char* message);
	CProgressCtrl m_Progress;
	void SetPos(int pos);
	void SetMax(int max);
	virtual BOOL OnInitDialog();
};
