//
// Created by vista on 2017/10/30.
//

#ifndef FACEA_INTERFACE_H
#define FACEA_INTERFACE_H

#include "common.h"
#include "FaceDetect.h"

extern bool bInit;

extern "C" {
    /*
     * return 0 if success , otherwise failed.
     * */
    int init(const char *model1, const char* model2,const char *outpath);

    void hello();

    int swap_head(const char *a, const char *b);

    int swap_face(const char *a, const char *b);
};


#endif //FACEA_INTERFACE_H
