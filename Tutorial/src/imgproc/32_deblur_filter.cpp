#include <iostream>

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui.hpp>

constexpr auto IMAGE_PATH = "D:/repos/OpenCV/Tutorial/in_files/corvette.jpg";


namespace deblur_filter32
{
    void calcPSF(cv::Mat& outputImg, cv::Size filterSize, int R);
    void fftshift(const cv::Mat& inputImg, cv::Mat& outputImg);
    void filter2DFreq(const cv::Mat& inputImg, cv::Mat& outputImg, const cv::Mat& H);
    void calcWnrFilter(const cv::Mat& input_h_PSF, cv::Mat& output_G, double nsr);

    void run()
    {
        cv::Mat imgIn = cv::imread(IMAGE_PATH, cv::IMREAD_GRAYSCALE);
        if (imgIn.empty())
        {
            std::cout << "Could not read the image: " << IMAGE_PATH << '\n';
            return;
        }

        int R = 53;
        int snr = 5200;

        cv::Mat imgOut;

        // it needs to process even image only
        cv::Rect roi = cv::Rect(0, 0, imgIn.cols & -2, imgIn.rows & -2);

        //Hw calculation (start)
        cv::Mat Hw, h;
        calcPSF(h, roi.size(), R);
        calcWnrFilter(h, Hw, 1.0 / double(snr));

        //Hw calculation (stop)
        // filtering (start)
        filter2DFreq(imgIn(roi), imgOut, Hw);

        // filtering (stop)
        imgOut.convertTo(imgOut, CV_8U);
        cv::normalize(imgOut, imgOut, 0, 255, cv::NORM_MINMAX);
        cv::imshow("result", imgOut);

        cv::waitKey();
    }


    void calcPSF(cv::Mat& outputImg, cv::Size filterSize, int R)
    {
        cv::Mat h(filterSize, CV_32F, cv::Scalar(0));
        cv::Point point(filterSize.width / 2, filterSize.height / 2);
        circle(h, point, R, 255, -1, 8);
        cv::Scalar summa = sum(h);
        outputImg = h / summa[0];
    }


    void fftshift(const cv::Mat& inputImg, cv::Mat& outputImg)
    {
        outputImg = inputImg.clone();
        int cx = outputImg.cols / 2;
        int cy = outputImg.rows / 2;
        cv::Mat q0(outputImg, cv::Rect(0, 0, cx, cy));
        cv::Mat q1(outputImg, cv::Rect(cx, 0, cx, cy));
        cv::Mat q2(outputImg, cv::Rect(0, cy, cx, cy));
        cv::Mat q3(outputImg, cv::Rect(cx, cy, cx, cy));
        cv::Mat tmp;
        q0.copyTo(tmp);
        q3.copyTo(q0);
        tmp.copyTo(q3);
        q1.copyTo(tmp);
        q2.copyTo(q1);
        tmp.copyTo(q2);
    }


    void filter2DFreq(const cv::Mat& inputImg, cv::Mat& outputImg, const cv::Mat& H)
    {
        cv::Mat planes[2] = { cv::Mat_<float>(inputImg.clone()), cv::Mat::zeros(inputImg.size(), CV_32F) };
        cv::Mat complexI;
        cv::merge(planes, 2, complexI);
        cv::dft(complexI, complexI, cv::DFT_SCALE);
        cv::Mat planesH[2] = { cv::Mat_<float>(H.clone()), cv::Mat::zeros(H.size(), CV_32F) };
        cv::Mat complexH;
        cv::merge(planesH, 2, complexH);
        cv::Mat complexIH;
        cv::mulSpectrums(complexI, complexH, complexIH, 0);
        cv::idft(complexIH, complexIH);
        cv::split(complexIH, planes);
        outputImg = planes[0];
    }


    void calcWnrFilter(const cv::Mat& input_h_PSF, cv::Mat& output_G, double nsr)
    {
        cv::Mat h_PSF_shifted;
        fftshift(input_h_PSF, h_PSF_shifted);
        cv::Mat planes[2] = { cv::Mat_<float>(h_PSF_shifted.clone()), cv::Mat::zeros(h_PSF_shifted.size(), CV_32F) };
        cv::Mat complexI;
        cv::merge(planes, 2, complexI);
        cv::dft(complexI, complexI);
        cv::split(complexI, planes);
        cv::Mat denom;
        cv::pow(cv::abs(planes[0]), 2, denom);
        denom += nsr;
        cv::divide(planes[0], denom, output_G);
    }
}


void deblur_filter()
{
    deblur_filter32::run();
}