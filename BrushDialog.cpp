#include "stdafx.h"
#include "WinTILD.h"
#include "BrushDialog.h"


IMPLEMENT_DYNAMIC(CBrushDialog, CDialog)
CBrushDialog::CBrushDialog(const LOGBRUSH& brush, CWnd* pParent /*=NULL*/)
	: CDialog(CBrushDialog::IDD, pParent)
{
	m_Brush = brush;
}

CBrushDialog::~CBrushDialog()
{
}

void CBrushDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_PICTURE_BRUSH_COLOR, m_csColor);
	DDX_Control(pDX, IDC_COMBO_BRUSH_STYLE, m_bsComboBox);
	DDX_Control(pDX, IDC_COMBO_BRUSH_HATCH, m_bhComboBox);

}

BEGIN_MESSAGE_MAP(CBrushDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_BRUSH_COLOR, OnBnClickedButtonBrushColor)
	ON_CBN_SELCHANGE(IDC_COMBO_BRUSH_STYLE, OnCbnSelchangeComboBrushStyle)
	ON_CBN_SELCHANGE(IDC_COMBO_BRUSH_HATCH, OnCbnSelchangeComboBrushHatch)
END_MESSAGE_MAP()


void CBrushDialog::OnBnClickedButtonBrushColor()
{
	CColorDialog dlg( m_csColor.GetColor() );

	if ( IDOK == dlg.DoModal() ){
		COLORREF color = dlg.GetColor();
		m_csColor.SetColor( color );
		m_bsComboBox.SetColor( color );
		m_bhComboBox.SetColor( color );
	}
}

void CBrushDialog::OnOK()
{
	UpdateData();

	m_Brush.lbColor = m_csColor.GetColor();
	m_Brush.lbStyle = m_bsComboBox.GetStyle();
	m_Brush.lbHatch = m_bhComboBox.GetHatch();

	CDialog::OnOK();
}

BOOL CBrushDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_bsComboBox.CreateList();
	m_bhComboBox.CreateList();

	m_csColor.SetColor( m_Brush.lbColor );
	m_bsComboBox.SetBrush( m_Brush );
	m_bhComboBox.SetBrush( m_Brush );

	UpdateControls( m_Brush.lbStyle );

	return TRUE;
}

void CBrushDialog::UpdateControls(UINT style)
{
	m_bhComboBox.EnableWindow( style == BS_HATCHED );
	m_csColor.EnableWindow( style != BS_NULL );
	GetDlgItem(IDC_BUTTON_BRUSH_COLOR)->EnableWindow( style != BS_NULL );

}

void CBrushDialog::OnCbnSelchangeComboBrushStyle()
{
	UINT style = m_bsComboBox.GetStyle();
	m_bhComboBox.SetStyle( style );
	UpdateControls(style);
}

void CBrushDialog::OnCbnSelchangeComboBrushHatch()
{
	m_bsComboBox.SetHatch( m_bhComboBox.GetHatch() );
}
