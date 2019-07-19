/*
 * Copyright (c) 2019. Parrot Faurecia Automotive S.A.S. All rights reserved.
 */

package com.safe.silent.opencvc;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.widget.ImageView;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        // Example of a call to a native method
        final TextView tv = (TextView) findViewById(R.id.sample_text);
//        tv.setText(stringFromJNI());
        final Bitmap bitmap = BitmapFactory.decodeResource(getResources(),R.drawable.card);

        ImageView imageView = findViewById(R.id.dst);
            new Thread(){
            @Override
            public void run() {
                super.run();
                //银行卡识别
                Bitmap dst = findNumber(bitmap);
            }
        }.start();

//        imageView.setImageBitmap(dst);

        //数据训练
        final Bitmap a1 = BitmapFactory.decodeResource(getResources(),R.drawable.a1);
        Bitmap a2 = BitmapFactory.decodeResource(getResources(),R.drawable.a2);
        Bitmap a3 = BitmapFactory.decodeResource(getResources(),R.drawable.a3);
        Bitmap a4 = BitmapFactory.decodeResource(getResources(),R.drawable.a4);
        final Bitmap b1 = BitmapFactory.decodeResource(getResources(),R.drawable.b1);
        final Bitmap b2 = BitmapFactory.decodeResource(getResources(),R.drawable.b2);
        Bitmap b3 = BitmapFactory.decodeResource(getResources(),R.drawable.b3);
        final Bitmap i = BitmapFactory.decodeResource(getResources(),R.drawable.liu);
        final Bitmap arr[] = {a1,a2,a3,a4};
        final Bitmap brr[] = {b1,b2,b3};
//        new Thread(){
//            @Override
//            public void run() {
//                super.run();
//                //数据训练
////                train(arr,brr);
//
//                //数据识别
//                final String re = checkMsg(a1);
//                MainActivity.this.runOnUiThread(new Runnable() {
//                    @Override
//                    public void run() {
//                        tv.setText(re);
//                    }
//                });
//            }
//        }.start();

//        mat_At();
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native Bitmap findNumber(Bitmap bitmap);

    /**
     * 对数据进行训练
     */
    public native void train(Bitmap[] bitmap_a,Bitmap[] bitmap_b);

    /**
     * 识别数据
     */
    public native String checkMsg(Bitmap bitmap);


    public native void mat_At();
}
