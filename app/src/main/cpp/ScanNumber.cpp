//
// Created by ts on 19-7-15.
//


#include <android/bitmap.h>
#include <android/log.h>
#include "BitmapUtils.h"
#include "ScanNumber.h"
#include <vector>
#include <opencv2/ml/ml.hpp>
#include <iostream>
#include<cmath>



using namespace std;
using namespace ml;

void filterRect(Mat& gray,int thresh,string& number);
bool isInErrorPosition(int i);

ScanNumber::ScanNumber() {

}

// 初始图，防止被修改
Mat qm;
// 结果调优参数
bool optimizeGgain = false;


void ScanNumber::cutNumberArea(Mat& mat,string& number) {

    //剪切矩形
    Rect cR;
    cR.x = mat.cols / 20;
    cR.y = mat.rows / 2;
    cR.width = mat.cols * 18 / 20;
    cR.height = mat.rows * 3 / 4-cR.y;

    imwrite("/storage/emulated/0/mat.jpg", mat);
    Mat rm(mat,cR);
    Mat dstImg = rm.clone();
    Mat gray;
    cvtColor(rm, gray, COLOR_BGRA2GRAY); //转为灰度图
    imwrite("/storage/emulated/0/gray.jpg", gray);
    qm=gray.clone();
    optimizeGgain = false;
    // 最大阀值限定在 40 - 100 之间查找卡号，超过了还没找到就结束识别
    filterRect(gray,30,number);
}

void filterRect(Mat& gray,int thresh,string& number){

    __android_log_print(ANDROID_LOG_ERROR,"thresh--","%d",thresh);
    //最大阀值限定在 40 - 100 之间查找卡号，超过了还没找到就结束识别
    if (thresh>150){
        //没找到合适字符
        number = "识别失败，请把卡片放到扫描区域内！";
        return;
    }
    Mat gg = gray.clone();
    Mat th;
    threshold(gg, th, thresh, 255, THRESH_BINARY);
    imwrite("/storage/emulated/0/th.jpg", th);

    Mat bitwis;
    bitwise_not(th,bitwis);
    imwrite("/storage/emulated/0/bitwise_not.jpg",  bitwis);

    vector<vector<Point> > contours;
    vector<Vec4i> hierarcy;
    findContours(bitwis, contours, hierarcy, RETR_TREE, CHAIN_APPROX_NONE); //查找所有轮廓
    vector<Rect> boundRect(contours.size()); //定义外接矩形集合

    //获得所有矩形
    for (int i = 0; i < contours.size(); i++) {
        boundRect[i] = boundingRect(contours[i]); //查找每个轮廓的外接矩形
    }

    //面积 过滤
    int w0 = 0, h0 = 0;
    float m_area = bitwis.cols * bitwis.rows;
    vector<Rect> filer_area;
    for (int i = 0; i < boundRect.size(); ++i) {
        w0 = boundRect[i].width;
        h0 = boundRect[i].height;
        float r_area = w0*h0;
        if (r_area<m_area/60&&r_area>m_area/250 && (h0<bitwis.rows/2&&h0>bitwis.rows/4)){
            filer_area.push_back(boundRect[i]);
        }
    }

    //查看效果
    __android_log_print(ANDROID_LOG_ERROR,"filer_area-1-","%d",filer_area.size());
    for (int i = 0; i < filer_area.size(); ++i) {
        int x1 = filer_area[i].x;
        int y1 = filer_area[i].y;
        int w1 = filer_area[i].width;
        int h1 = filer_area[i].height;
        //绘制第i个外接矩形
        rectangle(gg, Point(x1, y1), Point(x1 + w1, y1 + h1), Scalar(0, 255, 0), 2, 8);
    }
    imwrite("/storage/emulated/0/gg.jpg", gg);

    //检测本次扫描的结果是否满足
    if(filer_area.size()<=0||filer_area.size()<16){
        thresh += 5;
        __android_log_print(ANDROID_LOG_ERROR,"filtera-r-","%d",thresh);
        gg.release();
        th.release();
        bitwis.release();
        number = "";
        filterRect(qm,thresh,number);
        return;
    }
    //排序 过滤
    Rect swapRect;
    for (int i = 0; i < filer_area.size() - 1; ++i) {
        for (int j = 0; j < filer_area.size() - 1 - i; ++j) {
            if (filer_area[j].y > filer_area[j + 1].y) {
                swapRect = filer_area[j];
                filer_area[j] = filer_area[j + 1];
                filer_area[j + 1] = swapRect;
            }
        }
    }
    int num = 1;
    bool isNeedBreak = false;
    vector<Rect> xy_filter;
    // y坐标 过滤
    for (int i = 0; i < filer_area.size(); ++i) {
        if(i>0){
            if (abs(filer_area[i].y-filer_area[i-1].y)<5){
                if(num==1){
                    xy_filter.push_back(filer_area[i-1]);
                }
                num += 1;
                xy_filter.push_back(filer_area[i]);
                isNeedBreak = true;
            } else if (isNeedBreak){
                break;
            }
        }
    }

    __android_log_print(ANDROID_LOG_ERROR,"filtera-1-","%d,%d,%d",num,thresh,xy_filter.size());

    // 检测是否满足条件
    if (num<16){
        thresh += 5;
        __android_log_print(ANDROID_LOG_ERROR,"filtera-2-","%d",thresh);
        gg.release();
        th.release();
        bitwis.release();
        number = "";
        filterRect(qm,thresh,number);
        return;
    } else if (num>=20){
        thresh += 5;
        __android_log_print(ANDROID_LOG_ERROR,"filtera-3-","%d",thresh);
        gg.release();
        th.release();
        bitwis.release();
        number = "";
        filterRect(qm,thresh,number);
        return;
    } else {
        __android_log_print(ANDROID_LOG_ERROR,"filtera-xx-","%d",thresh);
        //x排序 过滤
        Rect swapRect_x;
        for (int i = 0; i < xy_filter.size() - 1; ++i) {
            for (int j = 0; j < xy_filter.size() - 1 - i; ++j) {
                if (xy_filter[j].x > xy_filter[j + 1].x) {
                    swapRect_x = xy_filter[j];
                    xy_filter[j] = xy_filter[j + 1];
                    xy_filter[j + 1] = swapRect_x;
                }
            }
        }

        int average_value = 0;
        int all_width = 0;
        //取最大宽度值
        for (int i = 0; i < xy_filter.size(); ++i) {
            all_width += xy_filter[i].width;
        }
        average_value = all_width/xy_filter.size();

        // x坐标 过滤
        for (int i = 0; i < xy_filter.size(); ++i) {
            if (i>0){

                __android_log_print(ANDROID_LOG_ERROR,"filtera-x-","%d,%d,%d",average_value,xy_filter[i].width,xy_filter[i].x-
                                                                                         (xy_filter[i-1].x +
                                                                                         xy_filter[i-1].width));
            }
            if (i>0&&abs(xy_filter[i].x-(xy_filter[i-1].x + xy_filter[i-1].width))>average_value && !isInErrorPosition(i)){
                thresh += 5;
                __android_log_print(ANDROID_LOG_ERROR,"filtera-x2-","%d",i);
                gg.release();
                th.release();
                bitwis.release();
                number = "";
                filterRect(qm,thresh,number);
                return;
            }
        }
    }

    /**
     * 为了让筛选更优，当过滤条件都满足后，我们再稍微扩大阀值，让数字更清晰
     */
    if(!optimizeGgain && thresh < 150){
        optimizeGgain = true;
        thresh += 5;
        __android_log_print(ANDROID_LOG_ERROR,"filter-","optimizeGgain");
        gg.release();
        th.release();
        bitwis.release();
        number = "";
        filterRect(qm,thresh,number);
        return;
    }

    /**
     * 数字分割，识别
     * 走到这一步其实我们就已经把所有的数字位置给找到了，并且放到了 xy_filter[] 数组中，
     * xy_filter[]是我们最终过滤的数字的数组
     * th
     */

    Ptr<ml::SVM> svm = SVM::load("/storage/emulated/0/number_svm.xml");

    for (int i = 0; i < xy_filter.size(); ++i) {

        Mat split_rect(bitwis,xy_filter[i]);
        //缩放
        int nRows = 11;
        int nCols = 7;
        Mat dst(nRows, nCols, split_rect.type());
        resize(split_rect, dst, dst.size(), 0, 0, INTER_LINEAR);
        //保存训练模型的数据---（数据采集）--------
//        char name[50];
//        sprintf(name,"/storage/emulated/0/dstImg_%d.png",i);
//        imwrite(name, dst);

        //识别数字-------（数据识别）-----
        Mat p = dst.reshape(1, 1);
        p.convertTo(p, CV_32FC1);
        //float x = svm->predict(p,m);
        float x = svm->predict(p);
        // float 转string
        ostringstream buffer;
        buffer << x;
        number += buffer.str();
    }
    __android_log_print(ANDROID_LOG_ERROR,"num_str-","%s",number.c_str());
}

//特殊位置过了
bool isInErrorPosition(int i){

    return i == 6 || i == 4 || i==8 || i==12||i==16;
}

void ScanNumber::distinguishNumber(Mat &mat, string &res) {

}

ScanNumber::~ScanNumber() {

}

