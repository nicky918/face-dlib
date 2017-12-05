#include "../header/Utils.h"
using namespace std;
using namespace cv;

//dlib::shape_predictor sp;

cv::Rect GetPtsBound(std::vector<Point> pts)
{

    double min_h,min_w,max_h,max_w;
    min_h = min_w = 10000.0;
    max_h = max_w = -1;

    size_t count = pts.size();
    for(int i = 0; i < count; ++i)
    {
        if( pts[i].x < min_w )
        {
            min_w = pts[i].x;
        }

        if( pts[i].x > max_w )
        {
            max_w = pts[i].x;
        }

        if( pts[i].y < min_h )
        {
            min_h = pts[i].y;
        }

        if( pts[i].y > max_h )
        {
            max_h = pts[i].y;
        }
    }

    return cv::Rect( cv::Point2d(min_w,min_h) , cv::Point2d(max_w+1,max_h+1) );
}

Mat fix_bound(std::vector<cv::Point> pts, Rect rect) {

    Mat mask_img = Mat(rect.size(),CV_8U);

    for (int h = 0; h < rect.height; ++h) {
        uchar * data = mask_img.ptr<uchar>(h);
        for (int w = 0; w < rect.width; ++w) {
            if( cv::pointPolygonTest( pts, Point(w + rect.x, h + rect.y) , false ) >= 0 )
            {
                *(data + w) = 255;

                if( w + 1 < rect.width )
                    *(data+w+1) = 255;

                if( w - 1 >= 0)
                    *(data+w-1) = 255;

                if( h > 0 )
                    *(mask_img.ptr<uchar>(h-1)+w) = 255;

                if ( h+1 < rect.height )
                    *(mask_img.ptr<uchar>(h+1)+w) = 255;
            }
        }
    }

    return mask_img;
}


