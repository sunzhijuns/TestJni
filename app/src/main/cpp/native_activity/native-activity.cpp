//
// Created by sunzhijun on 2018/1/3.
//
#define LOG_TAG "native-activity"

#include <sys/types.h>

#include<bits/pthread_types.h>
#include <pthread.h>
#include "../bitmap/JniUtil.h"

#include "native-activity.h"


/**
 * 绑定生命周期函数到activity
 */
void bindLifeCycle(ANativeActivity * activity)
{
    ALOGE("bindLifeCycle");
    activity->callbacks->onStart = onStart;
    activity->callbacks->onResume = onResume;
    activity->callbacks->onSaveInstanceState = onSaveInstanceState;
    activity->callbacks->onPause = onPause;
    activity->callbacks->onStop = onStop;
    activity->callbacks->onDestroy = onDestroy;
    activity->callbacks->onWindowFocusChanged = onWindowFocusChanged;
    activity->callbacks->onConfigurationChanged = onConfigurationChanged;
    activity->callbacks->onNativeWindowCreated = onNativeWindowCreated;
    activity->callbacks->onNativeWindowDestroyed = onNativeWindowDestroyed;
    activity->callbacks->onInputQueueCreated = onInputQueueCreated;
    activity->callbacks->onInputQueueDestroyed = onInputQueueDestroyed;
    activity->callbacks->onLowMemory = onLowMemory;
}

/**
 * NaviveActivity的入口函数
 */
void ANativeActivity_onCreate(ANativeActivity * activity,
                              void *savedState, size_t savedStateSize)
{
    ALOGE("ANativeActivity_onCreate");
    bindLifeCycle(activity);

}

static bool isLoop = false;

static pthread_t loopID;

/**
 * 处理事件队列的线程函数
 */
void *looper(void *args){
    ANativeActivity * activity = (ANativeActivity *) args;
    AInputQueue * queue = (AInputQueue *) activity->instance;
    AInputEvent * event = NULL;

    while(isLoop){
        if(!AInputQueue_hasEvents(queue))//判断队列中是否有未处理事件
            continue;
        AInputQueue_getEvent(queue,&event);//从队列中获取一个事件
        switch (AInputEvent_getType(event)){
            case AINPUT_EVENT_TYPE_MOTION: {//触摸事件类型
                switch (AMotionEvent_getAction(event)){
                    case AMOTION_EVENT_ACTION_DOWN:{
                        //按下
                        float x = AMotionEvent_getX(event,0);
                        float y = AMotionEvent_getY(event,0);
                        ALOGE("X:%f,Y:%f",x,y);
                        break;
                    }
                    case AMOTION_EVENT_ACTION_UP:{//抬起
                        break;
                    }

                }
                break;
            }
            case AINPUT_EVENT_TYPE_KEY:{
                switch (AKeyEvent_getAction(event)) {
                    case AKEY_EVENT_ACTION_DOWN: {// 按下
                        switch (AKeyEvent_getKeyCode(event)){
                            case AKEYCODE_BACK://返回
                                ANativeActivity_finish(activity);//退出Activity
                                break;
                        }
                        break;
                    }
                    case AKEY_EVENT_ACTION_UP:
                        break;
                }
                break;
            }
        }
        AInputQueue_finishEvent(queue,event,1);//将事件从事件队列中移除
    }

    return args;
}

//第一部分
void onStart(ANativeActivity * activity)
{
    ALOGE("onStart");
}
void onResume(ANativeActivity * activity)
{
    ALOGE("onResume");
}

void *onSaveInstanceState(ANativeActivity * activity, size_t * outSize)
{
    ALOGE("onSaveInstanceState");
    return NULL;
}

void onPause(ANativeActivity * activity)
{
    ALOGE("onPause");
}

void onStop(ANativeActivity * activity)
{
    ALOGE("onStop");
}

void onDestroy(ANativeActivity * activity)
{
    ALOGE("onDestroy");
}

/**
 * 窗口改变时回调。对于游戏应用相当有用，当应用失去焦点时，应该暂停游戏
 */
void onWindowFocusChanged(ANativeActivity * activity, int hasFocus)
{
    ALOGE("onWindowFocusChanged");
}

void onConfigurationChanged(ANativeActivity * activity)
{
    ALOGE("onConfigurationChanged");
}

//第二部分
/**
 * 可绘制的窗口被创建，可以通过ANativeWindow对象的缓冲区绘制图案
 */
void onNativeWindowCreated(ANativeActivity * activity, ANativeWindow * window)
{
    ALOGE("onNativeWindowCreated");
}
/**
 * 绘制窗口被销毁时回调
 */
void onNativeWindowDestroyed(ANativeActivity * activity, ANativeWindow * window)
{
    ALOGE("onNativeWindowDestroyed");
}
//第三部分
/**
 * 输入事件队列被创建时回调
 */
void onInputQueueCreated(ANativeActivity * activity, AInputQueue * queue)
{
    ALOGE("onInputQueueCreated");
    isLoop = true;
    activity->instance = (void *)queue;
    pthread_create(&loopID,NULL,looper,activity);
}
/**
 * 输入事件队列被销毁时回调
 */
void onInputQueueDestroyed(ANativeActivity * activity, AInputQueue * queue)
{
    ALOGE("onInputQueueDestroyed");
    isLoop = false;
}


//第四部分
void onLowMemory(ANativeActivity * activity)
{
    ALOGE("onInputQueueDestroyed");
}
