#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>

constexpr auto IMAGE_PATH_1 = "D:/repos/OpenCV/Tutorial/in_files/mclaren.jpg";
constexpr auto IMAGE_PATH_2 = "D:/repos/OpenCV/Tutorial/in_files/cadillac.jpg";
constexpr auto IMAGE_PATH_3 = "D:/repos/OpenCV/Tutorial/in_files/corvette.jpg";


void compare_hist()
{
    
    cv::Mat src_base = cv::imread(IMAGE_PATH_1, cv::IMREAD_COLOR);
    cv::Mat src_test1 = cv::imread(IMAGE_PATH_2, cv::IMREAD_COLOR);
    cv::Mat src_test2 = cv::imread(IMAGE_PATH_3, cv::IMREAD_COLOR);
    if (src_base.empty() || src_test1.empty() || src_test2.empty())
    {
        std::cout << "Could not open or find the images!\n";
    }

    cv::Mat hsv_base, hsv_test1, hsv_test2;
    cv::cvtColor(src_base, hsv_base, cv::COLOR_BGR2HSV);
    cv::cvtColor(src_test1, hsv_test1, cv::COLOR_BGR2HSV);
    cv::cvtColor(src_test2, hsv_test2, cv::COLOR_BGR2HSV);

    cv::Mat hsv_half_down = hsv_base(cv::Range(hsv_base.rows / 2, hsv_base.rows), cv::Range(0, hsv_base.cols));

    int h_bins = 50, s_bins = 60;
    int histSize[] = { h_bins, s_bins };

    // hue varies from 0 to 179, saturation from 0 to 255
    float h_ranges[] = { 0, 180 };
    float s_ranges[] = { 0, 256 };
    const float* ranges[] = { h_ranges, s_ranges };

    // Use the 0-th and 1-st channels
    int channels[] = { 0, 1 };

    bool uniform = true, accumulate = false;

    auto const make_hist = [&](cv::Mat& src)
    {
        cv::Mat hist_dst;
        cv::calcHist(&src, 1, channels, cv::Mat(), hist_dst, 2, histSize, ranges, uniform, accumulate);
        cv::normalize(hist_dst, hist_dst, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

        return hist_dst;
    };

    auto hist_base = make_hist(hsv_base);
    auto hist_half_down = make_hist(hsv_half_down);
    auto hist_test1 = make_hist(hsv_test1);
    auto hist_test2 = make_hist(hsv_test2);

    for (int compare_method = 0; compare_method < 4; compare_method++)
    {
        double base_base = cv::compareHist(hist_base, hist_base, compare_method);
        double base_half = cv::compareHist(hist_base, hist_half_down, compare_method);
        double base_test1 = cv::compareHist(hist_base, hist_test1, compare_method);
        double base_test2 = cv::compareHist(hist_base, hist_test2, compare_method);
        std::cout << "Method " << compare_method << " Perfect, Base-Half, Base-Test(1), Base-Test(2) : "
            << base_base << " / " << base_half << " / " << base_test1 << " / " << base_test2 << '\n';
    }
    std::cout << "Done \n";
}