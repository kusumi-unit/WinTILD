#pragma once
#include "calculation.h"
#include "trajectory.h"
#include "FileMappedDynamicArray.h"
#include <vector>


#define ID_UPDATE_PROGRESS 0x00

class CDivMSD :
	public CCalculation
{
protected:
	CFileMappedArray<double> *MakeCalculate(CTrajectory& t, CSize& sz);
	void DeleteCalculate(CFileMappedArray<double> *p);
	long m_Progress;
	long m_ProgressMax;
	double m_Dmicro;
	HWND m_hProgress;
public:
	CDivMSD(void);
	~CDivMSD(void);
	// Calculate DivMSD from the previous value
	void Calculate(CTrajectory& t);
    // Calculate DivMSD from back to front
	void CalculateReverse(CTrajectory& t);
	// Returns the number of valid pixels in line [y]
	int GetWidthAt(int y);
	int GetHeightAt(int x);
	long GetProgress()const{return m_Progress;};
	long GetProgressMax()const{return m_ProgressMax;};
	void SetMessageHandle(HWND hProgress){m_hProgress = hProgress;}

    // Update the progress of a calculation, and send a message to the dialog
	void UpdateProgress();

	// short term average diffusion coefficient
	void setDmicro(CTrajectory& t, int start, int end); 
	double get1Dmicro( CTrajectory& t, int term);
	double getDmicro(){ return m_Dmicro;}

	//---	override	---
	double GetPixel(int w, int i){ return m_Dmicro*Pixel(w,i);}
	double GetMax(){return CCalculation::GetMax()*getDmicro();}
	double GetSlope(std::vector<double>&x, std::vector<double>& y);
};
