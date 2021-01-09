#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>

using namespace cv;
using namespace std;


namespace bounding28
{
    constexpr auto IMAGE_PATH = "D:/repos/OpenCV/Tutorial/in_files/corvette.jpg";

    Mat src_gray;
    int thresh = 100;
    RNG rng(12345);


    void thresh_callback(int, void*);


    void run()
    {
        cv::Mat src = cv::imread(IMAGE_PATH, cv::IMREAD_COLOR);
        if (src.empty())
        {
            std::cout << "Could not read the image: " << IMAGE_PATH << '\n';
            return;
        }

        cvtColor(src, src_gray, COLOR_BGR2GRAY);
        blur(src_gray, src_gray, Size(3, 3));

        const char* source_window = "Source";
        namedWindow(source_window);
        imshow(source_window, src);

        const int max_thresh = 255;
        createTrackbar("Canny thresh:", source_window, &thresh, max_thresh, thresh_callback);

        thresh_callback(0, 0);
        waitKey();
    }


    void thresh_callback(int, void*)
    {
        Mat canny_output;
        Canny(src_gray, canny_output, thresh, thresh * 2);

        vector<vector<Point> > contours;
        findContours(canny_output, contours, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

        vector<RotatedRect> minRect(contours.size());
        vector<RotatedRect> minEllipse(contours.size());
        for (size_t i = 0; i < contours.size(); i++)
        {
            minRect[i] = minAreaRect(contours[i]);
            if (contours[i].size() > 5)
            {
                minEllipse[i] = fitEllipse(contours[i]);
            }
        }

        Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
        for (size_t i = 0; i < contours.size(); i++)
        {
            Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));

            // contour
            drawContours(drawing, contours, (int)i, color);

            // ellipse
            ellipse(drawing, minEllipse[i], color, 2);

            // rotated rectangle
            Point2f rect_points[4];
            minRect[i].points(rect_points);
            for (int j = 0; j < 4; j++)
            {
                line(drawing, rect_points[j], rect_points[(j + 1) % 4], color);
            }
        }

        imshow("Contours", drawing);
    }
}


void bounding_rotated()
{
    bounding28::run();
}