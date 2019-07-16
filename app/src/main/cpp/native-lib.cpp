#include <jni.h>
#include <string>
// 路径配置
#include <opencv2/opencv.hpp>
#include <android/bitmap.h>
#include "BitmapUtils.h"
#include <android/log.h>



using namespace cv;

extern "C" JNIEXPORT jstring JNICALL
Java_com_safe_silent_opencvc_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_safe_silent_opencvc_MainActivity_findNumber(JNIEnv *env, jobject instance, jobject bitmap) {

    BitmapUtils *bitmapUtils = new BitmapUtils();
    Mat mat;
    bitmapUtils->Bitmap2mat(env,bitmap,&mat);

    Rect rect;
    bitmapUtils->findCardArea(mat,rect);

    Mat cardRect(mat,rect);

//    imwrite("/storage/emulated/0/card_number.jpg",cardRect);

    bitmapUtils->Mat2Bitmap(env,bitmap,cardRect);

    return bitmap;
}