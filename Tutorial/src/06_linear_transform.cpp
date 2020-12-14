#include "tutorial.hpp"
#include "../../utils/transform_pixels.hpp"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

constexpr auto IMAGE_1 = "D:/repos/OpenCV/Tutorial/in_files/corvette.jpg";


void linear_transform()
{
    cv::Mat img_1 = cv::imread(IMAGE_1, cv::IMREAD_COLOR);
    if (img_1.empty())
    {
        std::cout << "Could not read the image()s/n";
        return;
    }

    double alpha = 3.0; // [1.0 - 3.0]
    int beta = 100; // [0 - 255]

    cv::Mat dst = cv::Mat::zeros(img_1.size(), img_1.type());

    /*auto const func = [&](cv::Vec3b const& src, cv::Vec3b& dst)
    {
        for (int c = 0; c < img_1.channels(); ++c)
        {
            dst[c] = cv::saturate_cast<uchar>(alpha * src[c] + beta);
        }
    };

    utils::transform_pixels_par(img_1, dst, func);*/

    auto depth_unchanged = -1;

    img_1.convertTo(dst, depth_unchanged, alpha, beta);

    cv::imshow("Original Image", img_1);
    cv::imshow("New Image", dst);

    cv::waitKey();
}