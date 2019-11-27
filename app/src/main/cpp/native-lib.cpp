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

    // ----------------------------------------

    // 灰度处理

    // 指向指定矩阵行的指针
//    uchar *ptr = imgData.ptr(0);

    // 修改像素 ARGB
//    for (int i = 0; i < w * h; i++) {
//        int grayScale = (int) (ptr[4 * i + 2] * 0.299 + ptr[4 * i + 1] * 0.587 +
//                               ptr[4 * i + 0] * 0.114);
//        ptr[4 * i + 1] = static_cast<uchar>(grayScale);
//        ptr[4 * i + 2] = static_cast<uchar>(grayScale);
//        ptr[4 * i + 0] = static_cast<uchar>(grayScale);
//    }

    // ----------------------------------------

    // 进行平滑操作，可以使用GaussianBlur()、blur()、medianBlur()或bilateralFilter()
//    GaussianBlur(imgData, imgData, cv::Size(51, 51), 20, 20);

    // ----------------------------------------
/*

    // 灰度处理
    Mat dst, mGray, edge;

    //创建与src同类型和大小的矩阵
    dst.create(imgData.size(), imgData.type());

    cvtColor(imgData, mGray, COLOR_BGRA2GRAY);

    blur(mGray, edge, Size(3, 3));

    //使用canny算子
    Canny(edge, edge, 3, 9, 3);

    // 恢复格式
    cvtColor(edge, imgData, COLOR_GRAY2BGRA);
*/

    // ----------------------------------------

    //变量定义
    Mat src_gray, dst, abs_dst;

    //使用高斯滤波消除噪声
    GaussianBlur(imgData, imgData, Size(3, 3), 0, 0, BORDER_DEFAULT);
    //转换为灰度图
    cvtColor(imgData, src_gray, COLOR_RGBA2GRAY);
    //使用Laplace函数
    Laplacian(src_gray, dst, CV_16S, 3, 1, 0, BORDER_DEFAULT);
    //计算绝对值
    convertScaleAbs(dst, abs_dst);

    // 恢复格式
    //cvtColor(abs_dst, imgData, COLOR_GRAY2BGRA);

    Mat dst2;
    //将dstImage内所有元素为0
    dst2 = Scalar::all(0);

    //使用Laplacian算子输出的边缘图，abs_dst作为掩码，来将原图imgData拷贝到目标图dst2中
    imgData.copyTo(dst2, abs_dst);

    int size = w * h;

    jint *re = (jint *) dst2.data;

    jintArray result = env->NewIntArray(size);
    env->SetIntArrayRegion(result, 0, size, re);
    env->ReleaseIntArrayElements(buf, srcBuf, 0);

    // ----------------------------------------

/*    int size = w * h;
    // 申请数组
    jintArray result = env->NewIntArray(size);
    // 数据复制到result数组
    env->SetIntArrayRegion(result, 0, size, srcBuf);
    // 释放C数组资源
    env->ReleaseIntArrayElements(buf, srcBuf, 0);*/
    return result;
}
