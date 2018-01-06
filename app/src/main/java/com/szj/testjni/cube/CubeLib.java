package com.szj.testjni.cube;

/**
 * Created by sunzhijun on 2018/1/6.
 */

public class CubeLib {
    static {
        System.loadLibrary("cube-lib");
    }
    public static native boolean init();
    public static native void resize(int width, int height);
    public static native void step();
    public static native void destory();

}
