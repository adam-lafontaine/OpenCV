#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>

namespace threashold_09
{
    int threshold_value = 0;
    int threshold_type = 3;
    int const max_value = 255;
    int const max_type = 4;
    int const max_binary_value = 255;

    cv::Mat src, src_gray, dst;

    const char* window_name = "Threshold Demo";
    const char* trackbar_type = "Type: \n 0: Binary \n 1: Binary Inverted \n 2: Truncate \n 3: To Zero \n 4: To Zero Inverted";
    const char* trackbar_value = "Value";

    constexpr auto IMAGE_PATH = "D:/repos/OpenCV/Tutorial/in_files/tesla.jpg";


    static void Threshold_Demo(int, void*)
    {
        /* 0: Binary
         1: Binary Inverted
         2: Threshold Truncated
         3: Threshold to Zero
         4: Threshold to Zero Inverted
        */
        cv::threshold(src_gray, dst, threshold_value, max_binary_value, threshold_type);
        cv::imshow(window_name, dst);
    }


    void run()
    {
        src = cv::imread(IMAGE_PATH, cv::IMREAD_COLOR);
        if (src.empty())
        {
            std::cout << "Could not read the image: " << IMAGE_PATH << '\n';
            return;
        }

        cv::cvtColor(src, src_gray, cv::COLOR_BGR2GRAY); // Convert the image to Gray
        cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE); // Create a window to display results

        cv::createTrackbar(trackbar_type,
            window_name, &threshold_type,
            max_type, Threshold_Demo); // Create a Trackbar to choose type of Threshold

        cv::createTrackbar(trackbar_value,
            window_name, &threshold_value,
            max_value, Threshold_Demo); // Create a Trackbar to choose Threshold value

        Threshold_Demo(0, 0); // Call the function to initialize

        cv::waitKey();
    }
}


void threashold()
{
    threashold_09::run();
}