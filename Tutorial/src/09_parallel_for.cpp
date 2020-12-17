#include "./tutorial.hpp"
#include "../../utils/stopwatch.hpp"

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

#include <iostream>


int mandelbrot(const std::complex<float>& z0, const int max)
{
    std::complex<float> z = z0;
    for (int t = 0; t < max; t++)
    {
        if (z.real() * z.real() + z.imag() * z.imag() > 4.0f) return t;
        z = z * z + z0;
    }

    return max;
}


int mandelbrotFormula(const std::complex<float>& z0, const int maxIter = 500) {
    int value = mandelbrot(z0, maxIter);
    if (maxIter - value == 0)
    {
        return 0;
    }

    return cvRound(sqrt(value / (float)maxIter) * 255);
}


uchar do_mandelbrot(cv::Mat& img, float x0, float y0)
{
    std::complex<float> z0(x0, y0);
    return (uchar)mandelbrotFormula(z0);
}


void sequential(cv::Mat& img, float x1, float y1, float scaleX, float scaleY)
{
    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            float x0 = j / scaleX + x1;
            float y0 = i / scaleY + y1;

            img.ptr<uchar>(i)[j] = do_mandelbrot(img, x0, y0);
        }
    }
}


void parallel(cv::Mat& img, float x1, float y1, float scaleX, float scaleY)
{
    auto const func = [&](cv::Range const& range)
    {
        for (int r = range.start; r < range.end; r++)
        {
            int i = r / img.cols;
            int j = r % img.cols;

            float x0 = j / scaleX + x1;
            float y0 = i / scaleY + y1;

            img.ptr<uchar>(i)[j] = do_mandelbrot(img, x0, y0);
        }
    };

    cv::Range range(0, img.rows * img.cols);
    cv::parallel_for_(range, func);
}


void parallel_for()
{
    cv::Mat mandelbrotImg(4800, 5400, CV_8U);
    float x1 = -2.1f, x2 = 0.6f;
    float y1 = -1.2f, y2 = 1.2f;
    float scaleX = mandelbrotImg.cols / (x2 - x1);
    float scaleY = mandelbrotImg.rows / (y2 - y1);

    Stopwatch sw;

    sw.start();
    sequential(mandelbrotImg, x1, y1, scaleX, scaleY);

    std::cout << "Sequential: " << sw.get_time_sec() << " s\n"; // 12 sec

    sw.start();
    parallel(mandelbrotImg, x1, y1, scaleX, scaleY);
    std::cout << "Parallel: " << sw.get_time_sec() << " s\n"; // 1.9 sec
}