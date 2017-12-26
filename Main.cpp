//
// Created by vista on 2017/10/25.
//

#include "header/Utils.h"

#include "header/FaceDetect.h"

using namespace std;
using namespace cv;

int main(int argc, const char ** argv)
{
    try {

        // load model
        if( faceSwap.init(modelPath) == -1)
            return -1;

        string photo = "/Users/vista/Documents/PIC/searchimg/model/4.jpg";
        string modelpath = "/Users/vista/Documents/PIC/searchimg/model/17.jpg";

        swap_head_ex(photo,modelpath);

//        detect_face_and_swap(photo,modelpath);

    }catch (cv::Exception & cve){
        std::cerr << cve.what() << std::endl;
        return -1;
    }catch (std::exception & se){
        std::cerr << se.what() << std::endl;
        return -1;
    }

    return 0;
}