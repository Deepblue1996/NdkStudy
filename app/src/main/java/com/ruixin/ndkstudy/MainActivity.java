package com.ruixin.ndkstudy;

import android.annotation.SuppressLint;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.widget.ImageView;

import androidx.appcompat.app.AppCompatActivity;

import com.ruixin.ndkstudy.inter.JNIUtils;

public class MainActivity extends AppCompatActivity {

    @SuppressLint("WrongThread")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        final ImageView imageView = findViewById(R.id.imageView);
        Drawable drawable = getResources().getDrawable(R.mipmap.test_temp, getTheme());
        final Bitmap bitmap = ((BitmapDrawable) drawable).getBitmap();

        int w = bitmap.getWidth();
        int h = bitmap.getHeight();

        int[] pixels = new int[w*h];

        bitmap.getPixels(pixels, 0, w, 0, 0, w, h);

        int[] resultInt = JNIUtils.bitmap2Grey(pixels, w, h);

        Bitmap resultImg = Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888);

        resultImg.setPixels(resultInt, 0, w, 0, 0, w, h);

        imageView.setImageBitmap(resultImg);
        
    }
}
