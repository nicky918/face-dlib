//
// Created by vista on 2017/10/25.
//

#ifndef FACEA_FACEDETECT_H
#define FACEA_FACEDETECT_H

#include "common.h"
#include "Face.h"


#include "face_detection.h"
#include "FaceSwap.h"

using namespace seeta;

//int detect_by_seetface(const Mat &img, dlib::rectangle &face, cv::Mat & gallery_img_gray);

extern FaceSwap faceSwap;

int swap_head_ex(string photo, string model_photo);

int detect_face_and_swap(string photo, string modelpath);

int detect_by_seetface(const Mat & , dlib::rectangle & , cv::Mat & );

int detect_face_rect(std::string path,cv::Mat & image, cv::Rect & rect);

int detect_and_draw( Mat& , dlib::array2d<dlib::rgb_pixel> & , Face & , double );

int detect_and_draw( Mat& , dlib::cv_image<dlib::rgb_pixel> & , Face & , double );



#endif //FACEA_FACEDETECT_H
