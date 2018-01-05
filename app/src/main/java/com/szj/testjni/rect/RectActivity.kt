package com.szj.testjni.rect

import android.opengl.GLSurfaceView
import android.os.Bundle
import android.support.v7.app.AppCompatActivity

/**
 * Created by sunzhijun on 2018/1/5.
 */
class RectActivity :AppCompatActivity() {
    lateinit var view:GLSurfaceView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        view = RectView(this)
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