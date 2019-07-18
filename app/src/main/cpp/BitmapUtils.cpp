//
// Created by ts on 19-7-15.
//


#include <android/bitmap.h>
#include <android/log.h>
#include "BitmapUtils.h"
#include <vector>

using namespace std;

BitmapUtils::BitmapUtils() {

}

int BitmapUtils::Bitmap2mat(JNIEnv *env, jobject bitmap, Mat *mat) {
    void *addrPtr;
    AndroidBitmap_lockPixels(env, bitmap, &addrPtr);

    AndroidBitmapInfo bitmapInfo;

    AndroidBitmap_getInfo(env,bitmap,&bitmapInfo);

    Mat createM(bitmapInfo.height,bitmapInfo.width,CV_8UC4);

    if (bitmapInfo.format == ANDROID_BITMAP_FORMAT_RGBA_8888){
        Mat tep(bitmapInfo.height,bitmapInfo.width,CV_8UC4,addrPtr);
        tep.copyTo(createM);
    } else if(bitmapInfo.format == ANDROID_BITMAP_FORMAT_RGB_565){
        Mat tep(bitmapInfo.height,bitmapInfo.width,CV_8UC2,addrPtr);
        cvtColor(tep,createM,COLOR_BGR5652RGBA);
    }
    createM.copyTo(*mat);
    createM.release();
    AndroidBitmap_unlockPixels(env,bitmap);

    return 0;
}

int BitmapUtils::Mat2Bitmap(JNIEnv *env, jobject bitmap, Mat& mat) {
/**
 * RGBA_8888 -> CV_8UC4 四颜色通道
 * RGB_565   -> CV_8UC2 三颜色通道
 * GRAY      ->  CV_8UC1 单通道（黑白）
 */
    AndroidBitmapInfo info;
    AndroidBitmap_getInfo(env, bitmap, &info);

    void * pixels;
    //锁定 bitmap
    AndroidBitmap_lockPixels(env, bitmap, &pixels);
    __android_log_print(ANDROID_LOG_ERROR,"Mat2Bitmap-RECT-","%d",mat.cols);
    if(info.format == ANDROID_BITMAP_FORMAT_RGBA_8888){ //bitmap
        Mat tmp(info.height, info.width, CV_8UC4, pixels);
        if(mat.type() == CV_8UC1){
            cvtColor(mat, tmp,COLOR_GRAY2RGBA);
        }else if(mat.type() == CV_8UC2){
            cvtColor(mat, tmp,COLOR_BGR5652BGRA);
        }else if(mat.type() == CV_8UC4){
            mat.copyTo(tmp);
        }
    }else if(info.format == ANDROID_BITMAP_FORMAT_RGB_565){
        Mat tmp(info.height, info.width, CV_8UC2, pixels);
        if(mat.type() == CV_8UC1){
            cvtColor(mat, tmp,COLOR_GRAY2BGR565);
        }else if(mat.type() == CV_8UC2){
            mat.copyTo(tmp);
        }else if(mat.type() == CV_8UC4){
            cvtColor(mat, tmp,COLOR_RGBA2BGR565);
        }
    }
    return 0;
}

void BitmapUtils::findCardArea(const Mat& mat,Rect& card_rect) {
    // 首先降噪
    __android_log_print(ANDROID_LOG_ERROR,"111-RECT-","%d",mat.cols);
    Mat blur;
    GaussianBlur(mat, blur, Size(5, 5), BORDER_DEFAULT, BORDER_DEFAULT);

    imwrite("/storage/emulated/0/blur.jpg",blur);

    // 梯度增强 , x 轴和 y 轴
    Mat grad_x, grad_y;
    Scharr(blur, grad_x, CV_32F, 1, 0);
    Scharr(blur, grad_y, CV_32F, 0, 1);
    Mat grad_abs_x, grad_abs_y;
    convertScaleAbs(grad_x, grad_abs_x);
    convertScaleAbs(grad_y, grad_abs_y);
    Mat grad;
    addWeighted(grad_abs_x, 0.5, grad_abs_y, 0.5, 0, grad);

    imwrite("/storage/emulated/0/grad.jpg",grad);

    // 二值化，进行轮廓查找
    Mat gray;
    cvtColor(grad, gray, COLOR_BGRA2GRAY);
    Mat binary;
    threshold(gray, binary, 40, 255, THRESH_BINARY);

    imwrite("/storage/emulated/0/binary.jpg",binary);

    // 轮廓查找
    vector<vector<Point> > contours;
    findContours(binary, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    for (int i = 0; i < contours.size(); ++i) {
        Rect rect = boundingRect(contours[i]);
        // 是不是符合规则
        if (rect.width > mat.cols / 2 && rect.width != mat.cols && rect.height > mat.rows / 2) {
            card_rect = rect;
            __android_log_print(ANDROID_LOG_ERROR,"22-RECT-","%d",card_rect.width);
            break;
        }
    }

    // release source
    blur.release();
    grad_x.release();
    grad_y.release();
    grad_abs_x.release();
    grad_abs_y.release();
    grad.release();
    gray.release();
    binary.release();

}

BitmapUtils::~BitmapUtils() {

}