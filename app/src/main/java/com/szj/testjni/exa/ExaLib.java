package com.szj.testjni.exa;

/**
 * Created by sunzhijun on 2018/1/4.
 */

public class ExaLib {
    static{
        System.loadLibrary("exa-lib");
    }
    //初始化本地GLES
    public static native boolean init();
    //为本地GLES设置宽和高
    public static native void resize(int width,int height);
    //用来绘制图像
    public static native void step();

}
