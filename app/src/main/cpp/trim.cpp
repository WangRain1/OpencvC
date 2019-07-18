//
// Created by ts on 19-7-17.
//

#include "trim.h"
#include <iostream>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/ml/ml.hpp>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <android/log.h>
#include <vector>
using namespace std;
using namespace cv;
using namespace ml;

void getBubble(vector<Mat> &trainingImages,vector<int> &trainingLabels,vector<Mat> &vMat);

void getBubble_b(vector<Mat> &trainingImages,vector<int> &trainingLabels,vector<Mat> &vMat);

vector<Mat> trainingImages; //用来存放训练图像信息的容器
vector<int> trainingLabels;


void TrimTest::startTrim(vector<Mat> &vMat,vector<Mat>& vMat_b) {

    Mat classes;
    getBubble(trainingImages,trainingLabels,vMat);
    getBubble_b(trainingImages,trainingLabels,vMat_b);

    Mat trainingData(trainingImages.size(),trainingImages[0].cols, CV_32FC1);
    for (int i = 0; i < trainingImages.size(); i++)
    {
        Mat temp(trainingImages[i]);
        temp.copyTo(trainingData.row(i));
    }
    trainingData.convertTo(trainingData, CV_32FC1);
    Mat(trainingLabels).copyTo(classes);
    classes.convertTo(classes, CV_32SC1);
    //以下是设置SVM训练模型的配置
    Ptr<SVM> model = SVM::create();
    model->setType(SVM::C_SVC);
    model->setKernel(SVM::LINEAR);
    model->setGamma(1);
    model->setC(1);
    model->setCoef0(0);
    model->setNu(0);
    model->setP(0);
    model->setTermCriteria(TermCriteria(1, 20000, 0.0001));

    Ptr<TrainData> tdata = TrainData::create(trainingData, ROW_SAMPLE, classes);
    imwrite("/storage/emulated/0/ccc.jpg",trainingData);

//    model->train(trainingData, ROW_SAMPLE, classes);
    model->trainAuto(tdata);
    model->save("/storage/emulated/0/car.txt");
}

void getBubble(vector<Mat> &tImages, vector<int> &tLabels,vector<Mat> &vMat)
{
    for (int i = 0; i < vMat.size(); ++i) {

        Mat gray = vMat[i];
        gray = gray.reshape(1, 1);
        tImages.push_back(gray);
        tLabels.push_back(1);//该样本为数字1
    }

}

void getBubble_b(vector<Mat> &traages,vector<int> &traels,vector<Mat> &vMat){
    for (int i = 0; i < vMat.size(); ++i) {

        Mat gray = vMat[i];
        gray = gray.reshape(1, 1);
        traages.push_back(gray);
        traels.push_back(2);//该样本为数字2
    }
}

float TrimTest::checkMsg(JNIEnv *env,cv::Mat &mat) {

    Ptr<ml::SVM>svm =SVM::load("/storage/emulated/0/car.txt");
    Mat p = mat.reshape(1, 1);
    p.convertTo(p, CV_32FC1);
//    imwrite("/storage/emulated/0/resoo.jpg",p);

    Mat m;
//    float x = svm->predict(p,m);
    float x = svm->predict(p);
    __android_log_print(ANDROID_LOG_ERROR,"checkresutl--","%f", x);
    return x;
}

TrimTest::TrimTest() {

}

TrimTest::~TrimTest() {

}
