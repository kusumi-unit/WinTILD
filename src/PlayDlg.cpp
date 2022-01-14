#include "stdafx.h"
#include "WinTILD.h"
#include "PlayDlg.h"
#include "afxdialogex.h"

#include <algorithm>


IMPLEMENT_DYNAMIC(CPlayDlg, CDialog)

CPlayDlg::CPlayDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPlayDlg::IDD, pParent), m_isLButtonDowned(FALSE), mScale(3.0)
{
}

CPlayDlg::~CPlayDlg()
{
}

void CPlayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPlayDlg, CDialog)
	ON_NOTIFY(UDN_DELTAPOS, IDC_PLAY_SPIN_FPS, &CPlayDlg::OnDeltaposPlaySpinFps)
	ON_NOTIFY(UDN_DELTAPOS, IDC_PLAY_SPIN_CURRENTFRAME, &CPlayDlg::OnDeltaposPlaySpinCurrentframe)
	ON_WM_HSCROLL()
	ON_WM_DRAWITEM()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_PLAY_BUTTON_PLAY, &CPlayDlg::OnBnClickedPlayButtonPlay)
	ON_BN_CLICKED(IDC_PLAY_BUTTON_STOP, &CPlayDlg::OnBnClickedPlayButtonStop)
	ON_BN_CLICKED(IDC_PLAY_BUTTON_PLAYBACKWARD, &CPlayDlg::OnBnClickedPlayButtonPlaybackward)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


BOOL CPlayDlg::Create( CWnd* pParentWnd )
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CPlayDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_frameStart = 0;
	m_frameEnd = 100;

	CEdit* pEdit;
	CSpinButtonCtrl* pSpin;
	pEdit = (CEdit*)GetDlgItem(IDC_PLAY_EDIT_CURRENTFRAME);
	pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_PLAY_SPIN_CURRENTFRAME);
	pSpin->SetRange(static_cast<short>(m_frameStart), static_cast<short>(m_frameEnd));
	pSpin->SetBuddy(pEdit);

	pEdit = (CEdit*)GetDlgItem(IDC_PLAY_EDIT_FPS);
	pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_PLAY_SPIN_FPS);
	pSpin->SetRange(FPS_MIN, FPS_MAX);
	pSpin->SetBuddy(pEdit);
	SetDlgItemInt(IDC_PLAY_EDIT_FPS, FPS_DEFAULT);

	CSliderCtrl* pSlider;
	pSlider = (CSliderCtrl*)GetDlgItem(IDC_PLAY_SLIDER);
	pSlider->SetParent(this);
	pSlider->SetRange(m_frameStart, m_frameEnd);
	pSlider->SetPos(m_frameStart);
	pSlider->Invalidate();

	pSlider = (CSliderCtrl*)GetDlgItem(IDC_PLAY_SLIDER_SCALE);
	pSlider->SetParent(this);
	pSlider->SetRange(1, MAX_SCALE);
	pSlider->SetPos(static_cast<int>(MAX_SCALE - mScale + 1));
	pSlider->Invalidate();

	


	GetDlgItem(IDC_PLAY_VIEW)->GetWindowRect(&mPreviewRect);
	ScreenToClient(&mPreviewRect);
	mPreviewRect.bottom -= PREVIEW_MARGIN;
	mPreviewRect.top += PREVIEW_MARGIN;
	mPreviewRect.left += PREVIEW_MARGIN;
	mPreviewRect.right -= PREVIEW_MARGIN;

	SetDlgItemInt(IDC_PLAY_TEXT_SCALE, static_cast<UINT>(mScale));

	return TRUE;
}

void CPlayDlg::SetWindow(int start, int end)
{
	m_frameStart = start;
	m_frameEnd = end;
	CSpinButtonCtrl* pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_PLAY_SPIN_CURRENTFRAME);
	pSpin->SetRange32(start, end);
	pSpin->SetPos(start);
	pSpin->Invalidate();
	SetDlgItemInt(IDC_PLAY_EDIT_CURRENTFRAME, start);
	SetDlgItemInt(IDC_PLAY_EDIT_ENDFRAME, end);

	CSliderCtrl* pSlider = (CSliderCtrl*)GetDlgItem(IDC_PLAY_SLIDER);
	pSlider->SetRange(start, end);
	pSlider->SetPos(start);
	pSlider->Invalidate();

	setCurrentFrame(start);
}


void CPlayDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CSliderCtrl* pSlider;
	pSlider = (CSliderCtrl*)GetDlgItem(IDC_PLAY_SLIDER);
	if ( (void*)pSlider == (void*)pScrollBar ){
		setCurrentFrame(pSlider->GetPos());
	}
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CPlayDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	CSliderCtrl* pSlider = (CSliderCtrl*)GetDlgItem(IDC_PLAY_SLIDER_SCALE);
	if( (void*)pSlider == (void*)pScrollBar) {
		float beforeScale = mScale;
		mScale = static_cast<float>(MAX_SCALE - pSlider->GetPos() + 1);
		m_previewDelta.x = static_cast<LONG>(m_previewDelta.x / beforeScale * mScale);
		m_previewDelta.y = static_cast<LONG>(m_previewDelta.y / beforeScale * mScale);
		const double normalScaleFactor = std::min(
			(double)(mPreviewRect.right - mPreviewRect.left) / (m_dataArea.getWidth()),
			(double)(mPreviewRect.bottom - mPreviewRect.top) / (m_dataArea.getHeight()) );

		RECT bgRect;
		bgRect.top = 0;
		bgRect.left = 0;
		bgRect.right = static_cast<LONG>(m_dataArea.getWidth() * normalScaleFactor * mScale);
		bgRect.bottom = static_cast<LONG>(m_dataArea.getHeight() * normalScaleFactor * mScale);
		int maxDeltaX = (bgRect.right - bgRect.left) / 2 - (mPreviewRect.right - mPreviewRect.left) / 2;
		int maxDeltaY = (bgRect.bottom - bgRect.top) / 2 - (mPreviewRect.bottom - mPreviewRect.top) / 2;

		if (maxDeltaX < 0) m_previewDelta.x = 0;
		if (maxDeltaY < 0) m_previewDelta.y = 0;

		InvalidateRect(&mPreviewRect);
		SetDlgItemInt(IDC_PLAY_TEXT_SCALE, static_cast<UINT>(mScale));
	}

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CPlayDlg::OnDeltaposPlaySpinFps(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	int id = IDC_PLAY_EDIT_FPS;
	int fps = GetDlgItemInt(id);

	fps += pNMUpDown->iDelta;
	
	fps = std::min(fps, FPS_MAX);
	fps = std::max(fps, FPS_MIN);

	SetDlgItemInt(id, fps);

	*pResult = 0;
}

CWinTILDDoc* CPlayDlg::GetDocument(void)
{
	if (theApp.m_pMainWnd==NULL)
		return NULL;
	return (CWinTILDDoc*)((CFrameWnd*)theApp.m_pMainWnd)->GetActiveDocument();
}


void CPlayDlg::OnDeltaposPlaySpinCurrentframe(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	int currentFrame = GetDlgItemInt(IDC_PLAY_EDIT_CURRENTFRAME);
	setCurrentFrame(currentFrame + pNMUpDown->iDelta);


	*pResult = 0;
}

void CPlayDlg::setCurrentFrame(int currentFrame)
{
	currentFrame = std::max(currentFrame, 0);
	currentFrame = std::min(currentFrame, m_frameEnd);
	m_frameCurrent = currentFrame;
	SetDlgItemInt(IDC_PLAY_EDIT_CURRENTFRAME, currentFrame);

	CSliderCtrl* pSlider;
	pSlider = (CSliderCtrl*)GetDlgItem(IDC_PLAY_SLIDER);
	pSlider->SetPos(currentFrame);
	pSlider->Invalidate();

	CSpinButtonCtrl* pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_PLAY_SPIN_CURRENTFRAME);
	pSpin->SetPos(currentFrame);
	pSlider->Invalidate();

	InvalidateRect(&mPreviewRect);

}


void CPlayDlg::SetData(CDataPtr data)
{
	const float FLOAT_MAX = 100;
	m_data = data;
	CTrajectory* traj = data->GetTrajectory();
	SetWindow(0, traj->GetTargetFrames()-1);
	const int tframe = traj->GetTargetFrames();
	float xmin = 1.0e10;
	float xmax = -1.0e10;
	float ymin = 1.0e10;
	float ymax = -1.0e10;
	for (int i = 0; i < tframe; i++) {
		const CTrajectoryPoint* p = traj->GetRelativeAvgAt(i);
		xmin = std::min(xmin, p->x);
		xmax = std::max(xmax, p->x);
		ymin = std::min(ymin, p->y);
		ymax = std::max(ymax, p->y);
	}
	m_dataArea.left = xmin;
	m_dataArea.right = xmax;
	m_dataArea.top = ymin;
	m_dataArea.bottom = ymax;

}

static void Circle(CDC *pDC, const CPoint& p,  int r, COLORREF color, BOOL fill)
{
	CPen pen(PS_SOLID,1,color) , *poldPen;
	CBrush brush(color), *poldBrush;

	poldPen = pDC->SelectObject(&pen);
	poldBrush=pDC->SelectObject(&brush);

	if ( fill==TRUE )
		pDC->Ellipse(p.x-r, p.y-r, p.x+r, p.y+r);
	else
		pDC->Arc(p.x-r, p.y-r, p.x+r, p.y+r ,p.x ,p.y ,p.x ,p.y);

	pDC->SelectObject(poldPen);
	pDC->SelectObject(poldBrush);

	pen.DeleteObject();
	brush.DeleteObject();
}

void CPlayDlg::OnPaint()
{
	if (!m_data) return;

	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages

	CDC dcMem;
	CBitmap bmp;
	dcMem.CreateCompatibleDC(&dc);

	const double additionalScaleFactor = 1.0;
	CTrajectoryPlotterOption* pO = GetDocument()->GetTrajectoryPlotterOption();


	// Adjust the center point
	CPoint viewCenterPoint;
	viewCenterPoint.x = (mPreviewRect.left + mPreviewRect.right)/2;
	viewCenterPoint.y = (mPreviewRect.top + mPreviewRect.bottom)/2;
	const double normalScaleFactor = std::min(
		(double)(mPreviewRect.right - mPreviewRect.left) / (m_dataArea.getWidth()),
		(double)(mPreviewRect.bottom - mPreviewRect.top) / (m_dataArea.getHeight()) );


	bmp.CreateCompatibleBitmap(&dc, static_cast<int>(m_dataArea.getWidth() * normalScaleFactor * mScale),
		static_cast<int>(m_dataArea.getHeight() * normalScaleFactor * mScale));
	CBitmap* pOldBmp = dcMem.SelectObject(&bmp);

	// Fill the background with white
	CBrush bkBrush;
    bkBrush.CreateSolidBrush(RGB(255,255,255));
	RECT bgRect;
	bgRect.top = 0;
	bgRect.left = 0;
	bgRect.right = static_cast<LONG>(m_dataArea.getWidth() * normalScaleFactor * mScale);
	bgRect.bottom = static_cast<LONG>(m_dataArea.getHeight() * normalScaleFactor * mScale);
	int bgRectCenterX = (bgRect.left + bgRect.right) / 2;
	int bgRectCenterY = (bgRect.top + bgRect.bottom) / 2;
	dcMem.FillRect(&bgRect, &bkBrush);

	dcMem.SetBkMode(pO->m_bkMode);


	CTrajectory* pTrajectory = m_data->GetTrajectory();
	int tframe = pTrajectory->GetTargetFrames();

	CPoint *p = new CPoint[ tframe ];

	for (int i=0; i<tframe; i++){
		CTrajectoryPoint* ptp = pTrajectory->GetRelativeAvgAt(i);
		if (ptp==NULL){
			delete[] p;
			return;
		}
		p[i].x = static_cast<LONG>((ptp->x - m_dataArea.getCenterX()) * normalScaleFactor * mScale + bgRectCenterX);
		p[i].y = static_cast<LONG>((ptp->y - m_dataArea.getCenterY()) * normalScaleFactor * mScale + bgRectCenterY);
	}


	// Draw borders
	CBorder* pBorder = m_data->GetBorder();
	COLORREF pointColor;
	if (pBorder->IsEmpty()){
		dcMem.Polyline(p,tframe);
	}
	else {
		int iBorder = 0;
		CPen pen, *poldPen;
		LOGPEN pens[COLOR_SIZE];
		pens[0] = pO->m_lpLine1;
		pens[1] = pO->m_lpLine2;
		pens[2] = pO->m_lpLine3;
		pens[3] = pO->m_lpLine4;
		pens[4] = pO->m_lpLine5;
		if ( pO->m_SelectedDomain == -1 || pO->m_SelectedDomain == 0 ){
			pen.CreatePen(pens[0].lopnStyle, pens[0].lopnWidth.x, pens[0].lopnColor);
		}
		else {
			pen.CreatePen(pO->m_pnBase.lopnStyle, pO->m_pnBase.lopnWidth.x, pO->m_pnBase.lopnColor);
		}
		poldPen = dcMem.SelectObject( &pen );
		dcMem.MoveTo(p[0]);
		for (int i=0; i<tframe; i++){
			// Change the color for every border
			if ( pBorder->GetSize() > iBorder && i==pBorder->GetAt(iBorder)+1 ){
				pen.DeleteObject();
				iBorder++;
				if ( pO->m_SelectedDomain == -1 || pO->m_SelectedDomain == iBorder ){
					LOGPEN borderPen = pens[iBorder%COLOR_SIZE];
					pen.CreatePen(borderPen.lopnStyle, borderPen.lopnWidth.x, borderPen.lopnColor);

				}
				else {
					// Draw with the base color
					pen.CreatePen(pO->m_pnBase.lopnStyle, pO->m_pnBase.lopnWidth.x, pO->m_pnBase.lopnColor);
				}
				dcMem.SelectObject( &pen );
			}
			dcMem.LineTo(p[i]);
			if (i == m_frameCurrent) {
				LOGPEN logpen;
				pen.GetLogPen(&logpen);
				// Draw a circle
				pointColor = logpen.lopnColor;
  			}

		}
		dcMem.SelectObject( poldPen );
		pen.DeleteObject();
	}
	Circle(&dcMem, p[m_frameCurrent], 5, pointColor, TRUE);

	// Match the position of the background with the center
	int prevWidth = mPreviewRect.right - mPreviewRect.left;
	int prevHeight = mPreviewRect.bottom - mPreviewRect.top;

	dc.BitBlt(mPreviewRect.left, mPreviewRect.top, prevWidth, prevHeight, &dcMem,
		(bgRect.left + bgRect.right) / 2 - prevWidth / 2 + m_previewDelta.x,
		(bgRect.top + bgRect.bottom) / 2 - prevHeight / 2 + m_previewDelta.y,
		SRCCOPY);
	bmp.DeleteObject();
	dcMem.DeleteDC();
	delete [] p;


//	CDialog::OnPaint();
}

UINT CPlayDlg::AnimationThread(LPVOID pParam)
{
	CPlayDlg* ppd = reinterpret_cast<CPlayDlg*>(pParam);
	DWORD lastUpdate = GetTickCount();
	
	while (ppd->m_animationThreadAlive) {
		DWORD now = GetTickCount();
		UINT fps = ppd->GetDlgItemInt(IDC_PLAY_EDIT_FPS);
		int frameMove = static_cast<int>(std::floor((double)(now - lastUpdate) / 1000 * fps));
		if (frameMove > 0) {
			lastUpdate = GetTickCount();
			int moved = ppd->m_frameCurrent + frameMove;
			moved = (moved > ppd->m_frameEnd) ? ppd->m_frameEnd : moved;
			ppd->setCurrentFrame(moved);
			if (ppd->m_frameCurrent == ppd->m_frameEnd) ppd->m_animationThreadAlive = FALSE;
		}
		::Sleep(1);
	}
//	for (; ppd->m_frameCurrent <= ppd->m_frameEnd;

	ppd->GetDlgItem(IDC_PLAY_BUTTON_PLAYBACKWARD)->EnableWindow(TRUE);
	ppd->GetDlgItem(IDC_PLAY_EDIT_CURRENTFRAME)->EnableWindow(TRUE);
	ppd->GetDlgItem(IDC_PLAY_EDIT_FPS)->EnableWindow(TRUE);
	ppd->GetDlgItem(IDC_PLAY_SLIDER)->EnableWindow(TRUE);
	ppd->GetDlgItem(IDC_PLAY_SPIN_CURRENTFRAME)->EnableWindow(TRUE);
	ppd->GetDlgItem(IDC_PLAY_SPIN_FPS)->EnableWindow(TRUE);
	ppd->GetDlgItem(IDC_PLAY_BUTTON_PLAY)->EnableWindow(TRUE);

	ppd->m_animationThreadAlive = FALSE;
	return 1;
}

UINT CPlayDlg::AnimationBackThread(LPVOID pParam)
{
	CPlayDlg* ppd = reinterpret_cast<CPlayDlg*>(pParam);
	DWORD lastUpdate = GetTickCount();
	
	while (ppd->m_animationThreadAlive) {
		DWORD now = GetTickCount();
		UINT fps = ppd->GetDlgItemInt(IDC_PLAY_EDIT_FPS);
		int frameMove = static_cast<int>(std::floor((double)(now - lastUpdate) / 1000 * fps));
		if (frameMove > 0) {
			lastUpdate = GetTickCount();
			int moved = ppd->m_frameCurrent - frameMove;
			moved = (moved < 0) ? 0 : moved;
			ppd->setCurrentFrame(moved);
			if (ppd->m_frameCurrent == 0) ppd->m_animationThreadAlive = FALSE;
		}
		::Sleep(1);
	}
//	for (; ppd->m_frameCurrent <= ppd->m_frameEnd;

	ppd->GetDlgItem(IDC_PLAY_BUTTON_PLAYBACKWARD)->EnableWindow(TRUE);
	ppd->GetDlgItem(IDC_PLAY_EDIT_CURRENTFRAME)->EnableWindow(TRUE);
	ppd->GetDlgItem(IDC_PLAY_EDIT_FPS)->EnableWindow(TRUE);
	ppd->GetDlgItem(IDC_PLAY_SLIDER)->EnableWindow(TRUE);
	ppd->GetDlgItem(IDC_PLAY_SPIN_CURRENTFRAME)->EnableWindow(TRUE);
	ppd->GetDlgItem(IDC_PLAY_SPIN_FPS)->EnableWindow(TRUE);
	ppd->GetDlgItem(IDC_PLAY_BUTTON_PLAY)->EnableWindow(TRUE);

	ppd->m_animationThreadAlive = FALSE;
	return 1;
}

void CPlayDlg::OnBnClickedPlayButtonPlay()
{
	CButton* pb = (CButton*)GetDlgItem(IDC_PLAY_BUTTON_PLAY);
	pb->EnableWindow(FALSE);
	GetDlgItem(IDC_PLAY_BUTTON_PLAYBACKWARD)->EnableWindow(FALSE);
	GetDlgItem(IDC_PLAY_EDIT_CURRENTFRAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_PLAY_EDIT_FPS)->EnableWindow(FALSE);
	GetDlgItem(IDC_PLAY_SLIDER)->EnableWindow(FALSE);
	GetDlgItem(IDC_PLAY_SPIN_CURRENTFRAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_PLAY_SPIN_FPS)->EnableWindow(FALSE);
	m_animationThreadAlive = TRUE;
	AfxBeginThread(CPlayDlg::AnimationThread, this);
	// TODO: Add your control notification handler code here
}


void CPlayDlg::OnBnClickedPlayButtonStop()
{
	m_animationThreadAlive = FALSE;
}


void CPlayDlg::OnBnClickedPlayButtonPlaybackward()
{
	CButton* pb = (CButton*)GetDlgItem(IDC_PLAY_BUTTON_PLAY);
	pb->EnableWindow(FALSE);
	GetDlgItem(IDC_PLAY_BUTTON_PLAYBACKWARD)->EnableWindow(FALSE);
	GetDlgItem(IDC_PLAY_EDIT_CURRENTFRAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_PLAY_EDIT_FPS)->EnableWindow(FALSE);
	GetDlgItem(IDC_PLAY_SLIDER)->EnableWindow(FALSE);
	GetDlgItem(IDC_PLAY_SPIN_CURRENTFRAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_PLAY_SPIN_FPS)->EnableWindow(FALSE);
	m_animationThreadAlive = TRUE;
	AfxBeginThread(CPlayDlg::AnimationBackThread, this);
}


void CPlayDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CPoint clientPoint = point;
	if (mPreviewRect.left <= clientPoint.x && clientPoint.x <= mPreviewRect.right
		&& mPreviewRect.top <= clientPoint.y && clientPoint.y <= mPreviewRect.bottom) {
			m_isLButtonDowned = TRUE;
			m_moveBeforePoint = point;
	}
	CDialog::OnLButtonDown(nFlags, point);
}


void CPlayDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_isLButtonDowned = FALSE;
	CDialog::OnLButtonUp(nFlags, point);
}


void CPlayDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CPoint clientPoint = point;
	if (m_isLButtonDowned) {
		const double normalScaleFactor = std::min(
			(double)(mPreviewRect.right - mPreviewRect.left) / (m_dataArea.getWidth()),
			(double)(mPreviewRect.bottom - mPreviewRect.top) / (m_dataArea.getHeight()) );

		RECT bgRect;
		bgRect.top = 0;
		bgRect.left = 0;
		bgRect.right = static_cast<LONG>(m_dataArea.getWidth() * normalScaleFactor * mScale);
		bgRect.bottom = static_cast<LONG>(m_dataArea.getHeight() * normalScaleFactor * mScale);
		int maxDeltaX = (bgRect.right - bgRect.left) / 2 - (mPreviewRect.right - mPreviewRect.left) / 2;
		int maxDeltaY = (bgRect.bottom - bgRect.top) / 2 - (mPreviewRect.bottom - mPreviewRect.top) / 2;

		m_previewDelta.x += - clientPoint.x + m_moveBeforePoint.x;
		m_previewDelta.y += - clientPoint.y + m_moveBeforePoint.y;
		m_moveBeforePoint = point;
		if (maxDeltaX < 0) m_previewDelta.x = 0;
		else if (m_previewDelta.x < - maxDeltaX) m_previewDelta.x = -maxDeltaX;
		else if (m_previewDelta.x > maxDeltaX) m_previewDelta.x = maxDeltaX;

		if (maxDeltaY < 0) m_previewDelta.y = 0;
		else if (m_previewDelta.y < - maxDeltaY) m_previewDelta.y = -maxDeltaY;
		else if (m_previewDelta.y > maxDeltaY) m_previewDelta.y = maxDeltaY;

		InvalidateRect(&mPreviewRect);
	}
	// TODO: Add your message handler code here and/or call default

	CDialog::OnMouseMove(nFlags, point);
}
