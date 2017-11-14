//
// Created by vista on 2017/10/25.
//

#ifndef FACEA_COMMON_H
#define FACEA_COMMON_H

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/photo/photo.hpp>

#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing.h>
#include <dlib/image_io.h>

#include <cctype>
#include <iostream>
#include <iterator>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <opencv/cv.h>
#include <ctime>
#include <string>
#include <limits>
#include <algorithm>
#include <cmath>
#include <vector>
#include <fstream>
#include <numeric>
#include <utility>

using std::vector;
using std::string;

using cv::Mat;
using cv::Rect;
using cv::Point2d;
using cv::Size2i;
using cv::Size2d;
using cv::Mat1d;
using cv::waitKey;

// 人脸特征点检测模型路径
extern string modelPath;

// 生成图片的输出路径，以*.jpg结尾
extern string out_path;

// 人脸检测模型路径
extern string fd_model;


//extern dlib::shape_predictor sp;



#endif //FACEA_COMMON_H
