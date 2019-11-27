//
// Created by mayn on 2019/11/27 0027.
//

#pragma once
#ifndef WLPLAYER_ANDROIDLOG_H
#define WLPLAYER_ANDROIDLOG_H

#include <android/log.h>
#define LOGD(FORMAT,...) __android_log_print(ANDROID_LOG_DEBUG,"deep",FORMAT,##__VA_ARGS__);
#define LOGE(FORMAT,...) __android_log_print(ANDROID_LOG_ERROR,"deep",FORMAT,##__VA_ARGS__);

#endif //WLPLAYER_ANDROIDLOG_H