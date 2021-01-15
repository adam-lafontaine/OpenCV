#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <iomanip>


constexpr auto IMAGE_PATH = "D:/repos/OpenCV/Tutorial/in_files/corvette.jpg";

namespace moments29
{
    cv::Mat src_gray;
    int thresh = 100;
    cv::RNG rng(12345);

    void thresh_callback(int, void*);

    void run()
    {
        cv::Mat src = cv::imread(IMAGE_PATH, cv::IMREAD_COLOR);
        if (src.empty())
        {
            std::cout << "Could not read the image: " << IMAGE_PATH << '\n';
            return;
        }

        cv::cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);
        cv::blur(src_gray, src_gray, cv::Size(3, 3));

        const char* source_window = "Source";
        cv::namedWindow(source_window);
        cv::imshow(source_window, src);

        const int max_thresh = 255;
        cv::createTrackbar("Canny thresh:", source_window, &thresh, max_thresh, thresh_callback);

        thresh_callback(0, 0);

        cv::waitKey();
    }


    void thresh_callback(int, void*)
    {
        cv::Mat canny_output;
        cv::Canny(src_gray, canny_output, thresh, thresh * 2, 3);

        std::vector<std::vector<cv::Point> > contours;
        cv::findContours(canny_output, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

        std::vector<cv::Moments> mu(contours.size());
        for (size_t i = 0; i < contours.size(); i++)
        {
            mu[i] = cv::moments(contours[i]);
        }

        std::vector<cv::Point2f> mc(contours.size());
        for (size_t i = 0; i < contours.size(); i++)
        {
            //add 1e-5 to avoid division by zero
            mc[i] = cv::Point2f(static_cast<float>(mu[i].m10 / (mu[i].m00 + 1e-5)),
                static_cast<float>(mu[i].m01 / (mu[i].m00 + 1e-5)));

            std::cout << "mc[" << i << "]=" << mc[i] << '\n';
        }

        cv::Mat drawing = cv::Mat::zeros(canny_output.size(), CV_8UC3);
        for (size_t i = 0; i < contours.size(); i++)
        {
            cv::Scalar color = cv::Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
            cv::drawContours(drawing, contours, (int)i, color, 2);
            cv::circle(drawing, mc[i], 4, color, -1);
        }

        cv::imshow("Contours", drawing);

        std::cout << "\t Info: Area and Contour Length \n";
        for (size_t i = 0; i < contours.size(); i++)
        {
            std::cout << " * Contour[" << i << "] - Area (M_00) = " << std::fixed << std::setprecision(2) << mu[i].m00
                << " - Area OpenCV: " << cv::contourArea(contours[i]) << " - Length: " << cv::arcLength(contours[i], true) << '\n';
        }
    }
}


void moments()
{
    moments29::run();
}