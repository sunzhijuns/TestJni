//
// Created by sunzhijun on 2018/1/4.
//
#include <jni.h>
#include<GLES3/gl3.h>
#include<android/log.h>

#include <stdlib.h>



#include <math.h>
#include <time.h>
#include <string.h>

#define STR(s) #s
#define STRV(s) STR(s)

#define POS_ATTRIB 0
#define COLOR_ATTRIB 1
#define SCALEROT_ATTRIB 2
#define OFFSET_ATTRIB 3

#define TWO_PI (2.0 * M_PI)
#define MAX_ROT_SPEED (0.3 * TWO_PI)

#define LOG_TAG "RECT-LIB"

#include"esUtil.h"
static const char VERTEX_SHADER[] =
        "#version 300 es\n"
                "layout(location = " STRV(POS_ATTRIB) ") in vec2 pos;\n"
                "layout(location = " STRV(COLOR_ATTRIB) ") in vec4 color;\n"
                "layout(location = " STRV(SCALEROT_ATTRIB) ") in vec4 scaleRot;\n"
                "layout(location = " STRV(OFFSET_ATTRIB) ") in vec2 offset;\n"
                "out vec4 vColor;"
                "void main(){\n"
                "   mat2 sr = mat2(scaleRot.xy, scaleRot.zw);\n"
                "   gl_Position = vec4(sr*pos + offset, 0.0, 1.0);\n"
                "   vColor = color;\n"
                "}\n";
static const char FRAGMENT_SHADER[] =
        "#version 300 es\n"
                "precision mediump float;\n"
                "in vec4 vColor;\n"
                "out vec4 outColor;\n"
                "void main(){\n"
                "   outColor = vColor;\n"
                "}\n";

struct Vertex{
    GLfloat pos[2];
    GLubyte rgba[4];
};
const Vertex QUAD[4] = {
        {{-0.7f,    -0.7f},{0x00,   0xFF,   0x00, 0x77}},
        {{ 0.7f,    -0.7f},{0x00,   0x00,   0xFF, 0x77}},
        {{-0.7f,     0.7f},{0xFF,   0x00,   0x00, 0x77}},
        {{ 0.7f,     0.7f},{0xFF,   0xFF,   0x00, 0x77}},
};

class Render{
public:
    bool  init();
    Render();
    ~Render();
    void resize(int w, int h);
    void render();

private:
    enum{
        VB_INSTANCE, VB_SCALEROT, VB_OFFSET, VB_COUNT
    };
    float mScale[2];
    float mAngularVelocity;
    uint64_t mLastFrameNs;
    float mAngles;
    GLuint mProgram;
    GLuint mVB[VB_COUNT];
    GLuint mVBState;
    float* mapTransformBuf();
    void unmapTransformBuf();
    void draw();
    void calcSceneParams(int w, int h);
    void step();
};

Render::Render() :mLastFrameNs(0),
                  mVBState(0),
                  mProgram(0){
    memset(mScale,0, sizeof(mScale));
    for (int i = 0; i < VB_COUNT; ++i) {
        mVB[i] = 0;
    }
}
Render::~Render() {
    glDeleteVertexArrays(1,&mVBState);
    glDeleteBuffers(VB_COUNT, mVB);
    glDeleteProgram(mProgram);
}

bool Render::init() {
    mProgram = createProgram(VERTEX_SHADER, FRAGMENT_SHADER);
    if (!mProgram){
        ALOGE("mProgram 创建失败");
        return false;
    }
    glGenBuffers(VB_COUNT,mVB);
    glBindBuffer(GL_ARRAY_BUFFER, mVB[VB_INSTANCE]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(QUAD),&QUAD[0],GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mVB[VB_SCALEROT]);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float),NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mVB[VB_OFFSET]);
    glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(float),NULL, GL_STATIC_DRAW);

    glGenVertexArrays(1,&mVBState);
    glBindVertexArray(mVBState);

    glBindBuffer(GL_ARRAY_BUFFER, mVB[VB_INSTANCE]);
    glVertexAttribPointer(POS_ATTRIB,2,GL_FLOAT,GL_FALSE, sizeof(Vertex),
                          (const GLvoid*)offsetof(Vertex,pos));
    glVertexAttribPointer(COLOR_ATTRIB,4,GL_UNSIGNED_BYTE,GL_TRUE, sizeof(Vertex),
                          (const GLvoid*)offsetof(Vertex,rgba));
    glEnableVertexAttribArray(POS_ATTRIB);
    glEnableVertexAttribArray(COLOR_ATTRIB);

    glBindBuffer(GL_ARRAY_BUFFER, mVB[VB_SCALEROT]);
    glVertexAttribPointer(SCALEROT_ATTRIB,4,GL_FLOAT,GL_FALSE,4* sizeof(float),0);
    glEnableVertexAttribArray(SCALEROT_ATTRIB);
    glVertexAttribDivisor(SCALEROT_ATTRIB,1);

    glBindBuffer(GL_ARRAY_BUFFER, mVB[VB_OFFSET]);
    glVertexAttribPointer(OFFSET_ATTRIB,2,GL_FLOAT,GL_FALSE,2* sizeof(float),0);
    glEnableVertexAttribArray(OFFSET_ATTRIB);
    glVertexAttribDivisor(OFFSET_ATTRIB,1);



    return true;
}

void Render::resize(int w, int h) {
    calcSceneParams(w,h);
    mAngles = float(drand48() * TWO_PI);
    mAngularVelocity = float(MAX_ROT_SPEED * (2.0 * drand48() - 1.0));
    mLastFrameNs = 0;
    glViewport(0,0,w,h);
}

void Render::calcSceneParams(int w, int h) {
    const float CELL_SIZE = 0.5f;
    const float scene2clip[2] = {
            1.0f, fmaxf(w,h) / fminf(w,h)
    };
    int major = w >= h ? 0 : 1;
    int minor = w >= h ? 1 : 0;

    mScale[major] = CELL_SIZE * scene2clip[0];
    mScale[minor] = CELL_SIZE * scene2clip[1];
}

void Render::step() {
    timespec now;
    clock_gettime(CLOCK_MONOTONIC,&now);
    auto nowNs = now.tv_sec * 1000000000ull + now.tv_nsec;

    if (mLastFrameNs > 0){
        float dt = float(nowNs - mLastFrameNs) * 0.000000001f;
        mAngles += mAngularVelocity * dt;
        if (mAngles >= TWO_PI){
            mAngles -= TWO_PI;
        }else if(mAngles <= -TWO_PI){
            mAngles += TWO_PI;
        }

        float * transforms = mapTransformBuf();
        float s = sinf(mAngles);
        float c = cosf(mAngles);

        transforms[0] = c * mScale[0];
        transforms[1] = s * mScale[1];
        transforms[2] =-s * mScale[0];
        transforms[3] = c * mScale[1];
    }
    unmapTransformBuf();
    mLastFrameNs = nowNs;
}

void Render::render() {
    step();

    glClearColor(0.2f, 0.2f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    draw();
    checkGlError("Render::render");
}

float * Render::mapTransformBuf() {
    glBindBuffer(GL_ARRAY_BUFFER, mVB[VB_SCALEROT]);
    return (float *) glMapBufferRange(GL_ARRAY_BUFFER,
                                      0, 4 * sizeof(float),
                                GL_MAP_WRITE_BIT|GL_MAP_INVALIDATE_BUFFER_BIT);
}

void Render::unmapTransformBuf() {
    glUnmapBuffer(GL_ARRAY_BUFFER);
}

void Render::draw() {
    glUseProgram(mProgram);
    glBindVertexArray(mVBState);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP,0,4,1);
}

Render *createRenderer(){
    Render * render = new  Render;
    if (!render->init()){
        delete render;
        return NULL;
    }
    return render;
}

static Render * g_render = NULL;

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_szj_testjni_rect_RectLib_init(JNIEnv *env, jclass type) {

    if (g_render){
        delete g_render;
        g_render = NULL;
    }
    g_render = createRenderer();
    if (!g_render){
        return JNI_FALSE;
    }
    return JNI_TRUE;

}
extern "C"
JNIEXPORT void JNICALL
Java_com_szj_testjni_rect_RectLib_resize(JNIEnv *env, jclass type, jint width, jint height) {

    if (g_render){
        g_render->resize(width,height);
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_szj_testjni_rect_RectLib_step(JNIEnv *env, jclass type) {

    if (g_render){
        g_render->render();
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_szj_testjni_rect_RectLib_destory(JNIEnv *env, jclass type) {

    delete g_render;
    g_render = NULL;

}

#undef LOG_TAG
