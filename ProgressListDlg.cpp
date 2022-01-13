#include "stdafx.h"
#include "WinTILD.h"
#include "ProgressListDlg.h"


IMPLEMENT_DYNAMIC(CProgressListDlg, CDialog)
CProgressListDlg::CProgressListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProgressListDlg::IDD, pParent)
{
	m_pListBox = new CProgressListBox;
}

CProgressListDlg::~CProgressListDlg()
{
	delete m_pListBox;
}

void CProgressListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROGRESS, *m_pListBox);
}


BEGIN_MESSAGE_MAP(CProgressListDlg, CDialog)
END_MESSAGE_MAP()


BOOL CProgressListDlg::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD,pParentWnd);
}

void CProgressListDlg::ResetList(void)
{
	m_pListBox->ResetContent();
}

int CProgressListDlg::AddText(LPCTSTR txt)
{
	return m_pListBox->AddString(txt);
}

void CProgressListDlg::SetItemData(int index, CDivMSD *pdmsd)
{
	VERIFY(LB_ERR != m_pListBox->SetItemData(index,(DWORD_PTR)pdmsd) );
}
