#include "StdAfx.h"
#include "trajectoryplotteroption.h"

#include <algorithm>

CTrajectoryPlotterOption::CTrajectoryPlotterOption(void)
{

///	SetBorderColor(0,0x00FF0000);// Red
//	SetBorderColor(1,0x00FFA000);// Orange
//	SetBorderColor(2,0x0000E000);// Green
//	SetBorderColor(3,0x0000A0FF);// Cyan
//	SetBorderColor(4,0x000000FF);// Blue
	
	m_PointRadius = 5;
	m_BorderRadius= 3;

	m_pnBase.lopnStyle = PS_SOLID;
	m_pnBase.lopnWidth = CPoint(1,0);        
	m_pnBase.lopnColor = 0x000000;

	m_SelectedDomain = -1;

	m_LegendX = "X";
	m_LegendY = "Y";

	m_lpszSection = "TRAJPLOT";
}

CTrajectoryPlotterOption::~CTrajectoryPlotterOption(void)
{
}

double CTrajectoryPlotterOption::GetMagnify(const CRect& plot, const CSide& side)
{
	double width = (double)plot.Width()/side.Width();
	double height= (double)plot.Height()/side.Height();
	return std::min(width,height);
}

void CTrajectoryPlotterOption::LoadProfiles(CWinApp *pApp)
{
	CPlotterOption::LoadProfiles(pApp);

	m_clStart = pApp->GetProfileInt(m_lpszSection,		"Start",	0x000000FF);
	m_clEnd =  pApp->GetProfileInt(m_lpszSection,			"End",		0x00FF0000);
	m_clBorderPoint = pApp->GetProfileInt(m_lpszSection,	"Boundary", 0x00115511);
}

void CTrajectoryPlotterOption::SaveProfiles(CWinApp *pApp)
{
	CPlotterOption::SaveProfiles(pApp);

	pApp->WriteProfileInt(m_lpszSection, "Start", m_clStart);
	pApp->WriteProfileInt(m_lpszSection, "End",	m_clEnd);
	pApp->WriteProfileInt(m_lpszSection, "Boundary", m_clBorderPoint);
}

void CTrajectoryPlotterOption::SetDefaultMargin()
{
	m_Margin.Left  = MARGIN/2;
	m_Margin.Top   = MARGIN/2;
	m_Margin.Right = MARGIN/2;
	m_Margin.Bottom= MARGIN/2;
}

void CTrajectoryPlotterOption::SetDefaultArea()
{
	m_Area.top   = INTERVAL;
	m_Area.bottom= INTERVAL + PLOTSIZE;
	m_Area.left  = INTERVAL;
	m_Area.right = INTERVAL + PLOTSIZE;
}
