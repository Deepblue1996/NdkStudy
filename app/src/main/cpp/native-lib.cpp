#include <jni.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/imgproc/types_c.h>
#include "myRect.h"
#include "BitmapMatUtil.h"
#include "WlAndroidLog.h"
#include "BitmapMatUtil.h"
#include <android/bitmap.h>

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
/*
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
    */

    // ----------------------------------------

    //对图像进行处理，转化为灰度图然后再转为二值图
    Mat grayImage;
    cvtColor(imgData, grayImage, COLOR_BGRA2GRAY);
    Mat binImage;
    //第4个参数为CV_THRESH_BINARY_INV是因为我的输入原图为白底黑字
    //若为黑底白字则选择CV_THRESH_BINARY即可
    threshold(grayImage, binImage, 100, 255, CV_THRESH_BINARY_INV);

    cvtColor(binImage, imgData, COLOR_GRAY2BGRA);

    // ----------------------------------------

    int size = w * h;
    // 申请数组
    jintArray result = env->NewIntArray(size);
    // 数据复制到result数组
    env->SetIntArrayRegion(result, 0, size, srcBuf);
    // 释放C数组资源
    env->ReleaseIntArrayElements(buf, srcBuf, 0);
    return result;
}

//求图片的像素和
int getPiexSum(Mat &image) {
    int sum = 0;
    for (int i = 0; i < image.cols; i++) {
        for (int j = 0; j < image.rows; j++) {
            sum += image.at<uchar>(j, i);
        }
    }
    return sum;
}

// 获取对比模块的边缘 Mat
Mat getContours(Mat grayImage) {
    Mat conImage = Mat::zeros(grayImage.size(), grayImage.type());
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    //指定CV_RETR_EXTERNAL寻找数字的外轮廓
    findContours(grayImage, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    //绘制轮廓
    drawContours(conImage, contours, -1, 255);

    //将每个数字，分离开，保存到容器中
    vector<myRect> sort_rect;
    for (int i = 0; i < contours.size(); i++) {
        //boundingRect返回轮廓的外接矩阵
        Rect tempRect = boundingRect(contours[i]);
        sort_rect.emplace_back(tempRect);
    }

    //对矩形进行排序，因为轮廓的顺序不一定是数字真正的顺序
    for (int i = 0; i < sort_rect.size(); i++) {
        for (int j = i + 1; j < sort_rect.size(); j++) {
            if (sort_rect[j] < sort_rect[i]) {
                myRect temp = sort_rect[j];
                sort_rect[j] = sort_rect[i];
                sort_rect[i] = temp;
            }
        }
    }

    // 分割打印
    for (int k = 0; k < sort_rect.size(); k++) {
        LOGD("子分割第%d个: x:%d y:%d w:%d h:%d", k, sort_rect[k].getRect().x, sort_rect[k].getRect().y,
             sort_rect[k].getRect().width, sort_rect[k].getRect().height);
    }

    Mat ROI;
    ROI = conImage(sort_rect[0].getRect());
    Mat dstROI = Mat::zeros(grayImage.size(), grayImage.type());
    resize(ROI, dstROI, grayImage.size(), 0, 0, INTER_NEAREST);
    return dstROI;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_ruixin_ndkstudy_inter_JNIUtils_findNumber(JNIEnv *env, jclass, jobject bitmap,
                                                   jobjectArray bitmapBuf) {

    Mat imgData = BitmapMatUtil::bitmap2Mat(env, bitmap);//图片源矩阵初始化

    //对图像进行处理，转化为灰度图然后再转为二值图
    Mat grayImage;
    cvtColor(imgData, grayImage, COLOR_BGRA2GRAY);
    Mat binImage;
    //第4个参数为CV_THRESH_BINARY_INV是因为我的输入原图为白底黑字
    //若为黑底白字则选择CV_THRESH_BINARY即可
    threshold(grayImage, binImage, 100, 255, CV_THRESH_BINARY_INV);

    //寻找轮廓，必须指定为寻找外部轮廓，不然一个数字可能有多个轮廓组成，比如4,6,8,9等数字
    Mat conImage = Mat::zeros(binImage.size(), binImage.type());
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    //指定CV_RETR_EXTERNAL寻找数字的外轮廓
    findContours(binImage, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    //绘制轮廓
    drawContours(conImage, contours, -1, 255);

    //将每个数字，分离开，保存到容器中
    vector<myRect> sort_rect;
    for (int i = 0; i < contours.size(); i++) {
        //boundingRect返回轮廓的外接矩阵
        Rect tempRect = boundingRect(contours[i]);
        sort_rect.emplace_back(tempRect);
    }

    //对矩形进行排序，因为轮廓的顺序不一定是数字真正的顺序
    for (int i = 0; i < sort_rect.size(); i++) {
        for (int j = i + 1; j < sort_rect.size(); j++) {
            if (sort_rect[j] < sort_rect[i]) {
                myRect temp = sort_rect[j];
                sort_rect[j] = sort_rect[i];
                sort_rect[i] = temp;
            }
        }
    }

    // 分割打印
    for (int k = 0; k < sort_rect.size(); k++) {
        LOGD("分割第%d个: x:%d y:%d w:%d h:%d", k, sort_rect[k].getRect().x, sort_rect[k].getRect().y,
             sort_rect[k].getRect().width, sort_rect[k].getRect().height);
    }

    // ----------------------------------------

    //加载模板
    vector<Mat> myTemplate;

    for (int c = 0; c < 10; c++) {

        jobject srcBufTemp = env->GetObjectArrayElement(bitmapBuf, c);

        Mat temp0 = BitmapMatUtil::bitmap2Mat(env, srcBufTemp);//图片源矩阵初始化

        //对图像进行处理，转化为灰度图然后再转为二值图
        Mat grayImage0;
        cvtColor(temp0, grayImage0, COLOR_BGRA2GRAY);
        Mat binImage0;
        //第4个参数为CV_THRESH_BINARY_INV是因为我的输入原图为白底黑字
        //若为黑底白字则选择CV_THRESH_BINARY即可
        threshold(grayImage0, binImage0, 100, 255, CV_THRESH_BINARY_INV);

        Mat binImage1 = getContours(binImage0);

        cvtColor(binImage1, temp0, COLOR_GRAY2BGRA);

        myTemplate.push_back(binImage1);
    }

    //按顺序取出和分割数字
    vector<Mat> myROI;
    for (int i = 0; i < sort_rect.size(); i++) {
        Mat ROI;
        ROI = conImage(sort_rect[i].getRect());
        Mat dstROI = Mat::zeros(myTemplate[0].size(), myTemplate[0].type());
        resize(ROI, dstROI, myTemplate[0].size(), 0, 0, INTER_NEAREST);
        myROI.push_back(dstROI);
    }

    //进行比较,将图片与模板相减，然后求全部像素和，和最小表示越相似，进而完成匹配
    vector<int> seq;//顺序存放识别结果
    for (int i = 0; i < myROI.size(); i++) {
        Mat subImage;
        int sum = 0;
        int min = 100000;
        int min_seq = 0;//记录最小的和对应的数字
        for (int j = 0; j < 10; j++) {
            //计算两个图片的差值
            absdiff(myROI[i], myTemplate[j], subImage);
            sum = getPiexSum(subImage);
            if (sum < min) {
                min = sum;
                min_seq = j;
            }
        }
        seq.push_back(min_seq);
    }

    char *string1 = (char *) malloc(seq.size());
    memset(string1, 0, seq.size());

    //输出结果
    LOGD("识别结果为:");
    for (int i = 0; i < seq.size(); i++) {
        LOGD("%d", seq[i]);
        sprintf(string1, "%s%d", string1, seq[i]);
    }

    cvtColor(conImage, imgData, COLOR_GRAY2BGRA);

    jstring encoding = (env)->NewStringUTF(string1);

    return encoding;
}
