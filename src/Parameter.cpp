#include "StdAfx.h"
#include "parameter.h"

CParameter::CParameter(void)
{
	// Do not initialize parameters in constructor
}

CParameter::CParameter(const CParameter& src)
{
	Copy(src);
}

CParameter::~CParameter(void)
{
}

void CParameter::Copy(const CParameter& src)
{
	SetThreshold( src.GetThreshold() );
	SetMaximum(   src.GetMaximum()   );
	SetWindowStart( src.GetWindowStart() );
	SetWindowEnd( src.GetWindowEnd() );
	SetRatio( src.GetRatio() );
	SetSkip( src.GetSkip() );
	SetSmoothing(src.GetSmoothing());
	SetAverageFrom( src.GetAverageFrom() );
	SetAverageTo( src.GetAverageTo() );
	SetDrawBoundaries( src.GetDrawBoundaries());
}


IMPLEMENT_SERIAL( CParameter, CObject, 1 )

void CParameter::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);

	if (ar.IsStoring())
	{	// storing code
		ar.Write( &m_Threshold,sizeof(m_Threshold) );
		ar.Write( &m_Maximum,  sizeof(m_Maximum)   );
		ar.Write( &m_WindowStart, sizeof(m_WindowStart) );
		ar.Write( &m_WindowEnd,sizeof(m_WindowEnd) );
		ar.Write( &m_Ratio,sizeof(m_Ratio) );
		ar.Write( &m_Skip,sizeof(m_Skip) );
		ar.Write( &m_AverageFrom,sizeof(m_AverageFrom) );
		ar.Write( &m_AverageTo,sizeof(m_AverageTo) );
		ar.Write( &m_drawboundaries, sizeof(m_drawboundaries));
        ar.Write(&m_smoothing, sizeof(m_smoothing));
	}
	else
	{	// loading code
		ar.Read( &m_Threshold,sizeof(m_Threshold) );
		ar.Read( &m_Maximum,  sizeof(m_Maximum)   );
		ar.Read( &m_WindowStart, sizeof(m_WindowStart) );
		ar.Read( &m_WindowEnd,sizeof(m_WindowEnd) );
		ar.Read( &m_Ratio,sizeof(m_Ratio) );
		ar.Read( &m_Skip,sizeof(m_Skip) );
		ar.Read( &m_AverageFrom,sizeof(m_AverageFrom) );
		ar.Read( &m_AverageTo,sizeof(m_AverageTo) );
		ar.Read( &m_drawboundaries, sizeof(m_drawboundaries));
        ar.Read(&m_smoothing, sizeof(m_smoothing));
	}
}
