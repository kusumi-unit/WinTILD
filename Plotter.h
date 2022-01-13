#pragma once
#include <memory>

#include "afx.h"
#include "data.h"
#include "border.h"
#include "PlotterOption.h"

class CPlotter :
	public CObject
{
protected:
	CDataPtr m_pData; // the data for drawing
	CPlotterOption *m_pOption;
	CWnd *m_pOwner;

public:
	CPlotter(void);
	~CPlotter(void);

    void Plot(CDC* pDC);
	void DrawBackground(CDC* pDC);
	void SetData(CDataPtr pData);
	void Replot(BOOL bErase = TRUE);
	void SetOwner(CWnd* pOwner = NULL){ m_pOwner = pOwner; };
	CPlotterOption* GetOption(){return m_pOption;};
	void SetOption(CPlotterOption* pOption){ m_pOption = pOption; };
};
