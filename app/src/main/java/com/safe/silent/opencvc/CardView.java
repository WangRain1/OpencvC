/*
 * Copyright (c) 2019. Parrot Faurecia Automotive S.A.S. All rights reserved.
 */

package com.safe.silent.opencvc;

import android.app.Activity;
import android.content.Context;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.RectF;
import android.util.AttributeSet;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class CardView extends SurfaceView {

    Context context;
    public SurfaceHolder surfaceHolder;

    public CardView(Context context) {
        this(context,null);
    }

    public CardView(Context context, AttributeSet attrs) {
        this(context, attrs,0);
    }

    public CardView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        this.context = context;
        this.surfaceHolder = this.getHolder();

        init();
    }
    Paint p;
    int widthPixels;
    RectF drawRect;


    int r_left;
    int r_top;
    int r_width;
    int r_height;
    int [] rectPosition = new int[4];
    private void init() {
        p = new Paint();
        p.setStyle(Paint.Style.STROKE);
        DisplayMetrics outMetrics = new DisplayMetrics();
        ((Activity)context).getWindowManager().getDefaultDisplay().getMetrics(outMetrics);
        widthPixels = outMetrics.widthPixels;
        int heightPixels = outMetrics.heightPixels;
        p.setColor(Color.RED);

        r_left = widthPixels/10;
        r_width = widthPixels - r_left*2;
        r_height = r_width*3/4;
        r_top = heightPixels/2-r_height/2;

        drawRect = new RectF(r_left,r_top,r_width + r_left,r_height + r_top);
        rectPosition[0]=r_left;
        rectPosition[1]=r_top;
        rectPosition[2]=r_width;
        rectPosition[3]=r_height;
    }

    public int[] getRectPosition(){

        return rectPosition;
    }

    @Override
    public void draw(Canvas canvas) {
        super.draw(canvas);
        canvas.drawRect(drawRect,p);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

    }
}
