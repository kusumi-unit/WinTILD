#include "StdAfx.h"
#include "trajectoryplotter.h"

#include <algorithm>

CTrajectoryPlotter::CTrajectoryPlotter(void)
{
}	

CTrajectoryPlotter::~CTrajectoryPlotter(void)
{
}

void CTrajectoryPlotter::Plot(CDC* pDC)
{
	CTrajectoryPlotterOption* pO = GetOption();
	CPlotter::Plot(pDC);
	if (!m_pData) return;

	COLORREF oldBC = pDC->SetBkColor(0xFFFFFF);
	UINT oldTA = pDC->SetTextAlign(TA_LEFT|TA_TOP);
	int oldBM = pDC->SetBkMode( OPAQUE );

	CTrajectory *pTrajectory = m_pData->GetTrajectory();
	const int tframe = pTrajectory->GetTargetFrames();

	CRect rc;
	pO->GetPlotArea( rc );
	pO->m_Magnify = pO->GetMagnify( rc, pO->m_Side );

    // Extract the moving average points in the target range
    std::vector<CTrajectoryPoint> points(tframe);
    for (int i = 0; i < tframe; i++) {
        CTrajectoryPoint* pt = pTrajectory->GetRelativeAvgAt(i);
        if (!pt) return; // not reachable
        points[i] = *pt;
    }
    // Transform points into points for drawing
    std::vector<CPoint> draw_points(tframe);
    for (int i = 0; i < tframe; ++i) {
        draw_points[i] = CPoint{ static_cast<int>((points[i].x - pO->m_Side.left) * pO->m_Magnify + rc.left),
                                 static_cast<int>((points[i].y - pO->m_Side.top) * pO->m_Magnify + rc.top) };
    }

	CFont ftText, *poldFont;
	ftText.CreateFontIndirect( &pO->m_lfText );
	poldFont= pDC->SelectObject( &ftText );

	pDC->SetBkMode(pO->m_bkMode);

	static CString txt;
	CBorder* pBorder = m_pData->GetBorder();

    if (pBorder->IsEmpty()) {
        pDC->Polyline(draw_points.data(), tframe);
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

		poldPen = pDC->SelectObject( &pen );
        pDC->MoveTo(draw_points[0]);
		for (int i=0; i<tframe; i++){
			// Change the color on every border
			if ( pBorder->GetSize() > iBorder && i==pBorder->GetAt(iBorder)+1 ){
				// ペンの再作成
				pen.DeleteObject();
				iBorder++;
				if ( pO->m_SelectedDomain == -1 || pO->m_SelectedDomain == iBorder ){
					LOGPEN borderPen = pens[iBorder%COLOR_SIZE];
					pen.CreatePen(borderPen.lopnStyle, borderPen.lopnWidth.x, borderPen.lopnColor);
				}
				else {
					pen.CreatePen(pO->m_pnBase.lopnStyle, pO->m_pnBase.lopnWidth.x, pO->m_pnBase.lopnColor);
				}
				pDC->SelectObject( &pen );
			}
			pDC->LineTo(draw_points[i]);
		}
		pDC->SelectObject( poldPen );
		pen.DeleteObject();
	}

	if ( tframe >1 && pO->ExtractStatus(TP_STARTEND) ){
		// Draw a circle on the endpoints of the trajectory
		Circle(pDC, draw_points[0], pO->m_PointRadius, pO->m_clStart, FALSE);
		Circle(pDC, draw_points[tframe-1], pO->m_PointRadius, pO->m_clEnd, FALSE);
	}
	if ( tframe >1 && pO->ExtractStatus(TP_WINDOWSTARTEND) ){
		// Draw a filled circle on the endpoints of the target window
		int start = m_pData->GetParameter()->GetWindowStart();
		int end   = m_pData->GetParameter()->GetWindowEnd();
		Circle(pDC, draw_points[start], pO->m_PointRadius, pO->m_clStart, TRUE );
		Circle(pDC, draw_points[ end ], pO->m_PointRadius, pO->m_clEnd,   TRUE );
	}

	if ( pO->ExtractStatus(TP_BOUNDARY) ){
		int index;
		CRect rc;
		pDC->SetTextAlign(TA_UPDATECP|TA_BASELINE|TA_LEFT);
		pDC->SetBkColor( pO->m_clBackgroundText );
		pDC->SetBkMode( pO->m_bkModeText );
		for (int i=0;i <pBorder->GetSize(); i++){
			index = pBorder->GetAt(i);
			// Draw a filled circle on the positions of the border
			Circle(pDC, draw_points[index], pO->m_BorderRadius, pO->m_clBorderPoint , TRUE);
			// Display the frame number on the left side of the border
            if (pO->ExtractStatus(TP_BOUNDARYTEXT)) {
                txt.Format("%d", index);
                rc.SetRect(draw_points[index].x + pO->m_PointRadius,
                           draw_points[index].y,
                           draw_points[index].x + pO->m_PointRadius + 20,
                           draw_points[index].y + 5);
                pDC->MoveTo(draw_points[index].x + pO->m_PointRadius, draw_points[index].y);
                pDC->TextOut(NULL, NULL, txt);
            }
		}
		pDC->SetTextAlign( oldTA );
	}

	// Display cordinates of the four corners
	if ( pO->ExtractStatus(TP_COORDINATES) ){
		int h=pO->m_lfText.lfHeight;

		pDC->SetBkMode( TRANSPARENT );

		oldTA = pDC->SetTextAlign( TA_TOP|TA_LEFT );
		txt.Format(" %.4f",pO->m_Side.left);
		pDC->TextOut(pO->m_Area.left,pO->m_Area.top,txt);
		txt.Format(" , %.4f", pO->m_Side.top);
		pDC->TextOut(pO->m_Area.left,pO->m_Area.top+h,txt);

		pDC->SetTextAlign( TA_TOP|TA_RIGHT );
		txt.Format("%.4f   ",pO->m_Side.right);
		pDC->TextOut(pO->m_Area.right,pO->m_Area.top,txt);
		txt.Format(", %.4f ", pO->m_Side.top);
		pDC->TextOut(pO->m_Area.right,pO->m_Area.top+h,txt);

		pDC->SetTextAlign( TA_BOTTOM|TA_LEFT );
		txt.Format(" %.4f",pO->m_Side.left);
		pDC->TextOut(pO->m_Area.left,pO->m_Area.bottom-h,txt);
		txt.Format(" , %.4f", pO->m_Side.bottom);
		pDC->TextOut(pO->m_Area.left,pO->m_Area.bottom,txt);

		pDC->SetTextAlign( TA_BOTTOM|TA_RIGHT );
		txt.Format("%.4f   ",pO->m_Side.right);
		pDC->TextOut(pO->m_Area.right,pO->m_Area.bottom-h,txt);
		txt.Format(", %.4f ", pO->m_Side.bottom);
		pDC->TextOut(pO->m_Area.right,pO->m_Area.bottom,txt);

		pDC->SetTextAlign( oldTA );
	}

	pDC->SetBkColor( oldBC );
	pDC->SetBkMode( oldBM );
	pDC->SelectObject( poldFont );
	ftText.DeleteObject();
}

void CTrajectoryPlotter::SetData(CDataPtr const& pData)
{
	CPlotter::SetData(pData);
	if (!pData) return;
	// Recalculate the four corners
	CalcMagnify(pData);
}

void CTrajectoryPlotter::CalcMagnify(CDataPtr const& pData)
{
	if(!pData) return;

	CTrajectoryPlotterOption *pO = GetOption();
	pData->GetTrajectory()->FindSide(pO->m_Side);

	// Calculate the rate of magnification to fit the drawing area
	CRect rc;
	pO->GetPlotArea(rc);

	float mx = (float)rc.Width()/(float)pO->m_Side.Width();
	float my = (float)rc.Height()/(float)pO->m_Side.Height();

	pO->m_Magnify = std::min(mx,my);	
}

void CTrajectoryPlotter::Circle(CDC *pDC, const CPoint& p,  int r, COLORREF color, BOOL fill)
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
