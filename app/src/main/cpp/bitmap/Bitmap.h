//
// Created by sunzhijun on 2018/1/2.
//

#ifndef TESTJNI_BITMAP_H
#define TESTJNI_BITMAP_H

#include<android/bitmap.h>
#include<memory.h>
#include"JniUtil.h"

typedef uint32_t ABsize;    //Android Bitmap size
typedef int32_t ABformat;   //Android Bitmap format

#define RGBA_4444

#ifdef ARGB_8888
    typedef uint32_t APixel;
    ABformat checkFormat = ANDROID_BITMAP_FORMAT_RGBA_8888;
#elif defined(RGBA_4444)
    typedef uint16_t APixel;
    ABformat checkFormat = ANDROID_BITMAP_FORMAT_RGBA_4444;
#elif defined(RGB_565)
    typedef uint16_t APixel;
    ABformat checkFormat = ANDROID_BITMAP_FORMAT_RGB_565;
#elif defined(ALPHA_8)
    typedef uint8_t APixel;
    ABformat checkFormat = ANDROID_BITMAP_FORMAT_A_8;
#else
    typedef uint32_t APixel;
    ABformat checkFormat = ANDROID_BITMAP_FORMAT_RGBA_8888;
#endif

class Bitmap{
    APixel * pixels;
    JNIEnv * jenv;
    _jobject * jbitmap;
    AndroidBitmapInfo info;
    int result;
    ABsize width;
    ABsize height;

public:
    Bitmap(int width, int height):jenv(NULL),jbitmap(NULL){
        pixels = (APixel *) malloc(sizeof(APixel) * width * height);
        memset(pixels,0,sizeof(APixel) * width * height);
    }
    Bitmap(JNIEnv * env, jobject bitmap):pixels(NULL),jenv(env),jbitmap(bitmap){
        if ((result = AndroidBitmap_getInfo(env,bitmap,&info))<0){
            ALOGE("bitmap init fialed ! error = %d",result);
            return;
        }
        if (info.format != checkFormat){
            ALOGE("Bitmap format is not selection !");
            return;
        }
        if((result = AndroidBitmap_lockPixels(env, bitmap, (void **) &pixels))<0){
            ALOGE("bitmap get pixels failed ! error = %d",result);
        }
    }
    ~Bitmap(){
        if(jenv){
            AndroidBitmap_unlockPixels(jenv,jbitmap);
        }else{
            free(pixels);
        }
    }
    ABsize getHeight(){
        return jenv ? info.height : height;
    }
    ABsize getWidth(){
        return jenv? info.width :width;
    }
    ABformat getType(){
        return checkFormat;
    }
    int getErrorCode(){
        return result;
    }
    operator APixel *(){
        return pixels;
    }
    APixel *operator[](int y){
        if (y>=getHeight()) return NULL;
        return pixels + y * getWidth();
    }

};


#endif //TESTJNI_BITMAP_H
