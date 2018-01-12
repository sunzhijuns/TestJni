//
// Created by sunzhijun on 2018/1/8.
//

#ifndef TESTJNI_SIMPLETEXTURECUBE_H
#define TESTJNI_SIMPLETEXTURECUBE_H

//SimpleTextureCube
//SIMPLETEXTURECUBE
#include "../protol.h"

class SimpleTextureCubeRender : public Render{
public:

    SimpleTextureCubeRender();
    virtual ~SimpleTextureCubeRender();
    virtual bool  init() override;
    virtual void resize(int w, int h) override;
    virtual void render() override;

protected:
    virtual void step() override;

    virtual void draw() override ;
};


#endif //TESTJNI_SIMPLETEXTURECUBE_H
