//
// Created by sunzhijun on 2018/1/8.
//

#ifndef TESTJNI_VERTEXBUFFEROBJECTGEN3_H
#define TESTJNI_VERTEXBUFFEROBJECTGEN3_H


#include "../protol.h"

class VBOGen3Render : public Render{
public:

    VBOGen3Render();
    virtual ~VBOGen3Render();
    virtual bool  init() override;
    virtual void resize(int w, int h) override;
    virtual void render() override;

protected:

    virtual void draw() override ;
};


#endif //TESTJNI_VERTEXBUFFEROBJECTGEN3_H
