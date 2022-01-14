#include "StdAfx.h"
#include "calculation.h"

CCalculation::CCalculation(void)
{
}

CCalculation::~CCalculation(void)
{
}

double &CCalculation::Pixel(int x, int y)
{
	return GetAt(static_cast<unsigned __int64>(y) * static_cast<unsigned __int64>(m_Width) + static_cast<unsigned __int64>(x));
}

void CCalculation::SetSize(int width, int height)
{
	m_Width = width;
	m_Height = height;
	CFileMappedDynamicArray<double>::SetSize(static_cast<unsigned __int64>(width) * static_cast<unsigned __int64>(height));
}


double CCalculation::GetValuePercent(float percent)
{
	int size = m_Width * m_Height;
	if (size < 0)
		throw;
	if ( percent < 0.0 || 1.0 < percent )
		throw;
	int index = static_cast<int>(size * percent);
	if (index==size)index--;
	return m_SortedValue[index];
}

double CCalculation::CalcThresholdRatio(double threshold)
{
	return threshold/m_Max;
}

void CCalculation::QuickSort(CFileMappedDynamicArray<double> *pArray, unsigned __int64 first, unsigned __int64 last)
{
	unsigned __int64 i, j;
	double x, t;

	x = pArray->GetAt(first + (last - first) / 2);
	i = first;
	j = last;
	for ( ; ; ) {
		while (pArray->GetAt(i) < x) {
			++i;
		}
		while (pArray->GetAt(j) > x) {
			--j;
		}
		if (i >= j) {
			break;
		}
		t = pArray->GetAt(i);
		pArray->SetAt(i, pArray->GetAt(j));
		pArray->SetAt(j, t);
		++i;
		--j;
	}
	if (i > 0 && first < i - 1) {
		QuickSort(pArray, first, i - 1);
	}
	if (j + 1 < last) {
		QuickSort(pArray, j + 1, last);
	}
}

void CCalculation::SortAndStore(void)
{
	m_SortedValue.Copy(*this);
	if (!m_SortedValue.IsEmpty()) {
		QuickSort(&m_SortedValue, 0, m_SortedValue.GetSize() - 1);
	}
}

void CCalculation::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		unsigned __int64 s;
		ar.Write(&m_Width, sizeof(m_Width) );
		ar.Write(&m_Height, sizeof(m_Height) );
		ar.Write(&m_Max, sizeof(m_Max) );
		ar.Write(&m_Min, sizeof(m_Min) );

		s = GetSize();
		ar.Write(&s, sizeof(s));
		for (unsigned __int64 i = 0; i < s; ++i) {
			double element(GetAt(i));
			ar.Write(&element, sizeof(element));
		}
	}
	else
	{	// loading code
		RemoveAll();

		unsigned __int64 s;
		ar.Read(&m_Width, sizeof(m_Width) );
		ar.Read(&m_Height, sizeof(m_Height) );
		ar.Read(&m_Max, sizeof(m_Max) );
		ar.Read(&m_Min, sizeof(m_Min) );

		ar.Read(&s, sizeof(s));
		CFileMappedDynamicArray<double>::SetSize(s);
		for (unsigned __int64 i = 0; i < s; ++i) {
			double element;
			ar.Read(&element, sizeof(element));
			SetAt(i, element);
		}
		SortAndStore();
	}
}
