//
// Created by sunzhijun on 2018/1/8.
//
#include "VertexBufferObject.h"
#include<GLES3/gl3.h>
#include<android/log.h>

#include <stdlib.h>

#include <jni.h>


#include "../esUtil.h"

#define LOG_TAG "EXA-LIB"

#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

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

static GLuint  program;

Render::Render(){

    return;
}

bool Render::init() {
    program = createProgram(VERTEX_SHADER,FRAGMENT_SHADER);
    if (!program){
        ALOGE("程序创建失败");
        return false;
    }
    glClearColor(0,0,0,0);
    return true;
}


Render::~Render() {
    ALOGE("删除 program ~Render");
    if (program){
        glDeleteProgram(program);
    }

}

void Render::resize(int w, int h) {
    glViewport(0,0,w,h);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Render::calcSceneParams(int w, int h) {
}

void Render::step() {

}

void Render::render() {

    step();
    glClear(GL_COLOR_BUFFER_BIT);
    draw();
    checkGlError("Render::render");
}

void Render::draw() {

    glUseProgram(program);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,VERTEX);
//    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),VERTEX);
    glEnableVertexAttribArray(0);

    glVertexAttrib4fv(1,COLOR);

    glDrawArrays(GL_TRIANGLES,0,6);

    glDisableVertexAttribArray(0);
}



