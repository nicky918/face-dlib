# change head

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
