//
// Created by vista on 2017/10/30.
//

#include "interface.h"

int init(const char *model1, const char* model2,const char *outpath)
{
    modelPath = std::string(model1);
    fd_model = std::string(model2);
    out_path = std::string(outpath);

    dlib::deserialize(modelPath) >> sp;

    return 0;
}


void run(const char *a, const char *b){
    detect_face_and_align(std::string(a),std::string(b));
}

void hello()
{
    std::cout << "hello, face" << std::endl;
}