//
// Created by sunzhijun on 2018/1/8.
//

#ifndef TESTJNI_SIMPLETEXTURE2D_H
#define TESTJNI_SIMPLETEXTURE2D_H

//SimpleTexture2D.h
//SIMPLETEXTURE2D
#include "../protol.h"

class SimpleTexture2DRender : public Render{
public:

    SimpleTexture2DRender();
    virtual ~SimpleTexture2DRender();
    virtual bool  init() override;
    virtual void resize(int w, int h) override;
    virtual void render() override;

protected:
    virtual void step() override;

    virtual void draw() override ;
};


#endif //TESTJNI_SIMPLETEXTURE2D_H
