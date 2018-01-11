//
// Created by sunzhijun on 2018/1/8.
//
#include "MipMap2D.h"
#include<GLES3/gl3.h>
#include<android/log.h>

#include <stdlib.h>

#include <jni.h>

#define LOG_TAG "MIPMAP2D-LIB"

#include "../esUtil.h"



typedef struct{
    GLuint program;

    GLint samplerLoc;
    GLint offsetLoc;

    GLuint textureId;
}UserData;

#define STR(s) #s
#define STRV(s) STR(s)

static GLboolean GenMipMap2D(GLubyte *src, GLubyte **dst,
                             int srcWidth,int srcHeight,int *dstWidth, int * dstHeight ){
    ALOGE("GenMipMap2D--1");
    int texelSize = 3;
    *dstWidth = srcWidth / 2;
    if (*dstWidth <= 0){
        *dstWidth = 1;
    }
    *dstHeight = srcHeight / 2;
    if (*dstHeight <= 0){
        *dstHeight = 1;
    }
    *dst = (GLubyte *) malloc(sizeof(GLubyte) * texelSize * (*dstWidth) * (*dstHeight));
    if (*dst == NULL){
        ALOGE("GenMipMap2D *dst == NULL");
        return GL_FALSE;
    }
    for (int y = 0; y < *dstHeight; ++y) {
        for (int x = 0; x < *dstWidth; ++x) {
            int srcIndex[4];
            float r = 0.0f,
                    g = 0.0f,
                    b = 0.0f;

            srcIndex[0] = ((y*2) * srcWidth + x*2)*texelSize;
            srcIndex[1] = ((y*2) * srcWidth + x*2+1)*texelSize;
            srcIndex[2] = ((y*2 + 1) * srcWidth + x*2)*texelSize;
            srcIndex[3] = ((y*2 + 1) * srcWidth + x*2 + 1)*texelSize;

            for (int sample = 0; sample < 4; ++sample) {
                r += src[srcIndex[sample]];
                g += src[srcIndex[sample] + 1];
                b += src[srcIndex[sample] + 2];
            }
            r /= 4.0;
            g /= 4.0;
            b /= 4.0;
//            (y*(*dstWidth) + x) * texelSize
            (*dst)[(y*(*dstWidth) + x) * texelSize] = (GLubyte) r;
            (*dst)[(y*(*dstWidth) + x) * texelSize + 1] = (GLubyte) g;
            (*dst)[(y*(*dstWidth) + x) * texelSize + 2] = (GLubyte) b;
        }
    }

    ALOGE("GenMipMap2D--2");
    return GL_TRUE;
}

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

static GLuint CreateMipMappedTexture2D(){
    ALOGE("CreateMipMappedTexture2D--1");
    GLuint textureId;

    int width = 256;
    int height = 256;

    int level;
    GLubyte * pixels;
    GLubyte * prevImage;
    GLubyte * newImage;

    pixels = GenCheckImage(width,height,8);

    if (pixels == NULL){
        return 0;
    }

    glGenTextures(1,&textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,pixels);

    level = 1;
    prevImage = &pixels[0];

    while (width > 1 && height > 1){
        int newWidth,newHeight;
        GenMipMap2D(prevImage,&newImage,width,height,&newWidth,&newHeight);
        glTexImage2D(GL_TEXTURE_2D,level,GL_RGB,newWidth,newHeight,0,
                     GL_RGB,GL_UNSIGNED_BYTE,newImage);
        free(prevImage);
        prevImage = newImage;
        level++;
        width = newWidth;
        height = newHeight;
    }
    free(newImage);


//    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    ALOGE("CreateMipMappedTexture2D--2");
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
static const GLfloat VERTEX[] = {
        -0.5f,   0.5f,   0.0f,  1.5f,          //position 0
         0.0f,   0.0f,             //texCoord 0
        -0.5f,  -0.5f,   0.0f,  0.75f,          //position 1
         0.0f,   1.0f,             //texCoord 1
         0.5f,  -0.5f,   0.0f,  0.75f,         //position 2
         1.0f,   1.0f,             //texCoord 2
         0.5f,   0.5f,   0.0f,  1.5f,         //position 3
         1.0f,   0.0f,             //texCoord 3
};

static const GLushort indices[] = {
        0,1,2,0,2,3,
};

static UserData  userData;

MipMap2DRender::MipMap2DRender(){

    return;
}

bool MipMap2DRender::init() {

    GLuint program = createProgram(VERTEX_SHADER,FRAGMENT_SHADER);
    if (!program){
        ALOGE("程序创建失败");
        return false;
    }
    userData.program = program;

    userData.samplerLoc = glGetUniformLocation(userData.program,"s_texture");
    userData.offsetLoc = glGetUniformLocation(userData.program,"u_offset");
    userData.textureId = CreateMipMappedTexture2D();

    glClearColor(0.0f,0.0f,0.0f,0.0f);
    return true;
}


MipMap2DRender::~MipMap2DRender() {
    ALOGE("删除 program MipMap2DRender3Render");
    if (userData.program){
        glDeleteProgram(userData.program);
    }
    glDeleteTextures(1,&userData.textureId);
}

void MipMap2DRender::resize(int w, int h) {
    glViewport(0,0,w,h);
    glClear(GL_COLOR_BUFFER_BIT);
}

void MipMap2DRender::step() {

}


void MipMap2DRender::render() {

    step();
    glClear(GL_COLOR_BUFFER_BIT);
    draw();
    checkGlError("MipMap2DRender::render");
}

void MipMap2DRender::draw() {

    glUseProgram(userData.program);

    glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,6* sizeof(GLfloat),VERTEX);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,6* sizeof(GLfloat),&VERTEX[3]);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,userData.textureId);
    glUniform1i(userData.samplerLoc,0);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glUniform1f(userData.offsetLoc,-0.51f);
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_SHORT,indices);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glUniform1f(userData.offsetLoc,0.51f);
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_SHORT,indices);

}






