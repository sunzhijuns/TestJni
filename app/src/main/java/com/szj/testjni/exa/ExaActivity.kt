package com.szj.testjni.exa

import android.os.Bundle
import android.support.v7.app.AppCompatActivity

/**
 * Created by sunzhijun on 2018/1/4.
 */
class ExaActivity : AppCompatActivity() {
    lateinit var exaView: ExaView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        exaView = ExaView(this)
        setContentView(exaView)
    }

    override fun onPause() {
        super.onPause()
        exaView.onPause()
    }

    override fun onResume() {
        super.onResume()
        exaView.onResume()
    }
}