#include <jni.h>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

extern "C" JNIEXPORT jintArray JNICALL
Java_com_ruixin_ndkstudy_inter_JNIUtils_bitmap2Grey(JNIEnv *env, jclass, jintArray buf,
                                                    jint w, jint h) {
    // 获得图片矩阵数组指针
    jint *srcBuf = env->GetIntArrayElements(buf, JNI_FALSE);
    if (srcBuf == nullptr) {
        return nullptr;
    }

    // 根据指针创建一个Mat 四个颜色通道
    Mat imgData(h, w, CV_8UC4, (unsigned char *) srcBuf);

    // 指向指定矩阵行的指针
    uchar *ptr = imgData.ptr(0);

    // 修改像素 ARGB
    for (int i = 0; i < w * h; i++) {
        int grayScale = (int) (ptr[4 * i + 2] * 0.299 + ptr[4 * i + 1] * 0.587 +
                               ptr[4 * i + 0] * 0.114);
        ptr[4 * i + 1] = static_cast<uchar>(grayScale);
        ptr[4 * i + 2] = static_cast<uchar>(grayScale);
        ptr[4 * i + 0] = static_cast<uchar>(grayScale);
    }

    int size = w * h;
    // 申请数组
    jintArray result = env->NewIntArray(size);
    // 数据复制到result数组
    env->SetIntArrayRegion(result, 0, size, srcBuf);
    // 释放C数组资源
    env->ReleaseIntArrayElements(buf, srcBuf, 0);
    return result;
}
