#pragma once
#include "plotteroption.h"

// Draw lines for "Window From/To"?
#define DP_WINDOW 0x01
// How are lines for "Window From/To" drew? : 0 => StrechBlt, 1 => LineTo
#define DP_WINDOWMETHOD 0x02
// Draw lines for a border?
#define DP_BORDER 0x04
// Draw a frame in the side of a border?
#define DP_BORDERNUMBER 0x08
// Draw a frame below "Window From/To"?
#define DP_WINDOWNUMBER 0x10
// Draw the rate of maginification?
#define DP_MAGNIFY 0x20
// Draw scales?
#define DP_SCALE 0x40

class CDivMSDPlotterOption :
	public CPlotterOption
{
public:
	RGBQUAD m_clThreshold; // the color for pixels whose value are smaller than the threshold value
	RGBQUAD m_clBase;
	RGBQUAD m_Palette[256];

	double m_Max;
	double m_Min;

	LOGPEN m_lpWindowStart;
	LOGPEN m_lpWindowEnd;
	LOGPEN m_lpBorder;

	int m_threshold_color;	// the color for the threshold value

	// the number of scales shown in the axis
	int m_ScaleCount;
	int m_ScaleLength;

	BOOL m_FixSize;
	CDivMSDPlotterOption(void);
	~CDivMSDPlotterOption(void);

	void MakePalette();

	void LoadProfiles(CWinApp *pApp);
	void SaveProfiles(CWinApp *pApp);

	virtual void SetDefaultMargin();
	virtual void SetDefaultArea();
	virtual void SetDefaultBackground();
};
