#include "stdafx.h"
#include "WinTILD.h"
#include "TrajectoryProperty.h"
#include "MainFrm.h"
#include "Function.h"
#include "BrushDialog.h"
#include "PenDialog.h"


IMPLEMENT_DYNAMIC(CTrajectoryProperty, CPropertyPage)
CTrajectoryProperty::CTrajectoryProperty()
	: CPropertyPage(CTrajectoryProperty::IDD)
{
	m_Lock = FALSE;
	m_CheckSquare = TRUE;
}

CTrajectoryProperty::~CTrajectoryProperty()
{
}

void CTrajectoryProperty::DoDataExchange(CDataExchange* pDX)
{
	CTrajectoryPlotterOption *pO = GetDocument()->GetTrajectoryPlotterOption();

	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE_TRAJ_PLOTAREA, m_bsPlotArea);
	DDX_Control(pDX, IDC_PICTURE_TRAJ_MARGIN, m_bsMargin);
	DDX_Control(pDX, IDC_PICTURE_TRAJ_FRAME, m_psFrame);
	DDX_Control(pDX, IDC_PICTURE_TRAJ_FROM, m_csFrom);
	DDX_Control(pDX, IDC_PICTURE_TRAJ_TO, m_csTo);
	DDX_Control(pDX, IDC_PICTURE_TRAJ_BOUNDARY, m_csBoundary);
	DDX_Control(pDX, IDC_PICTURE_TRAJ_BG, m_csBackground);
	DDX_Control(pDX, IDC_PICTURE_TRAJ_TEXTBG, m_csBackgroundText);
	DDX_Control(pDX, IDC_PICTURE_TRAJ_LINE1, m_psLine1);
	DDX_Control(pDX, IDC_PICTURE_TRAJ_LINE2, m_psLine2);
	DDX_Control(pDX, IDC_PICTURE_TRAJ_LINE3, m_psLine3);
	DDX_Control(pDX, IDC_PICTURE_TRAJ_LINE4, m_psLine4);
	DDX_Control(pDX, IDC_PICTURE_TRAJ_LINE5, m_psLine5);

	DDX_Width (pDX, IDC_EDIT_TRAJECTORY_WIDTH,  pO->m_Area);
	DDX_Height(pDX, IDC_EDIT_TRAJECTORY_HEIGHT, pO->m_Area);
	DDX_Check (pDX, IDC_CHECK_TRAJECTORY_SQUARE, m_CheckSquare);
	DDX_Brush (pDX, m_bsPlotArea, pO->m_lbPlotArea );
	DDX_Brush (pDX, m_bsMargin, pO->m_lbMargin );
	DDX_Pen   (pDX, m_psFrame, pO->m_lpFrame );
	DDX_Color (pDX, m_csFrom, pO->m_clStart );
	DDX_Color (pDX, m_csTo, pO->m_clEnd );
	DDX_Color (pDX, m_csBoundary, pO->m_clBorderPoint );
	DDX_Color (pDX, m_csBackground, pO->m_clBackground );
	DDX_Color (pDX, m_csBackgroundText, pO->m_clBackgroundText );
	DDX_Pen   (pDX, m_psLine1, pO->m_lpLine1 );
	DDX_Pen   (pDX, m_psLine2, pO->m_lpLine2 );
	DDX_Pen   (pDX, m_psLine3, pO->m_lpLine3 );
	DDX_Pen   (pDX, m_psLine4, pO->m_lpLine4 );
	DDX_Pen   (pDX, m_psLine5, pO->m_lpLine5 );


	DDX_BkMode(pDX, GetDlgItem(IDC_CHECK_TRAJ_BACKGROUND), pO->m_bkMode);
	DDX_BkMode(pDX, GetDlgItem(IDC_CHECK_TRAJ_TEXTBG), pO->m_bkModeText);

	DDX_Status(pDX, GetDlgItem(IDC_CHECK_TRAJ_DRAWSTARTEND), TP_STARTEND, pO->m_Status ); 
	DDX_Status(pDX, GetDlgItem(IDC_CHECK_TRAJ_DRAWFROMTO), TP_WINDOWSTARTEND, pO->m_Status ); 
	DDX_Status(pDX, GetDlgItem(IDC_CHECK_TRAJ_DRAWCOORDINATES), TP_COORDINATES, pO->m_Status ); 
	DDX_Status(pDX, GetDlgItem(IDC_CHECK_TRAJ_DRAWBOUNDARY), TP_BOUNDARY, pO->m_Status ); 
	DDX_Status(pDX, GetDlgItem(IDC_CHECK_TRAJ_DRAWBOUNDARYTEXT), TP_BOUNDARYTEXT, pO->m_Status ); 
}


BEGIN_MESSAGE_MAP(CTrajectoryProperty, CPropertyPage)
	ON_EN_CHANGE(IDC_EDIT_TRAJECTORY_WIDTH, OnEnChangeEditTrajectoryWidth)
	ON_EN_CHANGE(IDC_EDIT_TRAJECTORY_HEIGHT, OnEnChangeEditTrajectoryHeight)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_TRAJ_PLOTAREA, OnBnClickedButtonTrajPlotArea)
	ON_BN_CLICKED(IDC_BUTTON_TRAJ_MARGIN, OnBnClickedButtonTrajMargin)
	ON_BN_CLICKED(IDC_BUTTON_TRAJ_FRAME, OnBnClickedButtonTrajFrame)
	ON_BN_CLICKED(IDC_CHECK_TRAJECTORY_SQUARE, OnBnClickedCheckTrajectorySquare)
	ON_BN_CLICKED(IDC_BUTTON_TRAJ_FROM, OnBnClickedButtonTrajFrom)
	ON_BN_CLICKED(IDC_BUTTON_TRAJ_TO, OnBnClickedButtonTrajTo)
	ON_BN_CLICKED(IDC_BUTTON_TRAJ_BOUNDARY, OnBnClickedButtonTrajBoundary)
	ON_BN_CLICKED(IDC_CHECK_TRAJ_DRAWSTARTEND, OnBnClickedCheckTrajDrawstartend)
	ON_BN_CLICKED(IDC_CHECK_TRAJ_DRAWCOORDINATES, OnBnClickedCheckTrajDrawcoordinates)
	ON_BN_CLICKED(IDC_CHECK_TRAJ_DRAWFROMTO, OnBnClickedCheckTrajDrawfromto)
	ON_BN_CLICKED(IDC_CHECK_TRAJ_DRAWBOUNDARY, OnBnClickedCheckTrajDrawboundary)
	ON_BN_CLICKED(IDC_CHECK_TRAJ_DRAWBOUNDARYTEXT, OnBnClickedCheckTrajDrawboundarytext)
	ON_BN_CLICKED(IDC_BUTTON_TRAJ_BG, OnBnClickedButtonTrajBg)
	ON_BN_CLICKED(IDC_CHECK_TRAJ_BACKGROUND, OnBnClickedCheckTrajBackground)
	ON_BN_CLICKED(IDC_BUTTON_TRAJ_TEXTBG, OnBnClickedButtonTrajTextbg)
	ON_BN_CLICKED(IDC_CHECK_TRAJ_TEXTBG, OnBnClickedCheckTrajTextbg)
	ON_BN_CLICKED(IDC_BUTTON_TRAJ_LINE1, &CTrajectoryProperty::OnBnClickedButtonTrajLine1)
	ON_BN_CLICKED(IDC_BUTTON_TRAJ_LINE2, &CTrajectoryProperty::OnBnClickedButtonTrajLine2)
	ON_BN_CLICKED(IDC_BUTTON_TRAJ_LINE3, &CTrajectoryProperty::OnBnClickedButtonTrajLine3)
	ON_BN_CLICKED(IDC_BUTTON_TRAJ_LINE4, &CTrajectoryProperty::OnBnClickedButtonTrajLine4)
	ON_BN_CLICKED(IDC_BUTTON_TRAJ_LINE5, &CTrajectoryProperty::OnBnClickedButtonTrajLine5)
END_MESSAGE_MAP()


CWinTILDDoc* CTrajectoryProperty::GetDocument()
{
	if (theApp.m_pMainWnd!=NULL){
		return (CWinTILDDoc*)((CMainFrame*)theApp.m_pMainWnd)->GetActiveDocument();
	}
	else {
		return NULL;
	}
}

void CTrajectoryProperty::OnEnChangeEditTrajectoryWidth()
{
	if ( !m_Lock ){
		SetModified();
		if ( m_CheckSquare ){
			m_Lock = TRUE;
			SetDlgItemInt( IDC_EDIT_TRAJECTORY_HEIGHT , GetDlgItemInt( IDC_EDIT_TRAJECTORY_WIDTH ) );
			m_Lock = FALSE;
		}
	}
}

void CTrajectoryProperty::OnEnChangeEditTrajectoryHeight()
{
	if ( !m_Lock ){
		SetModified();
		if ( m_CheckSquare ){
			m_Lock = TRUE;
			SetDlgItemInt( IDC_EDIT_TRAJECTORY_WIDTH , GetDlgItemInt( IDC_EDIT_TRAJECTORY_HEIGHT ) );
			m_Lock = FALSE;
		}
	}
}

void CTrajectoryProperty::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CPropertyPage::OnShowWindow(bShow, nStatus);

	if ( bShow == TRUE ){
		CSpinButtonCtrl *pSpin;

		pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_TRAJECTORY_WIDTH );
		pSpin->SetBuddy( GetDlgItem(IDC_EDIT_TRAJECTORY_WIDTH ) );
		pSpin->SetRange32( 0, 1<<16 );

		pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_TRAJECTORY_HEIGHT );
		pSpin->SetBuddy( GetDlgItem(IDC_EDIT_TRAJECTORY_HEIGHT ) );
		pSpin->SetRange32( 0, 1<<16 );

		UpdateControlBG();
		UpdateControlTextBG();
		UpdateControls( GetDocument()->GetTrajectoryPlotterOption() );
	}
}

void CTrajectoryProperty::UpdateControlBG()
{
	BOOL enable = (((CButton*)GetDlgItem(IDC_CHECK_TRAJ_BACKGROUND))->GetCheck() == BST_CHECKED);
	GetDlgItem(IDC_BUTTON_TRAJ_BG)->EnableWindow(enable);
}

void CTrajectoryProperty::UpdateControlTextBG()
{
	BOOL enable = (((CButton*)GetDlgItem(IDC_CHECK_TRAJ_TEXTBG))->GetCheck() == BST_CHECKED);
	GetDlgItem(IDC_BUTTON_TRAJ_TEXTBG)->EnableWindow(enable);
}

void CTrajectoryProperty::UpdateControls( const CPlotterOption* pO )
{
	GetDlgItem(IDC_CHECK_TRAJ_DRAWBOUNDARYTEXT)->EnableWindow( pO->ExtractStatus(TP_BOUNDARY) );
}

void CTrajectoryProperty::DDX_Width(CDataExchange *pDX,int nIDC, CRect &rc)
{
	if (pDX->m_bSaveAndValidate){
		int width = GetDlgItemInt( nIDC );
		rc.right = rc.left + width;
	}
	else {
		SetDlgItemInt( nIDC,rc.Width() );
	}
}

void CTrajectoryProperty::DDX_Height(CDataExchange *pDX,int nIDC, CRect &rc)
{
	if (pDX->m_bSaveAndValidate){
		int height = GetDlgItemInt( nIDC );
		rc.bottom = rc.top + height;
	}
	else {
		SetDlgItemInt( nIDC,rc.Height() );
	}
}

void CTrajectoryProperty::OnOK()
{
	UpdateData(TRUE);
	GetDocument()->AutoAdjustArea();
	GetDocument()->Redraw();
	CPropertyPage::OnOK();
}

void CTrajectoryProperty::OnColorDlg( CColorStatic& cs )
{
	CColorDialog dlg( cs.GetColor() );
	if ( IDOK == dlg.DoModal() ){
		SetModified();
		cs.SetColor( dlg.GetColor() );
	}
}

void CTrajectoryProperty::OnBrushDlg( CBrushStatic& bs )
{
	CBrushDialog dlg( bs.GetBrush() );
	if ( IDOK == dlg.DoModal() ){
		SetModified();
		bs.SetBrush( dlg.GetBrush() );
	}
}

void CTrajectoryProperty::OnPenDlg( CPenStatic& ps )
{
	CPenDialog dlg( ps.GetPen() );
	if ( IDOK == dlg.DoModal() ){
		SetModified();
		ps.SetPen( dlg.GetPen() );
	}	
}

void CTrajectoryProperty::OnBnClickedButtonTrajPlotArea()
{
	OnBrushDlg( m_bsPlotArea );
}

void CTrajectoryProperty::OnBnClickedButtonTrajMargin()
{
	OnBrushDlg( m_bsMargin );
}

void CTrajectoryProperty::OnBnClickedButtonTrajFrame()
{
	OnPenDlg( m_psFrame );
}

void CTrajectoryProperty::OnBnClickedCheckTrajectorySquare()
{
	m_CheckSquare = (((CButton*)GetDlgItem(IDC_CHECK_TRAJECTORY_SQUARE))->GetCheck() == BST_CHECKED);
}

void CTrajectoryProperty::OnBnClickedButtonTrajFrom()
{
	OnColorDlg( m_csFrom );
}

void CTrajectoryProperty::OnBnClickedButtonTrajTo()
{
	OnColorDlg( m_csTo );
}

void CTrajectoryProperty::OnBnClickedButtonTrajBoundary()
{
	OnColorDlg( m_csBoundary );
}

void CTrajectoryProperty::OnBnClickedCheckTrajDrawstartend()
{
	SetModified();
}

void CTrajectoryProperty::OnBnClickedCheckTrajDrawcoordinates()
{
	SetModified();
}

void CTrajectoryProperty::OnBnClickedCheckTrajDrawfromto()
{
	SetModified();
}

void CTrajectoryProperty::OnBnClickedCheckTrajDrawboundary()
{
	UpdateData(TRUE);
	UpdateControls( GetDocument()->GetTrajectoryPlotterOption() );
	SetModified();
}

void CTrajectoryProperty::OnBnClickedCheckTrajDrawboundarytext()
{
	SetModified();
}

void CTrajectoryProperty::OnBnClickedButtonTrajBg()
{
	OnColorDlg(m_csBackground);
}

void CTrajectoryProperty::OnBnClickedCheckTrajBackground()
{
	UpdateControlBG();
	SetModified();
}

void CTrajectoryProperty::OnBnClickedButtonTrajTextbg()
{
	OnColorDlg(m_csBackgroundText);
}

void CTrajectoryProperty::OnBnClickedCheckTrajTextbg()
{
	UpdateControlTextBG();
	SetModified();
}


void CTrajectoryProperty::OnBnClickedButtonTrajLine1()
{
	OnPenDlg(m_psLine1);
}


void CTrajectoryProperty::OnBnClickedButtonTrajLine2()
{
	OnPenDlg(m_psLine2);
}


void CTrajectoryProperty::OnBnClickedButtonTrajLine3()
{
	OnPenDlg(m_psLine3);
}


void CTrajectoryProperty::OnBnClickedButtonTrajLine4()
{
	OnPenDlg(m_psLine4);
}


void CTrajectoryProperty::OnBnClickedButtonTrajLine5()
{
	OnPenDlg(m_psLine5);
}
