//
// Created by sunzhijun on 2018/1/8.
//

#ifndef TESTJNI_MIPMAP2D_H
#define TESTJNI_MIPMAP2D_H

//MipMap2D
//MIPMAP2D
#include "../protol.h"

class MipMap2DRender : public Render{
public:

    MipMap2DRender();
    virtual ~MipMap2DRender();
    virtual bool  init() override;
    virtual void resize(int w, int h) override;
    virtual void render() override;

protected:
    virtual void step() override;

    virtual void draw() override ;
};


#endif //TESTJNI_MIPMAP2D_H
