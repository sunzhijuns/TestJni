//
// Created by sunzhijun on 2018/1/8.
//

#ifndef TESTJNI_VERTEXBUFFEROBJECT_H
#define TESTJNI_VERTEXBUFFEROBJECT_H


#include "../protol.h"

class VBORender : public Render {
public:
    VBORender();
    virtual ~VBORender();
    virtual bool  init() override;
    virtual void resize(int w, int h) override;
    virtual void render() override;

protected:

    virtual void draw() override;
    void calcSceneParams(int w, int h);
};


#endif //TESTJNI_VERTEXBUFFEROBJECT_H
