package com.szj.testjni.rect;

/**
 * Created by sunzhijun on 2018/1/5.
 */

public class RectLib {
    static {
        System.loadLibrary("rect-lib");
    }
    public static native boolean init();
    public static native void resize(int width, int height);
    public static native void step();
    public static native void destory();

}
