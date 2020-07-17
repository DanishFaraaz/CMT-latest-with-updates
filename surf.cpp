#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <stdio.h>
#include <errno.h>

#include "opencv2/opencv.hpp"
#include "withrobot_camera.hpp"
#include<opencv2/opencv.hpp>
#include "opencv2/core.hpp"
#ifdef HAVE_OPENCV_XFEATURES2D
#include "opencv2/highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
using namespace cv;
using namespace cv::xfeatures2d;
using std::cout;
using std::endl;
int main( int argc, char* argv[] )
{
    int maximum, ind, i;
    int m = 0;


    // VideoCapture cap("newtest.mp4");

    // if (!cap.isOpened()){
    //     cout << "error" << endl;
    // }
    //Mat src = imread( samples::findFile( parser.get<String>( "@input" ) ), IMREAD_GRAYSCALE );

    const char* devPath = "/dev/video0";
    Withrobot::Camera camera(devPath);
        
    //cap.open(0); //Open default camera device

    /* bayer RBG 640 x 480 80 fps */
    camera.set_format(640, 480, Withrobot::fourcc_to_pixformat('G','B','G','R'), 1, 80);

    /*
     * get current camera format (image size and frame rate)
     */
    Withrobot::camera_format camFormat;
    camera.get_current_format(camFormat);

    /*
     * Print infomations
     */
    std::string camName = camera.get_dev_name();
    std::string camSerialNumber = camera.get_serial_number();

    printf("dev: %s, serial number: %s\n", camName.c_str(), camSerialNumber.c_str());
    printf("----------------- Current format informations -----------------\n");
    camFormat.print();
    printf("---------------------------------------------------------------\n");

    int brightness = camera.get_control("Gain");
    int exposure = camera.get_control("Exposure (Absolute)");

    //camera.set_control("Gain", 70);
    camera.set_control("Gain", 30);
    //camera.set_control("Exposure (Absolute)", 15);
    camera.set_control("Exposure (Absolute)", exposure);

    if (!camera.start()) {
        perror("Failed to start.");
        exit(0);
    }


    std::string windowName = camName + " " + camSerialNumber;
    cv::Mat srcImg(cv::Size(camFormat.width, camFormat.height), CV_8UC1);
    cv::Mat colorImg(cv::Size(camFormat.width, camFormat.height), CV_8UC3);

    while (1) {
        int size = camera.get_frame(srcImg.data, camFormat.image_size, 1);
        cv::cvtColor(srcImg, colorImg, cv::COLOR_BayerGB2BGR);
        // Mat frame;
        // cap >> frame;

        cout << "Colour image recieved" << endl;

        Mat gray;
        cvtColor(colorImg, gray, CV_BGR2GRAY);
        //-- Step 1: Detect the keypoints using SURF Detector
        int minHessian = 1000;
        Ptr<SURF> detector = SURF::create( minHessian );
        std::vector<KeyPoint> keypoints;
        detector->detect( gray, keypoints );
        //-- Draw keypoints
        Mat img_keypoints;
        drawKeypoints( gray, keypoints, img_keypoints );
        for (i=1; i<= int(sizeof(keypoints)); i++){
        cout << "Inside while loop" << endl;
        if (keypoints[i].size>m){
            cout << "Inside if loop" << endl;
            m = keypoints[i].size;
            ind = i;
            cout << ind << endl;
        }
    }

        cout << ind << endl;
        //-- Show detected (drawn) keypoints
        imshow("SURF Keypoints", img_keypoints );
        //imshow("Color",frame);
        //cout << keypoints[2].size << endl;
        waitKey(1);
        
    }
    return 0;
}
#else
int main()
{
    std::cout << "This tutorial code needs the xfeatures2d contrib module to be run." << std::endl;
    return 0;
}
#endif
