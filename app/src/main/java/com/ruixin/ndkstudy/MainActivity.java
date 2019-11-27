package com.ruixin.ndkstudy;

import android.annotation.SuppressLint;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.os.Bundle;
import android.widget.ImageView;

import androidx.appcompat.app.AppCompatActivity;

import com.ruixin.ndkstudy.inter.JNIUtils;

public class MainActivity extends AppCompatActivity {

    private int[] numberId = {
            R.mipmap.num_0,
            R.mipmap.num_1,
            R.mipmap.num_2,
            R.mipmap.num_3,
            R.mipmap.num_4,
            R.mipmap.num_5,
            R.mipmap.num_6,
            R.mipmap.num_7,
            R.mipmap.num_8,
            R.mipmap.num_9
    };

    @SuppressLint("WrongThread")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        final ImageView imageView = findViewById(R.id.imageView);

        Drawable drawable = getResources().getDrawable(R.mipmap.number, getTheme());
        final Bitmap bitmap = ((BitmapDrawable) drawable).getBitmap();

        int w = bitmap.getWidth();
        int h = bitmap.getHeight();

        int[] pixels = new int[w * h];

        bitmap.getPixels(pixels, 0, w, 0, 0, w, h);

        int[][] path = new int[10][80 * 80];
        for (int i = 0; i < 10; i++) {
            Drawable drawable2 = getResources().getDrawable(numberId[i], getTheme());
            final Bitmap bitmap2 = ((BitmapDrawable) drawable2).getBitmap();
            int[] pixels2 = new int[80 * 80];
            bitmap2.getPixels(pixels2, 0, 80, 0, 0, 80, 80);
            path[i] = pixels2;
        }

        int[] resultInt = JNIUtils.findNumber(pixels, w, h, path);

        Bitmap resultImg = Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888);

        resultImg.setPixels(resultInt, 0, w, 0, 0, w, h);
        //resultImg.setPixels(resultInt, 0, 80, 0, 0, 80, 80);

        imageView.setImageBitmap(resultImg);

    }
}
