#pragma once
#include "afxwin.h"
#include "ProgressListBox.h"
#include "DivMSD.h"


class CProgressListDlg : public CDialog
{
	DECLARE_DYNAMIC(CProgressListDlg)

protected:
	CListBox *m_pListBox;
public:
	CProgressListDlg(CWnd* pParent = NULL);
	virtual ~CProgressListDlg();
	BOOL Create(CWnd *pParentWnd = NULL);

// ダイアログ データ
	enum { IDD = IDD_PROGRESSLIST_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
public:
	void ResetList(void); 
	int AddText(LPCTSTR txt);
	void SetItemData(int index, CDivMSD *pdmsd);
	CProgressListBox* GetListBox()const{ return (CProgressListBox*)m_pListBox; };
};
