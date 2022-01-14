#include "StdAfx.h"
#include "plotter.h"

CPlotter::CPlotter(void)
{
	m_pOwner = NULL;
	m_pData = nullptr;
	m_pOption = NULL;
}


CPlotter::~CPlotter(void)
{
}

void CPlotter::DrawBackground(CDC* pDC)
{
	CPlotterOption *pO = GetOption();
	if (pO==NULL)
		return;
	int oldBM = pDC->SetBkMode( pO->m_bkMode );
	COLORREF oldBkColor = pDC->SetBkColor( pO->m_clBackground );

	CPen pnFrame;
	pnFrame.CreatePenIndirect(&pO->m_lpFrame);

	// Fill the area
	COLORREF c;
	if ( pO->m_bkMode == TRANSPARENT ){
		c = oldBkColor;
	}
	else {
		c = pO->m_clBackground;
	}
	pDC->FillSolidRect( pO->m_Area.left, pO->m_Area.top, pO->m_Area.Width()-1, pO->m_Area.Height()-1 , c );
	pDC->SetBkColor( pO->m_clBackground );

	// Margin
	CBrush bg;
	bg.CreateBrushIndirect(&pO->m_lbMargin);

	CBrush *poldBrush = pDC->SelectObject( &bg );
	CPen *poldPen = pDC->SelectObject( &pnFrame );
	pDC->Rectangle( &pO->m_Area );

	// Plot area
	bg.DeleteObject();
	bg.CreateBrushIndirect(&pO->m_lbPlotArea);
	pDC->SelectObject( &bg );

	LOGPEN pn = pO->m_lpFrame;
	pn.lopnStyle = PS_NULL;
	pnFrame.DeleteObject();
	pnFrame.CreatePenIndirect(&pn);
	pDC->SelectObject( &pnFrame );

	CRect rc;
	pO->GetPlotArea(rc);

	// Fill the area again, which are filled on drawing the margin
	if ( pO->m_bkMode == TRANSPARENT ){
		c = oldBkColor;
	}
	else {
		c = pO->m_clBackground;
	}
	pDC->FillSolidRect( rc.left, rc.top, rc.Width()-1, rc.Height()-1 , c );
	pDC->SetBkColor( pO->m_clBackground );

	pDC->Rectangle( &rc );

	pDC->SetBkMode( oldBM );
	pDC->SetBkColor( oldBkColor );
	pDC->SelectObject( poldPen );
	pDC->SelectObject( poldBrush );
	
	pnFrame.DeleteObject();

	bg.DeleteObject();

}

void CPlotter::Replot(BOOL bErase)
{
	if  (m_pOwner !=NULL){
		m_pOwner->Invalidate(bErase);
		m_pOwner->UpdateWindow();
	}
}

void CPlotter::SetData(CDataPtr pData)
{
	// Deselect when a trajectory is changed
	CPlotterOption *pO = GetOption();
	if (pO == NULL) return;
	pO->SelectDomain(-1);
	m_pData = pData;
}

void CPlotter::Plot(CDC* pDC)
{
	// Draw the axis scale
	CFont font, *oldFont;

	// general settings
	// Legend => TRANSPARENT
	int oldBM  = pDC->SetBkMode( TRANSPARENT );
	
	CPlotterOption *pO = GetOption();

	// the preference for x-axis
	UINT oldTA = pDC->SetTextAlign( TA_CENTER | TA_BOTTOM );
	font.CreateFontIndirect( &pO->m_lfLegend );
	CPoint x( pO->m_Area.Width()/2+pO->m_Area.left, pO->m_Area.bottom );
	oldFont = pDC->SelectObject( &font );
	pDC->TextOut( x.x,x.y,pO->m_LegendX );
	
    // the preference for y-axis
    font.DeleteObject();
	pDC->SetTextAlign( TA_CENTER | TA_TOP );
	font.CreateFont( pO->m_lfLegend.lfHeight, pO->m_lfLegend.lfWidth, pO->m_lfLegend.lfEscapement+900, pO->m_lfLegend.lfOrientation
		,pO->m_lfLegend.lfWeight, pO->m_lfLegend.lfItalic, pO->m_lfLegend.lfUnderline, pO->m_lfLegend.lfStrikeOut
		,pO->m_lfLegend.lfCharSet, pO->m_lfLegend.lfOutPrecision, pO->m_lfLegend.lfClipPrecision, pO->m_lfLegend.lfQuality
		,pO->m_lfLegend.lfPitchAndFamily, pO->m_lfLegend.lfFaceName );
	CPoint y( pO->m_Area.left , pO->m_Area.top+pO->m_Margin.Top+(pO->m_Area.Height()-pO->m_Margin.Top-pO->m_Margin.Bottom)/2 );
	pDC->SelectObject( &font );
	pDC->TextOut( y.x,y.y,pO->m_LegendY );

	pDC->SetBkMode( oldBM );
	pDC->SetTextAlign(oldTA);
	pDC->SelectObject( oldFont );
	font.DeleteObject();
}
