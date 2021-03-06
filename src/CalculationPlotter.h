#pragma once
#include "plotter.h"

class CDivMSDPlotter :
	public CPlotter
{
protected:
	RGBQUAD m_clThreshold; // しきい値以下の色
	RGBQUAD m_Palette[256];

	//表示上の最大・最小値
	double m_Max;
	double m_Min;
	float m_MaxPercent;
	float m_MinPercent;

	//しきい値を実際の値で指定
	double m_Threshold;

public:
	CDivMSDPlotter(void);
	~CDivMSDPlotter(void);
	virtual void Plot(CDC* pDC);
	// 表示内容を設定
	void SetData(CData *pData);
	// percent <=1.0
	void SetMaxPercent(float percent);
	void SetMinPercent(float percent);

	// 表示を元のサイズに合わせる
	void SetMagnification(CDC *pDC, float mx = 1.0, float my = 1.0);
	void SetThreshold(double threshold);

	//比率からしきい値を求める
	double CalcThreshold(double percent);
};
