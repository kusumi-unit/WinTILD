#include "stdafx.h"
#include "WinTILD.h"
#include "DivMSDProperty.h"
#include "MainFrm.h"
#include "Function.h"
#include "BrushDialog.h"
#include "PenDialog.h"


IMPLEMENT_DYNAMIC(CDivMSDProperty, CPropertyPage)
CDivMSDProperty::CDivMSDProperty()
	: CPropertyPage(CDivMSDProperty::IDD)
	, m_threshold_color_string(_T(""))
{
	m_Lock = FALSE;
}

CDivMSDProperty::~CDivMSDProperty()
{
}

void CDivMSDProperty::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	CDivMSDPlotterOption* pO = GetDocument()->GetDivMSDPlotterOption();

	DDX_Control(pDX, IDC_PICTURE_DMSD_BASECOLOR, m_csBase);
	DDX_Control(pDX, IDC_PICTURE_DMSD_THRESHOLD, m_csThreshold);
	DDX_Control(pDX, IDC_PICTURE_DMSD_STARTLINE, m_psStartLine);
	DDX_Control(pDX, IDC_PICTURE_DMSD_ENDLINE,   m_psEndLine);
	DDX_Control(pDX, IDC_PICTURE_DMSD_BOUNDARY,  m_psBoundary);
	DDX_Control(pDX, IDC_PICTURE_DMSD_BG,		 m_csBackground);
	DDX_Control(pDX, IDC_PICTURE_DMSD_TEXTBG,	 m_csBackgroundText);
	DDX_Control(pDX, IDC_PICTURE_DMSD_MARGIN,	 m_bsMargin);
	DDX_Control(pDX, IDC_PICTURE_DMSD_FRAME,	 m_psFrame);

	DDX_Size (pDX, IDC_EDIT_DIVMSD_SIZE ,	pO->m_Area );
	DDX_Text (pDX, IDC_EDIT_DIVMSD_MAGNIFY, pO->m_Magnify );
	DDX_Check(pDX, IDC_RADIO_USESIZE,		pO->m_FixSize);
	DDX_Color(pDX, m_csThreshold,			pO->m_clThreshold);
	DDX_Color(pDX, m_csBase,				pO->m_clBase);
	DDX_Pen	 (pDX, m_psStartLine,			pO->m_lpWindowStart);
	DDX_Pen	 (pDX, m_psEndLine,				pO->m_lpWindowEnd);
	DDX_Pen  (pDX, m_psBoundary,			pO->m_lpBorder);
	DDX_Pen	 (pDX, m_psFrame,				pO->m_lpFrame );
	DDX_Color(pDX, m_csBackground,			pO->m_clBackground);
	DDX_Color(pDX, m_csBackgroundText,		pO->m_clBackgroundText);
	DDX_Brush(pDX, m_bsMargin, pO->m_lbMargin);
	DDX_BkMode(pDX, GetDlgItem(IDC_CHECK_DMSD_BACKGROUND), pO->m_bkMode);
	DDX_BkMode(pDX, GetDlgItem(IDC_CHECK_DMSD_TEXTBG), pO->m_bkModeText);

	DDX_Status(pDX, GetDlgItem(IDC_CHECK_DMSD_DRAWFROMTO),		DP_WINDOW,		pO->m_Status);
	DDX_Status(pDX, GetDlgItem(IDC_CHECK_DMSD_DRAWFROMTOFRAME), DP_WINDOWNUMBER,pO->m_Status);
	DDX_Status(pDX, GetDlgItem(IDC_CHECK_DMSD_DRAWLINEMETHOD),	DP_WINDOWMETHOD,pO->m_Status);
	DDX_Status(pDX, GetDlgItem(IDC_CHECK_DMSD_DRAWBOUNDARY),	DP_BORDER,		pO->m_Status);
	DDX_Status(pDX, GetDlgItem(IDC_CHECK_DMSD_DRAWBOUNDARYFRAME),DP_BORDERNUMBER,pO->m_Status);
	DDX_Status(pDX, GetDlgItem(IDC_CHECK_DMSD_DRAWMAGNIFY),		DP_MAGNIFY,		pO->m_Status);
	DDX_Status(pDX, GetDlgItem(IDC_CHECK_DMSD_DRAWSCALE),		DP_SCALE,		pO->m_Status);

	GetDocument()->SetDrawBoundaries(pO->m_Status & DP_BORDER);

	//--- Set the colors for the threshold value ----
	DDX_Slider( pDX, IDC_SLIDER_THRESHOLD_COLOR, pO->m_threshold_color);
	DDX_Control(pDX, IDC_SLIDER_THRESHOLD_COLOR, m_threshold_color_control);
	DDX_Control(pDX, IDC_PICTURE_THRESHOLD_COLOR, m_picture_threshold_color);
	DDX_Text(pDX, IDC_EDIT2, m_threshold_color_string);
	DDV_MaxChars(pDX, m_threshold_color_string, 5);
}


BEGIN_MESSAGE_MAP(CDivMSDProperty, CPropertyPage)
	ON_WM_SHOWWINDOW()
	ON_EN_CHANGE(IDC_EDIT_DIVMSD_SIZE, OnEnChangeEditDivmsdSize)
	ON_BN_CLICKED(IDC_RADIO_USESIZE, OnBnClickedRadioUsesize)
	ON_BN_CLICKED(IDC_RADIO_USEMAGNIFY, OnBnClickedRadioUsemagnify)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DIVMSD_MAGNIFY, OnDeltaposSpinDivmsdMagnify)
	ON_EN_CHANGE(IDC_EDIT_DIVMSD_MAGNIFY, OnEnChangeEditDivmsdMagnify)
	ON_BN_CLICKED(IDC_BUTTON_DMSD_BASECOLOR, OnBnClickedButtonDmsdBasecolor)
	ON_BN_CLICKED(IDC_BUTTON_DMSD_THRESHOLD, OnBnClickedButtonDmsdThreshold)
	ON_BN_CLICKED(IDC_BUTTON_DMSD_BOUNDARY, OnBnClickedButtonDmsdBoundary)
	ON_BN_CLICKED(IDC_BUTTON_DMSD_ENDLINE, OnBnClickedButtonDmsdEndline)
	ON_BN_CLICKED(IDC_BUTTON_DMSD_STARTLINE, OnBnClickedButtonDmsdStartline)
	ON_BN_CLICKED(IDC_CHECK_DMSD_DRAWFROMTO, OnBnClickedCheckDmsdDrawfromto)
	ON_BN_CLICKED(IDC_CHECK_DMSD_DRAWLINEMETHOD, OnBnClickedCheckDmsdDrawlinemethod)
	ON_BN_CLICKED(IDC_CHECK_DMSD_DRAWFROMTOFRAME, OnBnClickedCheckDmsdDrawfromtoframe)
	ON_BN_CLICKED(IDC_CHECK_DMSD_DRAWBOUNDARY, OnBnClickedCheckDmsdDrawboundary)
	ON_BN_CLICKED(IDC_CHECK_DMSD_DRAWBOUNDARYFRAME, OnBnClickedCheckDmsdDrawboundaryframe)
	ON_BN_CLICKED(IDC_CHECK_DMSD_DRAWMAGNIFY, OnBnClickedCheckDmsdDrawmagnify)
	ON_BN_CLICKED(IDC_CHECK_DMSD_DRAWSCALE, OnBnClickedCheckDmsdDrawscale)
	ON_BN_CLICKED(IDC_BUTTON_DMSD_BG, OnBnClickedButtonDmsdBg)
	ON_BN_CLICKED(IDC_BUTTON_DMSD_TEXTBG, OnBnClickedButtonDmsdTextbg)
	ON_BN_CLICKED(IDC_CHECK_DMSD_BACKGROUND, OnBnClickedCheckDmsdBackground)
	ON_BN_CLICKED(IDC_CHECK_DMSD_TEXTBG, OnBnClickedCheckDmsdTextbg)
	ON_BN_CLICKED(IDC_BUTTON_DMSD_MARGIN, OnBnClickedButtonDmsdMargin)
	ON_BN_CLICKED(IDC_BUTTON_DMSD_FRAME, OnBnClickedButtonDmsdFrame)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

BOOL CDivMSDProperty::OnInitDialog(){
	CPropertyPage::OnInitDialog();
	m_threshold_color_control.SetRange(0x000000, 0x0000FF, true);
	SetPictureThresholdColor(GetDocument() -> GetDivMSDPlotterOption() -> m_threshold_color);
	m_threshold_color_string.Format("%d", GetDocument() -> GetDivMSDPlotterOption() -> m_threshold_color);
	UpdateData(FALSE);
	return true;
}


CWinTILDDoc* CDivMSDProperty::GetDocument()
{
	if (theApp.m_pMainWnd!=NULL){
		return (CWinTILDDoc*)((CMainFrame*)theApp.m_pMainWnd)->GetActiveDocument();
	}
	else {
		return NULL;
	}
}

void CDivMSDProperty::Update()
{
	CWinTILDDoc *pDoc = GetDocument();
	if (pDoc!=NULL){
	}

}

void CDivMSDProperty::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CPropertyPage::OnShowWindow(bShow, nStatus);

	if (bShow){
		CPropertyPage::OnShowWindow(bShow, nStatus);
		CSpinButtonCtrl *pSpin;

		pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_DIVMSD_MAGNIFY);
		pSpin->SetRange32(0,1<<16);

		pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_DIVMSD_SIZE);
		pSpin->SetRange32(0,1<<16);
		pSpin->SetBuddy( GetDlgItem(IDC_EDIT_DIVMSD_SIZE) );

		UpdateControl(IDC_CHECK_DMSD_DRAWFROMTO, IDC_CHECK_DMSD_DRAWFROMTOFRAME);
		UpdateControl(IDC_CHECK_DMSD_DRAWBOUNDARY, IDC_CHECK_DMSD_DRAWBOUNDARYFRAME);
		UpdateControl(IDC_CHECK_DMSD_BACKGROUND, IDC_BUTTON_DMSD_BG);
		UpdateControl(IDC_CHECK_DMSD_TEXTBG, IDC_BUTTON_DMSD_TEXTBG);
	}
}

void CDivMSDProperty::OnEnChangeEditDivmsdSize()
{
	SetModified();
}

void CDivMSDProperty::OnEnChangeEditDivmsdMagnify()
{
	SetModified();
}

void CDivMSDProperty::OnBnClickedRadioUsesize()
{
	SetModified();
}

void CDivMSDProperty::OnBnClickedRadioUsemagnify()
{
	SetModified();
}

void CDivMSDProperty::SetDlgItemDouble(int id,double d)
{
	char buff[64];
	SetDlgItemText( id, gcvt(d,3,buff) );
}

double CDivMSDProperty::GetDlgItemDouble(int id)
{
	static CString txt;
	GetDlgItemText( id, txt );
	return atof( txt );
}

void CDivMSDProperty::OnDeltaposSpinDivmsdMagnify(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	double v = GetDlgItemDouble(IDC_EDIT_DIVMSD_MAGNIFY);
	v*=(1.0 + (double)pNMUpDown->iDelta/100.0 );
	if ( v < 0 )v = 0;
	SetDlgItemDouble(IDC_EDIT_DIVMSD_MAGNIFY, v );
	*pResult = 0;
}


void CDivMSDProperty::OnOK()
{
	UpdateData(TRUE);
	GetDocument()->GetDivMSDPlotterOption()->MakePalette();
	GetDocument()->Redraw();
	CPropertyPage::OnOK();
}

void CDivMSDProperty::DDX_Size(CDataExchange *pDX,int /*nIDC*/, CRect &rc)
{
	if (pDX->m_bSaveAndValidate){
		int size = GetDlgItemInt( IDC_EDIT_DIVMSD_SIZE );
		rc.right = rc.left + size;
		rc.bottom = rc.top + size;
	}
	else {
		SetDlgItemInt( IDC_EDIT_DIVMSD_SIZE,rc.Width() );
	}
}

void CDivMSDProperty::DDX_Check(CDataExchange *pDX, int /*nIDC*/, BOOL &value)
{
	if (pDX->m_bSaveAndValidate){
		value = ( ((CButton*)GetDlgItem(IDC_RADIO_USESIZE))->GetCheck() == BST_CHECKED );
	}
	else {
		((CButton*)GetDlgItem(IDC_RADIO_USESIZE))->SetCheck( value?BST_CHECKED:BST_UNCHECKED);
		((CButton*)GetDlgItem(IDC_RADIO_USEMAGNIFY))->SetCheck( value?BST_UNCHECKED:BST_CHECKED);
	}
}

void CDivMSDProperty::OnColorDlg( CColorStatic& cs )
{
	COLORREF color = cs.GetColor();
	CColorDialog dlg( color );
	if ( IDOK == dlg.DoModal() ){
		SetModified();
		cs.SetColor( dlg.GetColor() );
	}
}

void CDivMSDProperty::OnBrushDlg( CBrushStatic& bs )
{
	CBrushDialog dlg( bs.GetBrush() );
	if ( IDOK == dlg.DoModal() ){
		SetModified();
		bs.SetBrush( dlg.GetBrush() );
	}
}

void CDivMSDProperty::OnPenDlg( CPenStatic& ps )
{
	CPenDialog dlg( ps.GetPen() );
	if ( IDOK == dlg.DoModal() ){
		SetModified();
		ps.SetPen( dlg.GetPen() );
	}	
}

void CDivMSDProperty::UpdateControl(int nIDParent, int nIDChile)
{
	GetDlgItem(nIDChile)->EnableWindow( ((CButton*)GetDlgItem(nIDParent))->GetCheck()==BST_CHECKED );
}

void CDivMSDProperty::OnBnClickedButtonDmsdBasecolor()
{
	OnColorDlg( m_csBase );
}

void CDivMSDProperty::OnBnClickedButtonDmsdThreshold()
{
	OnColorDlg( m_csThreshold );
}

void CDivMSDProperty::OnBnClickedButtonDmsdBoundary()
{
	OnPenDlg( m_psBoundary );
}

void CDivMSDProperty::OnBnClickedButtonDmsdEndline()
{
	OnPenDlg( m_psEndLine );
}

void CDivMSDProperty::OnBnClickedButtonDmsdStartline()
{
	OnPenDlg( m_psStartLine );
}

void CDivMSDProperty::OnBnClickedCheckDmsdDrawfromto()
{
	UpdateControl(IDC_CHECK_DMSD_DRAWFROMTO, IDC_CHECK_DMSD_DRAWFROMTOFRAME);
	SetModified();
}

void CDivMSDProperty::OnBnClickedCheckDmsdDrawlinemethod()
{
	SetModified();
}

void CDivMSDProperty::OnBnClickedCheckDmsdDrawfromtoframe()
{
	SetModified();
}

void CDivMSDProperty::OnBnClickedCheckDmsdDrawboundary()
{
	UpdateControl(IDC_CHECK_DMSD_DRAWBOUNDARY, IDC_CHECK_DMSD_DRAWBOUNDARYFRAME);
	SetModified();
}

void CDivMSDProperty::OnBnClickedCheckDmsdDrawboundaryframe()
{
	SetModified();
}

void CDivMSDProperty::OnBnClickedCheckDmsdDrawmagnify()
{
	SetModified();
}

void CDivMSDProperty::OnBnClickedCheckDmsdDrawscale()
{
	SetModified();
}

void CDivMSDProperty::OnBnClickedButtonDmsdBg()
{
	OnColorDlg( m_csBackground );
}

void CDivMSDProperty::OnBnClickedButtonDmsdTextbg()
{
	OnColorDlg( m_csBackgroundText );
}

void CDivMSDProperty::OnBnClickedCheckDmsdBackground()
{
	UpdateControl(IDC_CHECK_DMSD_BACKGROUND, IDC_BUTTON_DMSD_BG);
	SetModified();
}

void CDivMSDProperty::OnBnClickedCheckDmsdTextbg()
{
	UpdateControl(IDC_CHECK_DMSD_TEXTBG, IDC_BUTTON_DMSD_TEXTBG);
	SetModified();
}

void CDivMSDProperty::OnBnClickedButtonDmsdMargin()
{
	OnBrushDlg( m_bsMargin );
}

void CDivMSDProperty::OnBnClickedButtonDmsdFrame()
{
	OnPenDlg( m_psFrame );
}


void CDivMSDProperty::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    int pageSize;
    int min, max;
    int threshold_color = GetDocument()->GetDivMSDPlotterOption()->m_threshold_color;

    // Process only when the slider for the threshold color is scrolled
    if (*pScrollBar != m_threshold_color_control) return;
    
	((CSliderCtrl*) pScrollBar)->SetRange(0x000000, 0x0000FF);		//---	define range
	((CSliderCtrl*) pScrollBar)->SetPageSize(16);
	switch (nSBCode)
	{
		// Drag the slider
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			threshold_color = nPos;
		break;
		// Click the left side
		case SB_PAGELEFT:
			pageSize = ((CSliderCtrl*)pScrollBar)->GetPageSize();
			((CSliderCtrl*)pScrollBar)->GetRange(min, max);
		    
			threshold_color = (threshold_color -pageSize > min) 
				? threshold_color -pageSize : min;
			break;
		// Click the right side
		case SB_PAGERIGHT:
			pageSize = ((CSliderCtrl*)pScrollBar)->GetPageSize();
			((CSliderCtrl*)pScrollBar)->GetRange(min, max);
		    
			threshold_color = (threshold_color +pageSize < max) 
				? threshold_color +pageSize : max;
			break;
		default:
			break;
		}

    SetPictureThresholdColor(threshold_color);

	GetDocument() -> GetDivMSDPlotterOption() -> m_threshold_color = threshold_color;
	m_threshold_color_string.Format("%d", GetDocument() -> GetDivMSDPlotterOption() -> m_threshold_color);
	UpdateData(FALSE);
	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
	return;
}

void CDivMSDProperty::SetPictureThresholdColor(int threshold_color_value){
	threshold_color_value = threshold_color_value & 0x0000FF;	//	value should be less than 0x0000FF
	COLORREF color = 0x010101*threshold_color_value;
	m_picture_threshold_color.SetColor(color);
}

