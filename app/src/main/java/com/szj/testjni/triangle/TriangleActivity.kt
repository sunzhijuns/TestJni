package com.szj.testjni.triangle

import android.os.Bundle
import android.support.v7.app.AppCompatActivity

/**
 * Created by sunzhijun on 2018/1/4.
 */
class TriangleActivity : AppCompatActivity() {
    lateinit var triangleView:TriangleView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        triangleView = TriangleView(this)
        setContentView(triangleView)
    }

    override fun onPause() {
        super.onPause()
        triangleView.onPause()
    }

    override fun onResume() {
        super.onResume()
        triangleView.onResume()
    }
}