#include "../../../utils/video_test.hpp"

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>

namespace canny_15
{
    cv::Mat src, src_gray;
    cv::Mat dst, detected_edges;
    int lowThreshold = 0;
    const int max_lowThreshold = 254;
    const int ratio = 3;
    const int kernel_size = 3;

    constexpr auto IMAGE_PATH = "D:/repos/temp/seat_wrinkle.BMP"; // "D:/repos/OpenCV/Tutorial/in_files/cadillac.jpg";

    const char* window_name = "Edge Map";


    static void CannyThreshold(int, void*)
    {
        //cv::blur(src_gray, detected_edges, cv::Size(3, 3));
        cv::Canny(src_gray, detected_edges, lowThreshold, lowThreshold * ratio, kernel_size);
        dst = cv::Scalar::all(0);
        src_gray.copyTo(dst, detected_edges);
        cv::imshow(window_name, dst);
    }


    void canny()
    {
        src = cv::imread(IMAGE_PATH, cv::IMREAD_COLOR);
        if (src.empty())
        {
            std::cout << "Could not read the image: " << IMAGE_PATH << '\n';
            return;
        }

        dst.create(src.size(), src.type());

        cv::GaussianBlur(src, src, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);        

        cv::cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);

        cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);

        cv::createTrackbar("Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold);

        CannyThreshold(0, 0);
        cv::waitKey(0);
    }
}


void canny()
{
    canny_15::canny();
}


void canny_video()
{
    cv::Mat src, src_gray;
    cv::Mat dst, detected_edges;
    int threshold = 30;
    const int ratio = 3;
    const int kernel_size = 3;

    auto const canny_func = [&](cv::Mat& src, cv::Mat& dst) 
    {
        cv::GaussianBlur(src, src, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);

        cv::cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);

        cv::Canny(src_gray, detected_edges, threshold, threshold * ratio, kernel_size);
        dst = cv::Scalar::all(0);
        src_gray.copyTo(dst, detected_edges);
    };

    video_test(canny_func);
}