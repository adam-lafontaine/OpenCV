#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

#include <iostream>

const int max_value_H = 360 / 2;
const int max_value = 255;

const cv::String window_capture_name = "Video Capture";
const cv::String window_detection_name = "Object Detection";

int low_H = 0, low_S = 0, low_V = 0;
int high_H = max_value_H, high_S = max_value, high_V = max_value;

static void on_low_H_thresh_trackbar(int, void*)
{
    low_H = cv::min(high_H - 1, low_H);
    cv::setTrackbarPos("Low H", window_detection_name, low_H);
}


static void on_high_H_thresh_trackbar(int, void*)
{
    high_H = cv::max(high_H, low_H + 1);
    cv::setTrackbarPos("High H", window_detection_name, high_H);
}


static void on_low_S_thresh_trackbar(int, void*)
{
    low_S = cv::min(high_S - 1, low_S);
    cv::setTrackbarPos("Low S", window_detection_name, low_S);
}


static void on_high_S_thresh_trackbar(int, void*)
{
    high_S = cv::max(high_S, low_S + 1);
    cv::setTrackbarPos("High S", window_detection_name, high_S);
}


static void on_low_V_thresh_trackbar(int, void*)
{
    low_V = cv::min(high_V - 1, low_V);
    cv::setTrackbarPos("Low V", window_detection_name, low_V);
}


static void on_high_V_thresh_trackbar(int, void*)
{
    high_V = cv::max(high_V, low_V + 1);
    cv::setTrackbarPos("High V", window_detection_name, high_V);
}


void threshold_inrange()
{
    cv::VideoCapture cap(0);

    cv::namedWindow(window_capture_name);
    cv::namedWindow(window_detection_name);

    // Trackbars to set thresholds for HSV values
    cv::createTrackbar("Low H", window_detection_name, &low_H, max_value_H, on_low_H_thresh_trackbar);
    cv::createTrackbar("High H", window_detection_name, &high_H, max_value_H, on_high_H_thresh_trackbar);
    cv::createTrackbar("Low S", window_detection_name, &low_S, max_value, on_low_S_thresh_trackbar);
    cv::createTrackbar("High S", window_detection_name, &high_S, max_value, on_high_S_thresh_trackbar);
    cv::createTrackbar("Low V", window_detection_name, &low_V, max_value, on_low_V_thresh_trackbar);
    cv::createTrackbar("High V", window_detection_name, &high_V, max_value, on_high_V_thresh_trackbar);

    cv::Mat frame, frame_HSV, frame_threshold;

    while (true) {
        cap >> frame;
        if (frame.empty())
        {
            break;
        }
        // Convert from BGR to HSV colorspace
        cv::cvtColor(frame, frame_HSV, cv::COLOR_BGR2HSV);

        // Detect the object based on HSV Range Values
        cv::inRange(frame_HSV, cv::Scalar(low_H, low_S, low_V), cv::Scalar(high_H, high_S, high_V), frame_threshold);

        // Show the frames
        cv::imshow(window_capture_name, frame);
        cv::imshow(window_detection_name, frame_threshold);

        char key = (char)cv::waitKey(30);
        if (key == 'q' || key == 27)
        {
            break;
        }
    }
}