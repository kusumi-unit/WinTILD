#pragma once
#include "FileMappedDynamicArray.h"

class CCalculation :
	public CFileMappedDynamicArray<double>
{
protected:
	int m_Width;
	int m_Height;
	CFileMappedDynamicArray<double> m_SortedValue;

	// The acutual minimum and maximum values
	double m_Max;
	double m_Min;

	static void QuickSort(CFileMappedDynamicArray<double> *pArray, unsigned __int64 first, unsigned __int64 last);

public:
	CCalculation(void);
	virtual ~CCalculation(void);

    void SetSize(int width, int height);

    // Pixel-wise access
    double& Pixel(int x, int y);

    int GetWidth()const{return m_Width;};
	int GetHeight()const{return m_Height;};

    void GetSortedValue(CFileMappedDynamicArray<double>& dst){dst.Copy( m_SortedValue );};

    // Returns the pixel value in the position of [size] * [percent]
    // @note : 0.0 <= percent <= 1.0
	double GetValuePercent(float percent);

    double GetMax()const{return m_Max;};
	double GetMin()const{return m_Min;};

    double CalcThresholdRatio(double threshold);

protected:
	// Sort calculated values and store them to m_SortedValue
	void SortAndStore(void);

public:
	virtual void Serialize(CArchive& ar);
};
