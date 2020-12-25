#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>



void filter2d()
{
    // Declare variables
    cv::Mat src, dst;
    cv::Mat kernel;
    cv::Point anchor;

    double delta;
    int ddepth;
    int kernel_size;

    const char* window_name = "filter2D Demo";

    constexpr auto IMAGE_PATH = "D:/repos/OpenCV/Tutorial/in_files/tesla.jpg";

    // Loads an image
    src = cv::imread(IMAGE_PATH, cv::IMREAD_COLOR);
    if (src.empty())
    {
        std::cout << "Could not read the image: " << IMAGE_PATH << '\n';
        return;
    }

    // Initialize arguments for the filter
    anchor = cv::Point(-1, -1);
    delta = 0;
    ddepth = -1;

    // Loop - Will filter the image with different kernel sizes each 0.5 seconds    
    for (int ind = 0;; ++ind)
    {
        // Update kernel size for a normalized box filter
        kernel_size = 3 + 2 * (ind % 5);
        kernel = cv::Mat::ones(kernel_size, kernel_size, CV_32F) / (float)(kernel_size * kernel_size);

        // Apply filter
        cv::filter2D(src, dst, ddepth, kernel, anchor, delta, cv::BORDER_DEFAULT);
        cv::imshow(window_name, dst);

        char c = (char)cv::waitKey(500);
        // Press 'ESC' to exit the program
        if (c == 27)
        {
            break;
        }
    }
    
}