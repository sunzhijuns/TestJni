package com.szj.testjni.cube

import android.content.Context
import android.opengl.GLSurfaceView
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

/**
 * Created by sunzhijun on 2018/1/6.
 */
class CubeView(context: Context) : GLSurfaceView(context) {
    init {
        setEGLConfigChooser(8, 8, 8, 0, 16, 0);
        setEGLContextClientVersion(3)
        setRenderer(RectRender())
    }

    override fun onPause() {
        CubeLib.destory()
        super.onPause()
    }

    class RectRender : GLSurfaceView.Renderer {
        override fun onDrawFrame(gl: GL10?) {
            CubeLib.step()
        }

        override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
            CubeLib.resize(width, height)
        }

        override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
            CubeLib.init()
        }
    }
}