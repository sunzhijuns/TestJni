//
// Created by sunzhijun on 2018/1/3.
//

#ifndef TESTJNI_NATIVE_ACTIVITY_H
#define TESTJNI_NATIVE_ACTIVITY_H

#include<android/native_activity.h>

/**
 * 绑定生命周期函数到activity
 */
void bindLifeCycle(ANativeActivity * activity);

/**
 * NaviveActivity的入口函数
 */
void ANativeActivity_onCreate(ANativeActivity * activity, void *savedState, size_t savedStateSize);

/**
 * 处理事件队列的线程函数
 */
void *looper(void *args);

//第一部分
void onStart(ANativeActivity * activity);
void onResume(ANativeActivity * activity);

void *onSaveInstanceState(ANativeActivity * activity, size_t * outSize);

void onPause(ANativeActivity * activity);

void onStop(ANativeActivity * activity);

void onDestroy(ANativeActivity * activity);

 /**
  * 窗口改变时回调。对于游戏应用相当有用，当应用失去焦点时，应该暂停游戏
  */
void onWindowFocusChanged(ANativeActivity * activity, int hasFocus);

void onConfigurationChanged(ANativeActivity * activity);

//第二部分
/**
 * 可绘制的窗口被创建，可以通过ANativeWindow对象的缓冲区绘制图案
 */
void onNativeWindowCreated(ANativeActivity * activity, ANativeWindow * window);
/**
 * 绘制窗口被销毁时回调
 */
void onNativeWindowDestroyed(ANativeActivity * activity, ANativeWindow * window);
//第三部分
/**
 * 输入事件队列被创建时回调
 */
void onInputQueueCreated(ANativeActivity * activity, AInputQueue * queue);
/**
 * 输入事件队列被销毁时回调
 */
void onInputQueueDestroyed(ANativeActivity * activity, AInputQueue * queue);


//第四部分
void onLowMemory(ANativeActivity * activity);




#endif //TESTJNI_NATIVE_ACTIVITY_H
