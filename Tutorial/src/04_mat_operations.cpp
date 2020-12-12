#include "tutorial.hpp"

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>


constexpr size_t CV_BLUE = 0;
constexpr size_t CV_GREEN = 1;
constexpr size_t CV_RED = 2;


typedef struct
{
    uchar r;
    uchar g;
    uchar b;
} rgb_t;




constexpr rgb_t RED = { 255, 0, 0 };
constexpr rgb_t GREEN = { 0, 255, 0 };
constexpr rgb_t BLUE = { 0, 0, 255 };
constexpr rgb_t BLACK = { 0, 0, 0 };
constexpr rgb_t WHITE = { 255, 255, 255 };

constexpr auto IMAGE_PATH = "D:/repos/OpenCV/Tutorial/in_files/corvette.jpg";


cv::Vec3b make_pixel(rgb_t const& rgb)
{
    return cv::Vec3b(rgb.b, rgb.g, rgb.r);
}


void set_color(cv::Vec3b& p, rgb_t const& rgb)
{
    p.val[CV_BLUE] = rgb.b;
    p.val[CV_GREEN] = rgb.g;
    p.val[CV_RED] = rgb.r;
}


void mat_operations()
{
    auto image_path = "./in_files/corvette.jpg";

    cv::Mat img_color = cv::imread(image_path, cv::IMREAD_COLOR);
    cv::Mat img_gray = cv::imread(image_path, cv::IMREAD_GRAYSCALE);

    if (img_color.empty() || img_gray.empty())
    {
        std::cout << "Could not read the image: " << image_path << '\n';
        return;
    }

    size_t x_pos = 10;
    size_t y_pos = 15;

    cv::Vec3b pixel_color = img_color.at<cv::Vec3b>(y_pos, x_pos);
    pixel_color = img_color.at<cv::Vec3b>(cv::Point(x_pos, y_pos));
    uchar blue = pixel_color.val[CV_BLUE];
    uchar green = pixel_color.val[CV_GREEN];
    uchar red = pixel_color.val[CV_RED];

    cv::Scalar pixel_gray = img_gray.at<uchar>(y_pos, x_pos);

    // build an image
    size_t num_colors = 3;
    size_t color_height = 100;

    size_t img_width = 600;
    size_t img_height = num_colors * color_height;
    
    auto num_pixels = img_width * img_height;

    std::vector<cv::Vec3b> pixels;
    pixels.reserve(num_pixels);


    auto const add_pixels = [&](rgb_t const& c)
    {
        auto p = make_pixel(c);

        for (size_t i = 0; i < num_pixels / num_colors; ++i)
            pixels.push_back(p);
    };

    std::vector<rgb_t> colors{ BLUE, GREEN, RED };

    std::for_each(colors.begin(), colors.end(), add_pixels);

    auto num_rows = img_height;

    cv::Mat custom_img = cv::Mat(pixels).reshape(num_colors, num_rows);    

    cv::imshow("Horizontal", custom_img);

    // modify vector
    for (auto& p : pixels)
    {
        set_color(p, BLACK);
    }

    cv::imshow("Black", custom_img);

    // modify Mat
    for (auto y = 0; y < custom_img.rows; ++y)
    {
        for (auto x = 0; x < custom_img.cols; ++x)
        {
            set_color(custom_img.at<cv::Vec3b>(y, x), WHITE);
        }
    }

    cv::imshow("White", custom_img);


    auto const red_val = [&](auto x)
    {        
        return x >= (img_width * 2 / 3) ? 255 : 0;
    };

    auto const green_val = [&](auto x)
    {
        return x >= (img_width * 1 / 3) && x < (img_width * 2 / 3) ? 255 : 0;
    };

    auto const blue_val = [&](auto x)
    {
        return x < (img_width * 1 / 3) ? 255 : 0;
    };


    rgb_t new_color;
    for (auto y = 0; y < custom_img.rows; ++y)
    {
        for (auto x = 0; x < custom_img.cols; ++x)
        {
            new_color.r = red_val(x);
            new_color.g = green_val(x);
            new_color.b = blue_val(x);
            set_color(custom_img.at<cv::Vec3b>(y, x), new_color);
        }
    }

    cv::imshow("Vertical", custom_img);


    cv::Mat color_img = cv::imread(IMAGE_PATH, cv::IMREAD_COLOR);
    if (color_img.empty())
    {
        std::cout << "Could not read the image: " << IMAGE_PATH << '\n';
        return;
    }

    cv::imshow("Color", color_img);

    cv::Mat gray_img;
    cv::cvtColor(color_img, gray_img, cv::COLOR_BGR2GRAY);

    cv::imshow("Gray", gray_img);


    cv::Mat sobelx;
    cv::Sobel(gray_img, sobelx, CV_32F, 1, 0);
    double minVal, maxVal;
    cv::minMaxLoc(sobelx, &minVal, &maxVal); //find minimum and maximum intensities
    cv::Mat draw;
    sobelx.convertTo(draw, CV_8U, 255.0 / (maxVal - minVal), -minVal * 255.0 / (maxVal - minVal));
    
    imshow("image", draw);

    cv::waitKey();
}