#include "StdAfx.h"
#include "informationplotteroption.h"

CInformationPlotterOption::CInformationPlotterOption(void)
{
	m_lfText.lfHeight = 16;
	m_lpszSection = "INFOPLOT";
}

CInformationPlotterOption::~CInformationPlotterOption(void)
{
}

void CInformationPlotterOption::LoadProfiles(CWinApp *pApp)
{
	CPlotterOption::LoadProfiles(pApp);
}

void CInformationPlotterOption::SaveProfiles(CWinApp *pApp)
{
	CPlotterOption::SaveProfiles(pApp);
}

void CInformationPlotterOption::SetDefaultMargin()
{
	m_Margin.Left  = 5;
	m_Margin.Top   = 5;
	m_Margin.Right = 5;
	m_Margin.Bottom= 5;
}

void CInformationPlotterOption::SetDefaultArea()
{
	m_Area.top   = INTERVAL;
	m_Area.bottom= INTERVAL+PLOTSIZE;
	m_Area.left  = 2*INTERVAL+PLOTSIZE;
	m_Area.right = 2*(INTERVAL+PLOTSIZE);
}

void CInformationPlotterOption::SetDefaultLBBackGround()
{
	m_lbPlotArea.lbColor = RGB(255,255,180);
	m_lbPlotArea.lbHatch = HS_DIAGCROSS;
	m_lbPlotArea.lbStyle = BS_SOLID;
}

void CInformationPlotterOption::SetDefaultLBMargin()
{
	m_lbMargin.lbColor = RGB(255,255,180);
	m_lbMargin.lbHatch = HS_DIAGCROSS;
	m_lbMargin.lbStyle = BS_SOLID;
}

void CInformationPlotterOption::SetDefaultLPFrame()
{
	m_lpFrame.lopnStyle = PS_NULL;
}
