//
// Created by sunzhijun on 2018/1/3.
//

#ifndef TESTJNI_NATIVE_ACTIVITY_H_H
#define TESTJNI_NATIVE_ACTIVITY_H_H


#include <cstdlib>
#include<stdint.h>
#include<android/asset_manager.h>
#include <jni.h>

//该结构体定义了android.app.NativeActivity.对象。该对象有Android框架创建并启动
typedef struct ANativeActivity{

    /**
     * 声明周期回调方法结构体指针。注意，我们不能修改callbacks的指向，因为这是
     * 有框架所指定的，但我们可以向其中赋值
     */
    struct ANativeActivityCallbacks* callbacks;
    /**
     * Java虚拟机对象
     */
    JavaVM*vm;
    /**
     * JNI上下文对象，只能在主线程中被访问
     */
    JNIEnv * env;
    /**
     * 这是NativeActivity对象句柄
     * 注意：这是一个错误的变量名称（我就想知道这货被扣了多少钱的工资）。该变量本应该被命名为activity
     * 而不是clazz
     */
    jobject clazz;
    /**
     * 应用内数据目录路径名
     */
    const char * internalDataPath;
    /**
     * 外部设备存储目录路径名
     */
    const char * externalDataPath;
    /**
     * sdk版本
     */
    int32_t sdkVersion;
    /**
     * 这是一个用户专属变量，不受框架控制，用户可以将一些数据存储到此处，方便在其他地方使用
     */
    void* instance;

    /**
     * Asset Manager 对象。用户可以通过此对象将一些数据存储到此处，方便在其他地方使用。
     */
    AAssetManager* assetManager;
    /**
     * 指向应用内obb文件的目录，如果应用没有obb文件，此路径可能不存在。
     */
    const char* obbPath;
}ANativeActivity;
typedef int  ARect;
typedef int  ANativeWindow;
typedef int  AInputQueue;


//生命周期回调
typedef struct ANativeActivityCallbacks{
    //第一部分
    void (*onStart)(ANativeActivity * activity);
    void (*onResume)(ANativeActivity * activity);
    void*(*onSaveInstanceState)(ANativeActivity * activity, size_t * outSize);
    void (*onPause)(ANativeActivity * activity);
    void (*onStop)(ANativeActivity * activity);
    void (*onDestroy)(ANativeActivity * activity);
    /**
     * 窗口改变时回调。对于游戏应用相当有用，当应用失去焦点时，应该暂停游戏
     */
    void (*onWindowFocusChanged)(ANativeActivity* activity, int hasFoucs);
    void (*onContentRectChanged)(ANativeActivity* activity, const ARect * rect);
    void (*onConfigurationChanged)(ANativeActivity* activity);

    //第二部分
    /**
     * 可绘制的窗口被创建，可以通过ANativeWindow对象的缓冲区绘制图案
     */
    void (*onNativeWindowCreated)(ANativeActivity * activity,ANativeWindow * window);
    /**
     * 窗口尺寸改变。在不可分屏的设备上无意义
     */
    void (*onNativeWindowResized)(ANativeActivity *activity,ANativeWindow * window);
    /**
     * 窗口重绘
     */
    void (*onNativeWindowRedrawNeeded)(ANativeActivity *activity,ANativeWindow * window);
    /**
     * 绘制窗口被销毁时回调
     */
    void (*onNativeWindowDestroyed)(ANativeActivity *activity,ANativeWindow * window);

    //第三部分
    /**
     * 输入事件队列被创建时回调
     */
    void (*onInputQueueCreated)(ANativeActivity * activity, AInputQueue * queue);
    /**
     * 输入事件队列被销毁时回调
     */
    void (*onInputQueueDestroyed)(ANativeActivity * activity, AInputQueue * queue);

    //第四部分
    void (*onLowMemory)(ANativeActivity * activity);

}ANativeActivityCallbacks;

typedef void ANativeActivity_createFunc(ANativeActivity*activity,
    void* savedState, size_t savedStateSize);

extern ANativeActivity_createFunc ANativeActivity_onCreate;

/**
 * 与普通Activity中finish()方法相同，当调用此函数时，将会销毁此activity，注意，
 * 这个函数可以在任何线程被调用
 */
void ANativeActivity_finish(ANativeActivity * activity);

/**
 * 更改当前Activity中窗口的图像格式，目前可选WINDOW_FORMAT_RGBA_8888、WINDOW_FORMAT_RGBX_8888、
 * WINDOW_FORMAT_RGB_565，具体参数请参阅<native_window.h>。此函数可以在任何线程被调用
 */
void ANativeActivity_setWindowFormat(ANativeActivity * activity, int32_t format);

/**
 * 添加或移除当前Activity的窗口标记，比如AWINDOW_FLAG_FULLSCREEN等，参考<window.h>
 * 此函数可以在任何线程被调用
 */
void ANativeActivity_setWindowFlags(ANativeActivity_createFunc*activity,
    uint32_t addFlags, uint32_t removeFlags);

enum{
    //显示软键盘，被打开的键盘可以自动隐藏
    ANATIVEACTIVITY_SHOW_SOFT_INPUT_IMPLICIT = 0x0001,
    //不可以自动隐藏，除非调用相应的隐藏方法
    ANATIVEACTIVITY_SHOW_SOFT_INPUT_FORCED = 0x0002,
};

//在当前activity上打开软键盘，该函数可以在任何线程被调用
void ANativeActivity_showSoftInput(ANativeActivity* activity,uint32_t flags);

enum{
    //通知表示可以隐藏软键盘
    ANATIVEACTIVITY_NIDE_SOFT_INPUT_IMPLICIT_ONLY = 0x0001,
    ANATIVEACTIVITY_NIDE_SOFT_INPUT_NOT_ALWAYS = 0x0002,

};
void ANativeActivity_hideSoftInput(ANativeActivity* activity,uint32_t flags);

#endif //TESTJNI_NATIVE_ACTIVITY_H_H
