#include "StdAfx.h"
#include "divmsdplotter.h"
#include <algorithm>

//--
inline void COLORREF2RGBQUAD(RGBQUAD& dst, const COLORREF src)
{
	dst.rgbReserved = 0;
	dst.rgbRed   =  src&0xFF;
	dst.rgbGreen = (src>>8)&0xFF;
	dst.rgbBlue  = (src>>16)&0xFF;
}
//--

CDivMSDPlotter::CDivMSDPlotter(void)
{
}

CDivMSDPlotter::~CDivMSDPlotter(void)
{
}

void CDivMSDPlotter::Plot(CDC* pDC)
{
	CPlotter::Plot(pDC);
	CDivMSDPlotterOption* pO = GetOption();
	if (!m_pData) return;

	CParameter *pParam = m_pData->GetParameter(); 

	pO->m_Max = m_pData->GetParameter()->GetMaximum();	
	pO->m_Min = m_pData->GetParameter()->GetThreshold();

	CDivMSD *pdmsd = m_pData->GetDivMSD();
	int width  = pdmsd->GetWidth();
	int height = pdmsd->GetHeight();
	int rwidth = width;

	if (width <= 0 || height <=0 )
		return;

	// 4 byte alignment
	if (width % 4 != 0){
		width +=  ( 4 - width%4 );
	}

	BYTE *pBuffer = new BYTE[ width * height ];
	ZeroMemory( pBuffer, width*height );

	DWORD bmiSize	  = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*256;
	LPBITMAPINFO pbmi = (LPBITMAPINFO)new BYTE[ bmiSize ];
	ZeroMemory( pbmi, bmiSize );

	pbmi->bmiHeader.biSize	    = sizeof(BITMAPINFOHEADER);
	pbmi->bmiHeader.biBitCount  = sizeof(BYTE)*8;
	pbmi->bmiHeader.biClrUsed   = 256;
	pbmi->bmiHeader.biPlanes    = 1;
	pbmi->bmiHeader.biSizeImage = width * height;
	pbmi->bmiHeader.biWidth		= width;
	pbmi->bmiHeader.biHeight	= height;

	memcpy( pbmi->bmiColors, pO->m_Palette, sizeof(RGBQUAD)*256 );

	BYTE *_pb = pBuffer;
	double _p;
	if (pO->m_Max - pO->m_Min > 0.0){
		for (int y=0; y<height; y++){
			_pb  = pBuffer + y*width;
			for (int x=0; x<rwidth; x++){
				_p = pdmsd->GetPixel(x, y);
				if ( _p >= m_pData->CalcThreshold() ){
					int tmp = GetGrayScale(_p, pO->m_Min, pO->m_Max, pO->m_threshold_color);
					if (tmp<0)
						tmp = 0;
					else if (tmp > 254)
						tmp = 254;
					*_pb = static_cast<BYTE>(tmp);
				}
				else {
					*_pb = 255;
				}
				_pb++;
			}
		}
	}

	CRect rc;
	pO->GetPlotArea(rc);

	StretchDIBits( pDC->GetSafeHdc(), rc.left, rc.top, rc.Width(), rc.Height()
		, 0, 0, rwidth, height
		, pBuffer , pbmi, DIB_RGB_COLORS , SRCCOPY
	);

	delete [] pBuffer;
	pBuffer = NULL;

	// Font settings
	CFont ftText, *poldFont;
	ftText.CreateFontIndirect( &pO->m_lfText );
	poldFont = pDC->SelectObject( &ftText );

	static CString txt;
	int oldBM = pDC->SetBkMode( pO->m_bkMode );

	CBorder *pBorder = m_pData->GetBorder();

	if ( pO->ExtractStatus(DP_SCALE) ){
		// Plot axis scales
		float xScale = (float)rc.Width()/pO->m_ScaleCount;	
		for (float i = static_cast<float>(rc.Width()); i >= 0; i -= xScale){
            pDC->MoveTo(rc.left, static_cast<int>(rc.top + i));
            pDC->LineTo(rc.left - pO->m_ScaleLength, static_cast<int>(rc.top + i));
		}

		float yScale = (float)rc.Height()/pO->m_ScaleCount;
		for (float i=0; i<=rc.Height(); i+=yScale){
			pDC->MoveTo(static_cast<int>(rc.left + i), rc.bottom);
			pDC->LineTo(static_cast<int>(rc.left + i), rc.bottom + pO->m_ScaleLength);
		}
	}

	if (pO->m_SelectedDomain >= 0 && pBorder->GetSize() >= pO->m_SelectedDomain){
		// Highlight a selected area
		int top = ( pBorder->GetSize() ==  pO->m_SelectedDomain )?height-1:pBorder->GetAt( pO->m_SelectedDomain );
		int bottom = (pO->m_SelectedDomain ==0)?0:pBorder->GetAt( pO->m_SelectedDomain-1 );
	
		pbmi->bmiColors[0].rgbRed  =255;
		pbmi->bmiColors[0].rgbGreen=255;
		pbmi->bmiColors[0].rgbBlue =255;
		pbmi->bmiColors[0].rgbReserved=0;
		pbmi->bmiHeader.biClrUsed  =1;
		pbmi->bmiHeader.biSizeImage=1;
		pbmi->bmiHeader.biWidth	   =1;
		pbmi->bmiHeader.biHeight   =1;

		BYTE buffer = 0;
		StretchDIBits(pDC->GetSafeHdc(), rc.left, static_cast<int>(rc.bottom-bottom * pO->m_Magnify)
            , rc.Width(), static_cast<int>((bottom - top) * pO->m_Magnify)
			, 0, 0, 1, 1
			, &buffer , pbmi, DIB_RGB_COLORS , SRCINVERT
		);
	}

	UINT oldTA;
	CPen pen,*poldPen;
	// Draw lines in the start and the end potision
	if ( pO->ExtractStatus(DP_WINDOW) ){
		int start,end;
		if ( pO->ExtractStatus(DP_WINDOWMETHOD) ){
			BYTE *pLine = new BYTE[width];
			ZeroMemory( pLine, width );
			ZeroMemory( pbmi->bmiColors, sizeof(RGBQUAD)*256 );
			start = std::min( pParam->GetWindowStart(), rwidth-1 );
			end = std::min( pParam->GetWindowEnd(), rwidth-1 );
			pLine[ start ] = 1;
			pLine[ end  ] = 2;
			
			RGBQUAD clstart;
			RGBQUAD clend;
			COLORREF2RGBQUAD(clstart,pO->m_lpWindowStart.lopnColor);
			COLORREF2RGBQUAD(clend  ,pO->m_lpWindowEnd.lopnColor  );

			pbmi->bmiColors[1]			= clstart;
			pbmi->bmiColors[2]			= clend;
			pbmi->bmiHeader.biClrUsed   = 3;
			pbmi->bmiHeader.biSizeImage = width;
			pbmi->bmiHeader.biWidth		= width;
			pbmi->bmiHeader.biHeight	= 1;

			int oldSM = pDC->SetStretchBltMode( WHITEONBLACK );
			StretchDIBits( pDC->GetSafeHdc(), rc.left, rc.top, rc.Width(), rc.Height()+pO->m_ScaleLength
				, 0, 0, rwidth, 1
				, pLine , pbmi, DIB_RGB_COLORS , SRCPAINT
			);
			pDC->SetStretchBltMode( oldSM );

			delete[] pLine;
		}
		else {
			start = pParam->GetWindowStart();
			end = pParam->GetWindowEnd();

			pDC->SetBkMode( TRANSPARENT );

			pen.CreatePenIndirect( &pO->m_lpWindowStart );
			poldPen = pDC->SelectObject( &pen );
			pDC->MoveTo(static_cast<int>(rc.left + start * pO->m_Magnify), rc.top );
			pDC->LineTo(static_cast<int>(rc.left + start * pO->m_Magnify), rc.bottom+pO->m_ScaleLength);
			pen.DeleteObject();

			pen.CreatePenIndirect( &pO->m_lpWindowEnd );
			pDC->SelectObject( &pen );
			pDC->MoveTo(static_cast<int>(rc.left + end * pO->m_Magnify), rc.top);
			pDC->LineTo(static_cast<int>(rc.left + end * pO->m_Magnify), rc.bottom + pO->m_ScaleLength);
			pDC->SelectObject( poldPen );
			pen.DeleteObject();
		}
		if ( pO->ExtractStatus(DP_WINDOWNUMBER) ){
			// Display texts for the start and the end position
			oldTA = pDC->SetTextAlign( TA_CENTER|TA_TOP );
			pDC->SetBkMode( pO->m_bkModeText );
			pDC->SetBkColor( pO->m_clBackgroundText );
			txt.Format("%d", start );
			pDC->TextOut(static_cast<int>(rc.left + start * pO->m_Magnify), rc.bottom + pO->m_ScaleLength, txt);

			txt.Format("%d", end );
			pDC->TextOut(static_cast<int>(rc.left + end * pO->m_Magnify), rc.bottom+pO->m_ScaleLength, txt);

			pDC->SetTextAlign( oldTA );
		}
	}
	
	if ( pO->ExtractStatus(DP_BORDER) ){ //-------- border lines --------------
		int b;
		oldTA = pDC->SetTextAlign(TA_RIGHT|TA_BASELINE);
		if ( pO->ExtractStatus(DP_WINDOWMETHOD) ){
			int _size = height*4;
			BYTE *pLine = new BYTE[_size];
			ZeroMemory( pLine, _size );
			ZeroMemory( pbmi->bmiColors, sizeof(RGBQUAD)*256 );
			for (int i=0; i<pBorder->GetSize(); i++){
				b = pBorder->GetAt(i);
				memset( pLine+b*4, 1, 4 );
			}			
			RGBQUAD clBoundary;
			COLORREF2RGBQUAD(clBoundary,pO->m_lpBorder.lopnColor);

			pbmi->bmiColors[1]			= clBoundary;
			pbmi->bmiHeader.biClrUsed   = 2;
			pbmi->bmiHeader.biSizeImage = _size;
			pbmi->bmiHeader.biWidth		= 4;
			pbmi->bmiHeader.biHeight	= height;

			int oldSM = pDC->SetStretchBltMode( WHITEONBLACK );
			StretchDIBits( pDC->GetSafeHdc(), rc.left-pO->m_ScaleLength, rc.top, rc.Width()+pO->m_ScaleLength, rc.Height()
				, 0, 0, 4, height
				, pLine , pbmi, DIB_RGB_COLORS , SRCPAINT
			);
			pDC->SetStretchBltMode( oldSM );

			delete[] pLine;
		}
		else {
			pDC->SetBkMode( TRANSPARENT );
			pen.CreatePenIndirect( &pO->m_lpBorder );
			poldPen = pDC->SelectObject( &pen );
			
			for (int i=0; i<pBorder->GetSize(); i++){
				b = pBorder->GetAt(i);
				pDC->MoveTo(rc.left-pO->m_ScaleLength , static_cast<int>(rc.bottom - (b + 1) * pO->m_Magnify));
				pDC->LineTo(rc.right, static_cast<int>(rc.bottom - (b + 1) * pO->m_Magnify));
			}
			pen.DeleteObject();
			pDC->SelectObject( poldPen );
		}
		if ( pO->ExtractStatus(DP_BORDERNUMBER) ){
			pDC->SetBkMode( pO->m_bkModeText );
			pDC->SetBkColor( pO->m_clBackgroundText );

			for (int i=0; i<pBorder->GetSize(); i++){
					b = pBorder->GetAt(i);
					txt.Format("%d",b);
					pDC->TextOut(rc.left-pO->m_ScaleLength, static_cast<int>(rc.bottom - b * pO->m_Magnify), txt);
			}
		}
		pDC->SetTextAlign( oldTA );
	}
	if ( pO->ExtractStatus(DP_MAGNIFY) ){
		// Show the rate of magnification
		txt.Format("x%.2f",pO->m_Magnify);
		oldTA = pDC->SetTextAlign(TA_RIGHT|TA_TOP);
		pDC->SetBkMode( TRANSPARENT );
		pDC->TextOut( pO->m_Area.right , pO->m_Area.top, txt );
		pDC->SetTextAlign(oldTA);
	}
	delete[] (BYTE*)pbmi;

	pDC->SetBkMode( oldBM );
	pDC->SetTextAlign(oldTA);
	pDC->SelectObject( poldFont );

	ftText.DeleteObject();
}

void CDivMSDPlotter::CalcAreaSize()
{
	if (m_pData){
		CDivMSDPlotterOption* pO = GetOption();
		CDivMSD *pdmsd = m_pData->GetDivMSD();
		CSize sz( pdmsd->GetWidth(), pdmsd->GetHeight() );
		double m = pO->m_Magnify;
		if ( pO->m_FixSize == FALSE ){
			pO->m_Area.right = pO->m_Margin.Left + pO->m_Margin.Right + static_cast<LONG>(sz.cx * m) + pO->m_Area.left;
			pO->m_Area.bottom = pO->m_Margin.Top + pO->m_Margin.Bottom+ static_cast<LONG>(sz.cy * m) + pO->m_Area.top ;
		}
		else {
			// Ignore the rage of maginification
			double magx = (double)(pO->m_Area.right - pO->m_Margin.Left - pO->m_Margin.Right - pO->m_Area.left)/(double)sz.cx;
			double magy = (double)(pO->m_Area.bottom - pO->m_Margin.Top - pO->m_Margin.Bottom - pO->m_Area.top)/(double)sz.cy;
			pO->m_Magnify = std::min(magx,magy);
		}
	}
}

void CDivMSDPlotter::SetMaxPercent(float percent)
{
	if ( percent < 0.0 || 1.0 < percent)
		return;
	m_MaxPercent = percent;
	if (m_pData){
		GetOption()->m_Max = m_pData->GetDivMSD()->GetValuePercent( percent );
	}
}

void CDivMSDPlotter::SetMinPercent(float percent)
{
	if ( percent < 0.0 || 1.0 < percent)
		return;
	m_MinPercent = percent;
	if (m_pData){
		GetOption()->m_Min = m_pData->GetDivMSD()->GetValuePercent( percent );
	}
}

int CDivMSDPlotter::GetGrayScale(double x, double min, double max, int start_color){
	return start_color + static_cast<int>(std::floor((x - min) * (254 - start_color) / (max - min) + 0.5));
}
