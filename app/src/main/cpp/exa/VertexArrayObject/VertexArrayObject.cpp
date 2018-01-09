//
// Created by sunzhijun on 2018/1/9.
//
#include "VertexArrayObject.h"
#include<GLES3/gl3.h>
#include<android/log.h>

#include <stdlib.h>

#include <jni.h>
#include <string.h>

#define LOG_TAG "VAO|MAP-LIB"

#include "../esUtil.h"



typedef struct{
    GLuint program;
    GLuint vboIds[3];
    GLuint vaoId;
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
static const GLfloat VERTEX[3*VERTEX_POS_SIZE] = {
        -0.5f,   0.5f,   0.0f,          //v0
        -1.0f,  -0.5f,   0.0f,          //v1
        0.0f,  -0.5f,   0.0f,          //v2
};

static const GLfloat COLOR[4*VERTEX_COLOR_SIZE] = {
        1.0f,   0.0f,   0.0f,   0.0f,  //c0
        0.0f,   1.0f,   0.0f,   0.0f,  //c1
        0.0f,   0.0f,   1.0f,   0.0f,  //c2
};

static const GLint VTXSTRIDES[2] ={
        VERTEX_POS_SIZE * sizeof(GLfloat),
        VERTEX_COLOR_SIZE * sizeof(GLfloat),
};

static const GLushort INDICES[3] = {0,1,2};

static UserData  userData;

VAORender::VAORender(){
    userData = {0};
    return;
}

//initVAO(&userData,3,vtxBuffer,VTXSTRIDES,3,INDICES);
static void initVAO(UserData * userData,GLint numVertices, const GLfloat **vtxBuf,const GLint * vtxStrides,
                    GLint numIndices, const GLushort *indices) {

    GLfloat * vtxMappedBuf;
    GLushort * idxMappedBuf;

    glGenBuffers(3,userData->vboIds);

    //pos
    glBindBuffer(GL_ARRAY_BUFFER,userData->vboIds[0]);
//    glBufferData(GL_ARRAY_BUFFER,vtxStrides[0] * numVertices,vtxBuf[0],GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER,vtxStrides[0] * numVertices,NULL,GL_STATIC_DRAW);
    vtxMappedBuf = (GLfloat *) glMapBufferRange(GL_ARRAY_BUFFER, 0, vtxStrides[0] * numVertices,
                                        GL_MAP_WRITE_BIT|GL_MAP_INVALIDATE_BUFFER_BIT);
    if(vtxMappedBuf == NULL){
        checkGlError("glMapBufferRange Pos");
        ALOGE("mapping pos buf error");
        return;
    }
    memcpy(vtxMappedBuf,vtxBuf[0],vtxStrides[0] * numVertices);
    if (glUnmapBuffer(GL_ARRAY_BUFFER) == GL_FALSE){
        checkGlError("glUnmapBuffer Pos");
        ALOGE("unmapping pos buf error");
    }

    //color
    glBindBuffer(GL_ARRAY_BUFFER,userData->vboIds[1]);
//    glBufferData(GL_ARRAY_BUFFER,vtxStrides[1] * numVertices,vtxBuf[1],GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER,vtxStrides[1] * numVertices,NULL,GL_STATIC_DRAW);

    vtxMappedBuf = (GLfloat *) glMapBufferRange(GL_ARRAY_BUFFER, 0, vtxStrides[1] * numVertices,
                                                GL_MAP_WRITE_BIT|GL_MAP_INVALIDATE_BUFFER_BIT);
    if(vtxMappedBuf == NULL){
        checkGlError("glMapBufferRange Color");
        ALOGE("mapping color buf error");
        return;
    }
    memcpy(vtxMappedBuf,vtxBuf[1],vtxStrides[1] * numVertices);
    if (glUnmapBuffer(GL_ARRAY_BUFFER) == GL_FALSE){
        checkGlError("glUnmapBuffer Color");
        ALOGE("unmapping color buf error");
    }
    //index
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,userData->vboIds[2]);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * numIndices,indices,GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * numIndices,NULL,GL_STATIC_DRAW);
    idxMappedBuf = (GLushort *) glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLushort) * numIndices,
                                        GL_MAP_WRITE_BIT|GL_MAP_INVALIDATE_BUFFER_BIT);
    if(idxMappedBuf == NULL){
        checkGlError("glMapBufferRange index");
        ALOGE("mapping index buf error");
        return;
    }
    memcpy(idxMappedBuf,indices,sizeof(GLushort) * numIndices);
    if (glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER) == GL_FALSE){
        checkGlError("glUnmapBuffer index");
        ALOGE("unmapping index buf error");
    }

    glGenVertexArrays(1, &userData->vaoId);

    glBindVertexArray(userData->vaoId);

    glBindBuffer(GL_ARRAY_BUFFER, userData->vboIds[0]);
    glEnableVertexAttribArray(VERTEX_POS_INDX);
    glVertexAttribPointer(VERTEX_POS_INDX, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, vtxStrides[0],0);

    glBindBuffer(GL_ARRAY_BUFFER,userData->vboIds[1]);
    glEnableVertexAttribArray(VERTEX_COLOR_INDX);
    glVertexAttribPointer(VERTEX_COLOR_INDX, VERTEX_COLOR_SIZE, GL_FLOAT, GL_FALSE, vtxStrides[1],0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,userData->vboIds[2]);

    glBindVertexArray(0);

}

bool VAORender::init() {

    GLuint program = createProgram(VERTEX_SHADER,FRAGMENT_SHADER);
    if (!program){
        ALOGE("程序创建失败");
        userData.program = 0;
        return false;
    }
    userData.program = program;
    userData.vboIds[0] = 0;
    userData.vboIds[1] = 0;
    userData.vboIds[2] = 0;
    userData.offsetLoc = glGetUniformLocation(program,"u_offset");


    const GLfloat * vtxBuffer[2] = {VERTEX,COLOR};

    initVAO(&userData, 3, vtxBuffer, VTXSTRIDES, 3, INDICES);


    glClearColor(0.0f,0.0f,0.0f,0.0f);
    return true;
}


VAORender::~VAORender() {
    ALOGE("删除 program VAORender");
    if (userData.program){
        glDeleteProgram(userData.program);
    }
    glDeleteBuffers(3,userData.vboIds);
    userData.vboIds[0] = 0;
    userData.vboIds[1] = 0;
    userData.vboIds[2] = 0;
    if (userData.vaoId){
        glDeleteVertexArrays(1,&userData.vaoId);
    }
}

void VAORender::resize(int w, int h) {
    glViewport(0,0,w,h);
    glClear(GL_COLOR_BUFFER_BIT);
}

void VAORender::step() {

}


void VAORender::render() {

    step();
    glClear(GL_COLOR_BUFFER_BIT);
    draw();
    checkGlError("VAORender::render");
}


void VAORender::draw() {

    glUseProgram(userData.program);
    glUniform1f(userData.offsetLoc,0.5f);

    glBindVertexArray(userData.vaoId);

    glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_SHORT,0);

    glBindVertexArray(0);

}


