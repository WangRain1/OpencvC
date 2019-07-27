//
// Created by ts on 19-7-15.
//


#include <android/bitmap.h>
#include <android/log.h>
#include "BitmapUtils.h"
#include <vector>
#include <opencv2/ml/ml.hpp>
#include <iostream>

using namespace std;
using namespace ml;


BitmapUtils::BitmapUtils() {

}

int BitmapUtils::Bitmap2mat(JNIEnv *env, jobject bitmap, Mat *mat) {
    void *addrPtr;
    AndroidBitmap_lockPixels(env, bitmap, &addrPtr);

    AndroidBitmapInfo bitmapInfo;

    AndroidBitmap_getInfo(env, bitmap, &bitmapInfo);

    Mat createM(bitmapInfo.height, bitmapInfo.width, CV_8UC4);

    if (bitmapInfo.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
        Mat tep(bitmapInfo.height, bitmapInfo.width, CV_8UC4, addrPtr);
        tep.copyTo(createM);
    } else if (bitmapInfo.format == ANDROID_BITMAP_FORMAT_RGB_565) {
        Mat tep(bitmapInfo.height, bitmapInfo.width, CV_8UC2, addrPtr);
        cvtColor(tep, createM, COLOR_BGR5652RGBA);
    }
    createM.copyTo(*mat);
    createM.release();
    AndroidBitmap_unlockPixels(env, bitmap);

    return 0;
}

int BitmapUtils::Mat2Bitmap(JNIEnv *env, jobject bitmap, Mat &mat) {
/**
 * RGBA_8888 -> CV_8UC4 四颜色通道
 * RGB_565   -> CV_8UC2 三颜色通道
 * GRAY      ->  CV_8UC1 单通道（黑白）
 */
    AndroidBitmapInfo info;
    AndroidBitmap_getInfo(env, bitmap, &info);

    void *pixels;
    //锁定 bitmap
    AndroidBitmap_lockPixels(env, bitmap, &pixels);
    __android_log_print(ANDROID_LOG_ERROR, "Mat2Bitmap-RECT-", "%d", mat.cols);
    if (info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) { //bitmap
        Mat tmp(info.height, info.width, CV_8UC4, pixels);
        if (mat.type() == CV_8UC1) {
            cvtColor(mat, tmp, COLOR_GRAY2RGBA);
        } else if (mat.type() == CV_8UC2) {
            cvtColor(mat, tmp, COLOR_BGR5652BGRA);
        } else if (mat.type() == CV_8UC4) {
            mat.copyTo(tmp);
        }
    } else if (info.format == ANDROID_BITMAP_FORMAT_RGB_565) {
        Mat tmp(info.height, info.width, CV_8UC2, pixels);
        if (mat.type() == CV_8UC1) {
            cvtColor(mat, tmp, COLOR_GRAY2BGR565);
        } else if (mat.type() == CV_8UC2) {
            mat.copyTo(tmp);
        } else if (mat.type() == CV_8UC4) {
            cvtColor(mat, tmp, COLOR_RGBA2BGR565);
        }
    }
    return 0;
}

void BitmapUtils::findCardArea(const Mat &mat, Rect &card_rect) {
    // 首先降噪
    Mat blur;
    GaussianBlur(mat, blur, Size(5, 5), BORDER_DEFAULT, BORDER_DEFAULT);

    // 梯度增强 , x 轴和 y 轴
    Mat grad_x, grad_y;
    Scharr(blur, grad_x, CV_32F, 1, 0);
    Scharr(blur, grad_y, CV_32F, 0, 1);
    Mat grad_abs_x, grad_abs_y;
    convertScaleAbs(grad_x, grad_abs_x);
    convertScaleAbs(grad_y, grad_abs_y);
    Mat grad;
    addWeighted(grad_abs_x, 0.5, grad_abs_y, 0.5, 0, grad);

    // 二值化，进行轮廓查找
    Mat gray;
    cvtColor(grad, gray, COLOR_BGRA2GRAY);
    Mat binary;
    threshold(gray, binary, 40, 255, THRESH_BINARY);
    int x0 = 0, y0 = 0, w0 = 0, h0 = 0;
    // 轮廓查找
    vector<vector<Point> > contours;
    findContours(binary, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    for (int i = 0; i < contours.size(); ++i) {
        Rect rect = boundingRect(contours[i]);
//        drawContours(mat,contours,i,Scalar(255, 0, 0),1);
        x0 = rect.x;
        y0 = rect.y;
        w0 = rect.width;
        h0 = rect.height;
        //绘制第i个外接矩形
        rectangle(mat, Point(x0, y0), Point(x0 + w0, y0 + h0), Scalar(0, 255, 0), 2, 8);

        __android_log_print(ANDROID_LOG_ERROR, "recr-", "%d", rect.width);
        // 是不是符合规则
        if (rect.width > mat.cols / 2 && rect.width != mat.cols && rect.height > mat.rows / 2) {
            card_rect = rect;
            __android_log_print(ANDROID_LOG_ERROR, "rd_rect-", "%d", card_rect.width);
            break;
        }
        if (i == contours.size() - 1) {
            card_rect.x = 0;
            card_rect.y = 0;
            card_rect.width = binary.cols;
            card_rect.height = binary.rows;
        }
    }

    imwrite("/storage/emulated/0/mat.jpg", mat);
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

void BitmapUtils::findNumber(Mat &srcImg, string &num_str) {

//    adaptiveThreshold(gray, binary, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 3, 5);

    //让mat的宽高统一 == cols--2201,rows--1388
    __android_log_print(ANDROID_LOG_ERROR, "bianjuu-", "%d,%d", srcImg.cols, srcImg.rows);

    //缩放
    int srcRows = 1388;
    int srcCols = 2201;
    Mat srcScale(srcRows, srcCols, srcImg.type());
    resize(srcImg, srcScale, srcScale.size(), 0, 0, INTER_LINEAR);
    //剪切矩形
    Rect cR;
    cR.x = srcScale.cols / 20;
    cR.y = srcScale.rows / 2;
    cR.width = srcScale.cols * 18 / 20;
    cR.height = srcScale.rows / 6;

//    cR.x = srcImg.cols/20;
//    cR.y = srcImg.rows/3;
//    cR.width = srcImg.cols*18/20;
//    cR.height = srcImg.rows/4;

    Mat rectMat(srcScale, cR);

//    imwrite("/storage/emulated/0/rectMat2.jpg", rectMat);

    Mat dstImg = rectMat.clone();  //原图备份

    // 首先降噪 噪点可以选大一点
    Mat blur;
    GaussianBlur(rectMat, blur, Size(9, 9), BORDER_DEFAULT, BORDER_DEFAULT);
//    medianBlur(srcImg, srcImg, 5);  //中值滤波
    Mat g;
    cvtColor(blur, g, COLOR_BGRA2GRAY); //转为灰度图

    //获取自定义核
    //第一个参数MORPH_RECT表示矩形的卷积核，当然还可以选择椭圆形的、交叉型的
    Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
    Mat erodeOut;
    //腐蚀操作
    erode(g, erodeOut, element);
//    imwrite("/storage/emulated/0/erodeOut.jpg", erodeOut);
    //二值化 THRESH_OTSU自动阀值
    Mat th;
    // 自定义阀值
    // adaptiveThreshold(g, th, 150, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 3, 5);
    threshold(erodeOut, th, 39, 255, THRESH_OTSU);
//    imwrite("/storage/emulated/0/th.jpg", th);
    //膨胀
    Mat dilate_element = getStructuringElement(MORPH_RECT, Size(8, 8));
    Mat dilate_erodeOut;
    dilate(th, dilate_erodeOut, dilate_element);
//    imwrite("/storage/emulated/0/dilate_erodeOut.jpg", dilate_erodeOut);
    //fan 二值化
//    Mat bit;
//    bitwise_not(dilate_erodeOut,bit);
//    imwrite("/storage/emulated/0/bit.jpg", bit);
    vector<vector<Point> > contours;
    vector<Vec4i> hierarcy;
    findContours(dilate_erodeOut, contours, hierarcy, RETR_TREE, CHAIN_APPROX_NONE); //查找所有轮廓
    vector<Rect> boundRect(contours.size()); //定义外接矩形集合
    int x0 = 0, y0 = 0, w0 = 0, h0 = 0;
    int dilate_area = dilate_erodeOut.cols * dilate_erodeOut.rows;

    for (int i = 0; i < contours.size(); i++) {
        boundRect[i] = boundingRect(contours[i]); //查找每个轮廓的外接矩形
        x0 = boundRect[i].x;
        y0 = boundRect[i].y;
        w0 = boundRect[i].width;
        h0 = boundRect[i].height;
        //绘制第i个外接矩形
        rectangle(dstImg, Point(x0, y0), Point(x0 + w0, y0 + h0), Scalar(0, 255, 0), 2, 8);

        int area = boundRect[i].area();
        if (h0 < dilate_erodeOut.rows * 2 / 5) {
            drawContours(dilate_erodeOut, contours, i, Scalar(0), 1);
            contours.erase(contours.begin() + i);
        } else if (area < dilate_area / 200) {
            drawContours(dilate_erodeOut, contours, i, Scalar(0), 1);
            contours.erase(contours.begin() + i);
        }
        // TODO : 根据高度和面积 处理腐蚀，伐值化，膨胀 不掉的干扰点。大面积的干扰点。
    }

    imwrite("/storage/emulated/0/dstImg.jpg", dstImg);
    vector<Rect> rr(contours.size());

    vector<Rect> split_mat;
    for (int i = 0; i < contours.size(); i++) {
        rr[i] = boundingRect(contours[i]);
        x0 = rr[i].x;
        y0 = rr[i].y;
        w0 = rr[i].width;
        h0 = rr[i].height;
//        rectangle(dstImg, Point(x0, y0), Point(x0 + w0, y0 + h0), Scalar(0, 255, 0), 2, 8);

        if (rr[i].area() < dilate_area / 200 || rr[i].width > dilate_erodeOut.cols / 3) {
            continue;
        }

        split_mat.push_back(rr[i]);
    }

    Rect swapRect;
    //排序
    for (int i = 0; i < split_mat.size() - 1; ++i) {

        for (int j = 0; j < split_mat.size() - 1 - i; ++j) {

            if (split_mat[j].x > split_mat[j + 1].x) {
                swapRect = split_mat[j];
                split_mat[j] = split_mat[j + 1];
                split_mat[j + 1] = swapRect;
            }
        }
    }

    Ptr<ml::SVM> svm = SVM::load("/storage/emulated/0/number_svm.xml");
    for (int i = 0; i < split_mat.size(); ++i) {
        //分割
        Mat sp(dilate_erodeOut, split_mat[i]);
        //缩放
        int nRows = 11;
        int nCols = 7;
        Mat dst(nRows, nCols, sp.type());
        resize(sp, dst, dst.size(), 0, 0, INTER_LINEAR);
        Mat bit;
        bitwise_not(dst, bit);
        //输出------训练数据提取样本时候使用
//        char name[50];
//        //坑 保存的时候图片尺寸不要太大(由于刚开始图片保存的是110：70的导致怎么训练都不成功)，最好格式是png.
//        sprintf(name,"/storage/emulated/0/dstImg_%d.png",i);
//        imwrite(name, bit);

        //识别数字------------
        Mat p = bit.reshape(1, 1);
        p.convertTo(p, CV_32FC1);
//        float x = svm->predict(p,m);
        float x = svm->predict(p);
        // float 转string
        ostringstream buffer;
        buffer << x;
        num_str += buffer.str();
    }
}

BitmapUtils::~BitmapUtils() {

}