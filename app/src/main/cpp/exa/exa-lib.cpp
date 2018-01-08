//
// Created by sunzhijun on 2018/1/4.
//

#include<GLES3/gl3.h>
#include<android/log.h>

#include <stdlib.h>

#define LOG_TAG "EXA-LIB"

#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#include <jni.h>


#include"esUtil.h"

static const char VERTEX_SHADER[] =
        "#version 300 es\n"
                "layout(location = 0) in vec4 a_position;\n"
                "layout(location = 1) in vec4 a_color;\n"
                "out vec4 v_color;\n"
                "void main(){\n"
                "   v_color = a_color;\n"
                "   gl_Position = a_position;\n"
                "}\n";
static const char FRAGMENT_SHADER[] =
        "#version 300 es\n"
                "precision mediump float;\n"
                "in vec4 v_color;\n"
                "out vec4 fragColor;\n"
                "void main(){\n"
                "   fragColor = v_color;\n"
                "}\n";
static const GLfloat VERTEX[] = {
         0.0f,  0.8f,  0.0f,
        -0.5f,  0.5f,  0.0f,
         0.5f,  0.5f,  0.0f,
         0.0f,  0.5f,  0.0f,
        -0.5f, -0.5f,  0.0f,
         0.5f, -0.5f,  0.0f,
};

static const GLfloat COLOR[] = {
        0.0f, 1.0f, 0.0f, 1.0f,
};

GLuint  program;


extern "C"
JNIEXPORT jboolean JNICALL
Java_com_szj_testjni_exa_ExaLib_init(JNIEnv *env, jclass type) {

    ALOGE("程序初始化 init");

    program = createProgram(VERTEX_SHADER,FRAGMENT_SHADER);
    if (!program){
        ALOGE("程序创建失败");
        return JNI_FALSE;
    }
    glClearColor(0,0,0,0);
    return JNI_TRUE;

}

extern "C"
JNIEXPORT void JNICALL
Java_com_szj_testjni_exa_ExaLib_resize(JNIEnv *env, jclass type, jint width, jint height) {

    ALOGE("程序初始化 resize");
    glViewport(0,0,width,height);
    glClear(GL_COLOR_BUFFER_BIT);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_szj_testjni_exa_ExaLib_step(JNIEnv *env, jclass type) {

    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,VERTEX);
//    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),VERTEX);
    glEnableVertexAttribArray(0);
    glVertexAttrib4fv(1,COLOR);

    glDrawArrays(GL_TRIANGLES,0,6);

    glDisableVertexAttribArray(0);

}