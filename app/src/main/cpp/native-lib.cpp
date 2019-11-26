#include <jni.h>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

extern "C" JNIEXPORT jintArray JNICALL
Java_com_ruixin_ndkstudy_inter_JNIUtils_bitmap2Grey(JNIEnv *env, jclass type, jintArray buf,
                                                    jint w, jint h) {
    jint *cbuf;
    cbuf = env->GetIntArrayElements(buf, JNI_FALSE);
    if(cbuf == nullptr){
        return nullptr;
    }

    Mat imgData(h, w, CV_8UC4, (unsigned char*)cbuf);

    uchar* ptr = imgData.ptr(0);
    for(int i = 0; i < w*h; i ++){
        int grayScale = (int)(ptr[4*i+2]*0.299F + ptr[4*i+1]*0.587F + ptr[4*i+0]*0.114F);
        ptr[4*i+1] = static_cast<uchar>(grayScale);
        ptr[4*i+2] = static_cast<uchar>(grayScale);
        ptr[4*i+0] = static_cast<uchar>(grayScale);
    }

    int size=w * h;
    jintArray result = env->NewIntArray(size);
    env->SetIntArrayRegion(result, 0, size, cbuf);
    env->ReleaseIntArrayElements(buf, cbuf, 0);
    return result;
}
