#pragma once
#include "afx.h"
#include <vector>

// The point in each frame
struct CTrajectoryPoint {
    int frame; // the frame number
    float x, y;
};

// CSide containts the minimum, maximum x and y values of trajectory's points
struct CSide {
    float left, right, top, bottom;
    float Width()const { return right - left; };
    float Height()const { return bottom - top; };
};

class CTrajectory
{
protected:
    // the target frame; we process the frame [start_frame, end_frame]
    int start_frame_;
    int end_frame_;

private:
    std::vector<CTrajectoryPoint> points_;
    std::vector<CTrajectoryPoint> avg_points_;

public:
	CTrajectory(void);
	~CTrajectory(void);

    // Load a trajectory from a given file
	void Load(LPCTSTR filename);

	// Return the i-th cordinate from the start frame
	CTrajectoryPoint* GetRelativeAt(size_t i);

    // Return the i-th moving averaged cordinate from the start frame
    CTrajectoryPoint* GetRelativeAvgAt(size_t i);

    // Return the i-th cordinate (abusolute index)
    const CTrajectoryPoint& GetAt(size_t i) const { return points_[i]; }

    // Return the i-th moving averaged cordiate (abusolute index)
    const CTrajectoryPoint& GetAvgAt(size_t i) const { return avg_points_[i]; }

	void FindSide(CSide& side);

    int GetTargetFrames(int& start, int& end)const;
	int GetTargetFrames()const;
	void SetTargetFrames(int start, int end);

    int GetStartFrame() { return start_frame_; }
    int GetEndFrame() { return end_frame_; }

	virtual void Serialize(CArchive& ar);

    // Copy a trajectory reversely, and remove the old data
    void ReverseCopy(const CTrajectory& src);

    void RemoveAll();

    size_t GetSize() const { return this->points_.size(); }

    // Calculate the moving average of points, and update the internal data
    // @param width : the width with which we calculate the moving calculate
    // @note : avg_pt[i] := avg{ pt[i-(width-1)], ..., pt[i+(width-1)] }
    // @note : avg_pt[0] = avg { pt[0], .., pt[width-1] }
    void CalcMovingAverage(int smoothing);
};
