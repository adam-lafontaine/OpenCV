#include "../tutorial.hpp"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <cassert>

constexpr auto IMAGE_1 = "D:/repos/OpenCV/Tutorial/in_files/corvette.jpg";
constexpr auto IMAGE_2 = "D:/repos/OpenCV/Tutorial/in_files/tesla.jpg";


void blending()
{
    cv::Mat img_1 = cv::imread(IMAGE_1, cv::IMREAD_COLOR);
    cv::Mat img_2 = cv::imread(IMAGE_2, cv::IMREAD_COLOR);
    if (img_1.empty() || img_2.empty())
    {
        std::cout << "Could not read the image()s/n";
        return;
    }

    assert(img_1.rows == img_2.rows);
    assert(img_1.cols == img_2.cols);
    assert(img_1.type() == img_2.type());

    double alpha = 0.5;

    cv::Mat dst;
    cv::addWeighted(img_1, alpha, img_2, (1 - alpha), 0.0, dst);

    cv::imshow("Linear Blend", dst);

    cv::waitKey();
}


