//
// Created by sunzhijun on 2018/1/11.
//

#ifndef TESTJNI_SIMPLEVERTEXSHADER_H
#define TESTJNI_SIMPLEVERTEXSHADER_H

#include "../protol.h"
#include <stdint.h>
class SimpleVertexRender : public Render{
public:

    SimpleVertexRender();
    virtual ~SimpleVertexRender();
    virtual bool  init() override;
    virtual void resize(int w, int h) override;
    virtual void render() override;

protected:
    void step(float dt);
    virtual void step() override;

    virtual void draw() override ;

private:
    uint64_t lastFrameNs;
};

#endif //TESTJNI_SIMPLEVERTEXSHADER_H
