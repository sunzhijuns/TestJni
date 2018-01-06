package com.szj.testjni

import android.content.Intent
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import com.szj.testjni.cube.CubeActivity
import com.szj.testjni.rect.RectActivity
import com.szj.testjni.triangle.TriangleActivity
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // Example of a call to a native method
        sample_text.text = stringFromJNI()

        btn_draw_triangle.setOnClickListener {
            var intent = Intent(this,TriangleActivity::class.java)
            startActivity(intent)
        }

        btn_draw_rect.setOnClickListener {
            var intent = Intent(this, RectActivity::class.java)
            startActivity(intent)
        }

        btn_draw_cube.setOnClickListener {
            var intent = Intent(this, CubeActivity::class.java)
            startActivity(intent)
        }
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

}
