cmake_minimum_required(VERSION 3.9.3)
project(faceB)

set(CMAKE_CXX_STANDARD 11)

SET(CMAKE_BUILD_TYPE Debug)

find_package(OpenCV REQUIRED)

if (NOT OpenCV_FOUND)
    message(WARNING "OpenCV not found. project will not be built.")
else()
    message(STATUS ${OpenCV_INCLUDE_DIRS})
    include_directories(${OpenCV_INCLUDE_DIRS})
endif()

find_package(dlib REQUIRED)

if (NOT dlib_FOUND)
    message(WARNING "dlib not found. project will not be built.")
else()
#    message(STATUS ${dlib_INCLUDE_DIRS})
    include_directories(${dlib_INCLUDE_DIRS})
endif()

set(SEETA_FACE_LIBS /Users/vista/CLionProjects/SeetaFaceEngine/lib/libseeta_facedet_lib.dylib)

include_directories(../SeetaFaceEngine/FaceDetection/include)

set(source header/Utils.h src/FaceDetect.cpp src/Utils.cpp
        header/FaceDetect.h header/common.h
#        Main.cpp
        src/Face.cpp header/Face.h )

set(libfile ${source} interface.h interface.cpp)


#if you don't want to use openMP, you should comment this line
#FIND_PACKAGE( OpenMP REQUIRED)
#
#
#if(OPENMP_FOUND)
#    message("OPENMP FOUND")
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_C_FLAGS}")
#    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
#endif()

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

#add_executable(faceB ${source})

add_library(faceB SHARED ${libfile})

target_link_libraries(faceB ${OpenCV_LIBS} ${dlib_LIBS} ${SEETA_FACE_LIBS})
