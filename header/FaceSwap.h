//
// Created by vista on 2017/11/14.
//

#ifndef FACEB_FACESWAP_H
#define FACEB_FACESWAP_H

#include "common.h"
#include "Face.h"

class FaceSwap {
public:
    FaceSwap(){

    }

    int init(const std::string & landmarks_path);

    // Initialize face swapped with landmarks
    FaceSwap(const std::string & landmarks_path);
    ~FaceSwap();

    //Swaps faces in rects on frame
    void swapFaces(cv::Mat &frame, cv::Rect &rect_ann, cv::Rect &rect_bob);

    void swapFaces2(cv::Mat &frame, cv::Rect &rect_ann, cv::Rect &rect_bob);

private:
    // Returns minimal Mat containing both faces
    cv::Mat getMinFrame(const cv::Mat &frame, cv::Rect &rect_ann, cv::Rect &rect_bob);

    cv::Mat getMinFrame2(const cv::Mat &frame, cv::Rect &rect_ann, cv::Rect &rect_bob);

    // Finds facial landmarks on faces and extracts the useful points
    void getFacePoints(const cv::Mat &frame);

    void getFacePoints2(const cv::Mat &frame);

    // add by vista
    void getFacePoints(const cv::Mat &ann, const cv::Mat &bob);

    cv::Mat getRefinedMasks_ann();

    cv::Mat getRefinedMasks_bob();

    // Calculates transformation matrices based on points extracted by getFacePoints
    void getTransformationMatrices();

    // Creates masks for faces based on the points extracted in getFacePoints
    void getMasks();

    // Creates warpped masks out of masks created in getMasks to switch places
    void getWarppedMasks();

    // Returns Mat of refined mask such that warpped mask isn't bigger than original mask
    cv::Mat getRefinedMasks();

    // Extracts faces from images based on masks created in getMasks
    void extractFaces();

    // Creates warpped faces out of faces extracted in extractFaces
    cv::Mat getWarppedFaces();

    // Matches Ann face color to Bob face color and vice versa
    void colorCorrectFaces();

    // Blurs edges of mask
    void featherMask(cv::Mat &refined_masks);

    // Pastes faces on original frame
    void pasteFacesOnFrame();

    // Calculates source image histogram and changes target_image to match source hist
    void specifiyHistogram(const cv::Mat & source_image, cv::Mat target_image, cv::Mat mask);

    cv::Rect rect_ann, rect_bob;
    cv::Rect big_rect_ann, big_rect_bob;

    dlib::shape_predictor pose_model;
public:
    const dlib::shape_predictor &getPose_model() const;

private:
    dlib::full_object_detection shapes[2];
    dlib::rectangle dlib_rects[2];
//    dlib::cv_image<dlib::bgr_pixel> dlib_frame;
    cv::Point2f affine_transform_keypoints_ann[3], affine_transform_keypoints_bob[3];

    cv::Mat refined_ann_and_bob_warpped, refined_bob_and_ann_warpped;
    cv::Mat warpped_face_ann, warpped_face_bob;

    cv::Point2i points_ann[9], points_bob[9];
//    std::vector<cv::Point> pointsVet[2];
    std::vector<cv::Point> facePts[2];
    cv::Mat trans_ann_to_bob, trans_bob_to_ann;
    cv::Mat mask_ann, mask_bob;
    cv::Mat warpped_mask_ann, warpped_mask_bob;
    cv::Mat refined_masks;
    cv::Mat face_ann, face_bob;
    cv::Mat warpped_faces;

    cv::Mat small_frame;

    cv::Size frame_size;

    cv::Size feather_amount;

    uint8_t LUT[3][256];
    int source_hist_int[3][256];
    int target_hist_int[3][256];
    float source_histogram[3][256];
    float target_histogram[3][256];

};


#endif //FACEB_FACESWAP_H
