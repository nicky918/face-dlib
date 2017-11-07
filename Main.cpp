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
    dlib::deserialize(modelPath) >> sp;

    string baseImgPath = "/Users/vista/Documents/PIC/searchimg/model/34.jpg";
    string baseImgPath2 = "/Users/vista/Documents/PIC/searchimg/model/33.jpg";

    detect_face_and_align(baseImgPath,baseImgPath2);
}