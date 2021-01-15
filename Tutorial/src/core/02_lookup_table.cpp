#include "../tutorial.hpp"
#include "../../../utils/reduced_lookup.hpp"
#include "../../../utils/for_each_pixel.hpp"
#include "../../../utils/stopwatch.hpp"

#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <string>

constexpr auto IMAGE_PATH = "D:/repos/OpenCV/Tutorial/in_files/corvette.jpg";



void lookup_table()
{
    cv::Mat mat_lookup = utils::make_reduced_lookup_mat(8);

    cv::Mat src = cv::imread(IMAGE_PATH, cv::IMREAD_GRAYSCALE);
    if (src.empty())
    {
        std::cout << "Could not read the image: " << IMAGE_PATH << '\n';
        return;
    }

    cv::Mat dst_lut;
    

    Stopwatch sw;
    auto n_times = 1000;

    sw.start();

    for (auto i = 0; i < n_times; ++i)
    {
        cv::LUT(src, mat_lookup, dst_lut); // 54 ms
    }    

    auto time = sw.get_time_milli();
    std::cout << "cv::LUT time = " << time << " ms\n";

    
    cv::Mat dst_stl = src.clone();
    auto const table = utils::make_reduced_lookup(8);

    sw.start();

    for (auto i = 0; i < n_times; ++i)
    {
        //utils::LUT_c(src, table, dst_stl); // 250 ms
        utils::LUT_stl(src, table, dst_stl); // 125 ms
    }

    time = sw.get_time_milli();
    std::cout << "LUT_stl time = " << time << " ms\n";

    sw.start();

    for (auto i = 0; i < n_times; ++i)
    {
        utils::LUT_itr(src, table, dst_stl); // 125 ms
    }

    time = sw.get_time_milli();
    std::cout << "LUT_itr time = " << time << " ms\n";



    /*cv::namedWindow("Input", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("LUT", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("STL", cv::WINDOW_AUTOSIZE);

    cv::imshow("Input", src);
    cv::imshow("LUT", dst_lut);
    cv::imshow("STL", dst_stl);*/

    cv::waitKey();
}