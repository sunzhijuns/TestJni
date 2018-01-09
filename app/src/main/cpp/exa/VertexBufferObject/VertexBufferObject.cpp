//
// Created by sunzhijun on 2018/1/8.
//
#include "VertexBufferObject.h"
#include<GLES3/gl3.h>
#include<android/log.h>

#include <stdlib.h>

#include <jni.h>

#define LOG_TAG "EXA-LIB"

#include "../esUtil.h"



typedef struct{
    GLuint program;
    GLuint vboIds[2];
    GLuint offsetLoc;
}UserData;

#define VERTEX_POS_SIZE     3
#define VERTEX_COLOR_SIZE   4

#define VERTEX_POS_INDX 0
#define VERTEX_COLOR_INDX 1

static const char VERTEX_SHADER[] =
        "#version 300 es\n"
                "layout(location = 0) in vec4 a_position;\n"
                "layout(location = 1) in vec4 a_color;\n"
                "uniform float u_offset;\n"
                "out vec4 v_color;\n"
                "void main(){\n"
                "   v_color = a_color;\n"
                "   gl_Position = a_position;\n"
                "   gl_Position.x += u_offset;\n"
                "}\n";
static const char FRAGMENT_SHADER[] =
        "#version 300 es\n"
                "precision mediump float;\n"
                "in vec4 v_color;\n"
                "out vec4 fragColor;\n"
                "void main(){\n"
                "   fragColor = v_color;\n"
                "}\n";
//static const GLfloat VERTEX[] = {
//        0.0f,  0.8f,  0.0f,
//        -0.5f,  0.5f,  0.0f,
//        0.5f,  0.5f,  0.0f,
//        0.0f,  0.5f,  0.0f,
//        -0.5f, -0.5f,  0.0f,
//        0.5f, -0.5f,  0.0f,
//};
//
//static const GLfloat COLOR[] = {
//        0.0f, 1.0f, 0.0f, 1.0f,
//};

static const GLfloat VERTICES[3*(VERTEX_POS_SIZE + VERTEX_COLOR_SIZE)] = {

        -0.5f,   0.5f,   0.0f,          //v0
         1.0f,   0.0f,   0.0f,   1.0f,  //c0
        -1.0f,  -0.5f,   0.0f,          //v1
         0.0f,   1.0f,   0.0f,   0.0f,  //c1
         0.0f,  -0.5f,   0.0f,          //v2
         0.0f,   0.0f,   1.0f,   0.0f,  //c2
};

static const GLushort INDICES[3] = {0,1,2};

static UserData  userData;

VBORender::VBORender(){

    return;
}

bool VBORender::init() {

    GLuint program = createProgram(VERTEX_SHADER,FRAGMENT_SHADER);
    if (!program){
        ALOGE("程序创建失败");
        return false;
    }
    userData.program = program;
    userData.vboIds[0] = 0;
    userData.vboIds[1] = 0;
    userData.offsetLoc = glGetUniformLocation(program,"u_offset");

    glClearColor(0.0f,0.0f,0.0f,0.0f);
    return true;
}

VBORender::~VBORender() {
    ALOGE("删除 program VBOGen3Render3Render");
    if (userData.program){
        glDeleteProgram(userData.program);
    }
    glDeleteBuffers(2,userData.vboIds);
    userData.vboIds[0] = 0;
    userData.vboIds[1] = 0;

}

void VBORender::resize(int w, int h) {
    glViewport(0,0,w,h);
    glClear(GL_COLOR_BUFFER_BIT);
}

void VBORender::render() {

    step();
    glClear(GL_COLOR_BUFFER_BIT);
    draw();
    checkGlError("VBORender::render");
}
//DrawPrimitiveWithoutVBOs(VERTICES, sizeof(GLfloat) * (VERTEX_POS_SIZE + VERTEX_COLOR_SIZE),3,INDICES);
static void DrawPrimitiveWithoutVBOs(const GLfloat *vertices, GLint vtxStride, GLint numIndices,
                                     const GLushort *indices) {
    const GLfloat *vtxBuf = vertices;

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

    glEnableVertexAttribArray(VERTEX_POS_INDX);
    glEnableVertexAttribArray(VERTEX_COLOR_INDX);

    glVertexAttribPointer(VERTEX_POS_INDX,VERTEX_POS_SIZE,GL_FLOAT,GL_FALSE,vtxStride,vtxBuf);
    vtxBuf += VERTEX_POS_SIZE;

    glVertexAttribPointer(VERTEX_COLOR_INDX,VERTEX_COLOR_SIZE,GL_FLOAT,GL_FALSE,vtxStride,vtxBuf);

    glDrawElements(GL_TRIANGLES,numIndices,GL_UNSIGNED_SHORT,indices);

    glDisableVertexAttribArray(VERTEX_POS_INDX);
    glDisableVertexAttribArray(VERTEX_COLOR_INDX);

}

//DrawPrimitiveWithVBOs(&userData,3,VERTICES, sizeof(GLfloat) * (VERTEX_POS_SIZE + VERTEX_COLOR_SIZE),3,INDICES);
static void DrawPrimitiveWithVBOs(UserData * userData,GLint numVertices, const GLfloat *vtxBuf,GLint vtxStride,
                                  GLint numIndices, const GLushort *indices) {
    GLuint offset = 0;
    if (userData->vboIds[0] == 0 && userData->vboIds[1] == 0){
        glGenBuffers(2,userData->vboIds);

        glBindBuffer(GL_ARRAY_BUFFER,userData->vboIds[0]);
        glBufferData(GL_ARRAY_BUFFER,vtxStride * numVertices,vtxBuf,GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,userData->vboIds[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * numIndices,indices,GL_STATIC_DRAW);
    }

    glBindBuffer(GL_ARRAY_BUFFER, userData->vboIds[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,userData->vboIds[1]);

    glEnableVertexAttribArray(VERTEX_POS_INDX);
    glEnableVertexAttribArray(VERTEX_COLOR_INDX);

    glVertexAttribPointer(VERTEX_POS_INDX, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, vtxStride,
                          (const void *) offset);

    offset += VERTEX_POS_SIZE * sizeof(GLfloat);
    glVertexAttribPointer(VERTEX_COLOR_INDX, VERTEX_COLOR_SIZE, GL_FLOAT, GL_FALSE, vtxStride,
                          (const void *) offset);

    glDrawElements(GL_TRIANGLES,numIndices,GL_UNSIGNED_SHORT,0);

    glDisableVertexAttribArray(VERTEX_POS_INDX);
    glDisableVertexAttribArray(VERTEX_COLOR_INDX);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void VBORender::step() {

}

void VBORender::draw() {



    glUseProgram(userData.program);
    glUniform1f(userData.offsetLoc,0.0f);

    DrawPrimitiveWithoutVBOs(VERTICES, sizeof(GLfloat) * (VERTEX_POS_SIZE + VERTEX_COLOR_SIZE),3,INDICES);

    glUniform1f(userData.offsetLoc,1.0f);

    DrawPrimitiveWithVBOs(&userData,3,VERTICES, sizeof(GLfloat) * (VERTEX_POS_SIZE + VERTEX_COLOR_SIZE),3,INDICES);

}






