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

// 获取边缘 Mat
vector<Mat>
getContours(Mat grayImage, Mat templateOne) {
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

    // 重新确定大小，必须保持一致
    vector<Mat> myROI;
    for (int i = 0; i < sort_rect.size(); i++) {
        Mat ROI;
        ROI = conImage(sort_rect[i].getRect());
        Mat dstROI = Mat::zeros(templateOne.size(), templateOne.type());
        resize(ROI, dstROI, templateOne.size(), 0, 0, INTER_NEAREST);
        myROI.push_back(dstROI);
    }

    return myROI;
}

// 对每个模板图片进行灰化二值化处理
vector<Mat>
getTemplateArrayMat(JNIEnv *env, jobjectArray bitmapBuf) {
    vector<Mat> myTemplate;

    for (int c = 0; c < env->GetArrayLength(bitmapBuf); c++) {

        jobject srcBufTemp = env->GetObjectArrayElement(bitmapBuf, c);

        Mat temp0 = BitmapMatUtil::bitmap2Mat(env, srcBufTemp);//图片源矩阵初始化

        //对图像进行处理，转化为灰度图然后再转为二值图
        Mat grayImage0;
        cvtColor(temp0, grayImage0, COLOR_BGRA2GRAY);
        Mat binImage0;
        //第4个参数为CV_THRESH_BINARY_INV是因为我的输入原图为白底黑字
        //若为黑底白字则选择CV_THRESH_BINARY即可
        threshold(grayImage0, binImage0, 100, 255, CV_THRESH_BINARY_INV);

        // 获取边缘 Mat
        Mat binImage1 = getContours(binImage0, binImage0)[0];

        cvtColor(binImage1, temp0, COLOR_GRAY2BGRA);

        myTemplate.push_back(binImage1);
    }
    return myTemplate;
}

char *
matchingData(vector<Mat> myROI, vector<vector<Mat>> templateOnce, vector<char> templateOnceFirst) {

    // 进行比较,将图片与模板相减，然后求全部像素和，和最小表示越相似，进而完成匹配
    vector<char> seq;//顺序存放识别结果
    for (int i = 0; i < myROI.size(); i++) {
        Mat subImage;
        int sum = 0;
        int min = 100000;
        bool have = false;
        char min_seq = '0';// 记录最小的和对应的数字

        for (int a = 0; a < templateOnce.size(); a++) {
            for (int b = 0; b < templateOnce[a].size(); b++) {
                // 计算两个图片的差值
                absdiff(myROI[i], templateOnce[a][b], subImage);
                sum = getPiexSum(subImage);
                // 符合跳出
                if (sum < min) {
                    min = sum;
                    min_seq = (char) (templateOnceFirst[a] + b);
                    // 已经找到符合的，跳出继续寻找
                    have = true;
                    break;
                }
            }
            if (have) {
                // 已经找到符合的，跳出继续寻找
                break;
            }
        }

        if (!have) {
            LOGD("无法分析");
        }
        seq.push_back(min_seq);
    }

    char *stringText = (char *) malloc(seq.size());
    memset(stringText, 0, seq.size());

    for (int i = 0; i < seq.size(); i++) {
        sprintf(stringText, "%s%c", stringText, seq[i]);
    }

    return stringText;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_ruixin_ndkstudy_inter_JNIUtils_findNumber(JNIEnv *env, jclass,
                                                   jobject bitmap,              // 分析 原图片
                                                   jobjectArray bitmapBuf,      // 模板 数字
                                                   jobjectArray bitmapSmallBuf, // 模板 小写字母
                                                   jobjectArray bitmapBigBuf    // 模板 大写字母
) {
    // 图片源矩阵初始化
    Mat imgData = BitmapMatUtil::bitmap2Mat(env, bitmap);

    // ----------------------------------------

    // 对图像进行处理，转化为灰度图然后再转为二值图
    Mat grayImage;
    cvtColor(imgData, grayImage, COLOR_BGRA2GRAY);
    Mat binImage;
    // 第4个参数为CV_THRESH_BINARY_INV是因为我的输入原图为白底黑字
    // 若为黑底白字则选择CV_THRESH_BINARY即可
    threshold(grayImage, binImage, 100, 255, CV_THRESH_BINARY_INV);

    // ----------------------------------------

    // 模板列表
    vector<vector<Mat>> myTemplateList;
    // 每个模板顺序的首字母Ascii值
    vector<char> myTemplateCharList;

    // 加载数字模板
    vector<Mat> myTemplate = getTemplateArrayMat(env, bitmapBuf);

    // 加载小字母模板
    vector<Mat> myTemplateSmall = getTemplateArrayMat(env, bitmapSmallBuf);

    // 加载大字母模板
    vector<Mat> myTemplateBig = getTemplateArrayMat(env, bitmapBigBuf);

    myTemplateList.push_back(myTemplate);
    myTemplateList.push_back(myTemplateSmall);
    myTemplateList.push_back(myTemplateBig);

    myTemplateCharList.push_back('0');
    myTemplateCharList.push_back('a');
    myTemplateCharList.push_back('A');

    // ----------------------------------------

    // 获取边缘组数据，然后根据模板的大小生成对比数据
    vector<Mat> myROI = getContours(binImage, myTemplate[0]);

    LOGD("识别结果为:");
    char *number = matchingData(myROI, myTemplateList, myTemplateCharList);
    LOGD("%s", number);

    jstring encoding = (env)->NewStringUTF(number);

    return encoding;
}
