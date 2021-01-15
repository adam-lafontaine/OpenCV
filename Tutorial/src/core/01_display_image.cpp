#include "../tutorial.hpp"

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <string>

constexpr auto IMAGE_PATH = "D:/repos/OpenCV/Tutorial/in_files/corvette.jpg";

void display_image()
{
    cv::Mat img = cv::imread(IMAGE_PATH, cv::IMREAD_COLOR);
    if (img.empty())
    {
        std::cout << "Could not read the image: " << IMAGE_PATH << '\n';
        return;
    }

    cv::imshow("Display window", img);

    int k = cv::waitKey(0); // Wait for a keystroke in the window

    if (k == 's')
    {
        cv::imwrite("./out_files/corvette.png", img);
    }
}