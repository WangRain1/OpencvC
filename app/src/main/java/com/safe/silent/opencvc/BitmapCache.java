/*
 * Copyright (c) 2019. Parrot Faurecia Automotive S.A.S. All rights reserved.
 */

package com.safe.silent.opencvc;

import android.graphics.Bitmap;

import java.util.ArrayList;
import java.util.List;

public class BitmapCache {

    private static final BitmapCache BITMAP_CACHE = new BitmapCache();

    List<Bitmap> mBitmapList = new ArrayList<>(5);

    public static BitmapCache getBitmapCache() {
        return BITMAP_CACHE;
    }

    public void clearBitmap(){
        mBitmapList.clear();
    }

    public void addBitmap(Bitmap bitmap){
        mBitmapList.clear();
        mBitmapList.add(bitmap);
    }

    public Bitmap getBitamp(){

        return mBitmapList.get(0);
    }

}
