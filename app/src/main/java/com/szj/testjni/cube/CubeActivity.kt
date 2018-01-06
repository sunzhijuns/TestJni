package com.szj.testjni.cube

import android.opengl.GLSurfaceView
import android.os.Bundle
import android.support.v7.app.AppCompatActivity

/**
 * Created by sunzhijun on 2018/1/6.
 */
class CubeActivity: AppCompatActivity() {
    lateinit var view: GLSurfaceView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        view = CubeView(this)
        setContentView(view)
    }

    override fun onResume() {
        super.onResume()
        view.onResume()
    }

    override fun onPause() {
        super.onPause()
        view.onPause()
    }
}