#pragma once

#include "stdafx.h"
#include "WinTILD.h"

#include "WinTILDDoc.h"
#include "WinTILDView.h"
#include "GeneralOption.h"

#include <math.h>
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CWinTILDView

IMPLEMENT_DYNCREATE(CWinTILDView, CScrollView)

BEGIN_MESSAGE_MAP(CWinTILDView, CScrollView)
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_SIZE()
END_MESSAGE_MAP()


CWinTILDView::CWinTILDView()
{

	m_Lock = FALSE;

	m_plotTrajectory.SetOwner(this);
	m_plotDivMSD.SetOwner(this);
	m_plotInformation.SetOwner(this);

	m_bmpBuffer.Create(1,1,1);
	m_bValidBuffer = FALSE;

	m_hcsScroll = LoadCursor(NULL, IDC_SIZEALL);
	m_hcsArrow  = LoadCursor(NULL, IDC_ARROW );
	m_hcsMagnify= theApp.LoadCursor(IDC_MAGNIFY );
}

CWinTILDView::~CWinTILDView()
{
	m_bmpBuffer.Destroy();
	DeleteObject(m_hcsScroll);
	DeleteObject(m_hcsArrow );
}

BOOL CWinTILDView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CScrollView::PreCreateWindow(cs);
}


void CWinTILDView::OnDraw(CDC* pDC)
{
	CWinTILDDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CGeneralOption *pO = pDoc->GetGeneralOption();

	m_plotDivMSD.CalcAreaSize();
	CSize sz = pDoc->CalculateScrollSize();

	CRect rcClient;
	GetClientRect(&rcClient);
	sz.cx = std::max( sz.cx , (LONG)rcClient.Width() );
	sz.cy = std::max( sz.cy , (LONG)rcClient.Height());


	if (m_bValidBuffer == FALSE){
		m_bmpBuffer.Destroy();
		m_bmpBuffer.Create(sz.cx, sz.cy, 32);		
		SetScrollSizes(MM_TEXT, sz );
	}

	CDC *memDCPointer(CDC::FromHandle(m_bmpBuffer.GetDC()));
	CDC &memDC(*memDCPointer);

	if (m_bValidBuffer == FALSE){
		memDC.SetMapMode( MM_TEXT );
		memDC.SetStretchBltMode( HALFTONE );
		memDC.SetTextAlign( TA_BASELINE );
		memDC.FillSolidRect( 0, 0, sz.cx, sz.cy , pO->m_clBackGround );
		Draw(&memDC);
	}

	int oldBM = pDC->SetStretchBltMode( HALFTONE );
	int oldMM = pDC->SetMapMode( MM_TEXT );
	pDC->BitBlt( 0, 0, sz.cx, sz.cy, &memDC, 0, 0, SRCCOPY );

	if (m_bValidBuffer == FALSE){
		pDC->SetStretchBltMode( oldBM );
	}

	pDC->SetMapMode( oldMM );
	m_bmpBuffer.ReleaseDC();

	if (m_bValidBuffer == FALSE)
		m_bValidBuffer = TRUE;
}

void CWinTILDView::Draw(CDC* pDC)
{
#ifdef _DEBUG
	UINT oldTA =pDC->SetTextAlign(TA_TOP);
	COLORREF oldTC = pDC->SetTextColor(RGB(255,0,0));
	pDC->TextOut(0,0,"!! DEBUG BUILD !!");
	pDC->SetTextColor( oldTC );
	pDC->SetTextAlign(oldTA);
#endif

	m_plotTrajectory.DrawBackground( pDC );
	m_plotTrajectory.Plot( pDC );

	m_plotDivMSD.DrawBackground( pDC );
	m_plotDivMSD.Plot( pDC );

	m_plotInformation.DrawBackground( pDC );
	m_plotInformation.Plot( pDC );

}

void CWinTILDView::OnInitialUpdate()
{
	CWinTILDDoc* pDoc = GetDocument();
	m_plotTrajectory.SetOption( pDoc->GetTrajectoryPlotterOption() );
	m_plotDivMSD.SetOption( pDoc->GetDivMSDPlotterOption() );
	m_plotInformation.SetOption( pDoc->GetInformationPlotterOption() );

	CScrollView::OnInitialUpdate();
	CSize sizeTotal;
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


#ifdef _DEBUG
void CWinTILDView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CWinTILDView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CWinTILDDoc* CWinTILDView::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWinTILDDoc)));
	return (CWinTILDDoc*)m_pDocument;
}
#endif //_DEBUG


void CWinTILDView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* pHint) {
    // Convert to nullptr to CDataPtr's object which have nullptr
    CDataPtr pData = pHint != NULL ? *(CDataPtr*)pHint : nullptr;

	BOOL erase = FALSE;
	switch ( lHint ){
		case UPDATE_NEWDOCUMENT:
			m_plotTrajectory.SetData(nullptr);
			m_plotInformation.SetData(nullptr);
			m_plotDivMSD.SetData(nullptr);
			break;
		case UPDATE_PREVIEW:
			m_plotTrajectory.SetData(pData);
			m_plotInformation.SetData(pData);
			m_plotDivMSD.SetData(nullptr);
			break;
		case UPDATE_CHANGELISTITEM:
		case UPDATE_IMPORT:
		case UPDATE_DELETERESULT:
		case UPDATE_PRINT:
			m_plotTrajectory.SetData(pData);
			m_plotDivMSD.SetData(pData);
			m_plotInformation.SetData(pData);
			break;

		case UPDATE_THRESHOLD:
		case UPDATE_WINDOWSIZE:
		case UPDATE_SKIP:
		case UPDATE_UPDATEPARAMETER:
		case UPDATE_PARAMETER:
		case UPDATE_USEAVERAGE:
		case UPDATE_OPTION:
		case UPDATE_REDRAW:
			break;
		case UPDATE_PLOTAREA:
			m_plotTrajectory.CalcMagnify(pData);
			break;

		case UPDATE_LOCKVIEWS:
			Lock();
			break;
		case UPDATE_UNLOCKVIEWS:
			Unlock();
			break;

		default:
			break;
	}
	if ( !Locking() ){
		m_bValidBuffer = FALSE;
		Invalidate(erase);
		UpdateWindow();
	}
}

void CWinTILDView::Lock(void)
{
	m_Lock = TRUE;
	LockWindowUpdate();
}

BOOL CWinTILDView::Locking(void)
{
	return m_Lock;
}

void CWinTILDView::Unlock()
{
	UnlockWindowUpdate();
	m_Lock = FALSE;
}


BOOL CWinTILDView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CWinTILDDoc* pDoc = GetDocument();
	if ( nFlags & MK_CONTROL ){
		if (zDelta < 0)
			pDoc->SetDivMSDMagnify( pDoc->GetMagnify() * 1.02 );
		if (zDelta > 0)
			pDoc->SetDivMSDMagnify( pDoc->GetMagnify() / 1.02 );
		OnUpdate(this,NULL,NULL);
		
	}
	else if ( nFlags & MK_RBUTTON){
		int pos = GetScrollPos(SB_HORZ);
		SetScrollPos(SB_HORZ, pos - zDelta);
		Invalidate(FALSE);
		UpdateWindow();		
	}
	else {
		int pos = GetScrollPos(SB_VERT);
		SetScrollPos(SB_VERT, pos - zDelta);
		Invalidate(FALSE);
		UpdateWindow();
	}

	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}

void CWinTILDView::OnMouseMove(UINT nFlags, CPoint point)
{
	if ( nFlags & MK_LBUTTON ){
		CPoint current = GetScrollPosition();
		ScrollToPosition( current+(m_PrevPoint-point));
		m_PrevPoint = point;
	}
	if ( nFlags & MK_RBUTTON ){
		CDivMSDPlotterOption *pO = m_plotDivMSD.GetOption();
		CWinTILDDoc *pDoc = GetDocument();
		if (pO->m_FixSize == FALSE){
			double m = 1.0 + (m_PrevPoint.y - point.y) / 2000.0;
			if (m != 0.0 && m != 1.0){
				pDoc->SetDivMSDMagnify( pDoc->GetMagnify() / m );
				m_PrevPoint = point;
			}
		}
		else {
			CRect rc;
			pO->GetArea(rc);
			int div = point.y-m_PrevPoint.y;
			rc.right += div;
			rc.bottom += div;
			pO->SetArea(rc);
			m_PrevPoint = point;
		}
		OnUpdate(NULL,NULL,NULL);
	}

	CScrollView::OnMouseMove(nFlags, point);
}

void CWinTILDView::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCursor( m_hcsScroll );
	m_PrevPoint = point;
	CScrollView::OnLButtonDown(nFlags, point);
}

void CWinTILDView::OnLButtonUp(UINT nFlags, CPoint point)
{
	SetCursor( m_hcsArrow );
	CScrollView::OnLButtonUp(nFlags, point);
}

void CWinTILDView::OnRButtonDown(UINT nFlags, CPoint point)
{
	SetCursor( m_hcsMagnify );
	m_PrevPoint = point;
	CScrollView::OnRButtonDown(nFlags, point);
}

void CWinTILDView::OnRButtonUp(UINT nFlags, CPoint point)
{
	SetCursor( m_hcsArrow );
	CScrollView::OnRButtonUp(nFlags, point);
}

BOOL CWinTILDView::OnSetCursor(CWnd* /*Wnd*/, UINT nHitTest, UINT /*message*/)
{

	switch (nHitTest){
		case HTCLIENT:
			if ( GetKeyState(VK_LBUTTON)<0 )
				SetCursor( m_hcsScroll );
			else if ( GetKeyState(VK_RBUTTON)<0 )
				SetCursor( m_hcsMagnify );
			else
				SetCursor( m_hcsArrow );
			break;
		default:
			break;
	}

	return TRUE;
}


BOOL CWinTILDView::OnPreparePrinting(CPrintInfo* pInfo)
{
	CWinTILDDoc* pDoc = GetDocument();
	CPrintDialog *pdlg = pInfo->m_pPD;
	if ( pdlg->PrintAll() ){
		pInfo->SetMinPage( 1 );
		pInfo->SetMaxPage( pDoc->GetResult()->GetSize() );
	}
	else {
		pInfo->SetMinPage( pdlg->GetFromPage() );
		pInfo->SetMaxPage( pdlg->GetToPage() );
	}

	return DoPreparePrinting(pInfo);
}

void CWinTILDView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	m_pPrintTrajectory = new CTrajectoryPlotter;
	m_pPrintDivMSD     = new CDivMSDPlotter;
	m_pPrintInformation= new CInformationPlotter;

	memcpy(m_pPrintTrajectory, &m_plotTrajectory, sizeof(m_plotTrajectory) );
	memcpy(m_pPrintDivMSD, &m_plotDivMSD, sizeof(m_plotDivMSD) );
	memcpy(m_pPrintInformation, &m_plotInformation, sizeof(m_plotInformation) );
}

void CWinTILDView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	delete m_pPrintTrajectory;
	delete m_pPrintDivMSD;
	delete m_pPrintInformation;
}

void CWinTILDView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	// Print to EMF, and expand it

	CWinTILDDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CGeneralOption *pO = pDoc->GetGeneralOption();

	// Set the page for printing
	const int index = pInfo->m_nCurPage - 1;
	auto pData = (pDoc->GetResult()->InRange(index) ? pDoc->GetResult()->GetAt(index) : nullptr);
	m_pPrintTrajectory->SetData(pData);
	m_pPrintDivMSD->SetData(pData);
	m_pPrintInformation->SetData(pData);

	// Calculate the size of the page
	m_pPrintDivMSD->CalcAreaSize();	
	CSize sz = pDoc->CalculateScrollSize();

	// Create an EMF
	CMetaFileDC memDC;
	memDC.CreateEnhanced( pDC,NULL,NULL,"made by WinTILD");

	memDC.SetMapMode( MM_TEXT );
	memDC.SetStretchBltMode( HALFTONE );
	memDC.SetTextAlign( TA_BASELINE );
	memDC.FillSolidRect( 0, 0, sz.cx, sz.cy , pO->m_clBackGround );

	// Plot to EMF
	m_pPrintTrajectory->DrawBackground( &memDC );
	m_pPrintTrajectory->Plot( &memDC );

	m_pPrintDivMSD->DrawBackground( &memDC );
	m_pPrintDivMSD->Plot( &memDC );

	m_pPrintInformation->DrawBackground( &memDC );
	m_pPrintInformation->Plot( &memDC );
	
	// Create an EMF file on memory
	HENHMETAFILE hMeta = memDC.CloseEnhanced();

	// Calculate a rate of maginication to fit the display area
	double magnifyx = (double)pInfo->m_rectDraw.Width()/(double)sz.cx;
	double magnifyy = (double)pInfo->m_rectDraw.Height()/(double)sz.cy;
	double magnify = std::min( magnifyx, magnifyy );

	CRect rc(pInfo->m_rectDraw.left, pInfo->m_rectDraw.top , static_cast<int>(sz.cx * magnify), static_cast<int>(sz.cy * magnify));

	PlayEnhMetaFile( pDC->m_hDC, hMeta, &rc );

	DeleteEnhMetaFile( hMeta );
}

void CWinTILDView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	m_bValidBuffer = FALSE;
}
