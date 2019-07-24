/*
 * Copyright (c) 2019. Parrot Faurecia Automotive S.A.S. All rights reserved.
 */

package com.safe.silent.opencvc;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    TextView number;
    TextView sacn_btn;
    ImageView src;
    ImageView dst;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        final Bitmap bitmap = BitmapFactory.decodeResource(getResources(),R.drawable.card);
        src = findViewById(R.id.src);
        dst = findViewById(R.id.dst);
        number = (TextView) findViewById(R.id.number);
        sacn_btn = (TextView) findViewById(R.id.sacn_btn);
        sacn_btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this,ScanActivity.class);
                startActivityForResult(intent,1);
            }
        });

        scanCard(bitmap);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == 1 && resultCode == 3) {
            src.setImageBitmap(BitmapCache.getBitmapCache().getBitamp());
            scanCard(BitmapCache.getBitmapCache().getBitamp());
        }
    }

    public void scanCard(final Bitmap bitmap){
        new Thread(){
            @Override
            public void run() {
                super.run();
                final String num = scanNumber(bitmap);
                MainActivity.this.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        number.setText(num);
                    }
                });
                Log.e("findNumber","number - " + num);
            }
        }.start();
    }

    public void trainModle(){
        new Thread(){
            @Override
            public void run() {
                super.run();

                //数据训练 放在了 drawable下面，有新的数据需要训练时拿出来一起训练
                Bitmap zero1 = BitmapFactory.decodeFile("/storage/emulated/0/zero1.png");
                Bitmap zero2 = BitmapFactory.decodeFile("/storage/emulated/0/zero2.png");
                Bitmap zero3 = BitmapFactory.decodeFile("/storage/emulated/0/zero3.png");
                Bitmap zero4 = BitmapFactory.decodeFile("/storage/emulated/0/zero4.png");
                Bitmap one1 = BitmapFactory.decodeFile("/storage/emulated/0/one1.png");
                Bitmap one2 = BitmapFactory.decodeFile("/storage/emulated/0/one2.png");
                Bitmap one3 = BitmapFactory.decodeFile("/storage/emulated/0/one3.png");
                Bitmap one4 = BitmapFactory.decodeFile("/storage/emulated/0/one4.png");
                Bitmap one5 = BitmapFactory.decodeFile("/storage/emulated/0/one5.png");
                Bitmap one6 = BitmapFactory.decodeFile("/storage/emulated/0/one6.png");
                Bitmap two1 = BitmapFactory.decodeFile("/storage/emulated/0/two1.png");
                Bitmap two2 = BitmapFactory.decodeFile("/storage/emulated/0/two2.png");
                Bitmap two3 = BitmapFactory.decodeFile("/storage/emulated/0/two3.png");
                Bitmap two4 = BitmapFactory.decodeFile("/storage/emulated/0/two4.png");
                Bitmap three1 = BitmapFactory.decodeFile("/storage/emulated/0/three1.png");
                Bitmap three2 = BitmapFactory.decodeFile("/storage/emulated/0/three2.png");
                Bitmap three3 = BitmapFactory.decodeFile("/storage/emulated/0/three3.png");
                Bitmap four1 = BitmapFactory.decodeFile("/storage/emulated/0/four1.png");
                Bitmap four2 = BitmapFactory.decodeFile("/storage/emulated/0/four2.png");
                Bitmap four3 = BitmapFactory.decodeFile("/storage/emulated/0/four3.png");
                Bitmap four4 = BitmapFactory.decodeFile("/storage/emulated/0/four4.png");
                Bitmap five1 = BitmapFactory.decodeFile("/storage/emulated/0/five1.png");
                Bitmap six1 = BitmapFactory.decodeFile("/storage/emulated/0/six1.png");
                Bitmap six2 = BitmapFactory.decodeFile("/storage/emulated/0/six2.png");
                Bitmap six3 = BitmapFactory.decodeFile("/storage/emulated/0/six3.png");
                Bitmap seven1 = BitmapFactory.decodeFile("/storage/emulated/0/seven1.png");
                Bitmap seven2 = BitmapFactory.decodeFile("/storage/emulated/0/seven2.png");
                Bitmap seven3 = BitmapFactory.decodeFile("/storage/emulated/0/seven3.png");
                Bitmap seven4 = BitmapFactory.decodeFile("/storage/emulated/0/seven4.png");
                Bitmap eight1 = BitmapFactory.decodeFile("/storage/emulated/0/eight1.png");
                Bitmap eight2 = BitmapFactory.decodeFile("/storage/emulated/0/eight2.png");
                Bitmap eight3 = BitmapFactory.decodeFile("/storage/emulated/0/eight3.png");
                Bitmap eight4 = BitmapFactory.decodeFile("/storage/emulated/0/eight4.png");
                Bitmap nine1 = BitmapFactory.decodeFile("/storage/emulated/0/nine1.png");
                Bitmap zero[] = {zero1,zero2,zero3,zero4};
                Bitmap one[] = {one1,one2,one3,one4,one5,one6};
                Bitmap two[] = {two1,two2,two3,two4};
                Bitmap three[] = {three1,three2,three3};
                Bitmap four[] = {four1,four2,four3,four4};
                Bitmap five[] = {five1};
                Bitmap six[] = {six1,six2,six3};
                Bitmap seven[] = {seven1,seven2,seven3,seven4};
                Bitmap eight[] = {eight1,eight2,eight3,eight4};
                Bitmap nine[] = {nine1};
                //数据训练
                train(zero,one,two,three,four,five,six,seven,eight,nine);
            }
        }.start();
    }

    /**
     * 扫描识别
     */
    public native String scanNumber(Bitmap bitmap);

    /**
     * 对数据进行训练
     */
    public native void train(Bitmap[] bitmap_0,Bitmap[] bitmap_1
            ,Bitmap[] bitmap_2,Bitmap[] bitmap_3,Bitmap[] bitmap_4,Bitmap[] bitmap_5,Bitmap[] bitmap_6
            ,Bitmap[] bitmap_7,Bitmap[] bitmap_8,Bitmap[] bitmap_9);

    // 废弃方法
    public native String stringFromJNI();
    public native String findNumber(Bitmap bitmap);
    public native String checkMsg(Bitmap bitmap);
}
