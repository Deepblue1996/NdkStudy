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
    private int[] ziSmallId = {
            R.mipmap.zi_a,
            R.mipmap.zi_b,
            R.mipmap.zi_c,
            R.mipmap.zi_d,
            R.mipmap.zi_e,
            R.mipmap.zi_f,
            R.mipmap.zi_g,
            R.mipmap.zi_h,
            R.mipmap.zi_i,
            R.mipmap.zi_j,
            R.mipmap.zi_k,
            R.mipmap.zi_l,
            R.mipmap.zi_m,
            R.mipmap.zi_n,
            R.mipmap.zi_o,
            R.mipmap.zi_p,
            R.mipmap.zi_q,
            R.mipmap.zi_r,
            R.mipmap.zi_s,
            R.mipmap.zi_t,
            R.mipmap.zi_u,
            R.mipmap.zi_v,
            R.mipmap.zi_w,
            R.mipmap.zi_x,
            R.mipmap.zi_y,
            R.mipmap.zi_z
    };
    private int[] ziBigId = {
            R.mipmap.zi_ab,
            R.mipmap.zi_bb,
            R.mipmap.zi_cb,
            R.mipmap.zi_db,
            R.mipmap.zi_eb,
            R.mipmap.zi_fb,
            R.mipmap.zi_gb,
            R.mipmap.zi_hb,
            R.mipmap.zi_ib,
            R.mipmap.zi_jb,
            R.mipmap.zi_kb,
            R.mipmap.zi_lb,
            R.mipmap.zi_mb,
            R.mipmap.zi_nb,
            R.mipmap.zi_ob,
            R.mipmap.zi_pb,
            R.mipmap.zi_qb,
            R.mipmap.zi_rb,
            R.mipmap.zi_sb,
            R.mipmap.zi_tb,
            R.mipmap.zi_ub,
            R.mipmap.zi_vb,
            R.mipmap.zi_wb,
            R.mipmap.zi_xb,
            R.mipmap.zi_yb,
            R.mipmap.zi_zb
    };

    @SuppressLint({"WrongThread", "SetTextI18n"})
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView text = findViewById(R.id.text);

        final Bitmap bitmap = newBitmap(R.mipmap.number);

        Bitmap[] pathBitmap = new Bitmap[numberId.length];
        for (int i = 0; i < numberId.length; i++) {
            pathBitmap[i] = newBitmap(numberId[i]);
        }
        Bitmap[] smallBitmap = new Bitmap[ziSmallId.length];
        for (int i = 0; i < ziSmallId.length; i++) {
            smallBitmap[i] = newBitmap(ziSmallId[i]);
        }
        Bitmap[] bigBitmap = new Bitmap[ziBigId.length];
        for (int i = 0; i < ziBigId.length; i++) {
            bigBitmap[i] = newBitmap(ziBigId[i]);
        }

        String number = JNIUtils.findNumber(bitmap, pathBitmap, smallBitmap, bigBitmap);

        text.setText("分析:" + number);

    }

    private Bitmap newBitmap(int id) {
        Bitmap map = ((BitmapDrawable) getResources().getDrawable(id, getTheme())).getBitmap();
        return map.copy(map.getConfig(), true);
    }
}
