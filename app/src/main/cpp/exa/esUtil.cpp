//
// Created by sunzhijun on 2018/1/4.
//

#include <string.h>
#include <math.h>
#include"esUtil.h"

bool checkGlError(const char *funcName) {
    GLint err = glGetError();
    if (err != GL_NO_ERROR) {
        ALOGE("GL error after %s(): 0x%08x\n", funcName, err);
        return true;
    }
    return false;
}


GLuint createShader(GLenum shaderType, const char *src) {

    ALOGE("创建 着色器 createShader");
    GLuint shader = glCreateShader(shaderType);
    if (!shader) {
        checkGlError("glCreateShader");
        return 0;
    }
    GLint compiled = GL_FALSE;
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLogLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);
        if (infoLogLen > 0) {
            GLchar *infoLog = (GLchar *) malloc(infoLogLen);
            if (infoLog) {
                glGetShaderInfoLog(shader, infoLogLen, NULL, infoLog);
                ALOGE("Could not compile %s shader:\n%s\n",
                      shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment", infoLog);
                free(infoLog);
            }
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}


GLuint createProgram(const char *vtxSrc, const char *fragSrc) {

    ALOGE("创建 着色器 createProgram");
    GLuint vtxShader = 0;
    GLuint fragShader = 0;
    GLuint program = 0;
    GLint linked = GL_FALSE;

    vtxShader = createShader(GL_VERTEX_SHADER, vtxSrc);
    if (!vtxShader){
        ALOGE("创建 着色器 cvtxShader 失败");
        goto exit;
    }

    fragShader = createShader(GL_FRAGMENT_SHADER,fragSrc);
    if (!fragShader){
        ALOGE("创建 着色器 fragShader 失败");
        goto exit;
    }

    program = glCreateProgram();
    if (!program){
        checkGlError("glCreateProgram");
        ALOGE("创建 program 失败");
        goto exit;
    }

    glAttachShader(program,vtxShader);
    glAttachShader(program,fragShader);

    glLinkProgram(program);
    glGetProgramiv(program,GL_LINK_STATUS,&linked);
    if (!linked){
        ALOGE("Could not link program");
        GLint infoLogLen = 0;
        glGetProgramiv(program,GL_INFO_LOG_LENGTH,&infoLogLen);
        if (infoLogLen){
            GLchar * infoLog = (GLchar *) malloc(infoLogLen);
            if (infoLog){
                glGetProgramInfoLog(program,infoLogLen,NULL,infoLog);
                ALOGE("Could not link program:\n%s\n",infoLog);
                free(infoLog);
            }
        }
        glDeleteProgram(program);
        program = 0;
    }

    exit:
    glDeleteShader(vtxShader);
    glDeleteShader(fragShader);
    return program;
}

int esGenCube(float scale,GLfloat **vertices,
              GLfloat **normals,GLfloat **texCoords,GLuint **indices){
    int i;
    int numVertices = 24;
    int numIndices = 36;

    GLfloat cubeVerts[] = {
            //前
            -0.5f,  -0.5f,  -0.5f,  //0, 8, 16
            -0.5f,  -0.5f,   0.5f,  //1, 12, 17
             0.5f,  -0.5f,   0.5f,  //2, 15, 21
             0.5f,  -0.5f,  -0.5f,  //3, 11, 20
            //后
            -0.5f,   0.5f,  -0.5f,  //4, 9, 19
            -0.5f,   0.5f,   0.5f,  //5, 13, 18
             0.5f,   0.5f,   0.5f,  //6, 14, 22
             0.5f,   0.5f,  -0.5f,  //7, 10, 23
            //下
            -0.5f,  -0.5f,  -0.5f,  //0, 8, 16
            -0.5f,   0.5f,  -0.5f,  //4, 9, 19
             0.5f,   0.5f,  -0.5f,  //7, 10, 23
             0.5f,  -0.5f,  -0.5f,  //3, 11, 20
            //上
            -0.5f,  -0.5f,   0.5f,  //1, 12, 17
            -0.5f,   0.5f,   0.5f,  //5, 13, 18
             0.5f,   0.5f,   0.5f,  //6, 14, 22
             0.5f,  -0.5f,   0.5f,  //2, 15, 21
            //左
            -0.5f,  -0.5f,  -0.5f,  //0, 8, 16
            -0.5f,  -0.5f,   0.5f,  //1, 12, 17
            -0.5f,   0.5f,   0.5f,  //5, 13, 18
            -0.5f,   0.5f,  -0.5f,  //4, 9, 19
            //右
             0.5f,  -0.5f,  -0.5f,  //3, 11, 20
             0.5f,  -0.5f,   0.5f,  //2, 15, 21
             0.5f,   0.5f,   0.5f,  //6, 14, 22
             0.5f,   0.5f,  -0.5f,  //7, 10, 23
    };
    GLfloat cubeNormals[] = {
            //前
            0.0f,-1.0f,0.0f,
            0.0f,-1.0f,0.0f,
            0.0f,-1.0f,0.0f,
            0.0f,-1.0f,0.0f,
            //后
            0.0f,1.0f,0.0f,
            0.0f,1.0f,0.0f,
            0.0f,1.0f,0.0f,
            0.0f,1.0f,0.0f,
            //下
            0.0f,0.0f,-1.0f,
            0.0f,0.0f,-1.0f,
            0.0f,0.0f,-1.0f,
            0.0f,0.0f,-1.0f,
            //上
            0.0f,0.0f,1.0f,
            0.0f,0.0f,1.0f,
            0.0f,0.0f,1.0f,
            0.0f,0.0f,1.0f,
            //左
            -1.0f,0.0f,0.0f,
            -1.0f,0.0f,0.0f,
            -1.0f,0.0f,0.0f,
            -1.0f,0.0f,0.0f,
            //右
            1.0f,0.0f,0.0f,
            1.0f,0.0f,0.0f,
            1.0f,0.0f,0.0f,
            1.0f,0.0f,0.0f,
    };
    GLfloat cubeTex[]={
            //前
            0.0f,0.0f,//0
            0.0f,1.0f,//1
            1.0f,1.0f,//2
            1.0f,0.0f,//3
            //后
            1.0f,0.0f,//4
            1.0f,1.0f,//5
            0.0f,1.0f,//6
            0.0f,0.0f,//7
            //下
            0.0f,0.0f,//0,8
            0.0f,1.0f,//1,9
            1.0f,1.0f,//2,10
            1.0f,0.0f,//3,11
            //上
            0.0f,0.0f,//0,8,12
            0.0f,1.0f,//1,9,13
            1.0f,1.0f,//2,10,14
            1.0f,0.0f,//3,11,15
            //左
            0.0f,0.0f,//0,8,12,16
            0.0f,1.0f,//1,9,13,17
            1.0f,1.0f,//2,10,14,18
            1.0f,0.0f,//3,11,15,19
            //右
            0.0f,0.0f,//0,8,12,16,20
            0.0f,1.0f,//1,9,13,17,21
            1.0f,1.0f,//2,10,14,18,22
            1.0f,0.0f,//3,11,15,19,23
    };
    //allocate memory for buffers
    if(vertices != NULL){
        *vertices = (GLfloat *) malloc(sizeof(GLfloat) * 3 * numVertices);
        memcpy(*vertices,cubeVerts, sizeof(cubeVerts));
        for (i = 0; i < numVertices * 3; ++i) {
            (*vertices)[i] *= scale;
        }
    }
    if (normals !=NULL){
        *normals = (GLfloat *) malloc(sizeof(GLfloat) * 3 * numVertices);
        memcpy(*normals, cubeNormals, sizeof(cubeNormals));
    }

    if (texCoords != NULL){
        *texCoords = (GLfloat *) malloc(sizeof(GLfloat) * 2 * numVertices);
        memcpy(*texCoords, cubeTex, sizeof(cubeTex));
    }

    if (indices != NULL){
        GLuint cubeIndices[] = {
                //前
                0,2,1,
                0,3,2,
                //后
                4,5,6,
                4,6,7,
                //下
                8,9,10,
                8,10,11,
                //上
                12,15,14,
                12,14,13,
                //左
                16,17,18,
                16,18,19,
                //右
                20,23,22,
                20,22,21,
        };
        *indices = (GLuint *) malloc(sizeof(GLuint) * numIndices);
        memcpy(*indices,cubeIndices, sizeof(cubeIndices));
    }
    return numIndices;
}




void esMatrixLoadIdentity(ESMatrix * result){
    memset(result, 0x0, sizeof(ESMatrix));
    result->m[0][0] = 1.0f;
    result->m[1][1] = 1.0f;
    result->m[2][2] = 1.0f;
    result->m[3][3] = 1.0f;
}


void esMatrixMultiply(ESMatrix * result, ESMatrix * srcA, ESMatrix * srcB){
    ESMatrix tmp;
    int i;
    for (i = 0; i < 4; ++i) {
        tmp.m[i][0] = (srcA->m[i][0] * srcB->m[0][0]) +
                      (srcA->m[i][1] * srcB->m[1][0]) +
                      (srcA->m[i][2] * srcB->m[2][0]) +
                      (srcA->m[i][3] * srcB->m[3][0]);

        tmp.m[i][1] = (srcA->m[i][0] * srcB->m[0][1]) +
                      (srcA->m[i][1] * srcB->m[1][1]) +
                      (srcA->m[i][2] * srcB->m[2][1]) +
                      (srcA->m[i][3] * srcB->m[3][1]);

        tmp.m[i][2] = (srcA->m[i][0] * srcB->m[0][2]) +
                      (srcA->m[i][1] * srcB->m[1][2]) +
                      (srcA->m[i][2] * srcB->m[2][2]) +
                      (srcA->m[i][3] * srcB->m[3][2]);

        tmp.m[i][3] = (srcA->m[i][0] * srcB->m[0][3]) +
                      (srcA->m[i][1] * srcB->m[1][3]) +
                      (srcA->m[i][2] * srcB->m[2][3]) +
                      (srcA->m[i][3] * srcB->m[3][3]);
    }
    memcpy(result, &tmp, sizeof(ESMatrix));
}


void esTranslate(ESMatrix * result, GLfloat tx, GLfloat ty, GLfloat tz){
    result->m[3][0] += result->m[0][0] * tx + result->m[1][0] * ty + result->m[2][0] * tz;
    result->m[3][1] += result->m[0][1] * tx + result->m[1][1] * ty + result->m[2][1] * tz;
    result->m[3][2] += result->m[0][2] * tx + result->m[1][2] * ty + result->m[2][2] * tz;
    result->m[3][3] += result->m[0][3] * tx + result->m[1][3] * ty + result->m[2][3] * tz;
}

void esRotate(ESMatrix * result,GLfloat  angle, GLfloat x, GLfloat y, GLfloat z){
    GLfloat sinAngle,cosAngle;
    GLfloat mag = sqrtf(x*x + y*y + z*z);
    sinAngle = sinf(float(angle * M_PI / 180.f ));
    cosAngle = cosf(float(angle * M_PI / 180.f));

    if (mag > 0.0f){
        GLfloat xx,yy,zz,xy,yz,zx,xs,ys,zs;
        GLfloat oneMinusCos;
        ESMatrix rotMat;

        x/=mag;
        y/=mag;
        z/=mag;

        xx = x*x;
        yy = y*y;
        zz = z*z;
        xy = x*y;
        yz = y*z;
        zx = z*x;
        xs = x*sinAngle;
        ys = y*sinAngle;
        zs = z*sinAngle;

        oneMinusCos = 1.0f - cosAngle;

        rotMat.m[0][0] = (oneMinusCos * xx) + cosAngle;
        rotMat.m[0][1] = (oneMinusCos * xy) - zs;
        rotMat.m[0][2] = (oneMinusCos * zx) + ys;
        rotMat.m[0][3] = 0.0f;

        rotMat.m[1][0] = (oneMinusCos * xy) + zs;
        rotMat.m[1][1] = (oneMinusCos * yy) + cosAngle;
        rotMat.m[1][2] = (oneMinusCos * yz) - xs;
        rotMat.m[1][3] = 0.0f;

        rotMat.m[2][0] = (oneMinusCos * zx) - ys;
        rotMat.m[2][1] = (oneMinusCos * yz) + xs;
        rotMat.m[2][2] = (oneMinusCos * zz) + cosAngle;
        rotMat.m[2][3] = 0.0f;

        rotMat.m[3][0] = 0.0f;
        rotMat.m[3][1] = 0.0f;
        rotMat.m[3][2] = 0.0f;
        rotMat.m[3][3] = 1.0f;

        esMatrixMultiply(result, &rotMat, result);
    }
}

void esFrustum(ESMatrix * result, float left, float right,
               float bottom, float top, float nearZ, float farZ){
    float deltaX = right - left;
    float deltaY = top - bottom;
    float deltaZ = farZ - nearZ;
    ESMatrix frust;

    if ((deltaX <= 0.0f) || (deltaY <= 0.0f) || (deltaZ <= 0.0f)
        ||(nearZ <= 0.0f) || (farZ <= 0.0f)){
        return;
    }
    frust.m[0][0] = 2.0f * nearZ / deltaX;
    frust.m[0][1] = frust.m[0][2] = frust.m[0][3] = 0.0f;

    frust.m[1][1] = 2.0f * nearZ / deltaY;
    frust.m[1][0] = frust.m[1][2] = frust.m[1][3] = 0.0f;

    frust.m[2][0] = (right + left) / deltaX;
    frust.m[2][1] = (top + bottom) / deltaY;
    frust.m[2][2] = -(nearZ + farZ) / deltaZ;
    frust.m[2][3] = -1.0f;

    frust.m[3][2] = -2.0f * nearZ * farZ / deltaZ;
    frust.m[3][0] = frust.m[3][1] = frust.m[3][3] = 0.0f;

    esMatrixMultiply(result,&frust,result);
}
void esPerspective(ESMatrix *result, float fovy, float aspect, float nearZ, float farZ){
    GLfloat  frustumW, frustumH;
    frustumH = tanf(fovy / 360.0f * M_PI) * nearZ;
    frustumW = frustumH * aspect;
    esFrustum(result, -frustumW,frustumW,-frustumH,frustumH,nearZ,farZ);
}
