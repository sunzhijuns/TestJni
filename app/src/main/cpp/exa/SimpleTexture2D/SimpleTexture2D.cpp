//
// Created by sunzhijun on 2018/1/8.
//
#include "SimpleTexture2D.h"
#include<GLES3/gl3.h>
#include<android/log.h>

#include <stdlib.h>

#include <jni.h>

#define LOG_TAG "Gen3-LIB"

#include "../esUtil.h"



typedef struct{
    GLuint program;

    GLint samplerLoc;
    GLuint textureId;
}UserData;

#define STR(s) #s
#define STRV(s) STR(s)

static GLuint CreateSimpleTexture2D(){
    GLuint textureId;

    GLubyte pixels[4*3] = {
            255,      0,      0,    //R
              0,    255,      0,    //G
              0,      0,    255,    //B
            255,    255,      0,    //Y
    };

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1,&textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,2,2,0,GL_RGB,GL_UNSIGNED_BYTE,pixels);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    return textureId;
}

static const char VERTEX_SHADER[] =
        "#version 300 es\n"
                "layout(location = 0) in vec4 a_position;\n"
                "layout(location = 1) in vec2 a_texCoord;\n"
                "out vec2 v_texCoord;\n"
                "void main(){\n"
                "   v_texCoord = a_texCoord;\n"
                "   gl_Position = a_position;\n"
                "}\n";
static const char FRAGMENT_SHADER[] =
        "#version 300 es\n"
                "precision mediump float;\n"
                "in vec2 v_texCoord;\n"
                "out vec4 fragColor;\n"
                "uniform sampler2D s_texture;\n"
                "void main(){\n"
                "   fragColor = texture(s_texture,v_texCoord);\n"
                "}\n";
static const GLfloat VERTEX[] = {
        -0.5f,   0.5f,   0.0f,          //position 0
         0.0f,   0.0f,             //texCoord 0
        -0.5f,  -0.5f,   0.0f,          //position 1
         0.0f,   1.0f,             //texCoord 1
         0.5f,  -0.5f,   0.0f,          //position 2
         1.0f,   1.0f,             //texCoord 2
         0.5f,   0.5f,   0.0f,          //position 3
         1.0f,   0.0f,             //texCoord 3
};

static const GLushort indices[] = {
        0,1,2,0,2,3,
};

static UserData  userData;

SimpleTexture2DRender::SimpleTexture2DRender(){

    return;
}

bool SimpleTexture2DRender::init() {

    GLuint program = createProgram(VERTEX_SHADER,FRAGMENT_SHADER);
    if (!program){
        ALOGE("程序创建失败");
        return false;
    }
    userData.program = program;

    userData.samplerLoc = glGetUniformLocation(userData.program,"s_texture");
    userData.textureId = CreateSimpleTexture2D();

    glClearColor(0.0f,0.0f,0.0f,0.0f);
    return true;
}


SimpleTexture2DRender::~SimpleTexture2DRender() {
    ALOGE("删除 program SimpleTexture2DRender3Render");
    if (userData.program){
        glDeleteProgram(userData.program);
    }
    glDeleteTextures(1,&userData.textureId);
}

void SimpleTexture2DRender::resize(int w, int h) {
    glViewport(0,0,w,h);
    glClear(GL_COLOR_BUFFER_BIT);
}

void SimpleTexture2DRender::step() {

}


void SimpleTexture2DRender::render() {

    step();
    glClear(GL_COLOR_BUFFER_BIT);
    draw();
    checkGlError("SimpleTexture2DRender::render");
}

void SimpleTexture2DRender::draw() {

    glUseProgram(userData.program);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5* sizeof(GLfloat),VERTEX);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5* sizeof(GLfloat),&VERTEX[3]);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,userData.textureId);
    glUniform1i(userData.samplerLoc,0);

    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_SHORT,indices);

}






