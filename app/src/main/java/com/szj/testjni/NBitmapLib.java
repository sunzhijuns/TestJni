package com.szj.testjni;

import android.graphics.Bitmap;
import android.util.Log;

/**
 * Created by sunzhijun on 2018/1/3.
 */

public class NBitmapLib {
    public static native void renderGray(Bitmap bitmap);

    //ARGB_4444
    public static void javaRenderGray(Bitmap bitmap){
        int MODEL = 0xFF;
        int height = bitmap.getHeight();
        int width = bitmap.getWidth();

        int pixelArray[] = new int[width * height];
        bitmap.getPixels(pixelArray,0,width,0,0,width,height);

        int color;
        int av;
        int alpha;
        Log.i("数组大小pixelArray.length",""+pixelArray.length);
        for (int i = 0; i<pixelArray.length;i++){
            color = pixelArray[i];
            alpha = color >> 24;
            av = 0;
            av += color & MODEL;
            av += (color >> 8) & MODEL;
            av += (color >> 16) & MODEL;
            av /= 3;

            color = (alpha << 8) + av;
            color = (color << 8) + av;
            color = (color << 8) + av;

            pixelArray[i] = color;
        }
        bitmap.setPixels(pixelArray,0,width,0,0,width,height);
    }

}
