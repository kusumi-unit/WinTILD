#pragma once


// CProgressListDialog �_�C�A���O

class CProgressListDialog : public CDialog
{
	DECLARE_DYNAMIC(CProgressListDialog)

public:
	CProgressListDialog(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CProgressListDialog();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_PROGRESSLIST_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
};
