#include "StdAfx.h"
#include "data.h"
#include <algorithm>

#include <cassert>

CData::CData(void)
{
	m_Average = 0.0;
}

CData::~CData(void)
{
}

void CData::LoadTrajectory(LPCTSTR filename)
{
	m_Trajectory.Load(filename);
}

CTrajectory* CData::GetTrajectory(void)
{
	return &m_Trajectory;
}

void CData::Calculate(LPCTSTR filename, int start, int end)
{
	m_Filename = filename;
	m_Trajectory.SetTargetFrames(start, end);
	m_DivMSD.Calculate(m_Trajectory); // the anterograde analysis
	m_DivMSD.CalculateReverse(m_Trajectory); // the retrograde analysis
}

void CData::FindDomain(const CParameter* pParameter)
{
	m_Border.RemoveAll();

	if ( pParameter != NULL )
		SetParameter(*pParameter);

	int wstart = m_Parameter.GetWindowStart();
	int wend = m_Parameter.GetWindowEnd();
	if ( wstart > wend )
		return;

	const int data_size = m_Trajectory.GetTargetFrames();
	const double thHeight = CalcThreshold();
	TRACE("finding domain\n");
	m_DivMSD.setDmicro(m_Trajectory, m_Parameter.GetAverageFrom(), m_Parameter.GetAverageTo());
	std::vector<int> threshold_count(data_size - 1); // the number of points (i) whose value is larger than the threshold value
    for (int i = 0; i < data_size - 1; ++i) {
        for (int w = wstart; w <= wend; ++w) {
            if (m_DivMSD.GetPixel(w, i) >= thHeight) {
                ++threshold_count[i];
            }
        }
    }

	if(threshold_count.empty()) {
		TRACE("thCount is empty.\n");
		return;
	}

	// 1. Detection of boundaries.
    // If detected boundaries are adjacent, we choose the most detected border
	int preBoundary = -2; // the most recently detected boundary
	int setCount = 0;     // how many times have the above boundary been detected
	double windowSizeThreshold = (wend+1-wstart)*m_Parameter.GetRatio();
	for (int i=0; i<data_size-1; i++){
		if (threshold_count[i] >= windowSizeThreshold ){
			if ( preBoundary+1 != i ){ //　not adjacent
				m_Border.Add( i );
				setCount = threshold_count[i];
			}
			else {
				if ( setCount <= threshold_count[i] ){
					setCount = threshold_count[i];
					m_Border.RemoveAt( m_Border.GetSize() - 1 );
					m_Border.Add( i );
				}
			}
			preBoundary = i;
		}
	}

	if (m_Border.IsEmpty() )
		return;

	int skip = m_Parameter.GetSkip();
	// 2. Remove boundaries whose distance from the previous boundary is smaller than [skip]
	preBoundary = m_Border[0];
	for (int i=1; i<m_Border.GetSize(); i++){
		if (m_Border[i] - preBoundary <= skip){
			m_Border.RemoveAt( i-- );
		}
		else {
			preBoundary = m_Border[i];
		}
	}
}

CDivMSD* CData::GetDivMSD(void)
{
	return &m_DivMSD;
}

void CData::SetParameter(const CParameter& parameter)
{
	CParameter p( parameter );
	int lastFrame = m_Trajectory.GetTargetFrames() - 1;
	if ( p.GetWindowStart() > lastFrame )
		p.SetWindowStart( lastFrame );
	if ( p.GetWindowEnd() > lastFrame )
		p.SetWindowEnd( lastFrame );
	m_Parameter.Copy(p);
}

void CData::Clear(void)
{
	m_Filename = "";
	m_Trajectory.RemoveAll();
	//m_Parameter;
	m_DivMSD.RemoveAll();
	m_Border.RemoveAll();

}

double CData::CalcThreshold(void)
{
	return m_Parameter.GetThreshold();
}

double CData::CalcAverage(const CParameter* pParameter)
{
	if (pParameter != NULL)
		SetParameter( *pParameter );
	
	CParameter *p = GetParameter();
	m_DivMSD.setDmicro(m_Trajectory, p->GetAverageFrom(), p->GetAverageTo());
	return m_DivMSD.getDmicro();
}

double CData::UpdateAverage(const CParameter *parameter)
{
	m_Average = CalcAverage(parameter);
	return m_Average;
}

void CData::GetValuesToCalculateAverage(CArray<double>& v)
{
	v.RemoveAll();

	CParameter *p = GetParameter();
	for (int x = p->GetAverageFrom(); x<=p->GetAverageTo(); x++){
		for (int y = m_DivMSD.GetHeightAt(x)-1; y>=0; y--){
			v.Add( m_DivMSD.GetPixel(x,y) );
		}
	}
}

IMPLEMENT_SERIAL( CData, CObject, 2 )

void CData::Serialize(CArchive& ar)
{
	int length;
	CObject::Serialize(ar);
	if (ar.IsStoring())
	{
		ar.Write( &IDENTSIZE, sizeof(IDENTSIZE) );
		ar.Write( FORMATIDENT_CDATA, IDENTSIZE);

		ar.Write( &m_Average, sizeof(m_Average) );
        { // a dummy value for compatibility
            BOOL dummy = FALSE;
            ar.Write(&dummy, sizeof(dummy));
        }

		length = m_Filename.GetLength()+1;
		ar.Write( &length, sizeof(length) );
		ar.Write( m_Filename.GetString(), sizeof(char)*length );
	}
	else
	{
		char identsize;
		ar.Read( &identsize, sizeof(identsize) );

		if ( identsize != IDENTSIZE ){
			throw CString("Unsupported format.");
		}
		char FormatIdent[ IDENTSIZE ];
		ar.Read( &FormatIdent, IDENTSIZE );
		if ( strcmp( FormatIdent , FORMATIDENT_CDATA ) != 0 ){
			CString txt;
			txt.Format("Can't managed this file.\n"
				"This file's version is %s.\n"
				"Require %s.", FormatIdent, FORMATIDENT_CDATA );
			throw txt;
		}

		ar.Read( &m_Average, sizeof(m_Average) );
        { // a dummy value for compatibility
            BOOL dummy;
            ar.Read(&dummy, sizeof(dummy));
        }

		ar.Read( &length, sizeof(length) );
		char *buffer = new char[length];
		ar.Read( buffer, sizeof(char)*length );
		m_Filename = buffer;
		delete[] buffer;
	}

	m_Trajectory.Serialize(ar);
	m_Parameter.Serialize(ar);
    if (!ar.IsStoring()) {
        // Calculate the moving average cordinates of a trajectoriy because we do not save it in a project file.
        // We do not save this because of compatibility
        m_Trajectory.CalcMovingAverage(m_Parameter.GetSmoothing());

        // Calculate a dmicro value because m_DivMSD does not save it in a project file
        m_DivMSD.setDmicro(m_Trajectory, m_Parameter.GetAverageFrom(), m_Parameter.GetAverageTo());
    }
	m_DivMSD.Serialize(ar);
	m_Border.Serialize(ar);
}

void CData::SetTargetFrames(int start, int end)
{
	GetParameter()->SetWindowStart(0);
	GetParameter()->SetWindowEnd(end-start);
	GetTrajectory()->SetTargetFrames(start,end);
}

CTrajectoryPoint CData::getAvgCordinate(int start, int end){
	CTrajectoryPoint tp;
	if(start < 0) start = 0;
	if(end >= static_cast<int>(m_Trajectory.GetSize()) - 1) end = static_cast<int>(m_Trajectory.GetSize()) - 1;
	tp.frame = 0;
	tp.x = 0;
	tp.y = 0;
	for (int i=start; i<=end; i++){
		CTrajectoryPoint *ptp = m_Trajectory.GetRelativeAvgAt(i);
		tp.x += ptp->x;
		tp.y += ptp->y;
		tp.frame ++;
	}
	if(tp.frame!=0){
		tp.x /= tp.frame; tp.y /= tp.frame;
	}
	return tp;
}

std::vector<int> CData::FindPeak(std::vector<double> const & xs, double sel, double threshold)
{
	const int n = static_cast<int>(xs.size());
	if(n == 0) return std::vector<int>();

	std::vector<int> peak_indices;
	std::vector<double> peak_mags;
	{
		auto p = std::minmax_element(std::begin(xs), std::end(xs));
		sel = (*p.second - *p.first) / 8;
	}
	std::vector<double> dx(n - 1);
	for(int i = 0; i + 1 < n; ++i) {
		dx[i] = xs[i + 1] - xs[i];
		if(dx[i] == 0) dx[i] = -1e10; // This is so we find the first of repeated values
	}

    std::vector<int> inds;
	for(int i = 0; i + 1 < (int)dx.size(); ++i) {
		if(dx[i] * dx[i + 1] < 0) inds.push_back(i + 1);
	}

    const int len = static_cast<int>(inds.size());
	std::vector<double> xs2(len);
	for(int i = 0; i < len; ++i) {
		xs2[i] = xs[inds[i]];
	}

	const double min_mag = *std::min_element(std::begin(xs2), std::end(xs2));
	double left_min = std::min(xs[0], xs2[0]);
	if(len > 2) {
		double temp_mag = min_mag;
		int temp_loc = -1;
		bool found_peak = false;

		std::vector<int> peak_loc;
		std::vector<double> peak_mag;
		int i = (xs2[0] < xs2[1] ? 1 : 0);
		while(i < len) {
			if(found_peak) {
				temp_mag = min_mag;
				found_peak = false;
			}

			if(xs2[i] > temp_mag && xs2[i] > left_min + sel) {
				temp_loc = i;
				temp_mag = xs2[i];
			}

			if(i == len - 1) break;

			++i;
			if(!found_peak && temp_mag > sel + xs2[i]) {
				found_peak = true;
				left_min = xs2[i];
				peak_loc.push_back(temp_loc);
				peak_mag.push_back(temp_mag);
			} else if(xs2[i] < left_min) {
				left_min = xs2[i];
			}
		}

		if(!found_peak) {
			if(xs2.back() > temp_mag && xs2.back() > left_min + sel) {
				peak_loc.push_back(len - 1);
				peak_mag.push_back(xs2.back());
			} else if(temp_mag > std::min(xs.back(), xs2.back()) + sel) {
				peak_loc.push_back(temp_loc);
				peak_mag.push_back(temp_mag);
			}
		}

		if(peak_loc.size() > 1u) {
			for(auto idx : peak_loc) {
				peak_indices.push_back(inds[idx]);
			}
			peak_mags = std::move(peak_mag);
		}
	}

	{ // Filter with the threshold value
		std::vector<int> tmp_inds;
		for(int i = 0; i < (int)peak_indices.size(); ++i) {
			if(peak_mags[i] > threshold) {
				tmp_inds.push_back(peak_indices[i]);
			}
		}
		peak_indices = std::move(tmp_inds);
	}
	return peak_indices;
}


// ============================================================================
// CDataPtr
// ============================================================================

void CDataPtr::Reset() noexcept
{
	ptr.reset();
}

void CDataPtr::Reset(CData* p)
{
	ptr.reset(p);
}

CDataPtr MakeCDataPtr()
{
	return CDataPtr(std::make_shared<CData>());
}
