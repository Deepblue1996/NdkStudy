//
// Created by mayn on 2019/11/27 0027.
//

#ifndef NDKSTUDY_MYRECT_H
#define NDKSTUDY_MYRECT_H

#include <jni.h>
#include <opencv2/opencv.hpp>

using namespace cv;

class myRect
{
public:
    myRect(){}
    ~myRect(){}
    myRect(Rect &temp):myRc(temp){}
    //比较矩形左上角的横坐标，以便排序
    bool operator<(myRect &rect)
    {
        return this->myRc.x < rect.myRc.x;
    }
    //重载赋值运算符
    myRect operator=(myRect &rect)
    {
        this->myRc = rect.myRc;
        return *this;
    }
    //获取矩形
    Rect getRect()
    {
        return myRc;
    }
private:
    Rect myRc;//存放矩形
};

#endif //NDKSTUDY_MYRECT_H
