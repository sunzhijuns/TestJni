//
// Created by sunzhijun on 2018/1/8.
//

#ifndef TESTJNI_TEXTUREWRAP_H
#define TESTJNI_TEXTUREWRAP_H

//TextureWrap
//TEXTUREWRAP
#include "../protol.h"

class TextureWrapRender : public Render{
public:

    TextureWrapRender();
    virtual ~TextureWrapRender();
    virtual bool  init() override;
    virtual void resize(int w, int h) override;
    virtual void render() override;

protected:
    virtual void step() override;

    virtual void draw() override ;
};


#endif //TESTJNI_TEXTUREWRAP_H
