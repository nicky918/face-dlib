change head or swap face

use seetaface & dlib

1. git clone dlib from https://github.com/davisking/dlib
2. git clone seetaface from https://github.com/seetaface/SeetaFaceEngine
3. install opencv or from source https://github.com/opencv/opencv
4. build all of up
5. make sure dlib & seetaface in same parent folder as this project for config
6. linux system , use ``CMakeLists.txt.linux``
7. build

```
cd facedlib
mkdir release
cmake ..
make install -j4
```

8. get ``libfaceB.so``
9. use it.


# how to use

``` c
// model1 - file ``seeta_fd_frontal_v1.0.bin`` [@SeetaFaceEngine to find it]
// model2 - file of landmarks, you can download it from http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2
int init(const char * model1, const char* model2);

// a - filename : full path
set_outpath(const char *a);
```

u must run init at first and once

then run set_outpath

``` c
// return -1 if failed, otherwise success
// a - photo of head given : full path
// b - model of body : full path
int swap_head(const char *a, const char *b);

// return -1 if failed, otherwise success
// a - photo of face given : full path
// b - model of body : full path
int swap_face(const char *a, const char *b);
```
