#include "../tutorial.hpp"

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>

constexpr auto IMAGE_1 = "D:/repos/OpenCV/Tutorial/in_files/corvette.jpg";


void discrete_fourier_transform()
{
    cv::Mat img_1 = cv::imread(IMAGE_1, cv::IMREAD_COLOR);
    if (img_1.empty())
    {
        std::cout << "Could not read the image()s/n";
        return;
    }

    cv::Mat padded;                            //expand input image to optimal size
    int m = cv::getOptimalDFTSize(img_1.rows);
    int n = cv::getOptimalDFTSize(img_1.cols); // on the border add zero values
    cv::copyMakeBorder(img_1, padded, 0, m - img_1.rows, 0, n - img_1.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

    cv::Mat planes[] = { cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32F) }; // THIS LINE CRASHES!
    cv::Mat complexI;
    cv::merge(planes, 2, complexI);         // Add to the expanded another plane with zeros

    cv::dft(complexI, complexI);            // this way the result may fit in the source matrix

    cv::split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    cv::magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
    cv::Mat magI = planes[0];

    magI += cv::Scalar::all(1);                    // switch to logarithmic scale
    cv::log(magI, magI);

    // crop the spectrum, if it has an odd number of rows or columns
    magI = magI(cv::Rect(0, 0, magI.cols & -2, magI.rows & -2));

    // rearrange the quadrants of Fourier image  so that the origin is at the image center
    int cx = magI.cols / 2;
    int cy = magI.rows / 2;
    cv::Mat q0(magI, cv::Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    cv::Mat q1(magI, cv::Rect(cx, 0, cx, cy));  // Top-Right
    cv::Mat q2(magI, cv::Rect(0, cy, cx, cy));  // Bottom-Left
    cv::Mat q3(magI, cv::Rect(cx, cy, cx, cy)); // Bottom-Right
    cv::Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);

    cv::normalize(magI, magI, 0, 1, cv::NORM_MINMAX); // Transform the matrix with float values into a
                                            // viewable image form (float between values 0 and 1).

    cv::imshow("Converted", magI);

    cv::waitKey();
}