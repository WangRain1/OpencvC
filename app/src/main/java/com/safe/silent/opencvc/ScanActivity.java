/*
 * Copyright (c) 2019. Parrot Faurecia Automotive S.A.S. All rights reserved.
 */

package com.safe.silent.opencvc;

import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.hardware.Camera;
import android.os.Build;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.OrientationEventListener;
import android.view.SurfaceHolder;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ImageView;

import java.io.IOException;
import java.util.List;

public class ScanActivity extends AppCompatActivity {

    CardView mSurfaceView;
    Camera camera;

    ImageView pic;
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        requestWindowFeature(Window.FEATURE_NO_TITLE);//remove title bar  即隐藏标题栏
        getSupportActionBar().hide();// 隐藏ActionBar
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);//remove notification bar  即全屏
        setContentView(R.layout.cm);
        initView();
    }
    private void initView() {
        DisplayMetrics outMetrics = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(outMetrics);
//        4：3
        mSurfaceView = findViewById(R.id.surfaceview);
        SurfaceHolder surfaceHolder = mSurfaceView.getHolder();
        surfaceHolder.addCallback(new TakePictureView(this));

        pic = findViewById(R.id.pic);
        findViewById(R.id.take).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                camera.takePicture(null, null, null, new Camera.PictureCallback() {
                    @Override
                    public void onPictureTaken(byte[] data, Camera camera) {

                        int[] rp = mSurfaceView.getRectPosition();
                        Bitmap b = BitmapFactory.decodeByteArray(data,0,data.length);

                        Matrix matrix = new Matrix();
                        matrix.postRotate(90);

                        Bitmap rotBitmap = Bitmap.createBitmap(b, 0, 0, b.getWidth(), b.getHeight()
                                , matrix, false);

                        Bitmap sizeBitmap = Bitmap.createScaledBitmap(rotBitmap,
                                mSurfaceView.getWidth(), mSurfaceView.getHeight(), true);

                        //截取
                        Bitmap rectBitmap = Bitmap.createBitmap(sizeBitmap, rp[0], rp[1], rp[2], rp[3]);

                        BitmapCache.getBitmapCache().addBitmap(rectBitmap);

                        Intent i = new Intent();
                        i.putExtra("result", "save");
                        setResult(3, i);
                        finish();
//                        pic.setImageBitmap(rectBitmap);
                    }
                });
            }
        });
    }

    public class TakePictureView implements SurfaceHolder.Callback {

        Camera.CameraInfo mCameraBackInfo = null;
        Camera.CameraInfo mCameraFrontInfo = null;
        Camera.Parameters parameters;
        Context mContext;
        OrientationEventListener listener;
        int preOrientation =-1;

        public TakePictureView(Context context) {
            mContext = context;
        }

        @Override
        public void surfaceCreated(SurfaceHolder holder) {
            getCameraInfo();
            openCamera();
            camera.setDisplayOrientation(90);
            listener = new OrientationEventListener(mContext) {
                @Override
                public void onOrientationChanged(int orientation) {
                    int rotation = getWindowManager().getDefaultDisplay().getRotation();

                    if (orientation<45||(orientation>=315&&orientation<360)){
                        orientation = 0;
                    }else if (orientation<135&&orientation>=45){
                        orientation = 90;
                    }else if (orientation<225&&orientation>=135){
                        orientation = 90;
                    }else if (orientation<315&&orientation>=225){
                        orientation = 90;
                    }

                    int o = mCameraBackInfo.orientation;
                    if (!(orientation == preOrientation))
                    {
                        camera.setDisplayOrientation(90);
                        preOrientation = orientation;
                    }
                    Log.e("onPictureTaken--" + o, rotation + "---------orientation------" + orientation);
                }
            };
            listener.enable();
            try {
                parameters = camera.getParameters();
                List<Camera.Size> sizes = parameters.getSupportedPictureSizes();

                if (!Build.MODEL.equals("KORIDY H30")) {
                    parameters.setFocusMode(Camera.Parameters.FOCUS_MODE_CONTINUOUS_PICTURE);// 1连续对焦
                }else{
                    parameters.setFocusMode(Camera.Parameters.FOCUS_MODE_AUTO);
                }

//                for (Camera.Size s : sizes) {
//                    if (s.width/s.height == 4/3){
//                        Log.e("------------" + s.width, "---Size---" + s.height);
//                        parameters.setPreviewSize(s.width,s.height);
////                        break;
//                    }
//                }
                parameters.setPreviewSize(1920,1080);
                parameters.setPictureSize(1920,1080);
                camera.setParameters(parameters);
                camera.setPreviewDisplay(holder);
                camera.startPreview();
                camera.autoFocus(new Camera.AutoFocusCallback() {
                    @Override
                    public void onAutoFocus(boolean success, Camera camera) {
                        if (success) {
                            camera.cancelAutoFocus();// 只有加上了这一句，才会自动对焦
                            doAutoFocus();
                        }
                        Log.e("-----onAutoFocus-------" , "---success---" + success);
                    }
                });
                // 2如果要实现连续的自动对焦，这一句必须加上
//                camera.cancelAutoFocus();

            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        @Override
        public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

        }

        @Override
        public void surfaceDestroyed(SurfaceHolder holder) {
            if (camera != null) {
                camera.release();
                listener.disable();
                listener = null;
                camera = null;
            }
        }

        private void getCameraInfo() {
            int cameraCount = Camera.getNumberOfCameras();
            for (int i = 0; i < cameraCount; i++) {
                Camera.CameraInfo cameraInfo = new Camera.CameraInfo();
                Camera.getCameraInfo(i, cameraInfo);
                if (cameraInfo.facing == Camera.CameraInfo.CAMERA_FACING_BACK) {
                    mCameraBackInfo = cameraInfo;
                } else if (cameraInfo.facing == Camera.CameraInfo.CAMERA_FACING_FRONT) {
                    mCameraFrontInfo = cameraInfo;
                }
            }
        }

        private void openCamera() {
            if (camera != null) {
                return;
            }
            camera = Camera.open(mCameraBackInfo.facing);
//            if (mCameraFrontInfo != null) {
//                //打开前置摄像头
//                camera = Camera.open(mCameraFrontInfo.facing);
//            } else {
//                //打开后置
//                camera = Camera.open(mCameraBackInfo.facing);
//            }
        }

        // handle button auto focus
        private void doAutoFocus() {
            parameters = camera.getParameters();
            parameters.setFocusMode(Camera.Parameters.FOCUS_MODE_AUTO);
            camera.setParameters(parameters);
            camera.autoFocus(new Camera.AutoFocusCallback() {
                @Override
                public void onAutoFocus(boolean success, Camera camera) {
                    if (success) {
                        camera.cancelAutoFocus();// 只有加上了这一句，才会自动对焦。
                        if (!Build.MODEL.equals("KORIDY H30")) {
                            Log.e("-----onAutoFocus-------" , "---doAutoFocus-1111--");
                            parameters = camera.getParameters();
                            parameters.setFocusMode(Camera.Parameters.FOCUS_MODE_CONTINUOUS_PICTURE);// 1连续对焦
                            camera.setParameters(parameters);
                        }else{
                            parameters = camera.getParameters();
                            parameters.setFocusMode(Camera.Parameters.FOCUS_MODE_AUTO);
                            camera.setParameters(parameters);
                        }
                    }
                }
            });
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (null != camera){
            camera.release();
            camera = null;
        }
    }
}