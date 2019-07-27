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

        /**
         *
         * OpenCv 4
         * |_samples (这个不做过多介绍)
         * |
         * |_sdk
         *      |_etc   训练好的分类器
         *      |_java  java开发需要的库
         *      |_native
         *              |_jni
         *                  |_ include 我们要用的c/c++的.h头文件 (重)
         *              |_libs
         *                  |_ armeabi-v7a 手机CPU(重)
         *                  |_ x86 (模拟器cpu)
         *              |_...
         *
         */


        /**
         * 总结：
         *  基于openCv NDK环境银行卡/身份证号识别
         *
         *  1.OpenCv银行卡卡号识别(1)-opencv ndk项目集成，
         *  银行卡卡号识别(3)-.so文件，CMakeLists.txt和jar包是干吗的有啥关系
         *  2.银行卡卡号识别(2)-bitmap 转 Mat，Mat的介绍
         *  4.银行卡卡号识别(4)-银行卡/身份证图像处理和卡号区域剪切
         *
         *  5.银行卡卡号识别(5)-基于openCv SVM支持向量积的机械学习模型训练(ml=machine learn)
         *  6.银行卡卡号识别(6)-银行卡/身份证卡号检测识别
         *
         *  7.银行卡卡号识别(7)-基于android相机的卡号区域提取
         *  坑：
         *  1.保存的时候图片尺寸不要太大(由于刚开始图片保存的是110：70的导致怎么训练都不成功)，最好格式是png.
         *  2.BitmapFactory.decodeResource();返回的图片尺寸不等于原尺寸，由于开始为了方便就直接在drawable
         *  下采集训练的图片，最后一直训练不成功，还好各种方式都尝试了，最终才发现是返回的bitmap的宽高变了。
         */
//        findNumber(bitmap);
//        trainModle();
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
                Bitmap zero5 = BitmapFactory.decodeFile("/storage/emulated/0/zero5.png");

                Bitmap one1 = BitmapFactory.decodeFile("/storage/emulated/0/one1.png");
                Bitmap one2 = BitmapFactory.decodeFile("/storage/emulated/0/one2.png");
                Bitmap one3 = BitmapFactory.decodeFile("/storage/emulated/0/one3.png");
                Bitmap one4 = BitmapFactory.decodeFile("/storage/emulated/0/one4.png");
                Bitmap one5 = BitmapFactory.decodeFile("/storage/emulated/0/one5.png");
                Bitmap one6 = BitmapFactory.decodeFile("/storage/emulated/0/one6.png");
                Bitmap one7 = BitmapFactory.decodeFile("/storage/emulated/0/one7.png");
                Bitmap one8 = BitmapFactory.decodeFile("/storage/emulated/0/one8.png");

                Bitmap two1 = BitmapFactory.decodeFile("/storage/emulated/0/two1.png");
                Bitmap two2 = BitmapFactory.decodeFile("/storage/emulated/0/two2.png");
                Bitmap two3 = BitmapFactory.decodeFile("/storage/emulated/0/two3.png");
                Bitmap two4 = BitmapFactory.decodeFile("/storage/emulated/0/two4.png");
                Bitmap two5 = BitmapFactory.decodeFile("/storage/emulated/0/two5.png");
                Bitmap two6 = BitmapFactory.decodeFile("/storage/emulated/0/two6.png");

                Bitmap three1 = BitmapFactory.decodeFile("/storage/emulated/0/three1.png");
                Bitmap three2 = BitmapFactory.decodeFile("/storage/emulated/0/three2.png");
                Bitmap three3 = BitmapFactory.decodeFile("/storage/emulated/0/three3.png");
                Bitmap three4 = BitmapFactory.decodeFile("/storage/emulated/0/three4.png");
                Bitmap three5 = BitmapFactory.decodeFile("/storage/emulated/0/three5.png");


                Bitmap four1 = BitmapFactory.decodeFile("/storage/emulated/0/four1.png");
                Bitmap four2 = BitmapFactory.decodeFile("/storage/emulated/0/four2.png");
                Bitmap four3 = BitmapFactory.decodeFile("/storage/emulated/0/four3.png");
                Bitmap four4 = BitmapFactory.decodeFile("/storage/emulated/0/four4.png");
                Bitmap four5 = BitmapFactory.decodeFile("/storage/emulated/0/four5.png");
                Bitmap four6 = BitmapFactory.decodeFile("/storage/emulated/0/four6.png");

                Bitmap five1 = BitmapFactory.decodeFile("/storage/emulated/0/five1.png");
                Bitmap five2 = BitmapFactory.decodeFile("/storage/emulated/0/five2.png");

                Bitmap six1 = BitmapFactory.decodeFile("/storage/emulated/0/six1.png");
                Bitmap six2 = BitmapFactory.decodeFile("/storage/emulated/0/six2.png");
                Bitmap six3 = BitmapFactory.decodeFile("/storage/emulated/0/six3.png");
                Bitmap six4 = BitmapFactory.decodeFile("/storage/emulated/0/six4.png");

                Bitmap seven1 = BitmapFactory.decodeFile("/storage/emulated/0/seven1.png");
                Bitmap seven2 = BitmapFactory.decodeFile("/storage/emulated/0/seven2.png");
                Bitmap seven3 = BitmapFactory.decodeFile("/storage/emulated/0/seven3.png");
                Bitmap seven4 = BitmapFactory.decodeFile("/storage/emulated/0/seven4.png");
                Bitmap seven5 = BitmapFactory.decodeFile("/storage/emulated/0/seven5.png");


                Bitmap eight1 = BitmapFactory.decodeFile("/storage/emulated/0/eight1.png");
                Bitmap eight2 = BitmapFactory.decodeFile("/storage/emulated/0/eight2.png");
                Bitmap eight3 = BitmapFactory.decodeFile("/storage/emulated/0/eight3.png");
                Bitmap eight4 = BitmapFactory.decodeFile("/storage/emulated/0/eight4.png");
                Bitmap eight5 = BitmapFactory.decodeFile("/storage/emulated/0/eight5.png");

                Bitmap nine1 = BitmapFactory.decodeFile("/storage/emulated/0/nine1.png");
                Bitmap nine2 = BitmapFactory.decodeFile("/storage/emulated/0/nine2.png");

                Bitmap x1 = BitmapFactory.decodeFile("/storage/emulated/0/x1.png");

                Bitmap zero[] = {zero1,zero2,zero3,zero4,zero5};
                Bitmap one[] = {one1,one2,one3,one4,one5,one6,one7,one8};
                Bitmap two[] = {two1,two2,two3,two4,two5,two6};
                Bitmap three[] = {three1,three2,three3,three4,three5};
                Bitmap four[] = {four1,four2,four3,four4,four5,four6};
                Bitmap five[] = {five1,five2};
                Bitmap six[] = {six1,six2,six3,six4};
                Bitmap seven[] = {seven1,seven2,seven3,seven4,seven5};
                Bitmap eight[] = {eight1,eight2,eight3,eight4,eight5};
                Bitmap nine[] = {nine1,nine2};
                Bitmap x[] = {x1};
                //数据训练
                train(zero,one,two,three,four,five,six,seven,eight,nine,x);
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
            ,Bitmap[] bitmap_7,Bitmap[] bitmap_8,Bitmap[] bitmap_9,Bitmap[] bitmap_10);

    // 废弃方法
    public native String stringFromJNI();
    public native String findNumber(Bitmap bitmap);
    public native String checkMsg(Bitmap bitmap);
}
