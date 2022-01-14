#include "StdAfx.h"
#include "dataplotter.h"

CDataPlotter::CDataPlotter(void)
: m_pData(NULL)
{
	m_pData = NULL;
	m_Max = m_Min = 0.0;

	m_clThreshold.rgbBlue = 0x11;
	m_clThreshold.rgbGreen= 0x44;
	m_clThreshold.rgbRed  = 0x11;
	m_clThreshold.rgbReserved =0;

	//255段階表示  256番目はしきい値の色
	for (int i=0; i<255; i++){
		m_Palette[i].rgbBlue = m_Palette[i].rgbGreen = m_Palette[i].rgbRed = i;
		m_Palette[i].rgbReserved = 0;
	}
	m_Palette[255] = m_clThreshold;

	m_MaxPercent = 0.99;
	m_MinPercent = 0.0;

	m_Threshold = 0.0;
}

CDataPlotter::~CDataPlotter(void)
{
}
/*
void CDataPlotter::Plot(CDC* pDC)
{
	if (m_pData != NULL){
		int width = m_pData->GetWidth();
		int height =m_pData->GetHeight();
		int rwidth = width;

		//4Byte境界に設定
		if (width % 4 != 0){
			width +=  ( 4 - width%4 );
		}

		BYTE *pBuffer = new BYTE[ width * height ];
		ZeroMemory( pBuffer, width*height );

		DWORD bmiSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*256;
		LPBITMAPINFO pbmi = (LPBITMAPINFO)new BYTE[ bmiSize ] ;
		ZeroMemory( pbmi, bmiSize );

		pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		pbmi->bmiHeader.biBitCount = sizeof(BYTE)*8;
		pbmi->bmiHeader.biClrUsed = 256;
		pbmi->bmiHeader.biPlanes = 1;

		pbmi->bmiHeader.biSizeImage = width * height;
		pbmi->bmiHeader.biWidth = width;
		pbmi->bmiHeader.biHeight= height;

		memcpy(pbmi->bmiColors, m_Palette, sizeof(RGBQUAD)*256);

		BYTE *_pBuffer = pBuffer;
		int tmp;
		double *_pData = m_pData->GetData();
		if (m_Max-m_Min > 0.0){
			for (int y=0; y<height; y++){
				_pBuffer = &pBuffer[y*width];
				_pData   = m_pData->ScanLine(y);
				for (int x=0; x<rwidth; x++){
					if ( -x+height -1 <= y ){
						*_pBuffer = 255;
					}
					else if ( *_pData >= m_Threshold ){
						tmp = (*_pData-m_Min)*255/(m_Max-m_Min);
						if (tmp<0)
							tmp = 0;
						else if (tmp > 254)
							tmp = 254;
						*_pBuffer = tmp;
					}
					else {
						*_pBuffer = 255;
					}
					_pBuffer++;_pData++;
				}
			}
		}

		CRect rc;
		GetPlotArea(rc);

		StretchDIBits( pDC->GetSafeHdc(), rc.left, m_PlusDirection*rc.top, rc.Width(), m_PlusDirection*rc.Height()
			, 0, 0, rwidth, height
			, pBuffer , pbmi, DIB_RGB_COLORS , SRCCOPY
		);

		delete[] (BYTE*)pbmi;
		delete[] pBuffer;
	}
}

// 表示内容を設定
void CDataPlotter::SetCalculation(CCalculation* pC)
{
	m_pData = pC;
	SetMaxPercent( m_MaxPercent );
	SetMinPercent( m_MinPercent );
}

// percent <=1.0
void CDataPlotter::SetMaxPercent(float percent)
{
	if ( percent < 0.0 || 1.0 < percent)
		return;
	m_MaxPercent = percent;
	if (m_pData != NULL){
		m_Max = m_pData->GetValuePercent( percent );
	}
}
void CDataPlotter::SetMinPercent(float percent)
{
	if ( percent < 0.0 || 1.0 < percent)
		return;
	m_MinPercent = percent;
	if (m_pData != NULL){
		m_Min = m_pData->GetValuePercent( percent );
	}
}

// 表示を元のサイズに合わせる
// MapModeがHIMETRICの時限定
void CDataPlotter::SetMagnification(CDC *pDC, float mx, float my)
{
	if (m_pData != NULL){
		CSize sz( m_pData->GetWidth(), m_pData->GetHeight() );
		pDC->DPtoHIMETRIC( &sz );
		m_Area.right = m_Margin.Left + m_Margin.Right + sz.cx + m_Area.left;
		m_Area.bottom = m_Margin.Top + m_Margin.Bottom+ sz.cy + m_Area.top ;
	}
}

void CDataPlotter::SetThreshold(double threshold)
{
	m_Threshold = threshold;
}

double CDataPlotter::CalcThreshold(double percent)
{
	if ( percent < 0.0 )
		percent = 0.0;
	else if ( percent > 1.0 )
		percent = 1.0;
	if (m_pData != NULL){
		return m_pData->GetMax()*percent;
	}
	return 0.0;
}
*/