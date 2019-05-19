package com.yk.libjpeg_sample.libjpeg;

import android.graphics.Bitmap;

public class JpegUtils {
    static {
        System.loadLibrary("jpeg-yk");
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native  static void native_Compress(Bitmap bitmap, int q, String path);

}
