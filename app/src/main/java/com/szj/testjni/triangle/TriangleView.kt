package com.szj.testjni.triangle

import android.content.Context
import android.opengl.GLSurfaceView
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

/**
 * Created by sunzhijun on 2018/1/4.
 */
class TriangleView(context:Context):GLSurfaceView(context) {
    init{
        setEGLConfigChooser(8,8,8,0,16,0)
        setEGLContextClientVersion(3)
        setRenderer(TriangleRender())
    }

    class TriangleRender : GLSurfaceView.Renderer {

        override fun onDrawFrame(gl: GL10?) {
            TriangleLib.step() //To change body of created functions use File | Settings | File Templates.
        }

        override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
            TriangleLib.resize(width,height) //To change body of created functions use File | Settings | File Templates.
        }

        override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
            TriangleLib.init() //To change body of created functions use File | Settings | File Templates.
        }
    }

}