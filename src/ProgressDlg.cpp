#include "stdafx.h"
#include "WinTILD.h"
#include "ProgressDlg.h"


IMPLEMENT_DYNAMIC(CProgressDlg, CDialog)
CProgressDlg::CProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProgressDlg::IDD, pParent)
{
}

CProgressDlg::~CProgressDlg()
{
}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
}


BEGIN_MESSAGE_MAP(CProgressDlg, CDialog)
END_MESSAGE_MAP()


BOOL CProgressDlg::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD,pParentWnd);
}

void CProgressDlg::SetMessage(const char* message)
{
	SetDlgItemText(IDC_STATIC_MESSAGE,message);
}

void CProgressDlg::SetPos(int pos)
{
	m_Progress.SetPos(pos);
	Invalidate(FALSE);
}

void CProgressDlg::SetMax(int max)
{
	m_Progress.SetRange32(0,max);
	Invalidate(FALSE);
}

BOOL CProgressDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;
}
