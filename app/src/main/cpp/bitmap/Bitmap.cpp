//
// Created by sunzhijun on 2018/1/3.
//

#define LOG_TAG "Bitmap"
#define RGBA_4444

#include<android/bitmap.h>
#include"Bitmap.h"

extern "C"
JNIEXPORT void JNICALL
Java_com_szj_testjni_NBitmapLib_renderGray(JNIEnv *env, jclass type, jobject bitmap) {

    Bitmap bm(env,bitmap);
    ABsize height = bm.getHeight();
    ABsize width = bm.getWidth();

    const APixel MODEL = 0xF;
    APixel color;
    APixel av;
    APixel alpha;

    APixel * pixelArray = bm;
    ABsize length = height * width;
    for (ABsize i = 0; i < length; ++i) {
        av = 0;

        color = pixelArray[i];

        alpha = color & MODEL;

        av += (color >>= 4) & MODEL;
        av += (color >>= 4) & MODEL;
        av += (color >>= 4) & MODEL;
        av /= 3;

        color = av;
        color = (color << 4) + av;
        color = (color << 4) + av;
        color = (color << 4) + alpha;

        pixelArray[i] = color;
    }

}

#undef ARGB_4444