package com.szj.testjni

import android.app.Application

/**
 * Created by sunzhijun on 2018/1/2.
 */
class NDKApplication :Application(){
    init {
        System.loadLibrary("native-lib")
    }
}