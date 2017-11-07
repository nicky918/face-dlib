#ifndef FACE_UTILS_H
#define FACE_UTILS_H

#include "common.h"

Rect GetPtsBound(std::vector<cv::Point> pts);

Mat fix_bound(std::vector<cv::Point> pts,Rect rect);

#endif
