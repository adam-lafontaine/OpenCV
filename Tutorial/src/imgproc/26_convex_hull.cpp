#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <vector>

namespace cvx_hull
{
    constexpr auto IMAGE_PATH = "D:/repos/OpenCV/Tutorial/in_files/mustang.jpg";


    cv::Mat src_gray;
    int thresh = 100;
    cv::RNG rng(12345);

    void thresh_callback(int, void*);
    cv::Scalar random_color();


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
        cv::Canny(src_gray, canny_output, thresh, thresh * 2);

        std::vector<std::vector<cv::Point> > contours;
        cv::findContours(canny_output, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

        std::vector<std::vector<cv::Point> >hull(contours.size());
        for (size_t i = 0; i < contours.size(); i++)
        {
            cv::convexHull(contours[i], hull[i]);
        }

        cv::Mat drawing = cv::Mat::zeros(canny_output.size(), CV_8UC3);
        for (size_t i = 0; i < contours.size(); i++)
        {
            auto color = random_color();
            cv::drawContours(drawing, contours, (int)i, color);
            cv::drawContours(drawing, hull, (int)i, color);
        }

        cv::imshow("Hull demo", drawing);
    }


    cv::Scalar random_color()
    {
        return cv::Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
    }

}


void convex_hull()
{
    cvx_hull::run();
}

