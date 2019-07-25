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
#include "ScanNumber.h"


using namespace std;

using namespace cv;

void pushMat(JNIEnv *env, BitmapUtils &bitmapUtils, jobjectArray &bitmap, vector<Mat> &vMat);

extern "C" JNIEXPORT jstring JNICALL
Java_com_safe_silent_opencvc_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_safe_silent_opencvc_MainActivity_findNumber(JNIEnv *env, jobject instance, jobject bitmap) {

    BitmapUtils *bitmapUtils = new BitmapUtils();
    Mat mat;
    bitmapUtils->Bitmap2mat(env, bitmap, &mat);

    Rect rect;
    bitmapUtils->findCardArea(mat, rect);

    Mat cardRect(mat, rect);
    string res;
    bitmapUtils->findNumber(cardRect, res);

    return env->NewStringUTF(res.c_str());;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_safe_silent_opencvc_MainActivity_checkMsg(JNIEnv *env, jobject instance, jobject bitmap) {

    BitmapUtils *bitmapUtils = new BitmapUtils();
    Mat tag;
    bitmapUtils->Bitmap2mat(env, bitmap, &tag);

    TrimTest *trim = new TrimTest();
    float result = trim->checkMsg(tag);

    ostringstream buffer;
    buffer << result;
    string res = buffer.str();

    return env->NewStringUTF(res.c_str());
}

extern "C"
JNIEXPORT void JNICALL
Java_com_safe_silent_opencvc_MainActivity_train(JNIEnv *env, jobject instance,
                                                jobjectArray bitmap_0,
                                                jobjectArray bitmap_1, jobjectArray bitmap_2,
                                                jobjectArray bitmap_3, jobjectArray bitmap_4,
                                                jobjectArray bitmap_5, jobjectArray bitmap_6,
                                                jobjectArray bitmap_7, jobjectArray bitmap_8,
                                                jobjectArray bitmap_9, jobjectArray bitmap_10) {

    vector<Mat> vMat_0;
    vector<Mat> vMat_1;
    vector<Mat> vMat_2;
    vector<Mat> vMat_3;
    vector<Mat> vMat_4;
    vector<Mat> vMat_5;
    vector<Mat> vMat_6;
    vector<Mat> vMat_7;
    vector<Mat> vMat_8;
    vector<Mat> vMat_9;
    vector<Mat> vMat_10;

    vector<vector<Mat>> vm;

    vm.push_back(vMat_0);
    vm.push_back(vMat_1);
    vm.push_back(vMat_2);
    vm.push_back(vMat_3);
    vm.push_back(vMat_4);
    vm.push_back(vMat_5);
    vm.push_back(vMat_6);
    vm.push_back(vMat_7);
    vm.push_back(vMat_8);
    vm.push_back(vMat_9);
    vm.push_back(vMat_10);

    vector<jobjectArray> bim;
    bim.push_back(bitmap_0);
    bim.push_back(bitmap_1);
    bim.push_back(bitmap_2);
    bim.push_back(bitmap_3);
    bim.push_back(bitmap_4);
    bim.push_back(bitmap_5);
    bim.push_back(bitmap_6);
    bim.push_back(bitmap_7);
    bim.push_back(bitmap_8);
    bim.push_back(bitmap_9);
    bim.push_back(bitmap_10);

    BitmapUtils *bitmapUtils = new BitmapUtils();

    for (int i = 0; i < bim.size(); ++i) {

        pushMat(env, *bitmapUtils, bim[i], vm[i]);
    }

    TrimTest *trim = new TrimTest();

    trim->startTrim(vm);

}

void pushMat(JNIEnv *env, BitmapUtils &bitmapUtils, jobjectArray &bitmap, vector<Mat> &vMat) {

    jsize size = env->GetArrayLength(bitmap);

    for (int i = 0; i < size; ++i) {
        jobject b = env->GetObjectArrayElement(bitmap, i);

        Mat mat;
        bitmapUtils.Bitmap2mat(env, b, &mat);

        Mat gray;
        cvtColor(mat, gray, COLOR_BGRA2GRAY);

        vMat.push_back(gray);
    }
}


extern "C"
JNIEXPORT jstring JNICALL
Java_com_safe_silent_opencvc_MainActivity_scanNumber(JNIEnv *env, jobject instance, jobject bitmap) {

    BitmapUtils *bitmapUtils = new BitmapUtils();
    Mat mat;
    bitmapUtils->Bitmap2mat(env, bitmap, &mat);

    Rect rect;
    bitmapUtils->findCardArea(mat, rect);

    Mat cardRect(mat, rect);

    ScanNumber *number = new ScanNumber();

    string res;
    number->cutNumberArea(cardRect, res);

    return env->NewStringUTF(res.c_str());;
}