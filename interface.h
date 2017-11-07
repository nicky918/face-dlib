//
// Created by vista on 2017/10/30.
//

#ifndef FACEA_INTERFACE_H
#define FACEA_INTERFACE_H

#include "header/common.h"
#include "header/FaceDetect.h"

extern "C" {
    /*
     * return 0 if success , otherwise failed.
     * */
    int init(const char *model1, const char* model2,const char *outpath);

    void hello();

    void run(const char *a, const char *b);
};


#endif //FACEA_INTERFACE_H
