#pragma once
#include "plotteroption.h"

class CInformationPlotterOption :
	public CPlotterOption
{
public:
	CInformationPlotterOption(void);
	~CInformationPlotterOption(void);
	void LoadProfiles(CWinApp *pApp);
	void SaveProfiles(CWinApp *pApp);

	virtual void SetDefaultMargin();
	virtual void SetDefaultArea();
	virtual void SetDefaultLBBackGround();
	virtual void SetDefaultLBMargin();
	virtual void SetDefaultLPFrame();
};
