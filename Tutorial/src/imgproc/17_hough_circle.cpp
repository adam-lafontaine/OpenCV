#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <vector>

constexpr auto IMAGE_PATH = "D:/repos/OpenCV/Tutorial/in_files/mclaren.jpg";

void hough_circle()
{
    // Loads an image
    cv::Mat src = cv::imread(IMAGE_PATH, cv::IMREAD_COLOR);
    if (src.empty())
    {
        std::cout << "Could not read the image: " << IMAGE_PATH << '\n';
        return;
    }

    cv::Mat gray;
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    cv::medianBlur(gray, gray, 5);

    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(gray, circles, cv::HOUGH_GRADIENT, 1,
        gray.rows / 16,  // change this value to detect circles with different distances to each other
        100, 30, 1, 30 // change the last two parameters
   // (min_radius & max_radius) to detect larger circles
    );

    for (size_t i = 0; i < circles.size(); i++)
    {
        cv::Vec3i c = circles[i];
        cv::Point center = cv::Point(c[0], c[1]);
        // circle center
        circle(src, center, 1, cv::Scalar(0, 100, 100), 3, cv::LINE_AA);
        // circle outline
        int radius = c[2];
        circle(src, center, radius, cv::Scalar(255, 0, 255), 3, cv::LINE_AA);
    }

    cv::imshow("detected circles", src);
    cv::waitKey();
}