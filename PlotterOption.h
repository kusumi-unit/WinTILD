#pragma once
#include "afx.h"

typedef struct tag_margin{
	int Top,Bottom,Left,Right;
}CMargin;

class CPlotterOption :
	public CObject
{

protected:
public:
	CRect m_Area;     // drawing area (including the area of m_Margin)
	CMargin m_Margin; // the area of axis, scale, etc
	double m_Magnify; // the rate of maginification
	int m_bkMode;
	int m_bkModeText;
	COLORREF m_clBackground;
	COLORREF m_clBackgroundText;
	CString m_LegendX;
	CString m_LegendY;
	LOGBRUSH m_lbPlotArea;
	LOGBRUSH m_lbMargin;
	LOGPEN m_lpFrame;
	LOGFONT m_lfText;
	LOGFONT m_lfLegend;
	LOGPEN m_lpLine1;
	LOGPEN m_lpLine2;
	LOGPEN m_lpLine3;
	LOGPEN m_lpLine4;
	LOGPEN m_lpLine5;


	CString m_lpszSection;

	// If this is not -1, the selected domain is displayed
	int m_SelectedDomain;
	// settings on drawing data
	unsigned long m_Status;
	CPlotterOption(void);
	~CPlotterOption(void);

	void GetPlotArea(CRect& rc)const;
	void GetArea(CRect& rc)const{ rc = m_Area; };
	void SetArea(CRect& rc){ m_Area = rc; };
	void SetStatus(unsigned long status){ m_Status = status; };
	void SetMargin(int l, int t, int r, int b);
	void SelectDomain(int index){ m_SelectedDomain = index;};
	int GetSelectedDomain()const{ return m_SelectedDomain; };

	BOOL ExtractStatus(unsigned long mode)const{ return mode&m_Status; };
	void SetState(unsigned long status, unsigned long mask);

	virtual void SetDefaultMargin();
	virtual void SetDefaultArea();
	virtual void SetDefaultLBBackGround();
	virtual void SetDefaultLBMargin();
	virtual void SetDefaultLPFrame();
	virtual void SetDefaultBackground();
	virtual void SetDefaultBackgroundText();
	virtual void SetDefaultLPLine1();
	virtual void SetDefaultLPLine2();
	virtual void SetDefaultLPLine3();
	virtual void SetDefaultLPLine4();
	virtual void SetDefaultLPLine5();

	void LoadProfiles(CWinApp *pApp);
	void SaveProfiles(CWinApp *pApp);

};
