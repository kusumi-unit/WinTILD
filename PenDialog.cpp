#include "stdafx.h"
#include "WinTILD.h"
#include "PenDialog.h"


IMPLEMENT_DYNAMIC(CPenDialog, CDialog)
CPenDialog::CPenDialog(const LOGPEN& pen,CWnd* pParent /*=NULL*/)
	: CDialog(CPenDialog::IDD, pParent)
{
	m_Pen = pen;
}

CPenDialog::~CPenDialog()
{
}

void CPenDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_PICTURE_PEN_COLOR, m_csColor);
	DDX_Control(pDX, IDC_COMBO_PEN_STYLE, m_psComboBox);

	DDX_Text(pDX, IDC_EDIT_PEN_WIDTH, m_nWidth);
	//DDV_MinMaxLong(pDX, m_nWidth, 1, 100);

}


BEGIN_MESSAGE_MAP(CPenDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_PEN_COLOR, OnBnClickedButtonPenColor)
	ON_EN_CHANGE(IDC_EDIT_PEN_WIDTH, OnEnChangeEditPenWidth)
	ON_CBN_SELCHANGE(IDC_COMBO_PEN_STYLE, OnCbnSelchangeComboPenStyle)
END_MESSAGE_MAP()


BOOL CPenDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_csColor.SetColor( m_Pen.lopnColor );
	m_nWidth = m_Pen.lopnWidth.x;

	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_PEN_WIDTH);
	pSpin->SetBuddy( GetDlgItem(IDC_EDIT_PEN_WIDTH) );
	pSpin->SetRange32(1,100);

	m_psComboBox.CreateList();
	m_psComboBox.SetPen( m_Pen );

	UpdateControls( m_Pen.lopnWidth.x );
	UpdateControls( m_Pen.lopnStyle );

	UpdateData(FALSE);

	return TRUE;
}

void CPenDialog::OnOK()
{
	UpdateData();

	m_Pen.lopnColor = m_csColor.GetColor();
	m_Pen.lopnStyle = m_psComboBox.GetStyle();
	m_Pen.lopnWidth.x = GetDlgItemInt(IDC_EDIT_PEN_WIDTH);

	CDialog::OnOK();
}

void CPenDialog::OnBnClickedButtonPenColor()
{
	CColorDialog dlg( m_csColor.GetColor() );

	if ( IDOK == dlg.DoModal() ){
		COLORREF color = dlg.GetColor();
		m_csColor.SetColor( color );
		m_psComboBox.SetColor( color );
	}
}

void CPenDialog::OnEnChangeEditPenWidth()
{
	UpdateData(TRUE);
	m_psComboBox.SetWidth( CPoint( m_nWidth, 0 ) );
	UpdateControls( m_nWidth );
}

void CPenDialog::UpdateControls( long width )
{
	GetDlgItem(IDC_COMBO_PEN_STYLE)->EnableWindow( width == 1 );
}

void CPenDialog::UpdateControls( UINT style )
{
	GetDlgItem(IDC_EDIT_PEN_WIDTH)->EnableWindow( style == PS_SOLID );
	GetDlgItem(IDC_SPIN_PEN_WIDTH)->EnableWindow( style == PS_SOLID );
	GetDlgItem(IDC_BUTTON_PEN_COLOR)->EnableWindow( style != PS_NULL );
}

void CPenDialog::OnCbnSelchangeComboPenStyle()
{
	UpdateData(TRUE);
	UpdateControls( m_psComboBox.GetStyle() );
}
