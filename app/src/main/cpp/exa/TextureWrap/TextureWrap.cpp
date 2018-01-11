//
// Created by sunzhijun on 2018/1/8.
//
#include "TextureWrap.h"
#include<GLES3/gl3.h>
#include<android/log.h>

#include <stdlib.h>

#include <jni.h>

#define LOG_TAG "TextureWrap-LIB"

#include "../esUtil.h"



typedef struct{
    GLuint program;

    GLint samplerLoc;
    GLint offsetLoc;

    GLuint textureId;
}UserData;

#define STR(s) #s
#define STRV(s) STR(s)

static GLubyte * GenCheckImage(int width, int height, int checkSize){
    ALOGE("GenCheckImage--1");
    GLubyte * pixels = (GLubyte *) malloc(width * height * 3);
    if (pixels == NULL){
        ALOGE("GenCheckImage pixels == NULL");
        return NULL;
    }
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            GLubyte r = 0;
            GLubyte b = 0;

            if ((x / checkSize) %2 == 0){
                r = (GLubyte) (255 * ((y / checkSize) % 2));;
                b = (GLubyte) (255 * (1 - (y / checkSize) % 2));
            }else{
                b = (GLubyte) (255 * ((y / checkSize) % 2));;
                r = (GLubyte) (255 * (1 - (y / checkSize) % 2));
            }
            pixels[(y*width + x) * 3] = r;
            pixels[(y*width + x) * 3 + 1] = 0;
            pixels[(y*width + x) * 3 + 2] = b;
        }
    }
    ALOGE("GenCheckImage--2");
    return pixels;

}

static GLuint CreateTexture2D(){
    ALOGE("CreateTexture2D--1");
    GLuint textureId;

    int width = 256;
    int height = 256;

    GLubyte * pixels;

    pixels = GenCheckImage(width,height,64);

    if (pixels == NULL){
        return 0;
    }

    glGenTextures(1,&textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,pixels);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    free(pixels);
    ALOGE("CreateTexture2D--2");
    return textureId;
}

static const char VERTEX_SHADER[] =
        "#version 300 es\n"
                "uniform float u_offset;\n"
                "layout(location = 0) in vec4 a_position;\n"
                "layout(location = 1) in vec2 a_texCoord;\n"
                "out vec2 v_texCoord;\n"
                "void main(){\n"
                "   gl_Position = a_position;\n"
                "   gl_Position.x += u_offset;\n"
                "   v_texCoord = a_texCoord;\n"
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

#define RANGE 0.2f
static const GLfloat VERTEX[] = {
        -RANGE,   RANGE,   0.0f,  1.0f,          //position 0
        -1.0f,   -1.0f,             //texCoord 0
        -RANGE,  -RANGE,   0.0f,  1.0f,          //position 1
        -1.0f,    2.0f,             //texCoord 1
         RANGE,  -RANGE,   0.0f,  1.0f,         //position 2
         2.0f,    2.0f,             //texCoord 2
         RANGE,   RANGE,   0.0f,  1.0f,         //position 3
         2.0f,   -1.0f,             //texCoord 3
};

static const GLushort indices[] = {
        0,1,2,0,2,3,
};

static UserData  userData;

TextureWrapRender::TextureWrapRender(){

    return;
}

bool TextureWrapRender::init() {

    GLuint program = createProgram(VERTEX_SHADER,FRAGMENT_SHADER);
    if (!program){
        ALOGE("程序创建失败");
        return false;
    }
    userData.program = program;

    userData.samplerLoc = glGetUniformLocation(userData.program,"s_texture");
    userData.offsetLoc = glGetUniformLocation(userData.program,"u_offset");
    userData.textureId = CreateTexture2D();

    glClearColor(0.0f,0.0f,0.0f,0.0f);
    return true;
}


TextureWrapRender::~TextureWrapRender() {
    ALOGE("删除 program TextureWrapRender3Render");
    if (userData.program){
        glDeleteProgram(userData.program);
    }
    glDeleteTextures(1,&userData.textureId);
}

void TextureWrapRender::resize(int w, int h) {
    glViewport(0,0,w,h);
    glClear(GL_COLOR_BUFFER_BIT);
}

void TextureWrapRender::step() {

}


void TextureWrapRender::render() {

    step();
    glClear(GL_COLOR_BUFFER_BIT);
    draw();
    checkGlError("TextureWrapRender::render");
}

void TextureWrapRender::draw() {

    glUseProgram(userData.program);

    glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,6* sizeof(GLfloat),VERTEX);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,6* sizeof(GLfloat),&VERTEX[4]);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,userData.textureId);
    glUniform1i(userData.samplerLoc,0);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glUniform1f(userData.offsetLoc,-0.7f);
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_SHORT,indices);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glUniform1f(userData.offsetLoc,0.0f);
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_SHORT,indices);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_MIRRORED_REPEAT);
    glUniform1f(userData.offsetLoc,0.7f);
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_SHORT,indices);

}






