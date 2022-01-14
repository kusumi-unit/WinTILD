#pragma once
#include "afx.h"

namespace constants {
    const int MIN_SMOOTHING = 1;
}

class CParameter :
	public CObject
{

DECLARE_SERIAL( CParameter )

protected:
	double m_Threshold; // percent(<=1.0)
	double m_Maximum;   // maximum value for the plot
	int m_WindowStart;  // the start frame to process
	int m_WindowEnd;    // the end frame to process (, including this frame)
	double m_Ratio;     // percent(<=1.0);
	int m_Skip;         // the minimum interval between borders
	int m_smoothing;    // the width to calculate moving average of trajectory's cordinates
	int m_AverageFrom;  // the range to calculate the average value used to calculate a threshold value
	int m_AverageTo;
	BOOL m_drawboundaries;	// Do draw boundries?

public:
	CParameter(void);
	CParameter(const CParameter& src);
	~CParameter(void);

	void SetThreshold(double threshold){ m_Threshold = threshold; };
	void SetMaximum(double maximum){m_Maximum = maximum;};
	void SetWindowStart(int frame){  m_WindowStart = frame; };
	void SetWindowEnd(int frame){ m_WindowEnd = frame; };
	void SetRatio(double ratio){ m_Ratio = ratio; };
	void SetSkip(int frames){ m_Skip = frames; };
	void SetSmoothing(int smoothing) { m_smoothing = smoothing; }
	void SetAverageFrom(int from){ m_AverageFrom = from; };
	void SetAverageTo(int to){ m_AverageTo = to; };
	void SetDrawBoundaries(BOOL drawboundaries){ m_drawboundaries = drawboundaries;};

	void Copy(const CParameter& src);

	double GetThreshold()const{ return m_Threshold; };
	double GetMaximum()const{ return m_Maximum;};
	double GetRatio()const{ return m_Ratio; };
	int GetWindowStart()const{return m_WindowStart; };
	int GetWindowEnd()const{return m_WindowEnd; };
	int GetSkip()const{return m_Skip; };
	int GetSmoothing() const { return m_smoothing; }
	int GetAverageFrom()const{return m_AverageFrom;};
	int GetAverageTo()const{return m_AverageTo;};
	BOOL GetDrawBoundaries()const{return m_drawboundaries;};

	virtual void Serialize(CArchive& ar);
};
