//
// Created by sunzhijun on 2018/1/10.
//

#ifndef TESTJNI_DRAWELEMENTS_H
#define TESTJNI_DRAWELEMENTS_H

#include "../protol.h"
class DrawElementsRender : public Render{
public:

    DrawElementsRender();
    virtual ~DrawElementsRender();
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


#endif //TESTJNI_DRAWELEMENTS_H
