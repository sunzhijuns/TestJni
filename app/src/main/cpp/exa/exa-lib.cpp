//
// Created by sunzhijun on 2018/1/4.
//
#include<android/log.h>

#include <stdlib.h>

#define LOG_TAG "EXA-LIB"

#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#include <jni.h>

#include "VertexBufferObject/VertexBufferObject.h"
#include "VertexBufferObjectGen3/VertexBufferObjectGen3.h"
#include "VertexArrayObject/VertexArrayObject.h"
#include "DrawElements/DrawElements.h"
#include "SimpleVertexShader/SimpleVertexShader.h"
#include "SimpleTexture2D/SimpleTexture2D.h"
#include "MipMap2D/MipMap2D.h"

static Render * createRender(){
    Render * render = new MipMap2DRender();
    if (!render->init()){
        delete render;
        return  NULL;
    }
    return render;
}

static Render * g_render = NULL;

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_szj_testjni_exa_ExaLib_init(JNIEnv *env, jclass type) {

    ALOGE("程序初始化 init");
    if (g_render){
        delete g_render;
        g_render = NULL;
    }
    g_render = createRender();
    if (!g_render){
        return JNI_FALSE;
    }
    return JNI_TRUE;

}

extern "C"
JNIEXPORT void JNICALL
Java_com_szj_testjni_exa_ExaLib_resize(JNIEnv *env, jclass type, jint width, jint height) {

    ALOGE("程序初始化 resize");
    if(g_render){
        g_render->resize(width,height);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_szj_testjni_exa_ExaLib_step(JNIEnv *env, jclass type) {

    if (g_render) {
        g_render->render();
    }

}

extern "C"
JNIEXPORT void JNICALL
Java_com_szj_testjni_exa_ExaLib_destory(JNIEnv *env, jclass type) {

    ALOGE("删除 g_render");
    delete g_render;
    g_render = NULL;

}