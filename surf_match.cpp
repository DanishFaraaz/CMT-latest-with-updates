#include <iostream>
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
const char* keys =
    "{ help h |                  | Print help message. }"
    "{ input1 | box.png          | Path to input image 1. }"
    "{ input2 | box_in_scene.png | Path to input image 2. }";
int main( int argc, char* argv[] )
{
    Mat img1 = imread( "bottle.jpeg", IMREAD_GRAYSCALE );
    resize(img1, img1, cv::Size(320, 240));
    // Mat img2 = imread( "box2.jpg", IMREAD_GRAYSCALE );
    VideoCapture cap("out.mp4");

    //cap.set(CV_CAP_PROP_AUTOFOCUS, false);

    if ( img1.empty())
    {
        cout << "Could not open or find the image!\n" << endl;
        return -1;
    }

    // Mat img2;
    // cap >> img2;
    // cvtColor(img2, img2, CV_BGR2GRAY);
    // // -- Step 1: Detect the keypoints using SURF Detector, compute the descriptors
    // int minHessian = 400;
    // Ptr<SIFT> detector = SIFT::create( minHessian );
    // std::vector<KeyPoint> keypoints1, keypoints2;

    // Mat descriptors1, descriptors2;
    // detector->detectAndCompute( img1, noArray(), keypoints1, descriptors1 );
    // detector->detectAndCompute( img2, noArray(), keypoints2, descriptors2 );
    // //-- Step 2: Matching descriptor vectors with a FLANN based matcher
    // // Since SURF is a floating-point descriptor NORM_L2 is used
    // Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
    // std::vector< std::vector<DMatch> > knn_matches;
    // matcher->knnMatch( descriptors1, descriptors2, knn_matches, 2 );
    // //-- Filter matches using the Lowe's ratio test
    // const float ratio_thresh = 0.7f;
    // std::vector<DMatch> good_matches;

    // for (size_t i = 0; i < knn_matches.size(); i++)
    //     {
    //         if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance)
    //         {
    //             good_matches.push_back(knn_matches[i][0]);
    //         }
    //     }

    // Mat img_matches;
    // drawMatches( img1, keypoints1, img2, keypoints2, good_matches, img_matches, Scalar::all(-1),
    //             Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

    // cv::resize(img_matches, img_matches, cv::Size(1280,960));
    // //-- Show detected matches
    // imshow("Good Matches", img_matches );
    // waitKey();

    int array[] = {1,2,3,4,5};
    Scalar means = mean((0,2,0,0,3),noArray());
    cout << sizeof(array)/sizeof(array[0]) << endl;
    while (1){

        Mat frame;
        cap >> frame;

        Mat img2;
        cvtColor(frame, img2, CV_BGR2GRAY);
        resize(img2, img2, cv::Size(320, 240));

        //-- Step 1: Detect the keypoints using SURF Detector, compute the descriptors
        int minHessian = 400;
        Ptr<SIFT> detector = SIFT::create( minHessian );
        std::vector<KeyPoint> keypoints1, keypoints2;

        Mat descriptors1, descriptors2;
        detector->detectAndCompute( img1, noArray(), keypoints1, descriptors1 );
        detector->detectAndCompute( img2, noArray(), keypoints2, descriptors2 );
        //-- Step 2: Matching descriptor vectors with a FLANN based matcher
        // Since SURF is a floating-point descriptor NORM_L2 is used
        Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
        std::vector< std::vector<DMatch> > knn_matches;
        matcher->knnMatch( descriptors1, descriptors2, knn_matches, 2 );
        //-- Filter matches using the Lowe's ratio test
        const float ratio_thresh = 0.8f;
        std::vector<DMatch> good_matches;
        for (size_t i = 0; i < knn_matches.size(); i++)
        {
            if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance)
            {
                good_matches.push_back(knn_matches[i][0]);
            }
        }


        // for (size_t i=0;i< knn_matches.size(); i++){
        //     good_matches.push_back(knn_matches[i][0]);
        // }
        //-- Draw matches
        Mat img_matches;
        drawMatches( img1, keypoints1, img2, keypoints2, good_matches, img_matches, Scalar::all(-1),
                    Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
        // -- Show detected matches
        if (good_matches.size() > 0.1*knn_matches.size()){
            cout << "Object Detected!" << endl;
        }
        else {
            cout << "Object not in frame!" << endl;
        }
        imshow("Good Matches", img_matches );
        // imshow("Camera", frame);
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