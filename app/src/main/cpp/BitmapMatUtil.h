//
// Created by mayn on 2019/11/28 0028.
//

#ifndef NDKSTUDY_BITMAPMATUTIL_H
#define NDKSTUDY_BITMAPMATUTIL_H

#include <android/bitmap.h>
#include "WlAndroidLog.h"
#include "opencv2/opencv.hpp"

using namespace cv;

class BitmapMatUtil {

public:
    static void BitmapToMat(JNIEnv *env, jobject& bitmap, Mat& mat);

    static void MatToBitmap(JNIEnv *env, Mat& mat, jobject& bitmap);

public:
    BitmapMatUtil();
    ~BitmapMatUtil();
};


#endif //NDKSTUDY_BITMAPMATUTIL_H
