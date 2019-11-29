package com.ruixin.ndkstudy;

import android.annotation.SuppressLint;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.os.Bundle;
import android.util.Log;
import android.widget.ImageView;
import android.widget.TextView;

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
        final ImageView imageView1 = findViewById(R.id.imageView1);
        final ImageView imageView2 = findViewById(R.id.imageView2);
        final ImageView imageView3 = findViewById(R.id.imageView3);
        final ImageView imageView4 = findViewById(R.id.imageView4);
        final ImageView imageView5 = findViewById(R.id.imageView5);
        final ImageView imageView6 = findViewById(R.id.imageView6);
        final ImageView imageView7 = findViewById(R.id.imageView7);
        final ImageView imageView8 = findViewById(R.id.imageView8);
        final ImageView imageView9 = findViewById(R.id.imageView9);
        final ImageView imageView10 = findViewById(R.id.imageView10);

        final Bitmap bitmap = newBitmap(R.mipmap.number);

        Bitmap[] pathBitmap = new Bitmap[10];
        for (int i = 0; i < 10; i++) {
            pathBitmap[i] = newBitmap(numberId[i]);
        }

        Bitmap bitmaps = JNIUtils.findNumber(bitmap, pathBitmap, pathBitmap[0]);

        imageView1.setImageBitmap(bitmaps);
//        imageView2.setImageBitmap(bitmaps[1]);
//        imageView3.setImageBitmap(bitmaps[2]);
//        imageView4.setImageBitmap(bitmaps[3]);
//        imageView5.setImageBitmap(bitmaps[4]);
//        imageView6.setImageBitmap(bitmaps[5]);
//        imageView7.setImageBitmap(bitmaps[6]);
//        imageView8.setImageBitmap(bitmaps[7]);
//        imageView9.setImageBitmap(bitmaps[8]);
//        imageView10.setImageBitmap(bitmaps[9]);
    }

    private Bitmap newBitmap(int id) {
        Bitmap map = ((BitmapDrawable) getResources().getDrawable(id, getTheme())).getBitmap();
        return map.copy(map.getConfig(), true);
    }
}
