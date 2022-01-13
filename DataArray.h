#pragma once
#include <memory>
#include <cassert>

#include "afxcoll.h"
#include "data.h"

class CDataArray
{
public:
	// Add trajectories from the given file
	void AddTrajectory(LPCTSTR filename);

    bool InRange(int nIndex) const {
		return 0 <= nIndex && nIndex < GetSize();
	}
	CDataPtr& GetAt(int nIndex) {
		assert(InRange(nIndex));
		return data_arr[nIndex];
	}
	const CDataPtr& GetAt(int nIndex) const {
		assert(InRange(nIndex));
		return data_arr[nIndex];
	}
	CDataPtr& operator[](int nIndex)              { return GetAt(nIndex); }
	const CDataPtr& operator[] (int nIndex) const { return GetAt(nIndex); }

	// Push a new element
	// @return : the index of the pushed element
	int Add(CDataPtr const& data);

    void Clear();

	// Remove a element in the position of [i]
    // @note : Update indices j (> i) to j-1.
	void RemoveAt(int i);

	int GetSize() const { return static_cast<int>(data_arr.size()); }

	bool IsEmpty() const { return data_arr.empty(); }

private:
	std::vector<CDataPtr> data_arr;
};
