#include "StdAfx.h"
#include "divmsd.h"
#include <math.h>
#include <algorithm>
#include "FileMappedArray.h"

CDivMSD::CDivMSD(void)
{
	m_Max = 0;
	m_Min = 0;
	m_Progress = 0;
	m_ProgressMax = 1000;
}

CDivMSD::~CDivMSD(void)
{
}

// Calculate DivMSD.
// This method also allocates an array for storing the result
CFileMappedArray<double> *CDivMSD::MakeCalculate(CTrajectory& t, CSize& sz)
{
    const int size = t.GetTargetFrames();
    const int sqSize = size * size;

    CFileMappedArray<double> avex(sqSize + 1);
    CFileMappedArray<double> avey(sqSize + 1);

    avex.Fill(0.0);
    avey.Fill(0.0);

    CCalculation radius;
    radius.SetSize(size, size);

    // Use the moving average value
    std::vector<CTrajectoryPoint> tp(size);
    for(int i = 0; i < size; i++) {
        tp[i] = *t.GetRelativeAvgAt(i);
    }

    TRACE("calculating average...");
    for(int i = 0; i < size; i++) {
        avex[i * size] = avey[i * size] = 0.0;
        auto _tp = &tp[i];
        for(int w = 0; w < size - i; w++, _tp++) { // w : window start   size-i : window size
            avex[i * size + w] += _tp->x;
            avey[i * size + w] += _tp->y;
            avex[i * size + w + 1] += avex[i * size + w];
            avey[i * size + w + 1] += avey[i * size + w];
            avex[i * size + w] /= (double)(w + 1);
            avey[i * size + w] /= (double)(w + 1);
        }
        UpdateProgress();
    }
    TRACE("done.\n");

    TRACE("calculating radius...");
    for(int i = 0; i < size; i++) { //i: window start
        for(int w = 1; w < size - i; w++) { //w : window size
            // Calculate r = max{ the distance from traj[i] to (avex, avey) within the window size }
            const double _avex = avex[size * i + w - 1];
            const double _avey = avey[size * i + w - 1];
            double maxR = 0.0;
            auto _tp = &tp[i];
            for(int j = i; j <= i + w; j++) {
                maxR = std::max(maxR, (_tp->x - _avex)*(_tp->x - _avex) + (_tp->y - _avey)*(_tp->y - _avey));
                _tp++;
            }
            radius.Pixel(w, i) = sqrt(maxR);

            UpdateProgress();
        }
    }
    TRACE("done.\n");
    tp.clear();

    // --- Calculate MSD ------------------------------------------------------
    TRACE("calculating MSD...");
    CCalculation msd;
    msd.SetSize(size, size);

    for(int i = 0; i < size; i++) {
        for(int w = 1; w < size - i; w++) {
            const double r = radius.Pixel(w, i);
            msd.Pixel(w, i) = (r * r) / (4.0 * w);
        }
        UpdateProgress();
    }
    TRACE("done.\n");

    // --- Calculate DivMSD ---------------------------------------------------
    TRACE("calculating DivMSD...");
    CFileMappedArray<double> *pDivMSD = new CFileMappedArray<double>(sqSize);
    pDivMSD->Fill(0.0);
    sz.cx = sz.cy = size;

    for(int i = 0; i < size - 1; i++) {
        for(int w = 1; w < size - 1 - i; w++) {
            const double _m0 = msd.Pixel(w, i);
            const double _m1 = msd.Pixel(w, i + 1);
            double _p0 = 0.0;
            if(_m1 != 0 && _m0 != 0) {
                _p0 = fabs(1.0 / _m1 - 1.0 / _m0);
            }
            pDivMSD->SetAt(static_cast<unsigned __int64>(i) * static_cast<unsigned __int64>(size) + static_cast<unsigned __int64>(w), _p0);
        }
        UpdateProgress();
    }
    TRACE("done.\n");
    return pDivMSD;
}

void CDivMSD::DeleteCalculate(CFileMappedArray<double> *p)
{
	delete p;
}


void CDivMSD::Calculate(CTrajectory& t)
{
	int size = t.GetTargetFrames();
	m_Progress = 0;
	m_ProgressMax = 6*size-2+2*size*size/2;

	CSize sz;
	CFileMappedArray<double> *pDivMSD = MakeCalculate(t, sz);
	SetSize(sz.cx, sz.cy);

	for (int y=0; y<sz.cy; y++){
		for (int x=0; x<sz.cx-y; x++){
			Pixel(x, y) = pDivMSD->GetAt(static_cast<unsigned __int64>(y) * static_cast<unsigned __int64>(sz.cy) + static_cast<unsigned __int64>(x));
		}
	}

	DeleteCalculate( pDivMSD );

	SortAndStore();

	m_Max = GetValuePercent(1.0);
	m_Min = GetValuePercent(0.0);
}

void CDivMSD::CalculateReverse(CTrajectory& t)
{
	CTrajectory rt;

	rt.ReverseCopy(t);

	CSize sz;
	CFileMappedArray<double> *pDivMSD = MakeCalculate(rt, sz);
	double DMSD;
	SetSize(sz.cx, sz.cy);

	for (int y=0; y<sz.cy; y++){
		for (int x=0; x<sz.cx-y; x++){
			DMSD = pDivMSD->GetAt(static_cast<unsigned __int64>(y) * static_cast<unsigned __int64>(sz.cy) + static_cast<unsigned __int64>(x));
			SetAt(static_cast<unsigned __int64>(sz.cy - y) * static_cast<unsigned __int64>(m_Width) - static_cast<unsigned __int64>(x) - 1, DMSD);
		}
	}

	DeleteCalculate( pDivMSD );

	SortAndStore();

	m_Max = GetValuePercent(1.0);
	m_Min = GetValuePercent(0.0);
}

int CDivMSD::GetWidthAt(int y)
{
	int ret = y-1;
	if (ret < 0)
		ret = 0;
	return ret;
}

int CDivMSD::GetHeightAt(int x)
{
	int ret = GetHeight()-x-2;
	if (ret < 0)
		ret = 0;
	return ret;
}

void CDivMSD::UpdateProgress()
{
    m_Progress++;
    // In general, UpdateProgress is called on every iteration of for-loop.
    // Sending message on every iteration decreases the performance of the application.
    // Therefore, we send the next message with an interval.
    const int send_interval = 1000;
    if(m_Progress % send_interval == 0) {
        PostMessage(m_hProgress, WM_COMMAND, ID_UPDATE_PROGRESS, NULL);
    }
}

// Calculate ShortTermDiffusion Coefficient(min,max).
// Default value is D2-4.
void CDivMSD::setDmicro(CTrajectory& t, int start, int end){
	std::vector<double> Dmicro_v(0);
	std::vector<double> x(0);
	for(int i=start; i<= end; ++i){
		Dmicro_v.push_back(get1Dmicro(t, i));
		x.push_back(1.0*i);
	}
	m_Dmicro = GetSlope(x, Dmicro_v)/4.0;
}

double CDivMSD::get1Dmicro(CTrajectory& t, int term){
	// short term diffusion coefficient is
	//	D(t) = |L(t)|^2 / t
	CTrajectoryPoint* ptraj;
	CTrajectoryPoint* ptraj_next;
	double Dmicro=0.0;
	int size = t.GetTargetFrames();
	int count=0;
	//---	error treatment	---
	if(term<=0 || size < 1)
		return 0.0;
	//-------------------------
	for (int i=0; i<=t.GetEndFrame()-t.GetStartFrame()-term; i++){
		ptraj      = t.GetRelativeAvgAt(i);
		ptraj_next = t.GetRelativeAvgAt(i+term);
		if (ptraj != NULL){
			Dmicro += (ptraj->x - ptraj_next->x)*(ptraj->x - ptraj_next->x);
			Dmicro += (ptraj->y - ptraj_next->y)*(ptraj->y - ptraj_next->y);
			count ++;
		}
	}
	if(count != 0) Dmicro /= count;
	return Dmicro;
}

double CDivMSD::GetSlope(std::vector<double>&x, std::vector<double>& y){
	double sum_xy=0.0, sum_x=0.0, sum_y=0.0, sum_xx=0.0;
	for(size_t i=0; i<x.size(); ++i){
		sum_xy += x[i]*y[i];
		sum_x  += x[i];
		sum_y  += y[i];
		sum_xx += x[i]*x[i];
	}
	return (sum_xy-sum_x*sum_y/x.size() )/(sum_xx - sum_x*sum_x/x.size());
}
