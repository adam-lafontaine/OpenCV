#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>

constexpr auto IMAGE_PATH = "D:/repos/OpenCV/Tutorial/in_files/cadillac.jpg";


void border()
{
    cv::Mat src, dst;
    int top, bottom, left, right;
    int borderType = cv::BORDER_CONSTANT;
    const char* window_name = "copyMakeBorder Demo";
    cv::RNG rng(12345);

    src = cv::imread(IMAGE_PATH, cv::IMREAD_COLOR);
    if (src.empty())
    {
        std::cout << "Could not read the image: " << IMAGE_PATH << '\n';
        return;
    }

    // Brief how-to for this program
    printf("\n \t copyMakeBorder Demo: \n");
    printf("\t -------------------- \n");
    printf(" ** Press 'c' to set the border to a random constant value \n");
    printf(" ** Press 'r' to set the border to be replicated \n");
    printf(" ** Press 'ESC' to exit the program \n");

    cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);

    // Initialize arguments for the filter
    top = (int)(0.05 * src.rows); bottom = top;
    left = (int)(0.05 * src.cols); right = left;
    for (;;)
    {
        cv::Scalar value(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        cv::copyMakeBorder(src, dst, top, bottom, left, right, borderType, value);
        cv::imshow(window_name, dst);

        char c = (char)cv::waitKey(500);
        if (c == 27)
        {
            break;
        }
        else if (c == 'c')
        {
            borderType = cv::BORDER_CONSTANT;
        }
        else if (c == 'r')
        {
            borderType = cv::BORDER_REPLICATE;
        }
    }
}