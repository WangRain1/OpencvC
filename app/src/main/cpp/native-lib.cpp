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

void pushMat(JNIEnv *env,BitmapUtils& bitmapUtils,jobjectArray &bitmap,vector<Mat>& vMat);

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
    bitmapUtils->Bitmap2mat(env,bitmap,&mat);

    Rect rect;
    bitmapUtils->findCardArea(mat,rect);

    Mat cardRect(mat,rect);
    string res;
    bitmapUtils->findNumber(cardRect,res);

    return env->NewStringUTF(res.c_str());;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_safe_silent_opencvc_MainActivity_checkMsg(JNIEnv *env, jobject instance, jobject bitmap) {

    BitmapUtils* bitmapUtils = new BitmapUtils();
    Mat tag;
    bitmapUtils->Bitmap2mat(env,bitmap,&tag);

    TrimTest* trim = new TrimTest();
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
                                                jobjectArray bitmap_9) {

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


    BitmapUtils* bitmapUtils = new BitmapUtils();

    for (int i = 0; i < bim.size(); ++i) {

        pushMat(env,*bitmapUtils,bim[i],vm[i]);
    }

    TrimTest* trim = new TrimTest();

    trim->startTrim(vm);

}

void pushMat(JNIEnv *env,BitmapUtils& bitmapUtils,jobjectArray &bitmap,vector<Mat>& vMat){

    jsize size = env->GetArrayLength(bitmap);

    for (int i = 0; i < size; ++i) {
        jobject b = env->GetObjectArrayElement(bitmap, i);

        Mat mat;
        bitmapUtils.Bitmap2mat(env,b,&mat);

        Mat gray;
        cvtColor(mat,gray,COLOR_BGRA2GRAY);

        vMat.push_back(gray);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_safe_silent_opencvc_MainActivity_test(JNIEnv *env, jobject instance, jobject bitmap) {

    BitmapUtils* bitmapUtils = new BitmapUtils();
    Mat mat;
    bitmapUtils->Bitmap2mat(env,bitmap,&mat);

    //剪切矩形
    Rect cR;
    cR.x = mat.cols/20;
    cR.y = mat.rows/2;
    cR.width = mat.cols*18/20;
    cR.height = mat.rows/6;

//    cR.x = srcImg.cols/20;
//    cR.y = srcImg.rows/3;
//    cR.width = srcImg.cols*18/20;
//    cR.height = srcImg.rows/4;

    Mat rectMat(mat,cR);

    // 首先降噪 噪点可以选大一点
    Mat blur;
    GaussianBlur(rectMat, blur, Size(9, 9), BORDER_DEFAULT, BORDER_DEFAULT);
//    medianBlur(srcImg, srcImg, 5);  //中值滤波
    Mat g;
    cvtColor(blur, g, COLOR_BGRA2GRAY); //转为灰度图
    imwrite("/storage/emulated/0/g.jpg",g);
    //获取自定义核
    //第一个参数MORPH_RECT表示矩形的卷积核，当然还可以选择椭圆形的、交叉型的
    Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
    Mat erodeOut;
    //腐蚀操作
    erode(g, erodeOut, element);
    imwrite("/storage/emulated/0/erodeOut.jpg", erodeOut);
    //二值化 THRESH_OTSU自动阀值
    Mat th;
    // 自定义阀值
    // adaptiveThreshold(g, th, 150, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 3, 5);
    threshold(erodeOut, th, 39, 255, THRESH_OTSU);
    imwrite("/storage/emulated/0/th.jpg", th);
    //膨胀
    Mat dilate_element = getStructuringElement(MORPH_RECT, Size(8, 8));
    Mat dilate_erodeOut;
    dilate(th, dilate_erodeOut, dilate_element);

    vector<vector<Point> > contours;
    vector<Vec4i> hierarcy;
    findContours(dilate_erodeOut, contours, hierarcy, RETR_TREE, CHAIN_APPROX_NONE); //查找所有轮廓
    vector<Rect> boundRect(contours.size()); //定义外接矩形集合
    int x0 = 0, y0 = 0, w0 = 0, h0 = 0;
//    int dilate_area = dilate_erodeOut.cols * dilate_erodeOut.rows;

    for (int i = 0; i < contours.size(); i++) {
        boundRect[i] = boundingRect(contours[i]); //查找每个轮廓的外接矩形
        x0 = boundRect[i].x;
        y0 = boundRect[i].y;
        w0 = boundRect[i].width;
        h0 = boundRect[i].height;
        //绘制第i个外接矩形
//        rectangle(dstImg, Point(x0, y0), Point(x0 + w0, y0 + h0), Scalar(0, 255, 0), 2, 8);
//
//        if (y0<dilate_erodeOut.rows*3/4&&y0>dilate_erodeOut.rows/4){
////            drawContours(mat,contours,i,Scalar(0,0,255),1);
//            rectangle(dilate_erodeOut, Point(x0, y0), Point(x0 + w0, y0 + h0), Scalar(0, 0, 255), 2, 8);
//            __android_log_print(ANDROID_LOG_ERROR,"找到了-","asdfadsf");
//
//        }

    }

    imwrite("/storage/emulated/0/dilate_erodeOut.jpg",dilate_erodeOut);

    __android_log_print(ANDROID_LOG_ERROR,"ZEROSc-","asdfadsf");
}