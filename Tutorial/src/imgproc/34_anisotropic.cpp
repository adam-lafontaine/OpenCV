#include <iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui.hpp>

constexpr auto IMAGE_PATH = "D:/repos/OpenCV/Tutorial/in_files/corvette.jpg";

namespace anisotropic33
{
    void calcGST(const cv::Mat& inputImg, cv::Mat& imgCoherencyOut, cv::Mat& imgOrientationOut, int w);

    void run()
    {
        int W = 52;             // window size is WxW
        double C_Thr = 0.43;    // threshold for coherency
        int LowThr = 35;        // threshold1 for orientation, it ranges from 0 to 180
        int HighThr = 57;       // threshold2 for orientation, it ranges from 0 to 180

        cv::Mat imgIn = cv::imread(IMAGE_PATH, cv::IMREAD_GRAYSCALE);
        if (imgIn.empty())
        {
            std::cout << "Could not read the image: " << IMAGE_PATH << '\n';
            return;
        }

        cv::Mat imgCoherency, imgOrientation;
        calcGST(imgIn, imgCoherency, imgOrientation, W);

        cv::Mat imgCoherencyBin;
        imgCoherencyBin = imgCoherency > C_Thr;

        cv::Mat imgOrientationBin;
        cv::inRange(imgOrientation, cv::Scalar(LowThr), cv::Scalar(HighThr), imgOrientationBin);

        cv::Mat imgBin;
        imgBin = imgCoherencyBin & imgOrientationBin;

        cv::normalize(imgCoherency, imgCoherency, 0, 255, cv::NORM_MINMAX);
        cv::normalize(imgOrientation, imgOrientation, 0, 255, cv::NORM_MINMAX);

        cv::imshow("result.jpg", 0.5 * (imgIn + imgBin));
        cv::imshow("Coherency.jpg", imgCoherency);
        cv::imshow("Orientation.jpg", imgOrientation);

        cv::waitKey();
    }


    void calcGST(const cv::Mat& inputImg, cv::Mat& imgCoherencyOut, cv::Mat& imgOrientationOut, int w)
    {
        cv::Mat img;
        inputImg.convertTo(img, CV_32F);

        // GST components calculation (start)
        // J =  (J11 J12; J12 J22) - GST
        cv::Mat imgDiffX, imgDiffY, imgDiffXY;
        cv::Sobel(img, imgDiffX, CV_32F, 1, 0, 3);
        cv::Sobel(img, imgDiffY, CV_32F, 0, 1, 3);
        cv::multiply(imgDiffX, imgDiffY, imgDiffXY);

        cv::Mat imgDiffXX, imgDiffYY;
        cv::multiply(imgDiffX, imgDiffX, imgDiffXX);
        cv::multiply(imgDiffY, imgDiffY, imgDiffYY);

        cv::Mat J11, J22, J12;      // J11, J22 and J12 are GST components
        cv::boxFilter(imgDiffXX, J11, CV_32F, cv::Size(w, w));
        cv::boxFilter(imgDiffYY, J22, CV_32F, cv::Size(w, w));
        cv::boxFilter(imgDiffXY, J12, CV_32F, cv::Size(w, w));

        // GST components calculation (stop)
        // eigenvalue calculation (start)
        // lambda1 = 0.5*(J11 + J22 + sqrt((J11-J22)^2 + 4*J12^2))
        // lambda2 = 0.5*(J11 + J22 - sqrt((J11-J22)^2 + 4*J12^2))
        cv::Mat tmp1, tmp2, tmp3, tmp4;
        tmp1 = J11 + J22;
        tmp2 = J11 - J22;
        cv::multiply(tmp2, tmp2, tmp2);
        cv::multiply(J12, J12, tmp3);
        cv::sqrt(tmp2 + 4.0 * tmp3, tmp4);

        cv::Mat lambda1, lambda2;
        lambda1 = tmp1 + tmp4;
        lambda1 = 0.5 * lambda1;      // biggest eigenvalue
        lambda2 = tmp1 - tmp4;
        lambda2 = 0.5 * lambda2;      // smallest eigenvalue

        // eigenvalue calculation (stop)
        // Coherency calculation (start)
        // Coherency = (lambda1 - lambda2)/(lambda1 + lambda2)) - measure of anisotropism
        // Coherency is anisotropy degree (consistency of local orientation)
        cv::divide(lambda1 - lambda2, lambda1 + lambda2, imgCoherencyOut);

        // Coherency calculation (stop)
        // orientation angle calculation (start)
        // tan(2*Alpha) = 2*J12/(J22 - J11)
        // Alpha = 0.5 atan2(2*J12/(J22 - J11))
        cv::phase(J22 - J11, 2.0 * J12, imgOrientationOut, true);
        imgOrientationOut = 0.5 * imgOrientationOut;
        // orientation angle calculation (stop)
    }
}


void anisotropic()
{
    anisotropic33::run();
}