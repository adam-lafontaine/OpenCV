#include "../../../utils/video_test.hpp"

#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>

constexpr auto IMAGE_PATH = "D:/repos/OpenCV/Tutorial/in_files/cadillac.jpg";

void laplacian()
{
    // Declare the variables we are going to use
    cv::Mat src, src_gray, dst;
    int kernel_size = 3;
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;

    const char* window_name = "Laplace Demo";

    src = cv::imread(IMAGE_PATH, cv::IMREAD_COLOR);
    if (src.empty())
    {
        std::cout << "Could not read the image: " << IMAGE_PATH << '\n';
        return;
    }

    // Remove noise by blurring with a Gaussian filter ( kernel size = 3 )
    cv::GaussianBlur(src, src, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);

    // Convert the image to grayscale
    cv::cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);

    cv::Mat abs_dst;
    cv::Laplacian(src_gray, dst, ddepth, kernel_size, scale, delta, cv::BORDER_DEFAULT);

    // converting back to CV_8U
    cv::convertScaleAbs(dst, abs_dst);

    cv::imshow(window_name, abs_dst);
    cv::waitKey(0);
}


void laplacian_video()
{
    cv::Mat src, mat_gray, mat_laplace, dst;
    int kernel_size = 3;
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;
    int max_binary = 255;
    int threshold_value = 50;

    auto const laplacian_func = [&](cv::Mat& src, cv::Mat& dst)
    {
        // Remove noise by blurring with a Gaussian filter ( kernel size = 3 )
        cv::GaussianBlur(src, src, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);

        // Convert the image to grayscale
        cv::cvtColor(src, mat_gray, cv::COLOR_BGR2GRAY);
                
        cv::Laplacian(mat_gray, mat_laplace, ddepth, kernel_size, scale, delta, cv::BORDER_DEFAULT);

        // converting back to CV_8U
        cv::convertScaleAbs(mat_laplace, mat_gray);

        cv::threshold(mat_gray, dst, threshold_value, max_binary, cv::THRESH_BINARY);
    };

    video_test(laplacian_func);
}