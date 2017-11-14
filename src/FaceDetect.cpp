#include "../header/FaceDetect.h"

using namespace std;

using namespace cv;

FaceSwap faceSwap;

int detect_face_and_swap(string photo, string modelpath)
{
    cv::Mat img,img2;
    cv::Rect rect,rect2;
    int ret = detect_face_rect(photo,img,rect);

    if(ret == -1)
    {
        std::cerr << photo << ": face detected failed" << std::endl;
        return -1;
    }

    ret = detect_face_rect(modelpath,img2,rect2);

    if(ret == -1)
    {
        std::cerr << modelpath << ":face detected failed" << std::endl;
        return -1;
    }

    rect2.x += img.cols;

    int height = std::max(img.rows,img2.rows);

    cv::Mat mixedimg = cv::Mat(cv::Size(img.cols+img2.cols,height),CV_8UC3, cv::Scalar::all(0));

    Mat imageROI1 = mixedimg(cv::Rect(0,0,img.cols,img.rows));
    Mat imageROI2 = mixedimg(cv::Rect(img.cols,0,img2.cols,img2.rows));

    img.copyTo(imageROI1);
    img2.copyTo(imageROI2);

    faceSwap.swapFaces2(mixedimg,rect,rect2);

    imageROI2 = mixedimg(cv::Rect(img.cols,0,img2.cols,img2.rows));

    cv::imwrite(out_path,imageROI2);

    return 0;
}

int swap_head_ex(string first_file_path, string second_file_path){

    double scale  = 1.0;

    Mat image = imread( first_file_path );
    if (image.empty()){
        cerr << __FILE__ << __FUNCTION__ << __LINE__ << endl;
        cerr << "read image fail" << endl;
        return -1;
    }
    Mat image2 = imread( second_file_path );
    if(image2.empty())
    {
        cerr << __FILE__ << __FUNCTION__ << __LINE__ << endl;
        cerr << "read image fail" << endl;
        return -1;
    }

//    dlib::array2d<dlib::rgb_pixel> dimg,dimg2;

//    dlib::load_image(dimg,first_file_path);
//
//    dlib::load_image(dimg2,second_file_path);

    dlib::cv_image<dlib::rgb_pixel> dimg,dimg2;

    dimg = image;

    dimg2 = image2;

    Face face,face2;
//    Mat img_proc ;
    if( detect_and_draw( image,dimg,face,scale ) < 0)
    {
        cerr << __FILE__ << __FUNCTION__ << __LINE__ << endl;
        cerr << "ERROR: Found 0 Face (1)" << endl;
        return -1;
    }
    face.GetOutLine(image);
    face.get_head_img(image);

//    Mat img_proc2;
    if( detect_and_draw( image2,dimg2, face2,scale ) < 0 )
    {
        cerr << __FILE__ << __FUNCTION__ << __LINE__ << endl;
        cerr << "ERROR: Found 0 Face (2)" << endl;
        return -1;
    }
    face2.GetOutLine(image2);
    face2.get_head_img(image2);

//    cv::polylines(image,face.getFacePts(),true,cv::Scalar(255,0,0));

//    cv::polylines(image2,face2.getHead_pts(),true,cv::Scalar(255,0,0));

//    cv::imshow("head",image);
//    cv::imshow("head2",image2);
//    waitKey(0);

    cv::Size newsize = cv::Size( face2.getFace_bound().width * face.getHead_bound().width / face.getFace_bound().width,
                                 face2.getFace_bound().width * face.getHead_bound().height / face.getFace_bound().width );

    int centerw = face2.getFace_bound().x + face2.getFace_bound().width/2;

    Mat nsImg , mask_resize_img;
    cv::resize(face.getHead_img(),nsImg,newsize);
    cv::resize(face.getMask_img(),mask_resize_img,newsize);

    int h0 = face2.getHead_bound().y + face2.getHead_bound().height - newsize.height;
    int w0 = centerw - newsize.width / 2;

    Mat dst = Mat( image2.size(), image2.type()  );

    image2.copyTo(dst);

    Mat face2_mask_img = face2.fill_bg(dst);

    face2.setMask_img(face2_mask_img);

//    for (int h = 0; h < face2.getMask_img().rows; ++h) {
//        for (int w = 0; w < face2.getMask_img().cols; ++w) {
//            if( *face2.getMask_img().ptr<uchar>(h,w) == 255 )
//            {
//                cv::circle( dst, Point( w + face2.getHead_bound().x , h + face2.getHead_bound().y ) , 0 , Scalar(0,0,192) );
//            }
//        }
//    }


//    for (int h = 0; h < mask_resize_img.rows; ++h) {
//        for (int w = 0; w < mask_resize_img.cols; ++w) {
//            if( *mask_resize_img.ptr<uchar>(h,w) == 255 )
//            {
//                cv::circle( dst, Point( w + w0 , h + h0 ) , 1 , Scalar(0,128,128,30) );
//            }
//        }
//    }

//    cv::imshow("dst",dst);

//    imshow("img",img_proc2);

    nsImg.copyTo(dst(cv::Rect(w0,h0,newsize.width,newsize.height)),mask_resize_img);

    Mat inMask = Mat::zeros( dst.size(), CV_8U  );

    face2.gen_mask_img( inMask, Point(w0,h0), mask_resize_img );


    Mat in_paint_img/* = Mat( dst.size(), dst.type() )*/;

    inpaint(dst,inMask,in_paint_img,3,CV_INPAINT_TELEA);

    cv::imwrite(out_path,in_paint_img);

//    cv::resize(in_paint_img,in_paint_img,cv::Size(in_paint_img.cols/2,in_paint_img.rows/2));

//    cv::imshow("in_paint_img",in_paint_img);

//    for (int h = 0; h < inMask.rows; ++h) {
//        for (int w = 0; w < inMask.cols; ++w) {
//            if( *inMask.ptr<uchar>(h,w) == 255 )
//            {
//                cv::circle( dst , Point( w , h ) , 0 , Scalar(0,128,0) );
//            }
//        }
//    }

//    cv::imshow("dst",dst);


//    for (int h = 0; h < face2.getMask_img().rows; ++h) {
//        for (int w = 0; w < face2.getMask_img().cols; ++w) {
//            if( *face2.getMask_img().ptr<uchar>(h,w) == 255 )
//            {
//                cv::circle( img_proc2, Point( w + face2.getHead_bound().x , h + face2.getHead_bound().y ) , 1 , Scalar(0,0,255) );
//            }
//        }
//    }

//    for (int h = 0; h < mask_resize_img.rows; ++h) {
//        for (int w = 0; w < mask_resize_img.cols; ++w) {
//            if( *mask_resize_img.ptr<uchar>(h,w) == 255 )
//            {
//                cv::circle( img_proc2, Point( w + w0 , h + h0 ) , 1 , Scalar(0,0,255,30) );
//            }
//        }
//    }


//    for (int h = 0; h < inMask.rows; ++h) {
//        for (int w = 0; w < inMask.cols; ++w) {
//            if( *inMask.ptr<uchar>(h,w) == 255 )
//            {
//                cv::circle( img_proc2, Point( w , h ) , 1 , Scalar(0,0,255,30) );
//            }
//        }
//    }

//    cv::polylines( img_proc2,face2.getHead_pts(),true,cv::Scalar(255,0,0));

//    imshow("result",img_proc2);


//    waitKey(0);
//
//    cvDestroyWindow("model");
//    cvDestroyWindow("result");

    return 0;
}

int detect_face_rect(std::string path,cv::Mat & image, cv::Rect & rect)
{
    image = imread( path );
    if (image.empty()){
        cerr << __FILE__ << __FUNCTION__ << __LINE__ << endl;
        cerr << "read image fail" << endl;
        return -1;
    }

    Mat gray;

    dlib::rectangle face_r;

    int ret = detect_by_seetface(image,face_r,gray);

    if(ret == -1)
        return -1;

    rect = cv::Rect((int)face_r.left(),(int)face_r.top(),(int)face_r.width(),(int)face_r.height());

    return 0;

}

int detect_and_draw( Mat& img, dlib::array2d<dlib::rgb_pixel> & img_dlib,
                    Face & face,
                    double scale ){
    int i = 0;
    int64 t1 = 0,t2 = 0;
    dlib::rectangle face_r;

    Mat gray;

    int ret = detect_by_seetface(img,face_r,gray);

    if(ret == -1)
        return -1;

    t1 = cvGetTickCount();

    dlib::full_object_detection shape = faceSwap.getPose_model()(img_dlib, face_r);

    t2 = cvGetTickCount() ;

//    face.get_chin_eyebrow(current_shape);
    face.get_chin_eyebrow(shape);


    return 0;
}

int detect_and_draw( Mat& img, dlib::cv_image<dlib::rgb_pixel> & img_dlib,
                     Face & face,
                     double scale ){
    int i = 0;
    int64 t1 = 0,t2 = 0;
    dlib::rectangle face_r;

    Mat gray;

    int ret = detect_by_seetface(img,face_r,gray);

    if(ret == -1)
        return -1;

    t1 = cvGetTickCount();

    dlib::full_object_detection shape = faceSwap.getPose_model()(img_dlib, face_r);

    t2 = cvGetTickCount() ;

//    face.get_chin_eyebrow(current_shape);
    face.get_chin_eyebrow(shape);


    return 0;
}


int detect_by_seetface(const Mat &img, dlib::rectangle &face, cv::Mat & gallery_img_gray) {
    // Initialize face detection model
    seeta::FaceDetection seet_detector( fd_model.c_str() );
    seet_detector.SetMinFaceSize(20);
    seet_detector.SetScoreThresh(2.f);
    seet_detector.SetImagePyramidScaleFactor(0.8f);
    seet_detector.SetWindowStep(4, 4);

    //先将彩色图转换为灰色图
    cv::cvtColor(img, gallery_img_gray, CV_BGR2GRAY);

//    ImageData gallery_img_data_color(img.cols, img.rows, img.channels());
//    gallery_img_data_color.data = img.data;

    ImageData gallery_img_data_gray(gallery_img_gray.cols, gallery_img_gray.rows, gallery_img_gray.channels());
    gallery_img_data_gray.data = gallery_img_gray.data;

    // Detect faces
    std::vector<seeta::FaceInfo> gallery_faces = seet_detector.Detect(gallery_img_data_gray);
    int32_t gallery_face_num = static_cast<int32_t>(gallery_faces.size());

    if(gallery_face_num == 0)
        return -1;

    int index = 0;
    double score = 0;
    for (int i = 0; i < gallery_faces.size(); ++i) {
        if( gallery_faces[i].score > score )
        {
            score = gallery_faces[i].score;
            index = i;
        }
    }

    face.set_left(gallery_faces[index].bbox.x);
    face.set_top(gallery_faces[index].bbox.y);
    face.set_right(gallery_faces[index].bbox.x + gallery_faces[index].bbox.width - 1);
    face.set_bottom(gallery_faces[index].bbox.y + gallery_faces[index].bbox.height - 1);

    return 0;
}

