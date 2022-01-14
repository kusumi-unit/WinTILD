#include "stdafx.h"
#include "WinTILD.h"
#include "GeneralProperty.h"
#include "Function.h"
#include "MainFrm.h"


IMPLEMENT_DYNAMIC(CGeneralProperty, CPropertyPage)
CGeneralProperty::CGeneralProperty()
	: CPropertyPage(CGeneralProperty::IDD)
{
}

CGeneralProperty::~CGeneralProperty()
{
}

void CGeneralProperty::DoDataExchange(CDataExchange* pDX)
{
	CGeneralOption *pO = GetDocument()->GetGeneralOption();
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_GENERAL_BACKGROUND, m_csBackground);

	DDX_Color( pDX, m_csBackground, pO->m_clBackGround );
}


BEGIN_MESSAGE_MAP(CGeneralProperty, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_GENERAL_BACKGROUND, OnBnClickedButtonGeneralBackground)
END_MESSAGE_MAP()


CWinTILDDoc* CGeneralProperty::GetDocument()
{
	if (theApp.m_pMainWnd!=NULL){
		return (CWinTILDDoc*)((CMainFrame*)theApp.m_pMainWnd)->GetActiveDocument();
	}
	else {
		return NULL;
	}
}

void CGeneralProperty::OnBnClickedButtonGeneralBackground()
{
	OnColorDlg( m_csBackground );
}

void CGeneralProperty::OnColorDlg( CColorStatic& cs )
{
	CColorDialog dlg( cs.GetColor() );
	if ( IDOK == dlg.DoModal() ){
		SetModified();
		cs.SetColor( dlg.GetColor() );
	}
}
void CGeneralProperty::OnOK()
{
	UpdateData(TRUE);
	GetDocument()->Redraw();
	CPropertyPage::OnOK();
}
