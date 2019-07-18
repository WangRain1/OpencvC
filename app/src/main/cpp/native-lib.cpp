#include <jni.h>
#include <string>
// 路径配置
#include <opencv2/opencv.hpp>
#include <android/bitmap.h>
#include "BitmapUtils.h"
#include <android/log.h>
#include "trim.h"
#include <vector>
#include <sstream>
#include <iostream>


using namespace std;

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

extern "C"
JNIEXPORT void JNICALL
Java_com_safe_silent_opencvc_MainActivity_train(JNIEnv *env, jobject instance, jobjectArray bitmap_a,
                                            jobjectArray bitmap_b) {

    vector<Mat> vMat;
    vector<Mat> vMat_b;
    BitmapUtils* bitmapUtils = new BitmapUtils();

    jsize size = env->GetArrayLength(bitmap_a);

    for (int i = 0; i < size; ++i) {
        jobject b = env->GetObjectArrayElement(bitmap_a, i);

        Mat mat;
        bitmapUtils->Bitmap2mat(env,b,&mat);

        Mat gray;
        cvtColor(mat,gray,COLOR_BGRA2GRAY);

        vMat.push_back(gray);
    }

    jsize sizeb = env->GetArrayLength(bitmap_b);

    for (int i = 0; i < sizeb; ++i) {
        jobject b = env->GetObjectArrayElement(bitmap_b, i);

        Mat mat;
        bitmapUtils->Bitmap2mat(env,b,&mat);

        Mat gray;
        cvtColor(mat,gray,COLOR_BGRA2GRAY);

        vMat_b.push_back(gray);
    }

    TrimTest* trim = new TrimTest();

    trim->startTrim(vMat,vMat_b);

}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_safe_silent_opencvc_MainActivity_checkMsg(JNIEnv *env, jobject instance, jobject bitmap) {

    BitmapUtils* bitmapUtils = new BitmapUtils();
    Mat tag;
    bitmapUtils->Bitmap2mat(env,bitmap,&tag);

    Mat gray;
    cvtColor(tag,gray,COLOR_BGRA2GRAY);

    TrimTest* trim = new TrimTest();

    float result = trim->checkMsg(env,gray);

    ostringstream buffer;
    buffer << result;
    string res = buffer.str();

    return env->NewStringUTF(res.c_str());
}

extern "C"
JNIEXPORT void JNICALL
Java_com_safe_silent_opencvc_MainActivity_mat_1At(JNIEnv *env, jobject instance) {

//    Mat remat = Mat::zeros(5, 1, CV_32SC1);    //样本标签矩阵

    unsigned char str[]="打";

    vector<char> vc;
    vc.push_back(str[0]);
    Mat classes;
    Mat(vc).copyTo(classes);

    imwrite("/storage/emulated/0/cl.jpg",classes);
    __android_log_print(ANDROID_LOG_ERROR,"ZEROSc-","%d",classes.cols);

    __android_log_print(ANDROID_LOG_ERROR,"ZEROSc-","%c",classes.at<char>(0,0));

}