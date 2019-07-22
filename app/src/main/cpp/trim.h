//
// Created by ts on 19-7-17.
//

#ifndef OPENCVC_TRIM_H
#define OPENCVC_TRIM_H

#include <cstdio>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <string.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>
#include <vector>
#include <jni.h>

class TrimTest{


public:
    TrimTest();

    void startTrim(std:: vector<std::vector<cv::Mat>>& vm);

    float checkMsg(cv::Mat& mat);

    ~TrimTest();
};


#endif //OPENCVC_TRIM_H
