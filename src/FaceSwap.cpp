//
// Created by vista on 2017/11/14.
//

#include "../header/FaceSwap.h"

FaceSwap::FaceSwap(const std::string & landmarks_path)
{
    init(landmarks_path);
}

FaceSwap::~FaceSwap()
{

}

void FaceSwap::swapFaces(cv::Mat &frame, cv::Rect &rect_ann, cv::Rect &rect_bob)
{
    this->pointsVet[0].clear();
    this->pointsVet[1].clear();

    small_frame = getMinFrame(frame, rect_ann, rect_bob);

    frame_size = cv::Size(small_frame.cols, small_frame.rows);

    getFacePoints2(small_frame);

    getTransformationMatrices();

    mask_ann.create(frame_size, CV_8UC1);
    mask_bob.create(frame_size, CV_8UC1);

    getMasks();

    getWarppedMasks();

    refined_masks = getRefinedMasks();

    extractFaces();

    warpped_faces = getWarppedFaces();

    colorCorrectFaces();

    auto refined_mask_ann = refined_masks(big_rect_ann);
    auto refined_mask_bob = refined_masks(big_rect_bob);
    featherMask(refined_mask_ann);
    featherMask(refined_mask_bob);

    pasteFacesOnFrame();
}


void FaceSwap::swapFaces2(cv::Mat &frame, cv::Rect &rect_ann, cv::Rect &rect_bob)
{
    this->pointsVet[0].clear();
    this->pointsVet[1].clear();

    this->rect_ann = rect_ann;
    this->rect_bob = rect_bob;

    this->small_frame = frame;

    frame_size = cv::Size(small_frame.cols, small_frame.rows);

    getFacePoints2(small_frame);

    getTransformationMatrices();

    mask_ann.create(frame_size, CV_8UC1);
    mask_bob.create(frame_size, CV_8UC1);

    getMasks();

    getWarppedMasks();

    cv::Mat mask_ann = getRefinedMasks_ann();

//    cv::imshow("img",mask_ann);
//    cv::waitKey(0);

    cv::Mat mask_bob = getRefinedMasks_bob();

//    cv::imshow("img",mask_bob);
//    cv::waitKey(0);

    extractFaces();

    cv::Mat warpped_faces_ann(frame_size, CV_8UC3, cv::Scalar::all(0));
    cv::Mat warpped_faces_bob(frame_size, CV_8UC3, cv::Scalar::all(0));

    cv::warpAffine(face_ann, warpped_face_ann, trans_ann_to_bob, frame_size, cv::INTER_NEAREST, cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0));
    cv::warpAffine(face_bob, warpped_face_bob, trans_bob_to_ann, frame_size, cv::INTER_NEAREST, cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0));

    warpped_face_ann.copyTo(warpped_faces_ann, warpped_mask_ann);
    warpped_face_bob.copyTo(warpped_faces_bob, warpped_mask_bob);

//    cv::imshow("img",warpped_faces_ann);
//    cv::waitKey(0);

    cv::Rect rectAnn = cv::boundingRect(this->pointsVet[0]);
    cv::Rect rectBob = cv::boundingRect(this->pointsVet[1]);

//    cv::rectangle(frame,rectAnn,cv::Scalar(255,0,0));

    cv::Mat annWarppedMat = warpped_face_ann(rectBob);

    cv::Point center = cv::Point(rectBob.width/2,rectBob.height/2);
    cv::Mat bobSub = small_frame(rectBob);
    cv::Mat maskbobSub = mask_bob(rectBob);

    Mat output;
    cv::seamlessClone(annWarppedMat,bobSub,maskbobSub,center,output,cv::NORMAL_CLONE);

    output.copyTo(frame(rectBob));

    cv::Mat bobWarppedMat = warpped_face_bob(rectAnn);

    center = cv::Point(rectAnn.width/2,rectAnn.height/2);
    cv::Mat anaSub = small_frame(rectAnn);
    cv::Mat maskAnnSub = mask_ann(rectAnn);

    cv::seamlessClone(bobWarppedMat,anaSub,maskAnnSub,center,output,cv::NORMAL_CLONE);

    output.copyTo(frame(rectAnn));
}

cv::Mat FaceSwap::getMinFrame(const cv::Mat &frame, cv::Rect &rect_ann, cv::Rect &rect_bob)
{
    cv::Rect bounding_rect = rect_ann | rect_bob;

    bounding_rect -= cv::Point(50, 50);
    bounding_rect += cv::Size(100, 100);

    bounding_rect &= cv::Rect(0, 0, frame.cols, frame.rows);

    this->rect_ann = rect_ann - bounding_rect.tl();
    this->rect_bob = rect_bob - bounding_rect.tl();

    big_rect_ann = ((this->rect_ann - cv::Point(rect_ann.width / 4, rect_ann.height / 4)) + cv::Size(rect_ann.width / 2, rect_ann.height / 2)) & cv::Rect(0, 0, bounding_rect.width, bounding_rect.height);
    big_rect_bob = ((this->rect_bob - cv::Point(rect_bob.width / 4, rect_bob.height / 4)) + cv::Size(rect_bob.width / 2, rect_bob.height / 2)) & cv::Rect(0, 0, bounding_rect.width, bounding_rect.height);

    return frame(bounding_rect);
}

cv::Mat FaceSwap::getMinFrame2(const cv::Mat &frame, cv::Rect &rect_ann, cv::Rect &rect_bob)
{

    this->rect_ann = rect_ann;
    this->rect_bob = rect_bob;

    big_rect_ann = ((this->rect_ann - cv::Point(rect_ann.width / 4, rect_ann.height / 4)) + cv::Size(rect_ann.width / 2, rect_ann.height / 2)) & cv::Rect(0, 0, frame.cols, frame.rows);
    big_rect_bob = ((this->rect_bob - cv::Point(rect_bob.width / 4, rect_bob.height / 4)) + cv::Size(rect_bob.width / 2, rect_bob.height / 2)) & cv::Rect(0, 0, frame.cols, frame.rows);

    return frame;
}

void FaceSwap::getFacePoints(const cv::Mat &frame)
{
    using namespace dlib;

    dlib_rects[0] = rectangle(rect_ann.x, rect_ann.y, rect_ann.x + rect_ann.width, rect_ann.y + rect_ann.height);
    dlib_rects[1] = rectangle(rect_bob.x, rect_bob.y, rect_bob.x + rect_bob.width, rect_bob.y + rect_bob.height);

    dlib::cv_image<dlib::bgr_pixel> dlib_frame = frame;

    shapes[0] = pose_model(dlib_frame, dlib_rects[0]);
    shapes[1] = pose_model(dlib_frame, dlib_rects[1]);

    auto getPoint = [&](int shape_index, int part_index) -> const cv::Point2i
    {
        const auto &p = shapes[shape_index].part(part_index);
        return cv::Point2i(p.x(), p.y());
    };

    points_ann[0] = getPoint(0, 0);
    points_ann[1] = getPoint(0, 3);
    points_ann[2] = getPoint(0, 5);
    points_ann[3] = getPoint(0, 8);
    points_ann[4] = getPoint(0, 11);
    points_ann[5] = getPoint(0, 13);
    points_ann[6] = getPoint(0, 16);

    cv::Point2i nose_length = getPoint(0, 27) - getPoint(0, 30);
    points_ann[7] = getPoint(0, 26) + nose_length;
    points_ann[8] = getPoint(0, 17) + nose_length;


    points_bob[0] = getPoint(1, 0);
    points_bob[1] = getPoint(1, 3);
    points_bob[2] = getPoint(1, 5);
    points_bob[3] = getPoint(1, 8);
    points_bob[4] = getPoint(1, 11);
    points_bob[5] = getPoint(1, 13);
    points_bob[6] = getPoint(1, 16);

    nose_length = getPoint(1, 27) - getPoint(1, 30);
    points_bob[7] = getPoint(1, 26) + nose_length;
    points_bob[8] = getPoint(1, 17) + nose_length;

    affine_transform_keypoints_ann[0] = points_ann[3];
    affine_transform_keypoints_ann[1] = getPoint(0, 36);
    affine_transform_keypoints_ann[2] = getPoint(0, 45);

    affine_transform_keypoints_bob[0] = points_bob[3];
    affine_transform_keypoints_bob[1] = getPoint(1, 36);
    affine_transform_keypoints_bob[2] = getPoint(1, 45);

    feather_amount.width = feather_amount.height = (int)cv::norm(points_ann[0] - points_ann[6]) / 8;
}

void FaceSwap::getFacePoints2(const cv::Mat &frame)
{
    using namespace dlib;

    dlib_rects[0] = rectangle(rect_ann.x, rect_ann.y, rect_ann.x + rect_ann.width, rect_ann.y + rect_ann.height);
    dlib_rects[1] = rectangle(rect_bob.x, rect_bob.y, rect_bob.x + rect_bob.width, rect_bob.y + rect_bob.height);

    dlib::cv_image<dlib::bgr_pixel> dlib_frame = frame;

    shapes[0] = pose_model(dlib_frame, dlib_rects[0]);
    shapes[1] = pose_model(dlib_frame, dlib_rects[1]);

    auto getPoint = [&](int shape_index, int part_index) -> const cv::Point2i
    {
        const auto &p = shapes[shape_index].part(part_index);
        return cv::Point2i(p.x(), p.y());
    };

    for(int i = 0; i < 2; ++i)
    {
        pointsVet[i].push_back( getPoint(i, 0) );
        pointsVet[i].push_back( getPoint(i, 3) );
        pointsVet[i].push_back( getPoint(i, 5) );
        pointsVet[i].push_back( getPoint(i, 8) );
        pointsVet[i].push_back( getPoint(i, 11) );
        pointsVet[i].push_back( getPoint(i, 13) );
        pointsVet[i].push_back( getPoint(i, 16) );

        cv::Point2i nose_length = getPoint(i, 27) - getPoint(i, 30);
        pointsVet[i].push_back( getPoint(i, 26) + nose_length );
        pointsVet[i].push_back( getPoint(i, 17) + nose_length );
    }

    affine_transform_keypoints_ann[0] = pointsVet[0][3];
    affine_transform_keypoints_ann[1] = getPoint(0, 36);
    affine_transform_keypoints_ann[2] = getPoint(0, 45);

    affine_transform_keypoints_bob[0] = pointsVet[1][3];
    affine_transform_keypoints_bob[1] = getPoint(1, 36);
    affine_transform_keypoints_bob[2] = getPoint(1, 45);

    feather_amount.width = feather_amount.height = (int)cv::norm(pointsVet[0][0] - pointsVet[0][6]) / 8;
}

void FaceSwap::getFacePoints(const cv::Mat &ann, const cv::Mat &bob)
{
    using namespace dlib;

    dlib_rects[0] = dlib::rectangle(rect_ann.x, rect_ann.y, rect_ann.x + rect_ann.width, rect_ann.y + rect_ann.height);
    dlib_rects[1] = dlib::rectangle(rect_bob.x, rect_bob.y, rect_bob.x + rect_bob.width, rect_bob.y + rect_bob.height);

    dlib::cv_image<dlib::bgr_pixel> dlib_ann = ann;
    dlib::cv_image<dlib::bgr_pixel> dlib_bob = bob;

    shapes[0] = pose_model(dlib_ann, dlib_rects[0]);
    shapes[1] = pose_model(dlib_bob, dlib_rects[1]);

    auto getPoint = [&](int shape_index, int part_index) -> const cv::Point2i
    {
        const auto &p = shapes[shape_index].part(part_index);
        return cv::Point2i(p.x(), p.y());
    };

    points_ann[0] = getPoint(0, 0);
    points_ann[1] = getPoint(0, 3);
    points_ann[2] = getPoint(0, 5);
    points_ann[3] = getPoint(0, 8);
    points_ann[4] = getPoint(0, 11);
    points_ann[5] = getPoint(0, 13);
    points_ann[6] = getPoint(0, 16);

    cv::Point2i nose_length = getPoint(0, 27) - getPoint(0, 30);
    points_ann[7] = getPoint(0, 26) + nose_length;
    points_ann[8] = getPoint(0, 17) + nose_length;


    points_bob[0] = getPoint(1, 0);
    points_bob[1] = getPoint(1, 3);
    points_bob[2] = getPoint(1, 5);
    points_bob[3] = getPoint(1, 8);
    points_bob[4] = getPoint(1, 11);
    points_bob[5] = getPoint(1, 13);
    points_bob[6] = getPoint(1, 16);

    nose_length = getPoint(1, 27) - getPoint(1, 30);
    points_bob[7] = getPoint(1, 26) + nose_length;
    points_bob[8] = getPoint(1, 17) + nose_length;

    affine_transform_keypoints_ann[0] = points_ann[3];
    affine_transform_keypoints_ann[1] = getPoint(0, 36);
    affine_transform_keypoints_ann[2] = getPoint(0, 45);

    affine_transform_keypoints_bob[0] = points_bob[3];
    affine_transform_keypoints_bob[1] = getPoint(1, 36);
    affine_transform_keypoints_bob[2] = getPoint(1, 45);

    feather_amount.width = feather_amount.height = (int)cv::norm(points_ann[0] - points_ann[6]) / 8;
}

void FaceSwap::getTransformationMatrices()
{
    trans_ann_to_bob = cv::getAffineTransform(affine_transform_keypoints_ann, affine_transform_keypoints_bob);
    cv::invertAffineTransform(trans_ann_to_bob, trans_bob_to_ann);
}

void FaceSwap::getMasks()
{
    mask_ann.setTo(cv::Scalar::all(0));
    mask_bob.setTo(cv::Scalar::all(0));

//    cv::fillConvexPoly(mask_ann, points_ann, 9, cv::Scalar(255));
//    cv::fillConvexPoly(mask_bob, points_bob, 9, cv::Scalar(255));

    cv::fillConvexPoly(mask_ann, pointsVet[0], cv::Scalar(255));
    cv::fillConvexPoly(mask_bob, pointsVet[1], cv::Scalar(255));
}

void FaceSwap::getWarppedMasks()
{
    cv::warpAffine(mask_ann, warpped_mask_ann, trans_ann_to_bob, frame_size, cv::INTER_NEAREST, cv::BORDER_CONSTANT, cv::Scalar(0));
    cv::warpAffine(mask_bob, warpped_mask_bob, trans_bob_to_ann, frame_size, cv::INTER_NEAREST, cv::BORDER_CONSTANT, cv::Scalar(0));
}

cv::Mat FaceSwap::getRefinedMasks()
{
    cv::bitwise_and(mask_ann, warpped_mask_bob, refined_ann_and_bob_warpped);
    cv::bitwise_and(mask_bob, warpped_mask_ann, refined_bob_and_ann_warpped);

    cv::Mat refined_masks(frame_size, CV_8UC1, cv::Scalar(0));
    refined_ann_and_bob_warpped.copyTo(refined_masks, refined_ann_and_bob_warpped);
    refined_bob_and_ann_warpped.copyTo(refined_masks, refined_bob_and_ann_warpped);

    return refined_masks;
}

cv::Mat FaceSwap::getRefinedMasks_ann()
{
    cv::bitwise_and(mask_ann, warpped_mask_bob, refined_ann_and_bob_warpped);
    cv::bitwise_and(mask_bob, warpped_mask_ann, refined_bob_and_ann_warpped);

    cv::Mat refined_masks(frame_size, CV_8UC1, cv::Scalar(0));
    refined_ann_and_bob_warpped.copyTo(refined_masks, refined_ann_and_bob_warpped);

    return refined_masks;
}

cv::Mat FaceSwap::getRefinedMasks_bob()
{

    cv::Mat refined_masks(frame_size, CV_8UC1, cv::Scalar(0));
    refined_bob_and_ann_warpped.copyTo(refined_masks, refined_bob_and_ann_warpped);

    return refined_masks;
}

void FaceSwap::extractFaces()
{
    small_frame.copyTo(face_ann, mask_ann);
    small_frame.copyTo(face_bob, mask_bob);
}

cv::Mat FaceSwap::getWarppedFaces()
{
    cv::Mat warpped_faces(frame_size, CV_8UC3, cv::Scalar::all(0));

    cv::warpAffine(face_ann, warpped_face_ann, trans_ann_to_bob, frame_size, cv::INTER_NEAREST, cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0));
    cv::warpAffine(face_bob, warpped_face_bob, trans_bob_to_ann, frame_size, cv::INTER_NEAREST, cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0));

    warpped_face_ann.copyTo(warpped_faces, warpped_mask_ann);
    warpped_face_bob.copyTo(warpped_faces, warpped_mask_bob);

    return warpped_faces;
}

void FaceSwap::colorCorrectFaces()
{
    specifiyHistogram(small_frame(big_rect_ann), warpped_faces(big_rect_ann), warpped_mask_bob(big_rect_ann));
    specifiyHistogram(small_frame(big_rect_bob), warpped_faces(big_rect_bob), warpped_mask_ann(big_rect_bob));
}

void FaceSwap::featherMask(cv::Mat &refined_masks)
{
    cv::erode(refined_masks, refined_masks, getStructuringElement(cv::MORPH_RECT, feather_amount), cv::Point(-1, -1), 1, cv::BORDER_CONSTANT, cv::Scalar(0));

    cv::blur(refined_masks, refined_masks, feather_amount, cv::Point(-1, -1), cv::BORDER_CONSTANT);
}

inline void FaceSwap::pasteFacesOnFrame()
{
    for (size_t i = 0; i < small_frame.rows; i++)
    {
        auto frame_pixel = small_frame.row(i).data;
        auto faces_pixel = warpped_faces.row(i).data;
        auto masks_pixel = refined_masks.row(i).data;

        for (size_t j = 0; j < small_frame.cols; j++)
        {
            if (*masks_pixel != 0)
            {
                *frame_pixel = ((255 - *masks_pixel) * (*frame_pixel) + (*masks_pixel) * (*faces_pixel)) >> 8; // divide by 256
                *(frame_pixel + 1) = ((255 - *(masks_pixel + 1)) * (*(frame_pixel + 1)) + (*(masks_pixel + 1)) * (*(faces_pixel + 1))) >> 8;
                *(frame_pixel + 2) = ((255 - *(masks_pixel + 2)) * (*(frame_pixel + 2)) + (*(masks_pixel + 2)) * (*(faces_pixel + 2))) >> 8;
            }

            frame_pixel += 3;
            faces_pixel += 3;
            masks_pixel++;
        }
    }
}

void FaceSwap::specifiyHistogram(const cv::Mat source_image, cv::Mat target_image, cv::Mat mask)
{

    std::memset(source_hist_int, 0, sizeof(int) * 3 * 256);
    std::memset(target_hist_int, 0, sizeof(int) * 3 * 256);

    for (size_t i = 0; i < mask.rows; i++)
    {
        auto current_mask_pixel = mask.row(i).data;
        auto current_source_pixel = source_image.row(i).data;
        auto current_target_pixel = target_image.row(i).data;

        for (size_t j = 0; j < mask.cols; j++)
        {
            if (*current_mask_pixel != 0) {
                source_hist_int[0][*current_source_pixel]++;
                source_hist_int[1][*(current_source_pixel + 1)]++;
                source_hist_int[2][*(current_source_pixel + 2)]++;

                target_hist_int[0][*current_target_pixel]++;
                target_hist_int[1][*(current_target_pixel + 1)]++;
                target_hist_int[2][*(current_target_pixel + 2)]++;
            }

            // Advance to next pixel
            current_source_pixel += 3;
            current_target_pixel += 3;
            current_mask_pixel++;
        }
    }

    // Calc CDF
    for (size_t i = 1; i < 256; i++)
    {
        source_hist_int[0][i] += source_hist_int[0][i - 1];
        source_hist_int[1][i] += source_hist_int[1][i - 1];
        source_hist_int[2][i] += source_hist_int[2][i - 1];

        target_hist_int[0][i] += target_hist_int[0][i - 1];
        target_hist_int[1][i] += target_hist_int[1][i - 1];
        target_hist_int[2][i] += target_hist_int[2][i - 1];
    }

    // Normalize CDF
    for (size_t i = 0; i < 256; i++)
    {
        source_histogram[0][i] = (source_hist_int[0][255] ? (float)source_hist_int[0][i] / source_hist_int[0][255] : 0);
        source_histogram[1][i] = (source_hist_int[1][255] ? (float)source_hist_int[1][i] / source_hist_int[1][255] : 0);
        source_histogram[2][i] = (source_hist_int[2][255] ? (float)source_hist_int[2][i] / source_hist_int[2][255] : 0);

        target_histogram[0][i] = (target_hist_int[0][255] ? (float)target_hist_int[0][i] / target_hist_int[0][255] : 0);
        target_histogram[1][i] = (target_hist_int[1][255] ? (float)target_hist_int[1][i] / target_hist_int[1][255] : 0);
        target_histogram[2][i] = (target_hist_int[2][255] ? (float)target_hist_int[2][i] / target_hist_int[2][255] : 0);
    }

    // Create lookup table

    auto binary_search = [&](const float needle, const float haystack[]) -> uint8_t
    {
        uint8_t l = 0, r = 255, m;
        while (l < r)
        {
            m = (l + r) / 2;
            if (needle > haystack[m])
                l = m + 1;
            else
                r = m - 1;
        }
        // TODO check closest value
        return m;
    };

    for (size_t i = 0; i < 256; i++)
    {
        LUT[0][i] = binary_search(target_histogram[0][i], source_histogram[0]);
        LUT[1][i] = binary_search(target_histogram[1][i], source_histogram[1]);
        LUT[2][i] = binary_search(target_histogram[2][i], source_histogram[2]);
    }

    // repaint pixels
    for (size_t i = 0; i < mask.rows; i++)
    {
        auto current_mask_pixel = mask.row(i).data;
        auto current_target_pixel = target_image.row(i).data;
        for (size_t j = 0; j < mask.cols; j++)
        {
            if (*current_mask_pixel != 0)
            {
                *current_target_pixel = LUT[0][*current_target_pixel];
                *(current_target_pixel + 1) = LUT[1][*(current_target_pixel + 1)];
                *(current_target_pixel + 2) = LUT[2][*(current_target_pixel + 2)];
            }

            // Advance to next pixel
            current_target_pixel += 3;
            current_mask_pixel++;
        }
    }
}

const dlib::shape_predictor &FaceSwap::getPose_model() const {
    return pose_model;
}

int FaceSwap::init(const std::string &landmarks_path) {
    try
    {
        dlib::deserialize(landmarks_path) >> pose_model;
    }
    catch (std::exception& e)
    {
        std::cerr << "Error loading landmarks from " << landmarks_path << std::endl
                  << "You can download the file from http://sourceforge.net/projects/dclib/files/dlib/v18.10/shape_predictor_68_face_landmarks.dat.bz2" << std::endl;
        return -1;
    }
    return 0;
}
