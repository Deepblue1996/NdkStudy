package com.ruixin.ndkstudy.inter;

import android.graphics.Bitmap;

public class JNIUtils {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public static native int[] bitmap2Grey(int[] pixels, int w, int h);

    public static native Bitmap findNumber(Bitmap bitmap, Bitmap[] bitmapBuf, Bitmap bitmapBuf2);
}
