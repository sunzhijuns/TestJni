//
// Created by sunzhijun on 2018/1/10.
//
#define LOG_TAG "DRAW-ELEMENT-LIB"

#include "../esUtil.h"

#include "DrawElements.h"

#include<GLES3/gl3.h>
#include<android/log.h>
#include <jni.h>

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define NUM_INSTANCES 49
#define POSITION_LOC 0
#define COLOR_LOC 1
#define MVP_LOC 2

#define STR(s) #s
#define STRV(s) STR(s)

typedef struct{
    GLuint program;
    //VBOs
    GLuint positionVBO;
    GLuint colorVBO;
    GLuint mvpVBO;
    GLuint indicesIBO;

    //索引数量
    int numIndices;
    //旋转角度
    GLfloat  angle[NUM_INSTANCES];
    GLfloat aspect;
}UserData;

static const char VERTEX_SHADER[] =
        "#version 300 es\n"
                "layout(location = " STRV(POSITION_LOC) ") in vec4 a_position;\n"
                "layout(location = " STRV(COLOR_LOC) ") in vec4 a_color;\n"
                "layout(location = " STRV(MVP_LOC) ") in mat4 a_mvpMatrix;\n"
                "out vec4 v_color;\n"
                "void main(){\n"
                "   v_color = a_color;\n"
                "   gl_Position = a_mvpMatrix * a_position;\n"
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

DrawElementsRender::DrawElementsRender(){
    userData = {0};
    return;
}

bool DrawElementsRender::init() {

    GLuint program = createProgram(VERTEX_SHADER,FRAGMENT_SHADER);
    if (!program){
        ALOGE("程序创建失败");
        userData.program = 0;
        return false;
    }
    userData.program = program;

    GLfloat * positions;
    GLuint * indices;
    userData.numIndices = esGenCube(0.1f,&positions,NULL,NULL,&indices);

    //index buffer object
    glGenBuffers(1,&userData.indicesIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,userData.indicesIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*userData.numIndices,
                 indices,GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    free(indices);
    indices = NULL;

    //position vbo for cube model
    glGenBuffers(1, &userData.positionVBO);
    glBindBuffer(GL_ARRAY_BUFFER,userData.positionVBO);
    glBufferData(GL_ARRAY_BUFFER,24* sizeof(GLfloat) * 3,
                 positions,GL_STATIC_DRAW);
    free(positions);
    positions = NULL;

    //random color for each instance
    {
        GLubyte colors[NUM_INSTANCES][4];
        int instance;
        srand(0);
        for (instance = 0; instance < NUM_INSTANCES; ++instance) {
            colors[instance][0] = (GLubyte) (((float)rand() / (float)RAND_MAX) * 255.0);
            colors[instance][1] = (GLubyte) (((float)rand() / (float)RAND_MAX) * 255.0);
            colors[instance][2] = (GLubyte) (((float)rand() / (float)RAND_MAX) * 255.0);
            colors[instance][3] = (GLubyte) (((float)rand() / (float)RAND_MAX) * 255.0);
        }
        glGenBuffers(1,&userData.colorVBO);
        glBindBuffer(GL_ARRAY_BUFFER,userData.colorVBO);
        glBufferData(GL_ARRAY_BUFFER,NUM_INSTANCES * 4 * sizeof(GLubyte),colors,GL_STATIC_DRAW);
    }
    {
        int instance;
        for (instance = 0; instance < NUM_INSTANCES; ++instance) {
            userData.angle[instance] = ((float)rand() / (float)RAND_MAX) * 360.0f;
        }
        glGenBuffers(1,&userData.mvpVBO);
        glBindBuffer(GL_ARRAY_BUFFER,userData.mvpVBO);
        glBufferData(GL_ARRAY_BUFFER,NUM_INSTANCES * sizeof(ESMatrix),
                     NULL,GL_DYNAMIC_DRAW);
    }
    glBindBuffer(GL_ARRAY_BUFFER,0);

    glClearColor(0.0f,0.0f,0.0f,0.0f);
    return true;
}


DrawElementsRender::~DrawElementsRender() {
    ALOGE("删除 program DrawElementsRender");
    if (userData.program){
        glDeleteProgram(userData.program);
    }
    glDeleteBuffers(1,&userData.positionVBO);
    glDeleteBuffers(1,&userData.colorVBO);
    glDeleteBuffers(1,&userData.mvpVBO);
    glDeleteBuffers(1,&userData.indicesIBO);
}

void DrawElementsRender::resize(int w, int h) {
    userData.aspect = (GLfloat)w / (GLfloat)h;
    glViewport(0,0,w,h);
    glClear(GL_COLOR_BUFFER_BIT);
}
void DrawElementsRender::step() {

}
void DrawElementsRender::step(float dt) {
    ESMatrix * matrixBuf;
    ESMatrix perspective;
    int instance = 0;
    int numRows;
    int numColumns;

    esMatrixLoadIdentity(&perspective);
    esPerspective(&perspective,60.0f,userData.aspect,1.0f,20.0f);

    glBindBuffer(GL_ARRAY_BUFFER,userData.mvpVBO);
    matrixBuf = (ESMatrix *) glMapBufferRange(GL_ARRAY_BUFFER, 0,
                                              sizeof(ESMatrix) * NUM_INSTANCES,
                                              GL_MAP_WRITE_BIT);
    numRows = (int)sqrtf(NUM_INSTANCES);
    numColumns = numRows;

    for (instance = 0; instance < NUM_INSTANCES; ++instance) {
        ESMatrix modelView;
        float translateX = ((float)(instance % numRows) / (float) numRows)*2.0f - 1.0f;
        float translateY = ((float)(instance / numColumns) / (float) numColumns)*2.0f - 1.0f;

        esMatrixLoadIdentity(&modelView);
        esTranslate(&modelView, translateX,translateY,-2.0f);
        userData.angle[instance] += dt * 50.0f;


        if (userData.angle[instance] >= 360.0f){
            userData.angle[instance] = fmodf(userData.angle[instance], 360.0f);
        }

        esRotate(&modelView, userData.angle[instance],1.0 ,0.0 ,1.0);
        esMatrixMultiply(&matrixBuf[instance],&modelView,&perspective);
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);
}


void DrawElementsRender::render() {
    timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    auto nowNs = now.tv_sec * 1000000000ull + now.tv_nsec;

    if (lastFrameNs > 0){
        glClear(GL_COLOR_BUFFER_BIT);
        float dt = float(nowNs - lastFrameNs) * 0.000000001f;
        step(dt);
        draw();
    }
    lastFrameNs = nowNs;

    checkGlError("DrawElementsRender::render");
}


void DrawElementsRender::draw() {

    glUseProgram(userData.program);
    glBindBuffer(GL_ARRAY_BUFFER,userData.positionVBO);
    glVertexAttribPointer(POSITION_LOC,3,GL_FLOAT,GL_FALSE,3* sizeof(GLfloat),NULL);
    glEnableVertexAttribArray(POSITION_LOC);

    glBindBuffer(GL_ARRAY_BUFFER,userData.colorVBO);
    glVertexAttribPointer(COLOR_LOC,4,GL_UNSIGNED_BYTE,GL_TRUE,4*sizeof(GLubyte),NULL);
    glEnableVertexAttribArray(COLOR_LOC);
    glVertexAttribDivisor(COLOR_LOC,1);

    glBindBuffer(GL_ARRAY_BUFFER,userData.mvpVBO);
    glVertexAttribPointer(MVP_LOC + 0, 4, GL_FLOAT, GL_FALSE, sizeof(ESMatrix),
                          (const void *) NULL);
    glVertexAttribPointer(MVP_LOC + 1, 4, GL_FLOAT, GL_FALSE, sizeof(ESMatrix),
                          (const void *) (sizeof(GLfloat) * 4));
    glVertexAttribPointer(MVP_LOC + 2, 4, GL_FLOAT, GL_FALSE, sizeof(ESMatrix),
                          (const void *) (sizeof(GLfloat) * 8));
    glVertexAttribPointer(MVP_LOC + 3, 4, GL_FLOAT, GL_FALSE, sizeof(ESMatrix),
                          (const void *) (sizeof(GLfloat) * 12));
    glEnableVertexAttribArray(MVP_LOC + 0);
    glEnableVertexAttribArray(MVP_LOC + 1);
    glEnableVertexAttribArray(MVP_LOC + 2);
    glEnableVertexAttribArray(MVP_LOC + 3);

    glVertexAttribDivisor(MVP_LOC + 0, 1);
    glVertexAttribDivisor(MVP_LOC + 1, 1);
    glVertexAttribDivisor(MVP_LOC + 2, 1);
    glVertexAttribDivisor(MVP_LOC + 3, 1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,userData.indicesIBO);

    glDrawElementsInstanced(GL_TRIANGLES,userData.numIndices,
                            GL_UNSIGNED_INT,(const void *)NULL,NUM_INSTANCES);

}

