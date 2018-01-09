//
// Created by sunzhijun on 2018/1/9.
//

#ifndef TESTJNI_PROTOL_H
#define TESTJNI_PROTOL_H
class Render{
public:
    virtual ~Render() {}
    virtual bool init() = 0;
    virtual void resize(int w, int h) = 0;
    virtual void render() = 0;

protected:
    virtual void draw() = 0;
    virtual void step() = 0;
};
#endif //TESTJNI_PROTOL_H
