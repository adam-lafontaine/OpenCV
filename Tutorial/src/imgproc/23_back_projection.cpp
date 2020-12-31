#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>


constexpr auto IMAGE_PATH = "D:/repos/OpenCV/Tutorial/in_files/mclaren.jpg";

cv::Mat hue;
int bins = 25;

void Hist_and_Backproj(int, void*);


void back_projection()
{
    cv::Mat src = cv::imread(IMAGE_PATH, cv::IMREAD_COLOR);
    if (src.empty())
    {
        std::cout << "Could not read the image: " << IMAGE_PATH << '\n';
        return;
    }

    cv::Mat hsv;
    cvtColor(src, hsv, cv::COLOR_BGR2HSV);

    hue.create(hsv.size(), hsv.depth());

    int ch[] = { 0, 0 };
    cv::mixChannels(&hsv, 1, &hue, 1, ch, 1);

    const char* window_image = "Source image";
    cv::namedWindow(window_image);

    cv::createTrackbar("* Hue  bins: ", window_image, &bins, 180, Hist_and_Backproj);

    Hist_and_Backproj(0, 0);

    cv::imshow(window_image, src);
    // Wait until user exits the program
    cv::waitKey();
}



void Hist_and_Backproj(int, void*)
{
    int histSize = MAX(bins, 2);
    float hue_range[] = { 0, 180 };
    const float* ranges = { hue_range };

    cv::Mat hist;
    cv::calcHist(&hue, 1, 0, cv::Mat(), hist, 1, &histSize, &ranges, true, false);
    cv::normalize(hist, hist, 0, 255, cv::NORM_MINMAX, -1, cv::Mat());

    cv::Mat backproj;
    cv::calcBackProject(&hue, 1, 0, hist, backproj, &ranges, 1, true);

    cv::imshow("BackProj", backproj);

    int w = 400, h = 400;
    int bin_w = cvRound((double)w / histSize);

    cv::Mat histImg = cv::Mat::zeros(h, w, CV_8UC3);
    for (int i = 0; i < bins; i++)
    {
        rectangle(histImg, cv::Point(i * bin_w, h), cv::Point((i + 1) * bin_w, h - cvRound(hist.at<float>(i) * h / 255.0)),
            cv::Scalar(0, 0, 255), cv::FILLED);
    }
    imshow("Histogram", histImg);
}