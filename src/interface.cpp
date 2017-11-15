//
// Created by vista on 2017/10/30.
//

#include "../header/interface.h"

bool bInit = false;

int init(const char *model1, const char* model2)
{
    modelPath = std::string(model1);
    fd_model = std::string(model2);

//    dlib::deserialize(modelPath) >> sp;
    if( faceSwap.init(modelPath) == -1 )
        return -1;

    bInit = true;

    return 0;
}


int swap_head(const char *a, const char *b){
    if(!bInit)
        return -2;
    return swap_head_ex(std::string(a),std::string(b));
}


int swap_face(const char *a, const char *b){
    if(!bInit)
        return -2;
    return detect_face_and_swap(std::string(a),std::string(b));
}

void hello()
{
    std::cout << "hello, face" << std::endl;
}

void set_outpath(const char *a) {
    out_path = std::string(a);
}
