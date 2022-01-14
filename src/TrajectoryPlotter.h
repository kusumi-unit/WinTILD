#pragma once
#include <memory>

#include "plotter.h"
#include "TrajectoryPlotterOption.h"

class CTrajectoryPlotter :
	public CPlotter
{
protected:

public:
	CTrajectoryPlotter(void);
	~CTrajectoryPlotter(void);
	void Plot(CDC* pDC);
	// Set a trajectory for drawing
	void SetData(CDataPtr const& pData);
	void CalcMagnify(CDataPtr const& pData);
	void Circle(CDC *pDC, const CPoint& p,  int r, COLORREF color, BOOL fill);
	CTrajectoryPlotterOption* GetOption(){return (CTrajectoryPlotterOption*)m_pOption;};
};
