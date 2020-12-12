#include "./tutorial.hpp"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

constexpr auto IMAGE_PATH = "D:/repos/OpenCV/Tutorial/in_files/corvette.jpg";


void mask_operations()
{
    
    cv::Mat src = cv::imread(IMAGE_PATH, cv::IMREAD_GRAYSCALE);
    if (src.empty())
    {
        std::cout << "Could not read the image: " << IMAGE_PATH << '\n';
        return;
    }

    cv::namedWindow("Input", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("Output", cv::WINDOW_AUTOSIZE);

    cv::imshow("Input", src);

    cv::Mat kernel = (cv::Mat_<char>(3, 3) << 
        0, -1, 0,
        -1, 5, -1,
        0, -1, 0);

    cv::Mat dst;

    cv::filter2D(src, dst, src.depth(), kernel);

    cv::imshow("Output", dst);

    cv::waitKey();
}