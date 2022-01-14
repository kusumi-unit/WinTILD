#include "StdAfx.h"
#include "divmsdplotteroption.h"

CDivMSDPlotterOption::CDivMSDPlotterOption(void)
{
	m_Max = m_Min = 0.0;
	m_Magnify = 1.0;

	m_LegendX = "Window Size (frames)";
	m_LegendY = "Window Start (frame)";

	m_ScaleCount = 10;
	m_ScaleLength=  8;

	m_FixSize = TRUE;

	m_lpszSection = "DMSDPLOT";
}

void CDivMSDPlotterOption::MakePalette()
{
	for (int i=0; i<255; i++){
        m_Palette[i].rgbBlue = static_cast<BYTE>(i * m_clBase.rgbBlue / 255.0);
		m_Palette[i].rgbGreen= static_cast<BYTE>(i * m_clBase.rgbGreen / 255.0);
		m_Palette[i].rgbRed  = static_cast<BYTE>(i * m_clBase.rgbRed /255.0); 
		m_Palette[i].rgbReserved = 0;
	}
    // The 255th color is reserved for the threshold value
	m_Palette[255] = m_clThreshold;
}

CDivMSDPlotterOption::~CDivMSDPlotterOption(void)
{
}

void CDivMSDPlotterOption::LoadProfiles(CWinApp *pApp)
{
	CPlotterOption::LoadProfiles(pApp);

	m_FixSize 	 = pApp->GetProfileInt(m_lpszSection,"FixSize", TRUE);
	m_Magnify	 = (float)pApp->GetProfileInt("DMSDPlotOpt","Magnify", static_cast<int>(FLOATMAGNIFY)) / FLOATMAGNIFY;

	LPBYTE pByte(NULL);
	UINT size;
	pApp->GetProfileBinary(m_lpszSection,"Threshold", &pByte ,&size);
    if (size == sizeof(RGBQUAD)) {
        memcpy(&m_clThreshold, pByte, size);
    } else {
        // default
        m_clThreshold.rgbBlue = 0x10;
        m_clThreshold.rgbGreen = 0x30;
        m_clThreshold.rgbRed = 0x10;
        m_clThreshold.rgbReserved = 0;
    }
	if (pByte) {
		delete [] pByte;
		pByte = NULL;
	}

	pApp->GetProfileBinary(m_lpszSection,"Base", &pByte ,&size);
    if (size == sizeof(RGBQUAD)) {
        memcpy(&m_clBase, pByte, size);
    } else {
        // default
        m_clBase.rgbBlue = m_clBase.rgbGreen = m_clBase.rgbRed = 255;
    }
	if (pByte) {
		delete [] pByte;
		pByte = NULL;
	}
	MakePalette();

	pApp->GetProfileBinary(m_lpszSection,"Start",&pByte, &size);
    if (size == sizeof(LOGPEN)) {
        memcpy(&m_lpWindowStart, pByte, size);
    } else {
        // default
        m_lpWindowStart.lopnColor = 0x000000ff;
        m_lpWindowStart.lopnStyle = PS_SOLID;
        m_lpWindowStart.lopnWidth = CPoint(1, 0);
    }
	if (pByte) {
		delete [] pByte;
		pByte = NULL;
	}

	pApp->GetProfileBinary(m_lpszSection,"End",&pByte, &size);
    if (size == sizeof(LOGPEN)) {
        memcpy(&m_lpWindowEnd, pByte, size);
    } else {
        // default
        m_lpWindowEnd.lopnColor = 0x00ff0000;
        m_lpWindowEnd.lopnStyle = PS_SOLID;
        m_lpWindowEnd.lopnWidth = CPoint(1, 0);
    }
	if (pByte) {
		delete [] pByte;
		pByte = NULL;
	}

	pApp->GetProfileBinary(m_lpszSection,"Boundary",&pByte, &size);
    if (size == sizeof(LOGPEN)) {
        memcpy(&m_lpBorder, pByte, size);
    } else {
        // default
        m_lpBorder.lopnColor = 0x0000ff00;
        m_lpBorder.lopnStyle = PS_SOLID;
        m_lpBorder.lopnWidth = CPoint(1, 0);
    }
	if (pByte) {
		delete [] pByte;
		pByte = NULL;
	}

	pApp->GetProfileBinary(m_lpszSection,"ThresholdColor",&pByte, &size);
    if (size == sizeof(int)) {
        memcpy(&m_threshold_color, pByte, size);
        m_threshold_color = m_threshold_color & 0x0000FF;
    } else {
        // default
        m_threshold_color = 64;
    }
	if (pByte) {
		delete [] pByte;
		pByte = NULL;
	}
}

void CDivMSDPlotterOption::SetDefaultMargin()
{
	m_Margin.Left   = MARGIN;
	m_Margin.Top    = MARGIN/4;
	m_Margin.Right  = MARGIN/4;
	m_Margin.Bottom = MARGIN;
}

void CDivMSDPlotterOption::SetDefaultArea()
{
	m_Area.top   = 2*INTERVAL+PLOTSIZE;
	m_Area.bottom= 2*(INTERVAL+PLOTSIZE);
	m_Area.left  = INTERVAL;
	m_Area.right = INTERVAL+PLOTSIZE;
}

void CDivMSDPlotterOption::SaveProfiles(CWinApp *pApp)
{
	CPlotterOption::SaveProfiles(pApp);
	pApp->WriteProfileInt(m_lpszSection, "FixSize", m_FixSize);
	pApp->WriteProfileInt(m_lpszSection, "Magnify", static_cast<int>(m_Magnify * FLOATMAGNIFY));
	pApp->WriteProfileBinary(m_lpszSection, "Threshold", (LPBYTE)&m_clThreshold,sizeof(RGBQUAD));
	pApp->WriteProfileBinary(m_lpszSection, "Base",  (LPBYTE)&m_clBase,sizeof(RGBQUAD));
	pApp->WriteProfileBinary(m_lpszSection, "Start", (LPBYTE)&m_lpWindowStart, sizeof(LOGPEN) );
	pApp->WriteProfileBinary(m_lpszSection, "End",   (LPBYTE)&m_lpWindowEnd, sizeof(LOGPEN) );
	pApp->WriteProfileBinary(m_lpszSection, "Boundary",  (LPBYTE)&m_lpBorder, sizeof(LOGPEN) );
	pApp->WriteProfileBinary(m_lpszSection, "ThresholdColor",  (LPBYTE)&m_threshold_color, sizeof(int) );
}

void CDivMSDPlotterOption::SetDefaultBackground()
{
	m_clBackground = RGB(255,255,255);
} 
