//
// Created by sunzhijun on 2018/1/2.
//

#ifndef TESTJNI_BITMAP_H_H
#define TESTJNI_BITMAP_H_H

//响应结果定义
#include <stdint.h>
#include <jni.h>

#define ANDROID_BITMAP_RESULT_SUCCESS 0             //成功
#define ANDROID_BITMAP_RESULT_BAD_PARAMETER -1      //错误的参数
#define ANDROID_BITMAP_RESULT_JNI_EXCEPTION -2      //JNI异常
#define ANDROID_BITMAP_RESULT_ALLOCATION_FAILED -3  //内存分配错误

#define ANDROID_BITMAP_RESUT_SUCCESS ANDROID_BITMAP_RESULT_SUCCESS

//位图格式枚举
enum AndroidBitmapFormat{
    ANDROID_BITMAP_FORMAT_NONE      = 0,
    ANDROID_BITMAP_FORMAT_RGBA_8888 = 1,
    ANDROID_BITMAP_FORMAT_RGB_565   = 4,
    ANDROID_BITMAP_FORMAT_RGBA_4444 = 7,
    ANDROID_BITMAP_FORMAT_A_8       = 8,

};

//位图信息结构体
typedef struct{
    uint32_t    width;
    uint32_t    height;
    uint32_t    stride; //行跨度
    int32_t     format;
    uint32_t    flags;  //0 for now 已被弃用始终为0
}AndroidBitmapInfo;

//位图操作函数声明

/*
 * 获取当前位图信息
 */
int AndroidBitmap_getInfo(JNIEnv* env,
                          jobject jbitmap,AndroidBitmapInfo* info);
/*
 * 锁定当前位图像素，在锁定期间该Bitmap对象不会被回收，使用完成之后
 * 必须调用AndroidBitmap_unlockPixels来解除对像素的锁定
 */
int AndroidBitmap_lockPixels(JNIEnv* env, jobject jbitmap,void** addrPtr);
int AndroidBitmap_unlockPixels(JNIEnv* env, jobject jbitmap);

#endif //TESTJNI_BITMAP_H_H
