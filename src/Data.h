#pragma once
#include <memory>

#include "afx.h"
#include "Trajectory.h"
#include "Parameter.h"
#include "DivMSD.h"
#include "border.h"

class CData :
	public CObject
{
DECLARE_SERIAL( CData )

protected:
	CString m_Filename;
	CTrajectory m_Trajectory;
	CParameter m_Parameter;
	CDivMSD m_DivMSD;
	CBorder m_Border;

	// the average value of DivMSD
	double m_Average;

public:
	CData(void);
	~CData(void);

	// Load trajectory data
    void LoadTrajectory(LPCTSTR filename);

	CTrajectory* GetTrajectory();
	CString* GetFilename(){return &m_Filename;};
	CParameter* GetParameter(){return &m_Parameter;}

	// Calculate MSD and DivMSD
	void Calculate(LPCTSTR filename, int start, int end);

	// Find domains
	void FindDomain(const CParameter* pParameter=NULL);

    // Calculate the average value with the given parameter
    // @note : If pParameter = NULL, then calculate with the current parameter
	double CalcAverage(const CParameter* pParameter=NULL);

	// Return the average value
    // @note : this method does not invoke calculation of the average value
    double GetAverage()const{return m_Average;};

    CDivMSD* GetDivMSD(void);
	void SetParameter(const CParameter& parameter);

	CBorder* GetBorder(){ return &m_Border; };

    void Clear(void);

    // Calculate a threshold value with the current parameter
	double CalcThreshold(void);

	void GetValuesToCalculateAverage(CArray<double>& v);

	double UpdateAverage(const CParameter* parameter = NULL);
	virtual void Serialize(CArchive& ar);

	void SetTargetFrames(int start, int end);
	
	// Return the average cordinate in the range [start, end]
	CTrajectoryPoint getAvgCordinate(int start, int end);

private:
	// Find the local maximal values
    // note: Currently, we do not use this method.
	std::vector<int> FindPeak(std::vector<double> const& xs, double sel, double threshold);
};


// A wrapper (pointer) for CData
class CDataPtr : public CObject {
public:
	CDataPtr() {}
	CDataPtr(std::nullptr_t) {}
	CDataPtr(std::shared_ptr<CData> p)
		: ptr(p)
	{}
	CDataPtr(CDataPtr const& other)
		: ptr(other.ptr)
	{}
	CDataPtr& operator=(CDataPtr const& other)
	{
		if(this != &other) {
			ptr = other.ptr;
		}
		return *this;
	}
	CDataPtr(CDataPtr&& other) noexcept
	{
		*this = std::move(other); // implemented by move assignment
	}
	CDataPtr& operator=(CDataPtr&& other) noexcept
	{
		if(this != &other) {
			ptr = std::move(other.ptr);
		}
		return *this;
	}

	CDataPtr& operator=(std::nullptr_t)
	{
		ptr = nullptr;
		return *this;
	}
	CData* operator->() const noexcept
	{
		return Get();
	}

    explicit operator bool() const noexcept
	{
		return (Get() != nullptr);
	}

	CData* Get() const noexcept
	{
		return ptr.get();
	}


    void Reset() noexcept;

    void Reset(CData* p);
	
private:
	std::shared_ptr<CData> ptr;
};

CDataPtr MakeCDataPtr();
