//
// Created by ts on 19-7-23.
//

#ifndef OPENCVC_SCANNUMBER_H
#define OPENCVC_SCANNUMBER_H

#include <android/bitmap.h>
#include <opencv2/opencv.hpp>
#include <jni.h>

using namespace cv;
using namespace std;


class ScanNumber{


public:

    ScanNumber();

    void cutNumberArea(Mat& mat,string& number);

    void distinguishNumber(Mat& mat,string& res);

    ~ScanNumber();
};

#endif //OPENCVC_SCANNUMBER_H
