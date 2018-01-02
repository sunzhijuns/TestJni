//
// Created by sunzhijun on 2018/1/2.
//

#ifndef TESTJNI_JNIUTIL_H
#define TESTJNI_JNIUTIL_H

#include<jni.h>
#include<android/log.h>
#include<cstdlib>

using namespace std;

#ifndef LOG_TAG
#define LOG_TAG "NDK_LIB"
#endif

#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define ALOGI(...) __android_log_print(ANDROID_LOG_INFO,  LOG_TAG, __VA_ARGS__)
#define ALOGW(...) __android_log_print(ANDROID_LOG_WARN,  LOG_TAG, __VA_ARGS__)

#endif //TESTJNI_JNIUTIL_H
