//
// Created by ts on 19-7-15.
//

#ifndef OPENCVC_BITMAPUTILS_H
#define OPENCVC_BITMAPUTILS_H
#include <android/bitmap.h>
#include <opencv2/opencv.hpp>
#include <jni.h>

using namespace cv;

class BitmapUtils {
public:

    BitmapUtils();

    int Bitmap2mat(JNIEnv *env,
                           jobject bitmap,Mat *mat);
    int Mat2Bitmap(JNIEnv *env,
                           jobject bitmap,Mat& mat);

    void findCardArea(const Mat& mat,Rect& rect);
    ~BitmapUtils();
};

#endif //OPENCVC_BITMAPUTILS_H
