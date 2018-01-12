//
// Created by sunzhijun on 2018/1/4.
//

#ifndef TESTJNI_ESUTIL_H
#define TESTJNI_ESUTIL_H

#include<GLES3/gl3.h>
#include<android/log.h>
#include <jni.h>

#ifndef LOG_TAG
#define LOG_TAG "ES_LIB"
#endif

#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

typedef struct{
    GLfloat m[4][4];
}ESMatrix;


#include <stdlib.h>

//检查当前程序错误
bool checkGlError(const char *funcName);
//获取并编译着色器对象
GLuint createShader(GLenum shaderType, const char *src);
//使用着色器生成着色器程序对象
GLuint createProgram(const char *vtxSrc, const char *fragSrc);

//
void esMatrixLoadIdentity(ESMatrix * result);
void esMatrixMultiply(ESMatrix * result, ESMatrix * srcA, ESMatrix * srcB);
void esTranslate(ESMatrix * result, GLfloat tx, GLfloat ty, GLfloat tz);
void esRotate(ESMatrix * result,GLfloat  angle, GLfloat x, GLfloat y, GLfloat z);
void esFrustum(ESMatrix * result, float left, float right, float bottom, float top, float nearZ, float farZ);
void esPerspective(ESMatrix *result, float fovy, float aspect, float nearZ, float farZ);

int esGenCube(float scale,GLfloat **vertices,
              GLfloat **normals,GLfloat **texCoords,GLuint **indices);
int esGenSphere(int numSlices,float radius,GLfloat **vertices,
                GLfloat **normals,GLfloat **texCoords,GLuint **indices);

#endif //TESTJNI_ESUTIL_H
