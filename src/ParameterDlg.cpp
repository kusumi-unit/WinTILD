#include "stdafx.h"
#include "WinTILD.h"
#include "WinTILDDoc.h"
#include "ParameterDlg.h"

IMPLEMENT_DYNAMIC(CParameterDlg, CDialog)
CParameterDlg::CParameterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CParameterDlg::IDD, pParent)
{
	m_Lock = FALSE;
}

CParameterDlg::~CParameterDlg()
{
}
void CParameterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_DRAW_BOUNDARIES, m_draw_boundaries);
}

BEGIN_MESSAGE_MAP(CParameterDlg, CDialog)
	ON_EN_CHANGE(IDC_EDIT_THRESHOLD, OnEnChangeEditThreshold)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_THRESHOLD, OnDeltaposSpinThreshold)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_THRESHOLD_FINE, OnDeltaposSpinThresholdFine)
	ON_EN_CHANGE(IDC_EDIT_WFROM, OnEnChangeEditWfrom)
	ON_EN_CHANGE(IDC_EDIT_WTO, OnEnChangeEditWto)
	ON_WM_HSCROLL()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_WFROM, OnDeltaposSpinWfrom)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_WTO, OnDeltaposSpinWto)
	ON_EN_CHANGE(IDC_EDIT_RATIO, OnEnChangeEditRatio)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_RATIO, OnDeltaposSpinRatio)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_RATIO_FINE, OnDeltaposSpinRatioFine)
	ON_EN_CHANGE(IDC_EDIT_SKIP, OnEnChangeEditSkip)
	ON_BN_CLICKED(IDC_CHECK_INTERACTIVE, OnBnClickedCheckInteractive)
//	ON_BN_CLICKED(IDC_CHECK_USEAVERAGE, OnBnClickedCheckUseaverage)
//	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_P, OnDeltaposSpinP)
	ON_EN_CHANGE(IDC_EDIT_CALCAVEFROM, OnEnChangeEditCalcavefrom)
	ON_EN_CHANGE(IDC_EDIT_CALCAVETO, OnEnChangeEditCalcaveto)
//	ON_EN_CHANGE(IDC_EDIT_P, OnEnChangeEditP)
//	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_P_FINE, OnDeltaposSpinPFine)
	ON_EN_CHANGE(IDC_EDIT_MAXIMUM, &CParameterDlg::OnEnChangeEditMaximum)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MAXIMUM, &CParameterDlg::OnDeltaposSpinMaximum)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MAXIMUM_FINE, &CParameterDlg::OnDeltaposSpinMaximumFine)
	ON_BN_CLICKED(IDC_CHECK_DRAW_BOUNDARIES, &CParameterDlg::OnBnClickedCheckDrawBoundaries)	
END_MESSAGE_MAP()


CWinTILDDoc* CParameterDlg::GetDocument(void)
{
	if (theApp.m_pMainWnd==NULL)
		return NULL;
	return (CWinTILDDoc*)((CFrameWnd*)theApp.m_pMainWnd)->GetActiveDocument();
}
double CParameterDlg::GetDlgItemDouble(int id)
{
	CString str;
	GetDlgItemText(id,str);
	return atof( str );
}
void CParameterDlg::SetDlgItemDouble(int id, double v)
{
	char buffer[64];
	gcvt(v,5,buffer);
	SetDlgItemText(id,buffer);
}

BOOL CParameterDlg::Create( CWnd* pParentWnd )
{
	return CDialog::Create(IDD, pParentWnd);
}


BOOL CParameterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CEdit *pEdit;
	CSpinButtonCtrl *pSpin;

	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_WFROM);
	pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_WFROM);
	pSpin->SetRange(0,0);
	pSpin->SetBuddy(pEdit);

	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_WTO);
	pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_WTO);
	pSpin->SetRange(0,0);
	pSpin->SetBuddy(pEdit);

	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_THRESHOLD);
	pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_THRESHOLD);
	pSpin->SetRange(0,100);
	pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_THRESHOLD_FINE);
	pSpin->SetRange(0,100);

	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_RATIO);
	pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_RATIO);
	pSpin->SetRange(0,100);
	pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_RATIO_FINE);
	pSpin->SetRange(0,100);

	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_SKIP);
	pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_SKIP);
	pSpin->SetBuddy( pEdit );
	pSpin->SetRange(0,0);

	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_CALCAVEFROM);
	pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_CALCAVEFROM);
	pSpin->SetBuddy( pEdit );
	pSpin->SetRange(0,0);

	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_CALCAVETO);
	pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_CALCAVETO);
	pSpin->SetBuddy( pEdit );
	pSpin->SetRange(0,0);

	return TRUE;
}


void CParameterDlg::SetThreshold(double threshold)
{
	char buffer[32];
	SetDlgItemText( IDC_EDIT_THRESHOLD, gcvt(threshold,5,buffer) );
}

void CParameterDlg::SetMaximum(double maximum){
	char buffer[32];
	SetDlgItemText( IDC_EDIT_MAXIMUM, gcvt(maximum,5,buffer) );
}

void CParameterDlg::SetRatio(double ratio)
{
	char buffer[32];
	SetDlgItemText( IDC_EDIT_RATIO, gcvt(ratio*100,5,buffer) );
}

void CParameterDlg::SetSkip(int frames)
{
	SetDlgItemInt( IDC_EDIT_SKIP, frames );
}

void CParameterDlg::SetWindow(int start, int end)
{
	char buffer[64];	
	SetDlgItemText( IDC_EDIT_WFROM, itoa(start,buffer,10) );
	SetDlgItemText( IDC_EDIT_WTO  , itoa(end,buffer,10) );

	CSpinButtonCtrl *pSpin;

	pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_WFROM);
	pSpin->SetPos(start);

	pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_WTO);
	pSpin->SetPos(end);

	CSliderCtrl *pSlider;

	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_WFROM);
	pSlider->SetPos(start);

	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_WTO);
	pSlider->SetPos(end);
}

void CParameterDlg::SetAverageFrom(int from)
{
	char buffer[64];
	SetDlgItemText( IDC_EDIT_CALCAVEFROM, itoa(from,buffer,10) );
}

void CParameterDlg::SetAverageTo(int to)
{
	char buffer[64];
	SetDlgItemText( IDC_EDIT_CALCAVETO, itoa(to,buffer,10) );
}

void CParameterDlg::SetWindowRange(int start, int end)
{
	CSpinButtonCtrl *pSpin;

	pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_WFROM);
	pSpin->SetRange32(start,end);
	pSpin->Invalidate(FALSE);

	pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_WTO);
	pSpin->SetRange32(start,end);
	pSpin->Invalidate(FALSE);

	pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_SKIP);
	pSpin->SetRange32(start,end);
	pSpin->Invalidate(FALSE);

	CSliderCtrl *pSlider;

	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_WFROM);
	pSlider->SetRange(start,end);
	pSlider->Invalidate(FALSE);

	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_WTO);
	pSlider->SetRange(start,end);
	pSlider->Invalidate(FALSE);

	pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_CALCAVEFROM);
	pSpin->SetRange32(start,end);
	pSpin->Invalidate(FALSE);

	pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_CALCAVETO);
	pSpin->SetRange32(start,end);
	pSpin->Invalidate(FALSE);
}

void CParameterDlg::SetSmoothing(int smoothing) {
    smoothing_ = smoothing;
}


void CParameterDlg::OnEnChangeEditThreshold()
{
	if ( !Locking() ){
		CWinTILDDoc* pDoc = GetDocument();
		if (pDoc != NULL){
			pDoc->SetThreshold( GetDlgItemDouble(IDC_EDIT_THRESHOLD));
		}
	}
}

void CParameterDlg::OnEnChangeEditMaximum()
{
	if ( !Locking() ){
		CWinTILDDoc* pDoc = GetDocument();
		if (pDoc != NULL){
			pDoc->SetMaximum( GetDlgItemDouble(IDC_EDIT_MAXIMUM));
		}
	}
}

void CParameterDlg::OnEnChangeEditWfrom()
{
	if ( !Locking() ){
		CWinTILDDoc* pDoc = GetDocument();
		if (pDoc != NULL){
			pDoc->SetWindowStart( GetDlgItemInt(IDC_EDIT_WFROM) );
		}
	}
}
void CParameterDlg::OnEnChangeEditWto()
{
	if ( !Locking() ){
		CWinTILDDoc* pDoc = GetDocument();
		if (pDoc != NULL){
			pDoc->SetWindowEnd( GetDlgItemInt(IDC_EDIT_WTO) );
		}
	}
}
void CParameterDlg::OnEnChangeEditRatio()
{
	if ( !Locking() ){
		CWinTILDDoc* pDoc = GetDocument();
		if (pDoc != NULL){
			pDoc->SetRatio( GetDlgItemDouble(IDC_EDIT_RATIO)/100.0 );
		}
	}
}
void CParameterDlg::OnEnChangeEditSkip()
{
	if ( !Locking() ){
		CWinTILDDoc* pDoc = GetDocument();
		if (pDoc != NULL){
			pDoc->SetSkip( GetDlgItemInt( IDC_EDIT_SKIP ) );
		}
	}
}

void CParameterDlg::OnEnChangeEditCalcavefrom()
{
	if ( !Locking() ){
		CWinTILDDoc* pDoc = GetDocument();
		if (pDoc != NULL){
			pDoc->SetAverageFrom( GetDlgItemInt( IDC_EDIT_CALCAVEFROM ) );
		}
	}
}
void CParameterDlg::OnEnChangeEditCalcaveto()
{
	if ( !Locking() ){
		CWinTILDDoc* pDoc = GetDocument();
		if (pDoc != NULL){
			pDoc->SetAverageTo( GetDlgItemInt( IDC_EDIT_CALCAVETO ) );
		}
	}
}


const double thresholdDelta = 0.1;
void CParameterDlg::OnDeltaposSpinThreshold(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	DeltaDeltaControl(pNMUpDown, IDC_EDIT_THRESHOLD, thresholdDelta, 100.0, 0);

	*pResult = 0;
}

void CParameterDlg::OnDeltaposSpinThresholdFine(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	DeltaDeltaControl(pNMUpDown, IDC_EDIT_THRESHOLD, thresholdDelta*0.01, 100.0, 0);

	*pResult = 0;
}

const double maximumDelta = 1;
void CParameterDlg::OnDeltaposSpinMaximum(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	DeltaDeltaControl(pNMUpDown, IDC_EDIT_MAXIMUM, -maximumDelta, 100.0, 0);

	*pResult = 0;
}

void CParameterDlg::OnDeltaposSpinMaximumFine(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	DeltaDeltaControl(pNMUpDown, IDC_EDIT_MAXIMUM, -maximumDelta*0.01, 100.0, 0);

	*pResult = 0;
}


CParameter* CParameterDlg::GetParameter(void)
{
	static CParameter p;
	p.SetThreshold( GetDlgItemDouble(IDC_EDIT_THRESHOLD));
	p.SetMaximum(   GetDlgItemDouble(IDC_EDIT_MAXIMUM)  );
	p.SetRatio( GetDlgItemDouble(IDC_EDIT_RATIO)*0.01 );
	p.SetWindowStart( GetDlgItemInt(IDC_EDIT_WFROM) );
	p.SetWindowEnd( GetDlgItemInt(IDC_EDIT_WTO) );
	p.SetSkip( GetDlgItemInt(IDC_EDIT_SKIP) );
	p.SetSmoothing(smoothing_);
	p.SetAverageFrom( GetDlgItemInt(IDC_EDIT_CALCAVEFROM) );
	p.SetAverageTo( GetDlgItemInt( IDC_EDIT_CALCAVETO) );
	return &p;
}

void CParameterDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CSliderCtrl* pSlider;
	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_WFROM);
	if ( (void*)pSlider == (void*)pScrollBar ){
		SetDlgItemInt( IDC_EDIT_WFROM, pSlider->GetPos() );
	}
	
	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_WTO);
	if ( (void*)pSlider == (void*)pScrollBar ){
		SetDlgItemInt( IDC_EDIT_WTO, pSlider->GetPos() );
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CParameterDlg::OnDeltaposSpinWfrom(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	CSliderCtrl *pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_WFROM);
	pSlider->SetPos( pNMUpDown->iPos + pNMUpDown->iDelta );

	*pResult = 0;
}

void CParameterDlg::OnDeltaposSpinWto(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	CSliderCtrl *pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_WTO);
	pSlider->SetPos( pNMUpDown->iPos + pNMUpDown->iDelta );

	*pResult = 0;
}


void CParameterDlg::OnDeltaposSpinRatio(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	DeltaDeltaControl(pNMUpDown, IDC_EDIT_RATIO, 1.0, 100.0, 0);

	*pResult = 0;
}

void CParameterDlg::OnDeltaposSpinRatioFine(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	DeltaDeltaControl(pNMUpDown, IDC_EDIT_RATIO, 0.1, 100.0, 0);

	*pResult = 0;
}

void CParameterDlg::DeltaDeltaControl(LPNMUPDOWN pNMUpDown, int id, double delta, double upperlimit, double lowerlimit)
{
	double v = GetDlgItemDouble(id);
	v += pNMUpDown->iDelta*delta;
	if (v>upperlimit)
		v=upperlimit;
	else if (v<lowerlimit)
		v=lowerlimit;
	SetDlgItemDouble(id,v);
}


void CParameterDlg::OnBnClickedCheckInteractive()
{
	CWinTILDDoc* pDoc = GetDocument();
	if ( pDoc != NULL ){
		BOOL interactive = ( BST_CHECKED == ((CButton*)GetDlgItem( IDC_CHECK_INTERACTIVE ))->GetCheck() );
		pDoc->SetInteractiveUpdate( interactive );
	}
}

void CParameterDlg::OnBnClickedCheckDrawBoundaries()
{
	CWinTILDDoc* pDoc = GetDocument();
	if ( pDoc != NULL ){
		BOOL draw_boundaries = ( BST_CHECKED == ((CButton*)GetDlgItem( IDC_CHECK_DRAW_BOUNDARIES ))->GetCheck() );
		SetDrawBoundaries(draw_boundaries);
		pDoc->SetDrawBoundaries( draw_boundaries );
	}
}

void CParameterDlg::SetDrawBoundaries(BOOL drawboundaries){
	m_draw_boundaries.SetCheck(drawboundaries);
	UpdateData();
}
	
void CParameterDlg::UpdateControlForCalcMax(BOOL calc)
{
	GetDlgItem(IDC_EDIT_CALCAVEFROM)->EnableWindow(calc);
	GetDlgItem(IDC_SPIN_CALCAVEFROM)->EnableWindow(calc);
	GetDlgItem(IDC_EDIT_CALCAVETO)->EnableWindow(calc);
	GetDlgItem(IDC_SPIN_CALCAVETO)->EnableWindow(calc);
	GetDlgItem(IDC_EDIT_P)->EnableWindow(calc);
	GetDlgItem(IDC_SPIN_P)->EnableWindow(calc);
	GetDlgItem(IDC_SPIN_P_FINE)->EnableWindow(calc);
}
