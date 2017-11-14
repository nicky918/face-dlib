//
// Created by vista on 2017/10/25.
//

#include "header/Utils.h"

#include "header/FaceDetect.h"

using namespace std;
using namespace cv;

int main(int argc, const char ** argv)
{
    // load model
    if( faceSwap.init(modelPath) == -1)
        return -1;

    string photo = "/Users/vista/Documents/PIC/searchimg/model/41.jpg";
    string modelpath = "/Users/vista/Documents/PIC/searchimg/model/45.jpg";

//    detect_face_and_align(baseImgPath,baseImgPath2);

    detect_face_and_swap(photo,modelpath);

    return 0;
}