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
    TextView tv;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        ImageView d = findViewById(R.id.dst);

        // Example of a call to a native method
        tv = (TextView) findViewById(R.id.sample_text);
        tv.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this,ScanActivity.class);
                startActivityForResult(intent,1);
            }
        });
//        tv.setText(stringFromJNI());
//            new Thread(){
//            @Override
//            public void run() {
//                super.run();
//                //银行卡识别
//                Bitmap dst = findNumber(bitmap);
//            }
//        }.start();

//        imageView.setImageBitmap(dst);

        final Bitmap bitmap = BitmapFactory.decodeResource(getResources(),R.drawable.card);
        new Thread(){
            @Override
            public void run() {
                super.run();

                //数据训练
//                Bitmap zero1 = BitmapFactory.decodeFile("/storage/emulated/0/zero1.png");
//                Bitmap zero2 = BitmapFactory.decodeFile("/storage/emulated/0/zero2.png");
//                Bitmap one1 = BitmapFactory.decodeFile("/storage/emulated/0/one1.png");
//                Bitmap one2 = BitmapFactory.decodeFile("/storage/emulated/0/one2.png");
////                Bitmap one3 = BitmapFactory.decodeFile("/storage/emulated/0/one3");
//                Bitmap two1 = BitmapFactory.decodeFile("/storage/emulated/0/two1.png");
//                Bitmap two2 = BitmapFactory.decodeFile("/storage/emulated/0/two2.png");
//                Bitmap three1 = BitmapFactory.decodeFile("/storage/emulated/0/three1.png");
//                Bitmap four1 = BitmapFactory.decodeFile("/storage/emulated/0/four1.png");
//                Bitmap four2 = BitmapFactory.decodeFile("/storage/emulated/0/four2.png");
//                Bitmap five1 = BitmapFactory.decodeFile("/storage/emulated/0/five1.png");
//                Bitmap six1 = BitmapFactory.decodeFile("/storage/emulated/0/six1.png");
//                Bitmap six2 = BitmapFactory.decodeFile("/storage/emulated/0/six2.png");
//                Bitmap seven1 = BitmapFactory.decodeFile("/storage/emulated/0/seven1.png");
//                Bitmap eight1 = BitmapFactory.decodeFile("/storage/emulated/0/eight1.png");
//                Bitmap eight2 = BitmapFactory.decodeFile("/storage/emulated/0/eight2.png");
//                Bitmap nine1 = BitmapFactory.decodeFile("/storage/emulated/0/nine1.png");
//
//                Bitmap zero[] = {zero1,zero2};
//                Bitmap one[] = {one1,one2};
//                Bitmap two[] = {two1,two2};
//                Bitmap three[] = {three1};
//                Bitmap four[] = {four1,four2};
//                Bitmap five[] = {five1};
//                Bitmap six[] = {six1,six2};
//                Bitmap seven[] = {seven1};
//                Bitmap eight[] = {eight1,eight2};
//                Bitmap nine[] = {nine1};
//
//                //数据训练
//                train(zero,one,two,three,four,five,six,seven,eight,nine);

                //银行卡识别
                 final String re = findNumber(bitmap);
                 Log.e("findNumber","number - " + re);

                MainActivity.this.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        tv.setText(re);
                    }
                });

                //test
//                test(bitmap);
            }
        }.start();

//        mat_At();
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native String findNumber(Bitmap bitmap);

    /**
     * 对数据进行训练
     */
    public native void train(Bitmap[] bitmap_0,Bitmap[] bitmap_1
            ,Bitmap[] bitmap_2,Bitmap[] bitmap_3,Bitmap[] bitmap_4,Bitmap[] bitmap_5,Bitmap[] bitmap_6
            ,Bitmap[] bitmap_7,Bitmap[] bitmap_8,Bitmap[] bitmap_9);

    /**
     * 识别数据
     */
    public native String checkMsg(Bitmap bitmap);


    public native void test(Bitmap bitmap);

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        //此处可以根据两个Code进行判断，本页面和结果页面跳过来的值
        if (requestCode == 1 && resultCode == 3) {
            String result = data.getStringExtra("result");
            Log.e("findNumber","number - " + result);
            String num = findNumber(BitmapCache.getBitmapCache().getBitamp());
            tv.setText(num);
        }
    }
}
