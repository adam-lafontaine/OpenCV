#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>

constexpr auto IMAGE_PATH = "D:/repos/OpenCV/Tutorial/in_files/mclaren.jpg";


void affine_transform()
{
    cv::Mat src = cv::imread(IMAGE_PATH, cv::IMREAD_COLOR);
    if (src.empty())
    {
        std::cout << "Could not read the image: " << IMAGE_PATH << '\n';
        return;
    }

    cv::Point2f srcTri[3] = {
        cv::Point2f(0.f, 0.f),
        cv::Point2f(src.cols - 1.f, 0.f),
        cv::Point2f(0.f, src.rows - 1.f)
    };

    cv::Point2f dstTri[3] = {
        cv::Point2f(0.f, src.rows * 0.33f),
        cv::Point2f(src.cols * 0.85f, src.rows * 0.25f),
        cv::Point2f(src.cols * 0.15f, src.rows * 0.7f)
    };

    cv::Mat warp_mat = cv::getAffineTransform(srcTri, dstTri);
    cv::Mat warp_dst = cv::Mat::zeros(src.rows, src.cols, src.type());
    cv::warpAffine(src, warp_dst, warp_mat, warp_dst.size());

    cv::Point center = cv::Point(warp_dst.cols / 2, warp_dst.rows / 2);
    double angle = -50.0;
    double scale = 0.6;
    cv::Mat rot_mat = cv::getRotationMatrix2D(center, angle, scale);

    cv::Mat warp_rotate_dst;
    cv::warpAffine(warp_dst, warp_rotate_dst, rot_mat, warp_dst.size());

    cv::imshow("Source image", src);
    cv::imshow("Warp", warp_dst);
    cv::imshow("Warp + Rotate", warp_rotate_dst);
    cv::waitKey();
}