package com.szj.testjni.exa

import android.content.Context
import android.opengl.GLSurfaceView
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

/**
 * Created by sunzhijun on 2018/1/4.
 */
class ExaView(context:Context):GLSurfaceView(context) {
    init{
        setEGLConfigChooser(8,8,8,0,16,0)
        setEGLContextClientVersion(3)
        setRenderer(TriangleRender())
    }

    override fun onPause() {
        ExaLib.destory()
        super.onPause()
    }


    class TriangleRender : GLSurfaceView.Renderer {

        override fun onDrawFrame(gl: GL10?) {
            ExaLib.step() //To change body of created functions use File | Settings | File Templates.
        }

        override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
            ExaLib.resize(width,height) //To change body of created functions use File | Settings | File Templates.
        }

        override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
            ExaLib.init() //To change body of created functions use File | Settings | File Templates.
        }
    }

}