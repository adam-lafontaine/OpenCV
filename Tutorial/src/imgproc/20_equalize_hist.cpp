#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>

constexpr auto IMAGE_PATH = "D:/repos/OpenCV/Tutorial/in_files/mclaren.jpg";


void equalize_hist()
{
    cv::Mat src = cv::imread(IMAGE_PATH, cv::IMREAD_COLOR);
    if (src.empty())
    {
        std::cout << "Could not read the image: " << IMAGE_PATH << '\n';
        return;
    }

    cv::cvtColor(src, src, cv::COLOR_BGR2GRAY);
    cv::Mat dst;

    cv::equalizeHist(src, dst);
    cv::imshow("Source image", src);
    cv::imshow("Equalized Image", dst);
    cv::waitKey();
}