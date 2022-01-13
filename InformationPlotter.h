#pragma once
#include "plotter.h"
#include "InformationPlotterOption.h"

class CInformationPlotter :
	public CPlotter
{
protected:
public:
	CInformationPlotter(void);
	~CInformationPlotter(void);
	virtual void Plot(CDC* pDC);
	CInformationPlotterOption* GetOption();
protected:
	void Text(CDC* pDC, int line, const CString& txt);
};
