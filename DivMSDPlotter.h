#pragma once
#include "plotter.h"
#include "DivMSDPlotterOption.h"

class CDivMSDPlotter :
	public CPlotter
{
protected:
	float m_MaxPercent;
	float m_MinPercent;
public:
	CDivMSDPlotter(void);
	virtual ~CDivMSDPlotter(void);
	void Plot(CDC* pDC);

	// Fit the display area to the original size
	void CalcAreaSize();

	// percent <=1.0
	void SetMaxPercent(float percent);
	void SetMinPercent(float percent);

	// Return a grayscale color of [start_color]
    // @note : the return value is in [0, 254]
    int GetGrayScale(double x, double min, double max, int start_color);

	CDivMSDPlotterOption* GetOption(){return (CDivMSDPlotterOption*)m_pOption; };
};
