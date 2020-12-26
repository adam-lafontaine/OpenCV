#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "../../../utils/video_test.hpp"

#include <iostream>

constexpr auto IMAGE_PATH = "D:/repos/OpenCV/Tutorial/in_files/cadillac.jpg";

void do_sobel(cv::Mat& src_gray, cv::Mat& dst, int ksize, int scale, int delta);
void do_sobel_x(cv::Mat& src_gray, cv::Mat& dst, int ksize, int scale, int delta);
void do_sobel_y(cv::Mat& src_gray, cv::Mat& dst, int ksize, int scale, int delta);

void do_scharr(cv::Mat& src_gray, cv::Mat& dst, int scale, int delta);



void sobel_scharr()
{
    printf("The sample uses Sobel or Scharr OpenCV functions for edge detection\n\n");
    printf("{ksize   k|1|ksize (hit 'K' to increase its value at run time)}");
    printf("{scale   s|1|scale (hit 'S' to increase its value at run time)}");
    printf("{delta   d|0|delta (hit 'D' to increase its value at run time)}");
    printf("\nPress 'ESC' to exit program.\nPress 'R' to reset values ( ksize will be -1 equal to Scharr function )");
    
    // First we declare the variables we are going to use
    cv::Mat image, src, src_gray;
    cv::Mat sobel_dst;
    cv::Mat scharr_dst;

    auto sobel_name = "Sobel Demo - Simple Edge Detector";
    auto scharr_name = "Scharr Demo - Simple Edge Detector";

    int ksize = 1;
    int scale = 1;
    int delta = 0;
    

    image = cv::imread(IMAGE_PATH, cv::IMREAD_COLOR);
    if (image.empty())
    {
        std::cout << "Could not read the image: " << IMAGE_PATH << '\n';
        return;
    }

    // Remove noise by blurring with a Gaussian filter ( kernel size = 3 )
    cv::GaussianBlur(image, src, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);

    // Convert the image to grayscale
    cv::cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);

    for (;;)
    {
        do_sobel(src_gray, sobel_dst, ksize, scale, delta);
        cv::imshow(sobel_name, sobel_dst);

        do_scharr(src_gray, scharr_dst, scale, delta);
        cv::imshow(scharr_name, scharr_dst);


        char key = (char)cv::waitKey(0);

        switch (key)
        {
        case 27:
            return;

        case 'k':
        case 'K':
            ksize = ksize < 30 ? ksize + 2 : -1;
            break;

        case 's':
        case 'S':
            scale++;
            break;

        case 'd':
        case 'D':
            delta++;
            break;

        case 'r':
        case 'R':
            scale = 1;
            ksize = -1;
            delta = 0;
            break;
        }
    }
}


void sobel_video()
{
    int ksize = 3;
    int scale = 2;
    int delta = 0;
    int max_binary = 255;
    int threshold_value = 75;

    cv::Mat src_blur, src_gray, src_sobel, src_thresh;

    auto const sobel_func = [&](cv::Mat& src, cv::Mat& dst)
    { 
        // Remove noise by blurring with a Gaussian filter ( kernel size = 3 )
        cv::GaussianBlur(src, src_blur, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);

        // Convert the image to grayscale
        cv::cvtColor(src_blur, src_gray, cv::COLOR_BGR2GRAY);

        do_sobel(src_gray, src_sobel, ksize, scale, delta);

        cv::threshold(src_sobel, dst, threshold_value, max_binary, cv::THRESH_BINARY);
    };

    video_test(sobel_func);
}


void do_sobel(cv::Mat& src_gray, cv::Mat& dst, int ksize, int scale, int delta)
{
    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;

    int ddepth = CV_16S;

    cv::Sobel(src_gray, grad_x, ddepth, 1, 0, ksize, scale, delta, cv::BORDER_DEFAULT);
    cv::Sobel(src_gray, grad_y, ddepth, 0, 1, ksize, scale, delta, cv::BORDER_DEFAULT);

    // converting back to CV_8U
    cv::convertScaleAbs(grad_x, abs_grad_x);
    cv::convertScaleAbs(grad_y, abs_grad_y);

    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, dst);
}


void do_sobel_x(cv::Mat& src_gray, cv::Mat& dst, int ksize, int scale, int delta)
{
    cv::Mat grad_x;

    int ddepth = CV_16S;

    cv::Sobel(src_gray, grad_x, ddepth, 1, 0, ksize, scale, delta, cv::BORDER_DEFAULT);

    // converting back to CV_8U
    cv::convertScaleAbs(grad_x, dst);
}


void do_sobel_y(cv::Mat& src_gray, cv::Mat& dst, int ksize, int scale, int delta)
{
    cv::Mat grad_y;
    cv::Mat abs_grad_y;

    int ddepth = CV_16S;

    cv::Sobel(src_gray, grad_y, ddepth, 0, 1, ksize, scale, delta, cv::BORDER_DEFAULT);

    // converting back to CV_8U
    cv::convertScaleAbs(grad_y, dst);
}


void do_scharr(cv::Mat& src_gray, cv::Mat& dst, int scale, int delta)
{
    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;

    int ddepth = CV_16S;

    cv::Scharr(src_gray, grad_x, ddepth, 1, 0, scale, delta, cv::BORDER_DEFAULT);
    cv::Scharr(src_gray, grad_y, ddepth, 0, 1, scale, delta, cv::BORDER_DEFAULT);

    // converting back to CV_8U
    cv::convertScaleAbs(grad_x, abs_grad_x);
    cv::convertScaleAbs(grad_y, abs_grad_y);

    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, dst);
}