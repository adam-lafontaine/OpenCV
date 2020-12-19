#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>
#include <string>

int DELAY_CAPTION = 1500;
int DELAY_BLUR = 100;
int MAX_KERNEL_LENGTH = 31;

constexpr auto IMAGE_PATH = "D:/repos/OpenCV/Tutorial/in_files/corvette.jpg";

constexpr auto WINDOW_NAME = "Window";

void add_caption(cv::Mat& img, const char* caption, int num);

void smoothing()
{
    cv::Mat src = cv::imread(IMAGE_PATH, cv::IMREAD_COLOR);
    if (src.empty())
    {
        std::cout << "Could not read the image: " << IMAGE_PATH << '\n';
        return;
    }

    cv::Mat dst = src.clone();
    add_caption(dst, "Original Image", 0);
    cv::imshow(WINDOW_NAME, dst);
    cv::waitKey();
    
    for (int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2)
    {
        cv::blur(src, dst, cv::Size(i, i), cv::Point(-1, -1));
        add_caption(dst, "Homogeneous Blur", i);
        cv::imshow(WINDOW_NAME, dst);
        cv::waitKey(DELAY_BLUR);
    }
    cv::waitKey();

    for (int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2)
    {
        cv::GaussianBlur(src, dst, cv::Size(i, i), 0, 0);
        add_caption(dst, "Gaussian Blur", i);
        cv::imshow(WINDOW_NAME, dst);
        cv::waitKey(DELAY_BLUR);
    }
    cv::waitKey();


    for (int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2)
    {
        cv::medianBlur(src, dst, i);
        add_caption(dst, "Median Blur", i);
        cv::imshow(WINDOW_NAME, dst);
        cv::waitKey(DELAY_BLUR);
    }
    cv::waitKey();


    for (int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2)
    {
        cv::bilateralFilter(src, dst, i, i * 2, i / 2);
        add_caption(dst, "Bilateral Blur", i);
        cv::imshow(WINDOW_NAME, dst);
        cv::waitKey(DELAY_BLUR);
    }
    cv::waitKey();
}


void add_caption(cv::Mat& img, const char* caption, int num)
{
    auto str = std::string(caption) + " " + std::to_string(num);

    putText(img, str,
        cv::Point(img.cols / 48, img.rows * 29 / 30),
        cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(255, 255, 255));
}