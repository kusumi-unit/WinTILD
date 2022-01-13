#include "stdafx.h"
#include "WinTILD.h"
#include "MainFrm.h"
#include "DomainProperty.h"


IMPLEMENT_DYNAMIC(CDomainProperty, CPropertyPage)
CDomainProperty::CDomainProperty()
	: CPropertyPage(CDomainProperty::IDD)
{
	m_Lock = FALSE;
}

CDomainProperty::~CDomainProperty()
{
}

void CDomainProperty::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_DOMAININDEX, m_EditIndex);
	DDX_Control(pDX, IDC_SPIN_DOMAININDEX, m_SpinIndex);
}


BEGIN_MESSAGE_MAP(CDomainProperty, CPropertyPage)
	ON_EN_CHANGE(IDC_EDIT_DOMAININDEX, OnEnChangeEditDomainindex)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


BOOL CDomainProperty::Create(CWnd *pParentWnd)
{
	return CPropertyPage::Create(IDD,pParentWnd);
}

CWinTILDDoc* CDomainProperty::GetDocument()
{
	if (theApp.m_pMainWnd != NULL){
		return (CWinTILDDoc*)((CMainFrame*)theApp.m_pMainWnd)->GetActiveDocument();
	}
	return NULL;
}

BOOL CDomainProperty::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	
	m_SpinIndex.SetBuddy( &m_EditIndex );
	m_EditIndex.SetWindowText("-1");
	Update();

	return TRUE;
}

void CDomainProperty::OnEnChangeEditDomainindex()
{
	if ( !Locking() ){
		SelectDomain( GetDlgItemInt(IDC_EDIT_DOMAININDEX) );
	}
}

void CDomainProperty::Update()
{
	Lock();
	CWinTILDDoc* pDoc = GetDocument();
	auto pData = pDoc->GetSelectedData();
	if ( pDoc == NULL || !pData){
		m_SpinIndex.SetRange(0,0);
		return;
	}
    int count = static_cast<int>(pData->GetBorder()->GetSize());
	m_SpinIndex.SetRange(-1, static_cast<short>(count));
	m_SpinIndex.Invalidate(FALSE);
	m_SpinIndex.UpdateWindow();
	m_EditIndex.SetWindowText("-1");
	Unlock();
}

void CDomainProperty::SelectDomain(int index)
{
	CWinTILDDoc* pDoc = GetDocument();
	if ( pDoc == NULL)
		return;
	pDoc->SelectDomain(index);
}


void CDomainProperty::OnDestroy()
{
	SelectDomain(-1);
	CPropertyPage::OnDestroy();
}
