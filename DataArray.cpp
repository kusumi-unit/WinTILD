#include "StdAfx.h"
#include <cassert>
#include "dataarray.h"


void CDataArray::AddTrajectory(LPCTSTR filename)
{
	auto d = MakeCDataPtr();
	d->LoadTrajectory(filename);
	data_arr.push_back(d);
}

int CDataArray::Add(CDataPtr const& data)
{
	data_arr.push_back(data);
	return static_cast<int>(data_arr.size()) - 1;
}

void CDataArray::Clear()
{
	data_arr.clear();
}

void CDataArray::RemoveAt(int i) {
	assert(i >= 0 && i < GetSize());
	data_arr.erase(std::begin(data_arr) + i);
}
