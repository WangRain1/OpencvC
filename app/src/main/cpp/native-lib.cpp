#include <jni.h>
#include <string>
// 路径配置
#include <opencv2/opencv.hpp>
using namespace cv;

extern "C" JNIEXPORT jstring JNICALL
Java_com_safe_silent_opencvc_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    Mat test(2,2,CV_8UC3,Scalar(0,0,255));

    return env->NewStringUTF(hello.c_str());
}
