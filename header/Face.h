//
// Created by vista on 2017/10/25.
//

#ifndef FACEA_FACE_H
#define FACEA_FACE_H

#include "common.h"
#include "Utils.h"

class Face {
public:
    static const int left_eyebrow_size = 5;
    static const int right_eyebrow_size = 5;
    static const int chin_size = 17;

private:
    std::vector<cv::Point> left_eyebrow;
public:
    const vector<cv::Point, std::allocator<cv::Point>> &getLeft_eyebrow() const;

    const vector<cv::Point, std::allocator<cv::Point>> &getRight_eyebrow() const;

    const vector<cv::Point, std::allocator<cv::Point>> &getChin() const;

private:
    std::vector<cv::Point> right_eyebrow;
    std::vector<cv::Point> chin;
//    Point2d face_pts[left_eyebrow_size+right_eyebrow_size+chin_size];
    std::vector<cv::Point> facePts;
public:
    const std::vector<cv::Point, std::allocator<cv::Point>> &getFacePts() const;

private:
    Rect face_bound;
    std::vector<Point2d> outline[2];
    std::vector<cv::Point> head_pts;
public:
    const std::vector<cv::Point> &getHead_pts() const;

private:
    Rect head_bound;
public:
    const Rect &getHead_bound() const;

private:
    Mat head_img,mask_img;
public:
    void setMask_img(const Mat &mask_img);

public:
    const Mat &getHead_img() const;

    const Mat &getMask_img() const;

public:

    const Rect &getFace_bound() const;

public:

    void get_chin_eyebrow(Mat1d vec);
    void get_chin_eyebrow(const dlib::full_object_detection &shape);

    void GetOutLine(Mat & img);

    void get_head_img(Mat & img);

    Mat fill_bg(Mat & img);

    void gen_mask_img(Mat & mat,cv::Point pt,Mat & mask);
};


#endif //FACEA_FACE_H
