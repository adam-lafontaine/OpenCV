#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>

int erosion_elem = 0;
int erosion_size = 0;
int dilation_elem = 0;
int dilation_size = 0;
int const max_elem = 2;
int const max_kernel_size = 21;

constexpr auto IMAGE_PATH = "D:/repos/OpenCV/Tutorial/in_files/corvette.jpg";

constexpr auto EROSION_WINDOW = "Erosion Demo";
constexpr auto DILATION_WINDOW = "Dilation Demo";

cv::Mat src, erosion_dst, dilation_dst;

void Erosion(int i = 0, void* ptr = nullptr);  // void* circa 1999
void Dilation(int i = 0, void* ptr = nullptr);

void erode_dilate()
{
    src = cv::imread(IMAGE_PATH, cv::IMREAD_COLOR);
    if (src.empty())
    {
        std::cout << "Could not read the image: " << IMAGE_PATH << '\n';
        return;
    }

    cv::namedWindow(EROSION_WINDOW, cv::WINDOW_AUTOSIZE);
    cv::namedWindow(DILATION_WINDOW, cv::WINDOW_AUTOSIZE);
    cv::moveWindow(DILATION_WINDOW, src.cols, 0);

    cv::createTrackbar("Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Erosion Demo",
        &erosion_elem, max_elem,
        Erosion);

    cv::createTrackbar("Kernel size:\n 2n +1", "Erosion Demo",
        &erosion_size, max_kernel_size,
        Erosion);

    cv::createTrackbar("Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Dilation Demo",
        &dilation_elem, max_elem,
        Dilation);

    cv::createTrackbar("Kernel size:\n 2n +1", "Dilation Demo",
        &dilation_size, max_kernel_size,
        Dilation);

    Erosion();
    Dilation();

    cv::waitKey();
}


void Erosion(int, void*)
{
    int erosion_type = 0;

    if (erosion_elem == 0) { erosion_type = cv::MORPH_RECT; }
    else if (erosion_elem == 1) { erosion_type = cv::MORPH_CROSS; }
    else if (erosion_elem == 2) { erosion_type = cv::MORPH_ELLIPSE; }

    cv::Mat element = cv::getStructuringElement(erosion_type,
        cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
        cv::Point(erosion_size, erosion_size));
    cv::erode(src, erosion_dst, element);

    cv::imshow(EROSION_WINDOW, erosion_dst);
}


void Dilation(int, void*)
{
    int dilation_type = 0;

    if (dilation_elem == 0) { dilation_type = cv::MORPH_RECT; }
    else if (dilation_elem == 1) { dilation_type = cv::MORPH_CROSS; }
    else if (dilation_elem == 2) { dilation_type = cv::MORPH_ELLIPSE; }

    cv::Mat element = cv::getStructuringElement(dilation_type,
        cv::Size(2 * dilation_size + 1, 2 * dilation_size + 1),
        cv::Point(dilation_size, dilation_size));

    cv::dilate(src, dilation_dst, element);
    cv::imshow(DILATION_WINDOW, dilation_dst);
}