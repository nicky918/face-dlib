//
// Created by vista on 2017/10/25.
//

#ifndef FACEA_FACEDETECT_H
#define FACEA_FACEDETECT_H

#include "common.h"
#include "Face.h"


#include "face_detection.h"

using namespace seeta;

//int detect_by_seetface(const Mat &img, dlib::rectangle &face, cv::Mat & gallery_img_gray);

int detect_face_and_align(string photo, string model_photo);

int detect_by_seetface(const Mat & , dlib::rectangle & , cv::Mat & );

int detect_and_draw( Mat& , dlib::array2d<dlib::rgb_pixel> & , Face & , double );



#endif //FACEA_FACEDETECT_H
