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

void getBubble(vector<Mat> &trainingImages,vector<int> &trainingLabels,vector<vector<Mat>>& vm);


vector<Mat> trainingImages; //用来存放训练图像信息的容器
vector<int> trainingLabels;


void TrimTest::startTrim(vector<vector<Mat>>& vm) {


    Mat classes;

    getBubble(trainingImages,trainingLabels,vm);

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
    model->train(tdata);
    model->save("/storage/emulated/0/number_svm.xml");

    __android_log_print(ANDROID_LOG_ERROR,"训练完成...","over---");
}

void getBubble(vector<Mat> &tImages, vector<int> &tLabels,vector<vector<Mat>>& vm)
{

    for (int j = 0; j < vm.size(); j++) {
        int size = vm[j].size();
        for (int i = 0; i < size; i++) {

            Mat gray = vm[j][i];
            Mat reshapeMat;
            reshapeMat = gray.reshape(1,1);
            tImages.push_back(reshapeMat);
            tLabels.push_back(j);//该样本为数字1
        }
    }
}


float TrimTest::checkMsg(cv::Mat &mat) {

    Mat gray;
    cvtColor(mat,gray,COLOR_BGRA2GRAY);
    //大小尺寸一定要和训练的数据一致
    __android_log_print(ANDROID_LOG_ERROR,"checkresutl--","%d,%d", mat.cols,mat.rows);
    Ptr<ml::SVM>svm =SVM::load("/storage/emulated/0/number_svm.xml");
    Mat p = gray.reshape(1, 1);
    p.convertTo(p, CV_32FC1);
    Mat m;
//    float x = svm->predict(p,m);
    float x = svm->predict(p);
    return x;
}

TrimTest::TrimTest() {

}

TrimTest::~TrimTest() {

}
