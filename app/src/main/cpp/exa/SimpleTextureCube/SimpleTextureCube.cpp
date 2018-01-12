//
// Created by sunzhijun on 2018/1/8.
//
#include "SimpleTextureCube.h"
#include<GLES3/gl3.h>
#include<android/log.h>

#include <stdlib.h>

#include <jni.h>

#define LOG_TAG "TextureCube-LIB"

#include "../esUtil.h"



typedef struct{
    GLuint program;

    GLint samplerLoc;
    GLuint textureId;

    int numIndices;
    GLfloat *vertices;
    GLfloat *normals;
    GLuint *indices;
}UserData;

#define STR(s) #s
#define STRV(s) STR(s)

static GLuint CreateSimpleTextureCubemap(){
    GLuint textureId;
    GLubyte cubePixels[6][3] = {
            //0 , R
            255,    0,      0,
            //1 , G
            0,      255,    0,
            //2 , B
            0,      0,      255,
            //3 , Yellow
            255,    255,    0,
            //4 , Orange
            255,    128,    64,
            //5 , White
            255,    255,    255,

    };
    glGenTextures(1,&textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP,textureId);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,0,GL_RGB,1,1,0,
                 GL_RGB,GL_UNSIGNED_BYTE,&cubePixels[1]);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,0,GL_RGB,1,1,0,
                 GL_RGB,GL_UNSIGNED_BYTE,&cubePixels[2]);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,0,GL_RGB,1,1,0,
                 GL_RGB,GL_UNSIGNED_BYTE,&cubePixels[4]);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,0,GL_RGB,1,1,0,
                 GL_RGB,GL_UNSIGNED_BYTE,&cubePixels[0]);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,0,GL_RGB,1,1,0,
                 GL_RGB,GL_UNSIGNED_BYTE,&cubePixels[3]);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,0,GL_RGB,1,1,0,
                 GL_RGB,GL_UNSIGNED_BYTE,&cubePixels[5]);

    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    return textureId;
}

static const char VERTEX_SHADER[] =
        "#version 300 es\n"
                "layout(location = 0) in vec4 a_position;\n"
                "layout(location = 1) in vec3 a_normal;\n"
                "out vec3 v_normal;\n"
                "void main(){\n"
                "   v_normal = a_normal;\n"
                "   gl_Position = a_position;\n"
                "}\n";
static const char FRAGMENT_SHADER[] =
        "#version 300 es\n"
                "precision mediump float;\n"
                "in vec3 v_normal;\n"
                "out vec4 fragColor;\n"
                "uniform samplerCube s_texture;\n"
                "void main(){\n"
                "   fragColor = texture(s_texture,v_normal);\n"
                "}\n";

static UserData  userData;

SimpleTextureCubeRender::SimpleTextureCubeRender(){

    return;
}

bool SimpleTextureCubeRender::init() {

    GLuint program = createProgram(VERTEX_SHADER,FRAGMENT_SHADER);
    if (!program){
        ALOGE("程序创建失败");
        return false;
    }
    userData.program = program;

    userData.samplerLoc = glGetUniformLocation(userData.program,"s_texture");
    userData.textureId = CreateSimpleTextureCubemap();
    userData.numIndices = esGenSphere(30,0.75f,&userData.vertices,&userData.normals,
                                      NULL,&userData.indices);


    glClearColor(0.0f,0.0f,0.0f,0.0f);
    return true;
}


SimpleTextureCubeRender::~SimpleTextureCubeRender() {
    ALOGE("删除 program SimpleTextureCubeRender3Render");
    if (userData.program){
        glDeleteProgram(userData.program);
    }
    glDeleteTextures(1,&userData.textureId);

    free(userData.vertices);
    free(userData.normals);
}

void SimpleTextureCubeRender::resize(int w, int h) {
    glViewport(0,0,w,h);
    glClear(GL_COLOR_BUFFER_BIT);
}

void SimpleTextureCubeRender::step() {

}


void SimpleTextureCubeRender::render() {

    step();
    glClear(GL_COLOR_BUFFER_BIT);
    draw();
    checkGlError("SimpleTextureCubeRender::render");
}

void SimpleTextureCubeRender::draw() {

    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    glUseProgram(userData.program);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,userData.vertices);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,userData.normals);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP,userData.textureId);
    glUniform1i(userData.samplerLoc,0);
    glDrawElements(GL_TRIANGLES,userData.numIndices,GL_UNSIGNED_INT,userData.indices);


}






