// ProgressListDialog.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "WinDFR.h"
#include "ProgressListDialog.h"


// CProgressListDialog �_�C�A���O

IMPLEMENT_DYNAMIC(CProgressListDialog, CDialog)
CProgressListDialog::CProgressListDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CProgressListDialog::IDD, pParent)
{
}

CProgressListDialog::~CProgressListDialog()
{
}

void CProgressListDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CProgressListDialog, CDialog)
END_MESSAGE_MAP()


// CProgressListDialog ���b�Z�[�W �n���h��
