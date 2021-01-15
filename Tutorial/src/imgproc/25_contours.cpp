#include "../../../utils/video_test.hpp"

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <vector>


constexpr auto IMAGE_PATH = "D:/repos/OpenCV/Tutorial/in_files/tesla.jpg";


namespace find_contours
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
        cv::Canny(src_gray, canny_output, thresh, thresh * 2);

        std::vector<std::vector<cv::Point> > contours;
        std::vector<cv::Vec4i> hierarchy;

        cv::findContours(canny_output, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

        cv::Mat drawing = cv::Mat::zeros(canny_output.size(), CV_8UC3);
        for (size_t i = 0; i < contours.size(); i++)
        {
            cv::Scalar color = cv::Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
            cv::drawContours(drawing, contours, (int)i, color, 2, cv::LINE_8, hierarchy, 0);
        }

        cv::imshow("Contours", drawing);
    }
}


void contours()
{
    find_contours::run();
}


void contour_video()
{
    cv::Mat src_gray;
    cv::Mat detected_edges;
    const int ratio = 2;
    const int kernel_size = 3;
    cv::RNG rng(12345);

    auto const line_thickness = 2;
    auto const line_type = cv::LINE_8;

    auto const random_color = [&]() { return cv::Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256)); };
    auto const random_threshold = [&]() { return static_cast<double>(rng.uniform(30, 100)); };    

    auto const canny_func = [&](cv::Mat& src, cv::Mat& dst)
    {
        auto const thresh = random_threshold();

        cv::GaussianBlur(src, src, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);
        cv::cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);
        cv::Canny(src_gray, detected_edges, thresh, thresh * ratio, kernel_size);

        std::vector<std::vector<cv::Point> > contours;
        std::vector<cv::Vec4i> hierarchy;

        cv::findContours(detected_edges, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

        dst = cv::Mat::zeros(detected_edges.size(), CV_8UC3);
        for (size_t i = 0; i < contours.size(); i++)
        {
            cv::Scalar color = random_color();
            cv::drawContours(dst, contours, (int)i, color, line_thickness, line_type, hierarchy, 0);
        }
    };

    video_test::process_capture(canny_func);
}