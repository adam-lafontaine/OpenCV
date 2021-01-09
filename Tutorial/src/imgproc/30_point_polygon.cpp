#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>


void point_polygon()
{
    const int r = 100;
    cv::Mat src = cv::Mat::zeros(cv::Size(4 * r, 4 * r), CV_8U);

    std::vector<cv::Point2f> vert(6);
    vert[0] = cv::Point(3 * r / 2, static_cast<int>(1.34 * r));
    vert[1] = cv::Point(1 * r, 2 * r);
    vert[2] = cv::Point(3 * r / 2, static_cast<int>(2.866 * r));
    vert[3] = cv::Point(5 * r / 2, static_cast<int>(2.866 * r));
    vert[4] = cv::Point(3 * r, 2 * r);
    vert[5] = cv::Point(5 * r / 2, static_cast<int>(1.34 * r));

    for (int i = 0; i < 6; i++)
    {
        line(src, vert[i], vert[(i + 1) % 6], cv::Scalar(255), 3);
    }

    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(src, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    cv::Mat raw_dist(src.size(), CV_32F);
    for (int i = 0; i < src.rows; i++)
    {
        for (int j = 0; j < src.cols; j++)
        {
            raw_dist.at<float>(i, j) = (float)pointPolygonTest(contours[0], cv::Point2f((float)j, (float)i), true);
        }
    }

    double minVal, maxVal;
    cv::Point maxDistPt; // inscribed circle center
    cv::minMaxLoc(raw_dist, &minVal, &maxVal, NULL, &maxDistPt);
    minVal = abs(minVal);
    maxVal = abs(maxVal);

    cv::Mat drawing = cv::Mat::zeros(src.size(), CV_8UC3);
    for (int i = 0; i < src.rows; i++)
    {
        for (int j = 0; j < src.cols; j++)
        {
            if (raw_dist.at<float>(i, j) < 0)
            {
                drawing.at<cv::Vec3b>(i, j)[0] = (uchar)(255 - abs(raw_dist.at<float>(i, j)) * 255 / minVal);
            }
            else if (raw_dist.at<float>(i, j) > 0)
            {
                drawing.at<cv::Vec3b>(i, j)[2] = (uchar)(255 - raw_dist.at<float>(i, j) * 255 / maxVal);
            }
            else
            {
                drawing.at<cv::Vec3b>(i, j)[0] = 255;
                drawing.at<cv::Vec3b>(i, j)[1] = 255;
                drawing.at<cv::Vec3b>(i, j)[2] = 255;
            }
        }
    }

    cv::circle(drawing, maxDistPt, (int)maxVal, cv::Scalar(255, 255, 255));

    cv::imshow("Source", src);
    cv::imshow("Distance and inscribed circle", drawing);

    cv::waitKey();
}