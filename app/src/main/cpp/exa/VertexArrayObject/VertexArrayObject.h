//
// Created by sunzhijun on 2018/1/9.
//

#ifndef TESTJNI_VERTEXARRAYOBJECT_H
#define TESTJNI_VERTEXARRAYOBJECT_H

#include "../protol.h"

class VAORender : public Render{
public:

    VAORender();
    virtual ~VAORender();
    virtual bool  init() override;
    virtual void resize(int w, int h) override;
    virtual void render() override;

protected:
    virtual void step() override;

    virtual void draw() override ;
};


#endif //TESTJNI_VERTEXARRAYOBJECT_H
