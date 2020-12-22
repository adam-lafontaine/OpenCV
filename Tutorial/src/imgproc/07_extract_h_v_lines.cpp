#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>

static void show_wait_destroy(const char* winname, cv::Mat img);

constexpr auto IMAGE_PATH = "D:/repos/OpenCV/Tutorial/in_files/music_notes.png";

void extract_h_v_lines()
{
    cv::Mat src = cv::imread(IMAGE_PATH, cv::IMREAD_COLOR);
    if (src.empty())
    {
        std::cout << "Could not read the image: " << IMAGE_PATH << '\n';
        return;
    }

    // Show source image
    cv::imshow("src", src);

    // Transform source image to gray if it is not already
    cv::Mat gray;
    if (src.channels() == 3)
    {
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    }
    else
    {
        gray = src;
    }

    // Show gray image
    show_wait_destroy("gray", gray);

    // Apply adaptiveThreshold at the bitwise_not of gray, notice the ~ symbol
    cv::Mat bw;
    cv::adaptiveThreshold(~gray, bw, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 15, -2);

    // Show binary image
    show_wait_destroy("binary", bw);

    // Create the images that will use to extract the horizontal and vertical lines
    cv::Mat horizontal = bw.clone();
    cv::Mat vertical = bw.clone();

    // Specify size on horizontal axis
    int horizontal_size = horizontal.cols / 30;

    // Create structure element for extracting horizontal lines through morphology operations
    cv::Mat horizontalStructure = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(horizontal_size, 1));

    // Apply morphology operations
    cv::erode(horizontal, horizontal, horizontalStructure, cv::Point(-1, -1));
    cv::dilate(horizontal, horizontal, horizontalStructure, cv::Point(-1, -1));

    // Show extracted horizontal lines
    show_wait_destroy("horizontal", horizontal);

    // Specify size on vertical axis
    int vertical_size = vertical.rows / 30;

    // Create structure element for extracting vertical lines through morphology operations
    cv::Mat verticalStructure = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, vertical_size));

    // Apply morphology operations
    cv::erode(vertical, vertical, verticalStructure, cv::Point(-1, -1));
    cv::dilate(vertical, vertical, verticalStructure, cv::Point(-1, -1));

    // Show extracted vertical lines
    show_wait_destroy("vertical", vertical);

    // Inverse vertical image
    cv::bitwise_not(vertical, vertical);
    show_wait_destroy("vertical_bit", vertical);

    // Extract edges and smooth image according to the logic
    // 1. extract edges
    // 2. dilate(edges)
    // 3. src.copyTo(smooth)
    // 4. blur smooth img
    // 5. smooth.copyTo(src, edges)

    // Step 1
    cv::Mat edges;
    cv::adaptiveThreshold(vertical, edges, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 3, -2);
    show_wait_destroy("edges", edges);

    // Step 2
    cv::Mat kernel = cv::Mat::ones(2, 2, CV_8UC1);
    cv::dilate(edges, edges, kernel);
    show_wait_destroy("dilate", edges);

    // Step 3
    cv::Mat smooth;
    vertical.copyTo(smooth);

    // Step 4
    cv::blur(smooth, smooth, cv::Size(2, 2));

    // Step 5
    smooth.copyTo(vertical, edges);

    // Show final result
    show_wait_destroy("smooth - final", vertical);
}


static void show_wait_destroy(const char* winname, cv::Mat img)
{
    imshow(winname, img);
    cv::moveWindow(winname, 500, 0);
    cv::waitKey(0);
    cv::destroyWindow(winname);
}