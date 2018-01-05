//
// Created by sunzhijun on 2018/1/4.
//

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

