//
// Created by sunzhijun on 2018/1/11.
//
#define LOG_TAG "VERTEX-SHADER-LIB"

#include "../esUtil.h"
#include "../Macros.h"

#include "SimpleVertexShader.h"

#include<GLES3/gl3.h>
#include <jni.h>

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define POSITION_LOC 0
#define COLOR_LOC 1

#define STR(s) #s
#define STRV(s) STR(s)

typedef struct{
    GLuint program;

    GLint mvpLoc;

    GLfloat *vertices;
    GLuint *indices;
    int numIndices;

    GLfloat aspect;

    GLfloat angle;
    ESMatrix mvpMatrix;
}UserData;

static const char VERTEX_SHADER[] =
        "#version 300 es\n"
                "uniform mat4 u_mvpMatrix;\n"
                "layout(location = " STRV(POSITION_LOC) ") in vec4 a_position;\n"
                "layout(location = " STRV(COLOR_LOC) ") in vec4 a_color;\n"
                "out vec4 v_color;\n"
                "void main(){\n"
                "   v_color = a_color;\n"
                "   gl_Position = u_mvpMatrix * a_position;\n"
                "}\n";
static const char FRAGMENT_SHADER[] =
        "#version 300 es\n"
                "precision mediump float;\n"
                "in vec4 v_color;\n"
                "layout(location = 0) out vec4 fragColor;\n"
                "void main(){\n"
                "   fragColor = v_color;\n"
                "}\n";

static UserData  userData;

SimpleVertexRender::SimpleVertexRender(){
    userData = {0};
    return;
}

bool SimpleVertexRender::init() {

    GLuint program = createProgram(VERTEX_SHADER,FRAGMENT_SHADER);
    if (!program){
        ALOGE("程序创建失败");
        userData.program = 0;
        return false;
    }
    userData.program = program;

    userData.mvpLoc = glGetUniformLocation(program,"u_mvpMatrix");
    userData.numIndices = esGenCube(1.0,&userData.vertices,NULL,NULL,&userData.indices);

    userData.angle = 45.0f;

    glClearColor(0.0f,0.0f,0.0f,0.0f);
    return true;
}


SimpleVertexRender::~SimpleVertexRender() {
    ALOGE("删除 program SimpleVertexRender");
    if (userData.program){
        glDeleteProgram(userData.program);
    }
    SAFE_FREE(userData.vertices);
    SAFE_FREE(userData.indices);
}

void SimpleVertexRender::resize(int w, int h) {
    userData.aspect = (GLfloat)w / (GLfloat)h;
    glViewport(0,0,w,h);
    glClear(GL_COLOR_BUFFER_BIT);
}
void SimpleVertexRender::step() {

}
void SimpleVertexRender::step(float dt) {

    ESMatrix perspective;
    ESMatrix modelview;

    userData.angle += (dt * 40.0f);
    if (userData.angle >= 360){
        userData.angle = fmodf(userData.angle, 360.0f);
    }

    esMatrixLoadIdentity(&perspective);
    esPerspective(&perspective,60.0f,userData.aspect,1.0f,20.0f);

    esMatrixLoadIdentity(&modelview);
    esTranslate(&modelview, 0.0,0.0,-2.0);
    esRotate(&modelview,userData.angle,1.0,0.0,1.0);

    esMatrixMultiply(&userData.mvpMatrix, &modelview, &perspective);

}


void SimpleVertexRender::render() {
    timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    auto nowNs = now.tv_sec * 1000000000ull + now.tv_nsec;

    if (lastFrameNs > 0){
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        float dt = float(nowNs - lastFrameNs) * 0.000000001f;
        step(dt);
        draw();
    }
    lastFrameNs = nowNs;

    checkGlError("DrawElementsRender::render");
}


void SimpleVertexRender::draw() {

    glUseProgram(userData.program);
    glVertexAttribPointer(POSITION_LOC,3,GL_FLOAT,GL_FALSE,3* sizeof(GLfloat),
                          userData.vertices);
    glEnableVertexAttribArray(POSITION_LOC);

    glVertexAttrib4f(COLOR_LOC,1.0f,0.0f,0.0f,1.0f);

    glUniformMatrix4fv(userData.mvpLoc,1,
                       GL_FALSE,&userData.mvpMatrix.m[0][0]);
    glDrawElements(GL_TRIANGLES,userData.numIndices,GL_UNSIGNED_INT,userData.indices);


}

