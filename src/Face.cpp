//
// Created by vista on 2017/10/25.
//

#include "../header/Face.h"


void Face::get_chin_eyebrow(const dlib::full_object_detection &shape)
{
    left_eyebrow.clear();
    right_eyebrow.clear();
    chin.clear();
    facePts.clear();

    for (int i = 0; i < chin_size; ++i) {
        chin.push_back( cv::Point((int)shape.part(i).x(),(int)shape.part(i).y()) );
//        face_pts[i] = chin[i];
        facePts.push_back(chin[i]);
    }
    for (int i = 0 ; i < left_eyebrow_size; ++i) {
        left_eyebrow.push_back( cv::Point((int)shape.part(i + chin_size).x(),(int)shape.part(i + chin_size).y()) );
//        left_eyebrow.push_back( cv::Point((int)vec(i + chin_size,0),(int)vec(i + chin_size ,1)) );
//        left_eyebrow[i - 16] = Point2d(vec(i,0),vec(i,1));
    }
    for (int i = 0 ; i < right_eyebrow_size; ++i) {
        right_eyebrow.push_back( cv::Point((int)shape.part(i + chin_size+ left_eyebrow_size).x(),(int)shape.part(i + chin_size+ left_eyebrow_size).y()) );
//        right_eyebrow.push_back( cv::Point((int)vec(i + chin_size + left_eyebrow_size,0),(int)vec(i + chin_size + left_eyebrow_size ,1)) );
//        right_eyebrow[i - 22] = Point2d(vec(i,0),vec(i,1));
    }
    for (int j = 0; j < 5; ++j) {
//        face_pts[j+17] = right_eyebrow[4-j];
        facePts.push_back(right_eyebrow[4-j]);
    }
    for (int j = 0; j < 5; ++j) {
//        face_pts[j+22] = left_eyebrow[4-j];
        facePts.push_back(left_eyebrow[4-j]);
    }

//    face_bound = GetPtsBound(face_pts,27);
    face_bound = GetPtsBound(facePts);

}

void Face::get_chin_eyebrow(Mat1d vec){

    left_eyebrow.clear();
    right_eyebrow.clear();
    chin.clear();
    facePts.clear();

    for (int i = 0; i < chin_size; ++i) {
        chin.push_back( cv::Point((int)vec(i,0),(int)vec(i,1)) );
//        face_pts[i] = chin[i];
        facePts.push_back(chin[i]);
    }
    for (int i = 0 ; i < left_eyebrow_size; ++i) {
        left_eyebrow.push_back( cv::Point((int)vec(i + chin_size,0),(int)vec(i + chin_size ,1)) );
//        left_eyebrow[i - 16] = Point2d(vec(i,0),vec(i,1));
    }
    for (int i = 0 ; i < right_eyebrow_size; ++i) {
        right_eyebrow.push_back( cv::Point((int)vec(i + chin_size + left_eyebrow_size,0),(int)vec(i + chin_size + left_eyebrow_size ,1)) );
//        right_eyebrow[i - 22] = Point2d(vec(i,0),vec(i,1));
    }
    for (int j = 0; j < 5; ++j) {
//        face_pts[j+17] = right_eyebrow[4-j];
        facePts.push_back(right_eyebrow[4-j]);
    }
    for (int j = 0; j < 5; ++j) {
//        face_pts[j+22] = left_eyebrow[4-j];
        facePts.push_back(left_eyebrow[4-j]);
    }

//    face_bound = GetPtsBound(face_pts,27);
    face_bound = GetPtsBound(facePts);
}

void Face::GetOutLine(Mat & img) {
    Rect face_bound_ex = Rect( Point2d( std::max(face_bound.x - face_bound.width,0) ,
                                        std::max(face_bound.y - face_bound.height,0) ) ,
                               Size2i ( std::min(img.cols, std::min(face_bound.width * 3, img.cols - face_bound.x + face_bound.width) ),
                                        std::min(img.rows, std::min(face_bound.height * 3, img.rows - face_bound.y + face_bound.height) ) ) );

    Mat cropImg(img,face_bound_ex);

    Mat gray, canny ;
    cv::cvtColor(cropImg,gray,cv::COLOR_BGR2GRAY);
    cv::Canny(gray,canny,40,100);

//    imshow("sdf",canny);
//    waitKey(0);

//    std::vector<Point2d> outline[2] ;

    outline[0].clear();
    outline[1].clear();

    int step = 1;

    int nr = canny.rows;
    int nc = canny.channels() * canny.cols;

    for (int h = 0; h < nr ; h += step) {
        uchar * data = canny.ptr<uchar>(h);
        for (int w = 0; w < nc ; ++w) {
            if( *(data+w) > 150){
                outline[0].push_back( Point2d(w + face_bound_ex.x ,h + face_bound_ex.y ) );
                break;
            }
        }
        for (int w = nc - 1; w >= 0; --w) {
            if( *(data+w) > 150){
                outline[1].push_back( Point2d(w + face_bound_ex.x ,h + face_bound_ex.y ) );
                break;
            }
        }
    }

    outline[1].reserve(outline[1].size());

}

const Rect &Face::getFace_bound() const {
    return face_bound;
}

void Face::get_head_img(Mat &img) {

    int center_of_face = face_bound.y + face_bound.height * 2 / 3 ;

    head_pts.clear();

    size_t outline_0_size = outline[0].size();
    size_t outline_1_size = outline[1].size();
    for(int i = 0; i < outline_0_size; ++i)
    {
        if( outline[0][i].y > center_of_face )
            break;
        head_pts.push_back( outline[0][i] );
    }
    for(int i = 0; i < chin_size; ++i)
    {
        if( chin[i].y > center_of_face )
            head_pts.push_back( chin[i] );
    }
    for(int i = (int)outline_1_size - 1; i >= 0; --i)
    {
        if( outline[1][i].y <= center_of_face )
            head_pts.push_back( outline[1][i] );
    }

    head_bound = GetPtsBound(head_pts);

//    cv::polylines(img,head_pts,true,cv::Scalar(255,0,0));
//
//    cv::imshow("head",img);
//    waitKey(0);

    head_img = Mat(img,head_bound);

//    cv::imshow("head",head_img);
//    waitKey(0);

    mask_img = Mat( cv::Size(head_img.cols,head_img.rows) ,CV_8U);

    int nr = mask_img.rows;
    int nc = mask_img.cols * mask_img.channels();

    for (int h = 0 ; h < nr ; ++h) {
        uchar * data = mask_img.ptr<uchar>(h);
        for (int w = 0; w < nc ; ++w) {
            if( cv::pointPolygonTest(head_pts,Point2d(w+head_bound.x,h+head_bound.y), false ) < 0 )
            {
                *(data+w) = 0;
            }else
                *(data+w) = 255;
        }
    }

//    cv::imshow("mask",mask_img);
//    waitKey(0);
}

const Rect &Face::getHead_bound() const {
    return head_bound;
}

const Mat &Face::getHead_img() const {
    return head_img;
}

const Mat &Face::getMask_img() const {
    return mask_img;
}

const std::vector<cv::Point> &Face::getHead_pts() const {
    return head_pts;
}

Mat Face::fill_bg(Mat &img) {
    Mat mask_img = fix_bound(head_pts,head_bound);

    for (int h = 0; h < mask_img.rows; ++h) {

        uchar * data = img.ptr<uchar>( head_bound.y + h );
        uchar * mask_data = mask_img.ptr<uchar>(h);

        int left = head_bound.x - 10;
        int right = head_bound.x + head_bound.width + 10;

        int count = right - left + 1;

        int b0 = *(data + left * img.channels() );
        int g0 = *(data + left * img.channels() + 1);
        int r0 = *(data + left * img.channels() + 2);

        int b1 = *(data + right * img.channels() );
        int g1 = *(data + right * img.channels() + 1);
        int r1 = *(data + right * img.channels() + 2);

//        uchar b = (uchar) ( ((int)b0 + (int)b1)/2 );
//        uchar g = (uchar) ( ((int)g0 + (int)g1)/2 );
//        uchar r = (uchar) ( ((int)r0 + (int)r1)/2 );

        for(int w = 0; w < mask_img.cols; ++w)
        {
            if( *(mask_data + w) == 255) {
                *(data + (w + head_bound.x) * img.channels()) = (uchar)(b0 + (b1-b0) * (w+head_bound.x-left+1) / count);
                *(data + (w + head_bound.x) * img.channels() + 1) = (uchar)(g0 + (g1-g0) * (w+head_bound.x-left+1) / count);
                *(data + (w + head_bound.x) * img.channels() + 2) = (uchar)(r0 + (r1-r0) * (w+head_bound.x-left+1) / count);
            }
        }
    }

    return mask_img;
}

void Face::gen_mask_img(Mat & mat,cv::Point pt,Mat & mask) {

    int face_top = face_bound.y + face_bound.height * 2 / 3;

    cv::Point pt0( head_bound.x, head_bound.y);

    for (int h = 0; h < mask_img.rows; ++h) {
        uchar * data = mask_img.ptr<uchar>(h);
        uchar * data0 = mat.ptr<uchar>(h+pt0.y);
        for (int w = 0; w < mask_img.cols; ++w) {
            if( h + pt0.y > face_top && *(data+w) == 255 )
            {
                *(data0+w+pt0.x) = 255;
            }
        }
    }

    for (int h = 0; h < mask.rows; ++h) {
        uchar * data = mask.ptr<uchar>(h);
        uchar * data0 = mat.ptr<uchar>(h+pt.y);
        for (int w = 0; w < mask.cols; ++w) {
            if( /*h + pt.y > face_top &&*/ *(data+w) == 255 )
            {
                *(data0+w+pt.x) = 0;
            }
        }
    }
}

void Face::setMask_img(const Mat &mask_img) {
    Face::mask_img = mask_img;
}

const std::vector<cv::Point, std::allocator<cv::Point>> &Face::getFacePts() const {
    return facePts;
}

const vector<cv::Point, std::allocator<cv::Point>> &Face::getLeft_eyebrow() const {
    return left_eyebrow;
}

const vector<cv::Point, std::allocator<cv::Point>> &Face::getRight_eyebrow() const {
    return right_eyebrow;
}

const vector<cv::Point, std::allocator<cv::Point>> &Face::getChin() const {
    return chin;
}
