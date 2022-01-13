#pragma once


// CProgressListDialog ダイアログ

class CProgressListDialog : public CDialog
{
	DECLARE_DYNAMIC(CProgressListDialog)

public:
	CProgressListDialog(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CProgressListDialog();

// ダイアログ データ
	enum { IDD = IDD_PROGRESSLIST_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
