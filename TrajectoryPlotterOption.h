#pragma once
#include "plotteroption.h"
#include "trajectory.h"

#define COLOR_SIZE 5

// Display the texts for cordinates in four corners
#define TP_COORDINATES 0x1
// Draw a circle on endpoints of whole frames
#define TP_STARTEND 0x2
// Draw a circle on endpoints of target frames
#define TP_WINDOWSTARTEND 0x4
// Draw a point on borders
#define TP_BOUNDARY		0x8
// Draw a frame on borders
#define TP_BOUNDARYTEXT 0x10

class CTrajectoryPlotterOption :
	public CPlotterOption
{
public:
	CSide m_Side;
	LOGPEN m_pnBase;
	COLORREF m_clStart;
	COLORREF m_clEnd;
	COLORREF m_clBorderPoint;
	int m_PointRadius; // the radius of circles
	int m_BorderRadius;

	CTrajectoryPlotterOption(void);
	~CTrajectoryPlotterOption(void);

	double GetMagnify(const CRect& plot, const CSide& side);

	void LoadProfiles(CWinApp *pApp);
	void SaveProfiles(CWinApp *pApp);

	virtual void SetDefaultMargin();
	virtual void SetDefaultArea();
};
