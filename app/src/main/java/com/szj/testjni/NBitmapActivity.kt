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


        var kotlin_bitmap = bitmap.copy(Bitmap.Config.ARGB_4444,true)

        var startTime = System.currentTimeMillis()
        kotlinRenderGray(kotlin_bitmap)
        Log.e("--time--","kotlin Time:${System.currentTimeMillis() - startTime}")
        kotlin_to_gray.setImageBitmap(kotlin_bitmap)




        var jni_bitmap = bitmap.copy(Bitmap.Config.ARGB_4444,true)

        startTime = System.currentTimeMillis()
        NBitmapLib.renderGray(jni_bitmap)
        Log.e("--time--","JNI Time:${System.currentTimeMillis() - startTime}")
        native_to_gray.setImageBitmap(jni_bitmap)

        var java_bitmap = bitmap.copy(Bitmap.Config.ARGB_4444,true)

        startTime = System.currentTimeMillis()
        NBitmapLib.javaRenderGray(java_bitmap)
        Log.e("--time--","Java Time:${System.currentTimeMillis() - startTime}")
        java_to_gray.setImageBitmap(java_bitmap)

    }

    fun kotlinRenderGray(bitmap: Bitmap){
        val MODEL = 0xFF
        var height = bitmap.height
        var width = bitmap.width

        var pixelArray = IntArray(width * height)
        bitmap.getPixels(pixelArray,0,width,0,0,width,height)
        var color:Int
        var av:Int
        var alpha:Int
        for (i in pixelArray.indices){
            color = pixelArray[i]

            alpha = color shr 24

            av = 0
            av += color and MODEL
            av += color shr 8 and MODEL
            av += color shr 16 and MODEL
            av /= 3

            color = (alpha shl 8) + av
            color = (color shl 8) + av
            color = (color shl 8) + av

            pixelArray[i] = color
        }
        bitmap.setPixels(pixelArray,0,width,0,0,width,height)
    }
}