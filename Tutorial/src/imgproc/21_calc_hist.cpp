#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <vector>


constexpr auto IMAGE_PATH = "D:/repos/OpenCV/Tutorial/in_files/mclaren.jpg";


void calc_hist()
{
    cv::Mat src = cv::imread(IMAGE_PATH, cv::IMREAD_COLOR);
    if (src.empty())
    {
        std::cout << "Could not read the image: " << IMAGE_PATH << '\n';
        return;
    }

    std::vector<cv::Mat> bgr_planes;
    split(src, bgr_planes);

    int histSize = 256;
    float range[] = { 0, 256 }; //the upper boundary is exclusive
    const float* histRange = { range };
    bool uniform = true, accumulate = false;

    cv::Mat b_hist, g_hist, r_hist;
    calcHist(&bgr_planes[0], 1, 0, cv::Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
    calcHist(&bgr_planes[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
    calcHist(&bgr_planes[2], 1, 0, cv::Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);

    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound((double)hist_w / histSize);

    cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));
    normalize(b_hist, b_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
    normalize(g_hist, g_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
    normalize(r_hist, r_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());

    auto const blue = cv::Scalar(255, 0, 0);
    auto const green = cv::Scalar(0, 255, 0);
    auto const red = cv::Scalar(0, 0, 255);

    int const thickness = 2;
    int const line_type = 8;
    int const shift = 0;

    for (int i = 1; i < histSize; i++)
    {
        int x1 = bin_w * (i - 1);
        int x2 = bin_w * (i);

        int yb1 = hist_h - cvRound(b_hist.at<float>(i - 1));
        int yb2 = hist_h - cvRound(b_hist.at<float>(i));

        int yg1 = hist_h - cvRound(g_hist.at<float>(i - 1));
        int yg2 = hist_h - cvRound(g_hist.at<float>(i));

        int yr1 = hist_h - cvRound(r_hist.at<float>(i - 1));
        int yr2 = hist_h - cvRound(r_hist.at<float>(i));


        cv::line(histImage, 
            cv::Point(x1, yb1),
            cv::Point(x2, yb2),
            blue, thickness, line_type, shift);

        cv::line(histImage, 
            cv::Point(x1, yg1),
            cv::Point(x2, yg2),
            green, thickness, line_type, shift);

        cv::line(histImage, 
            cv::Point(x1, yr1),
            cv::Point(x2, yr2),
            red, thickness, line_type, shift);
    }

    cv::imshow("Source image", src);
    cv::imshow("calcHist Demo", histImage);
    cv::waitKey();
}