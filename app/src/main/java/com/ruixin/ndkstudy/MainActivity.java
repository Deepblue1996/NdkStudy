package com.ruixin.ndkstudy;

import android.annotation.SuppressLint;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.os.Bundle;
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
        final TextView numberText = findViewById(R.id.numberText);

        final Bitmap bitmap = getBitmap(R.mipmap.number);

        Bitmap[] pathBitmap = new Bitmap[10];
        for (int i = 0; i < 10; i++) {
            pathBitmap[i] = getBitmap(numberId[i]);
        }

        String name = JNIUtils.findNumber(bitmap, pathBitmap);

        numberText.setText(name);
    }

    private Bitmap getBitmap(int id) {
        return ((BitmapDrawable) getResources().getDrawable(id, getTheme())).getBitmap();
    }
}
