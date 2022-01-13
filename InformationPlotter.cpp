#include "StdAfx.h"
#include "WinTILD.h"
#include "informationplotter.h"

CInformationPlotter::CInformationPlotter(void)
{
}

CInformationPlotter::~CInformationPlotter(void)
{
}

void CInformationPlotter::Plot(CDC* pDC)
{
	if (m_pData) {
		CInformationPlotterOption *pO = GetOption();
		int oldBM = pDC->SetBkMode(TRANSPARENT);

		CFont ftText, *poldFont;
		LOGFONT lfText = pO->m_lfText;
		CSize szText(0,lfText.lfHeight);
		lfText.lfHeight = szText.cy;
		ftText.CreateFontIndirect( &lfText );

		poldFont = pDC->SelectObject( &ftText );

		static CString txt;
		CParameter *pParam = m_pData->GetParameter();
		CTrajectory *pTraj = m_pData->GetTrajectory();

		CRect rc;
		pO->GetPlotArea(rc);

		int line = 0;
		
		UINT oldTA = pDC->SetTextAlign(TA_TOP|TA_LEFT);

		Text(pDC, line++, ExtractFileName( *m_pData->GetFilename() ) );

		int start,end;
		txt.Format("input:%d(frames)", pTraj->GetTargetFrames(start,end) );
		Text(pDC, line++, txt );

		txt.Format("from:%d  to:%d(frame)",start,end );
		Text(pDC, line++, txt );

        txt.Format("smoothing: %d (N = %d)", pParam->GetSmoothing() * 2 - 1, pParam->GetSmoothing());
        Text(pDC, line++, txt);

		line++;

		Text(pDC, line++, "calculate with");
		txt.Format("from:[%d]  to:[%d](frame)",pParam->GetWindowStart() , pParam->GetWindowEnd() );
		Text(pDC, line++, txt );
	
		txt.Format("    value:[%g](pix^2/frame)", m_pData->GetAverage());
		Text(pDC, line++, txt );

		txt.Format("maximum:[%g]", m_pData->GetDivMSD()->GetMax() );
		Text(pDC, line++, txt );

		txt.Format("threshold:[%g]", pParam->GetThreshold());
		Text(pDC, line++, txt );

		txt.Format("ratio:[%g](%%)", (pParam->GetRatio())*100.0);
		Text(pDC, line++, txt );

		txt.Format("skip:%d(frame)", pParam->GetSkip() );
		Text(pDC, line++, txt );

		pDC->SelectObject( poldFont );
		ftText.DeleteObject();

		pDC->SetBkMode(oldBM);
		pDC->SetTextAlign(oldTA);
	}
}

void CInformationPlotter::Text(CDC* pDC, int line, const CString& txt)
{
	CInformationPlotterOption *pO = GetOption();
	CRect rc;
	pO->GetPlotArea(rc);
	pDC->TextOut(rc.left, rc.top + pO->m_lfText.lfHeight * line, txt );
}

CInformationPlotterOption* CInformationPlotter::GetOption()
{
	return (CInformationPlotterOption*)m_pOption;
}
