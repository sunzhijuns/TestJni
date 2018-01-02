package com.szj.testjni

import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import android.util.Log
import kotlinx.android.synthetic.main.activity_nbitmap.*

/**
 * Created by sunzhijun on 2018/1/3.
 */
class NBitmapActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_nbitmap)

        var bitmap = BitmapFactory.decodeResource(resources,R.drawable.hy)
        var jni_bitmap = bitmap.copy(Bitmap.Config.ARGB_4444,true)

        var startTime = System.currentTimeMillis()
        NBitmapLib.renderGray(jni_bitmap)
        Log.e("--time--","JNI Time:${System.currentTimeMillis() - startTime}")
        native_to_gray.setImageBitmap(jni_bitmap)

        var java_bitmap = bitmap.copy(Bitmap.Config.ARGB_4444,true)

        startTime = System.currentTimeMillis()
        NBitmapLib.javaRenderGray(java_bitmap)
        Log.e("--time--","Java Time:${System.currentTimeMillis() - startTime}")
        java_to_gray.setImageBitmap(java_bitmap)

    }
}