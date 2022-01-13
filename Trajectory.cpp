#include "StdAfx.h"
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "trajectory.h"


CTrajectory::CTrajectory()
    : start_frame_(0), end_frame_(0)
{}

CTrajectory::~CTrajectory()
{}

void CTrajectory::Load(LPCTSTR filename) {
    std::ifstream ifs(filename);
    if (!ifs) {
        std::string msg = "Cannot read file: ";
        msg += filename;
        AfxMessageBox(msg.c_str());
        return;
    }

    this->RemoveAll();

    std::string line;
    while (std::getline(ifs, line)) {
        CTrajectoryPoint point;
        sscanf(line.c_str(), "%d %f %f", &point.frame, &point.x, &point.y);
        points_.push_back(std::move(point));
    }
    avg_points_ = points_;
    start_frame_ = 0;
    end_frame_ = static_cast<int>(GetSize()) - 1;
}


CTrajectoryPoint* CTrajectory::GetRelativeAt(size_t i) {
    if (this->points_.empty() || i >= end_frame_ - start_frame_ + 1) return nullptr;

    return &this->points_[i + start_frame_];
}

CTrajectoryPoint* CTrajectory::GetRelativeAvgAt(size_t i) {
    if (avg_points_.empty() || i >= end_frame_ - start_frame_ + 1) return nullptr;

    return &avg_points_[i + start_frame_];
}

void CTrajectory::FindSide(CSide& side) {
    if (this->points_.empty()) {
        side.bottom = side.left = side.right = side.top = 0;
        return;
    } else {
        side.left = side.right = this->GetRelativeAvgAt(0)->x;
        side.top = side.bottom = this->GetRelativeAvgAt(0)->y;
    }

    const int num_of_frames = this->GetTargetFrames();
    for (int i = 1; i < num_of_frames; i++) {
        CTrajectoryPoint* traj_ptr = this->GetRelativeAvgAt(i);
        if (traj_ptr) {
            side.left = std::min(traj_ptr->x, side.left);
            side.right = std::max(traj_ptr->x, side.right);
            side.top = std::min(traj_ptr->y, side.top);
            side.bottom = std::max(traj_ptr->y, side.bottom);
        }
    }
}

int CTrajectory::GetTargetFrames(int& start, int& end) const {
    start = start_frame_;
    end = end_frame_;
    return end - start + 1;
}

int CTrajectory::GetTargetFrames() const {
    return end_frame_ - start_frame_ + 1;
}

void CTrajectory::SetTargetFrames(int start, int end) {
    if (end < start) return;

    const int endframes = std::max(0, static_cast<int>(GetSize()) - 1);
    start = std::clamp(start, 0, endframes);
    end = std::clamp(end, 0, endframes);

    start_frame_ = start;
    end_frame_ = end;
}

void CTrajectory::Serialize(CArchive& ar) {
    int size;
    if (ar.IsStoring()) {
        ar.Write(&start_frame_, sizeof(start_frame_));
        ar.Write(&end_frame_, sizeof(end_frame_));

        const int size = static_cast<int>(GetSize());
        ar.Write(&size, sizeof(size));
        ar.Write(points_.data(), sizeof(CTrajectoryPoint) * size);
    } else {
        RemoveAll();

        ar.Read(&start_frame_, sizeof(start_frame_));
        ar.Read(&end_frame_, sizeof(end_frame_));

        ar.Read(&size, sizeof(size));
        points_.resize(size);
        ar.Read(points_.data(), sizeof(CTrajectoryPoint)*size);
        avg_points_ = points_;
    }
}

void CTrajectory::ReverseCopy(const CTrajectory& src) {
    RemoveAll();

    const int ub = static_cast<int>(src.GetSize()) - 1;
    for (int i = ub; i >= 0; i--) {
        points_.push_back(src.GetAt(i));
        avg_points_.push_back(src.GetAvgAt(i));
    }

    int start, end;
    src.GetTargetFrames(start, end);
    SetTargetFrames(ub - end, ub - start);
}

void CTrajectory::RemoveAll() {
    points_.clear();
    avg_points_.clear();
}

void CTrajectory::CalcMovingAverage(int smoothing) {
    avg_points_.resize(points_.size());

    float x_sum = 0, y_sum = 0;
    const int size = points_.size();
    std::vector<CTrajectoryPoint> result(size);
    int cur_size = 0;
    for (int i = 0; i < std::min(smoothing, size); ++i) {
        x_sum += points_[i].x;
        y_sum += points_[i].y;
        ++cur_size;
    }
    for (int i = 0; i < size; ++i) {
        avg_points_[i].x = x_sum / cur_size;
        avg_points_[i].y = y_sum / cur_size;
        avg_points_[i].frame = points_[i].frame;

        const int removed_idx = i - (smoothing - 1);
        const int added_idx = i + smoothing;
        if (removed_idx >= 0) {
            x_sum -= points_[removed_idx].x;
            y_sum -= points_[removed_idx].y;
            --cur_size;
        }
        if (added_idx < size) {
            x_sum += points_[added_idx].x;
            y_sum += points_[added_idx].y;
            ++cur_size;
        }
    }
}
