//
// Created by sunzhijun on 2018/1/8.
//

#ifndef TESTJNI_VERTEXBUFFEROBJECT_H
#define TESTJNI_VERTEXBUFFEROBJECT_H


class Render{
public:
    bool  init();
    Render();
    ~Render();
    void resize(int w, int h);
    void render();

private:

    void draw();
    void calcSceneParams(int w, int h);
    void step();
};


#endif //TESTJNI_VERTEXBUFFEROBJECT_H
