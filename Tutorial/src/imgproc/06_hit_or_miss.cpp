#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

constexpr uchar N0 = 0;
constexpr uchar N1 = 255;

void hit_or_miss()
{
    cv::Mat input_image = (cv::Mat_<uchar>(8, 8) <<
        N0, N0, N0, N0, N0, N0, N0, N0,
        N0, N1, N1, N1, N0, N0, N0, N1,
        N0, N1, N1, N1, N0, N0, N0, N0,
        N0, N1, N1, N1, N0, N1, N0, N0,
        N0, N0, N1, N0, N0, N0, N0, N0,
        N0, N0, N1, N0, N0, N1, N1, N0,
        N0, N1, N0, N1, N0, N0, N1, N0,
        N0, N1, N1, N1, N0, N0, N0, N0);

    cv::Mat kernel = (cv::Mat_<int>(3, 3) <<
        0, 1, 0,
        1, -1, 1,
        0, 1, 0);

    cv::Mat output_image;
    cv::morphologyEx(input_image, output_image, cv::MORPH_HITMISS, kernel);

    const int rate = 50;

    kernel = (kernel + 1) * 127;
    kernel.convertTo(kernel, CV_8U);

    cv::resize(kernel, kernel, cv::Size(), rate, rate, cv::INTER_NEAREST);
    cv::imshow("kernel", kernel);

    cv::moveWindow("kernel", 0, 0);
    cv::resize(input_image, input_image, cv::Size(), rate, rate, cv::INTER_NEAREST);

    cv::imshow("Original", input_image);
    cv::moveWindow("Original", 0, 200);
    cv::resize(output_image, output_image, cv::Size(), rate, rate, cv::INTER_NEAREST);

    cv::imshow("Hit or Miss", output_image);
    cv::moveWindow("Hit or Miss", 500, 200);

    cv::waitKey();
}