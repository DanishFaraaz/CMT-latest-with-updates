#include "gui.h"
#include <iostream>

#include <opencv2/highgui/highgui.hpp>
#if CV_MAJOR_VERSION > 2
# include <opencv2/imgproc.hpp>
#endif

#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"

using cv::setMouseCallback;
using cv::Point;
using cv::Scalar;
using cv::Size;
using namespace cv;
using namespace cv::xfeatures2d;
using std::cout;
using std::endl;

int x_tl, y_tl, width, height;


void screenLog(Mat im_draw, const string text)
{
    int font = cv::FONT_HERSHEY_SIMPLEX;
    float font_scale = 0.5;
    int thickness = 1;
    int baseline;

    Size text_size = cv::getTextSize(text, font, font_scale, thickness, &baseline);

    Point bl_text = Point(0,text_size.height);
    Point bl_rect = bl_text;

    bl_rect.y += baseline;

    Point tr_rect = bl_rect;
    tr_rect.x = im_draw.cols; //+= text_size.width;
    tr_rect.y -= text_size.height + baseline;

    rectangle(im_draw, bl_rect, tr_rect, Scalar(0,0,0), -1);

    putText(im_draw, text, bl_text, font, font_scale, Scalar(255,255,255));
}

//For bbox selection
static string win_name_;
static Mat im_select;
static bool tl_set;
static bool br_set;
static Point tl;
static Point br;

static void onMouse(int event, int x, int y, int flags, void *param)
{
    Mat im_draw;
    im_select.copyTo(im_draw);

    if(event == CV_EVENT_LBUTTONUP && !tl_set)
    {
        tl = Point(x,y);
        tl_set = true;
    }

    else if(event == CV_EVENT_LBUTTONUP && tl_set)
    {
        br = Point(x,y);
        br_set = true;
        screenLog(im_draw, "Initializing...");
    }

    if (!tl_set) screenLog(im_draw, "Click on the top left corner of the object");
    else
    {
        rectangle(im_draw, tl, Point(x, y), Scalar(255,0,0));

        if (!br_set) screenLog(im_draw, "Click on the bottom right corner of the object");
    }

    imshow(win_name_, im_draw);
}

Rect getRect(const Mat im, const string win_name)
{

    int maximum, ind, i;
    int m = 0;

    Mat gray;
    cvtColor(im, gray, CV_BGR2GRAY);
    cout << "Converted to gray" << endl;

    //Mat src = imread( samples::findFile( parser.get<String>( "@input" ) ), IMREAD_GRAYSCALE );

    //-- Step 1: Detect the keypoints using SURF Detector
    int minHessian = 200;
    Ptr<SURF> detector = SURF::create( minHessian );
    std::vector<KeyPoint> keypoints;
    detector->detect( gray, keypoints );
    //-- Draw keypoints
    Mat img_keypoints;
    drawKeypoints( im, keypoints, img_keypoints );
    cout << "drawn keypoints" << endl;
    for (i=1; i<= int(sizeof(keypoints)); i++){
        cout << "Inside while loop" << endl;
        if (keypoints[i].size>m){
            cout << "Inside if loop" << endl;
            m = keypoints[i].size;
            ind = i;
            cout << ind << endl;
        }
    }
    //-- Show detected (drawn) keypoints
    //imshow("SURF Keypoints", img_keypoints );

    cout << "found maximum keypoint" << endl;

    x_tl = keypoints[ind].pt.x - (keypoints[ind].size/2);
    y_tl = keypoints[ind].pt.y - (keypoints[ind].size/2);
    width = keypoints[ind].size;
    height = keypoints[ind].size;

    cout << "SURF done!" << endl;

    return Rect(x_tl, y_tl, width, height);

    waitKey(10);
}

