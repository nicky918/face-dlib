//
// Created by vista on 2017/10/30.
//

#include "../header/interface.h"

bool bInit = false;

int init(const char *model1, const char* model2,const char *outpath)
{
    modelPath = std::string(model1);
    fd_model = std::string(model2);
    out_path = std::string(outpath);

//    dlib::deserialize(modelPath) >> sp;
    if( faceSwap.init(modelPath) == -1 )
        return -1;

    return 0;
}


int swap_head(const char *a, const char *b){
    return swap_head_ex(std::string(a),std::string(b));
}


int swap_face(const char *a, const char *b){
    return detect_face_and_swap(std::string(a),std::string(b));
}

void hello()
{
    std::cout << "hello, face" << std::endl;
}