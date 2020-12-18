#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

constexpr unsigned w = 400;

using namespace cv;

void MyEllipse(cv::Mat img, double angle);
void MyFilledCircle(cv::Mat img, cv::Point center);
void MyPolygon(cv::Mat img);
void MyLine(cv::Mat img, cv::Point start, cv::Point end);


void basic_draw()
{
    char atom_window[] = "Drawing 1: Atom";
    char rook_window[] = "Drawing 2: Rook";
    cv::Mat atom_image = cv::Mat::zeros(w, w, CV_8UC3);
    cv::Mat rook_image = cv::Mat::zeros(w, w, CV_8UC3);

    MyEllipse(atom_image, 90);
    MyEllipse(atom_image, 0);
    MyEllipse(atom_image, 45);
    MyEllipse(atom_image, -45);

    MyFilledCircle(atom_image, cv::Point(w / 2, w / 2));

    MyPolygon(rook_image);

    rectangle(rook_image,
        cv::Point(0, 7 * w / 8),
        cv::Point(w, w),
        Scalar(0, 255, 255),
        FILLED,
        LINE_8);

    MyLine(rook_image, cv::Point(0, 15 * w / 16), cv::Point(w, 15 * w / 16));
    MyLine(rook_image, cv::Point(w / 4, 7 * w / 8), cv::Point(w / 4, w));
    MyLine(rook_image, cv::Point(w / 2, 7 * w / 8), cv::Point(w / 2, w));
    MyLine(rook_image, cv::Point(3 * w / 4, 7 * w / 8), cv::Point(3 * w / 4, w));

    imshow(atom_window, atom_image);
    moveWindow(atom_window, 0, 200);
    imshow(rook_window, rook_image);
    moveWindow(rook_window, w, 200);

    waitKey(0);
}


void MyEllipse(cv::Mat img, double angle)
{
    int thickness = 2;
    int lineType = 8;
    ellipse(img,
        cv::Point(w / 2, w / 2),
        Size(w / 4, w / 16),
        angle,
        0,
        360,
        Scalar(255, 0, 0),
        thickness,
        lineType);
}


void MyFilledCircle(cv::Mat img, cv::Point center)
{
    circle(img,
        center,
        w / 32,
        Scalar(0, 0, 255),
        FILLED,
        LINE_8);
}


void MyPolygon(cv::Mat img)
{
    int lineType = LINE_8;
    cv::Point rook_points[1][20];
    rook_points[0][0] = cv::Point(w / 4, 7 * w / 8);
    rook_points[0][1] = cv::Point(3 * w / 4, 7 * w / 8);
    rook_points[0][2] = cv::Point(3 * w / 4, 13 * w / 16);
    rook_points[0][3] = cv::Point(11 * w / 16, 13 * w / 16);
    rook_points[0][4] = cv::Point(19 * w / 32, 3 * w / 8);
    rook_points[0][5] = cv::Point(3 * w / 4, 3 * w / 8);
    rook_points[0][6] = cv::Point(3 * w / 4, w / 8);
    rook_points[0][7] = cv::Point(26 * w / 40, w / 8);
    rook_points[0][8] = cv::Point(26 * w / 40, w / 4);
    rook_points[0][9] = cv::Point(22 * w / 40, w / 4);
    rook_points[0][10] = cv::Point(22 * w / 40, w / 8);
    rook_points[0][11] = cv::Point(18 * w / 40, w / 8);
    rook_points[0][12] = cv::Point(18 * w / 40, w / 4);
    rook_points[0][13] = cv::Point(14 * w / 40, w / 4);
    rook_points[0][14] = cv::Point(14 * w / 40, w / 8);
    rook_points[0][15] = cv::Point(w / 4, w / 8);
    rook_points[0][16] = cv::Point(w / 4, 3 * w / 8);
    rook_points[0][17] = cv::Point(13 * w / 32, 3 * w / 8);
    rook_points[0][18] = cv::Point(5 * w / 16, 13 * w / 16);
    rook_points[0][19] = cv::Point(w / 4, 13 * w / 16);
    const cv::Point* ppt[1] = { rook_points[0] };
    int npt[] = { 20 };
    fillPoly(img,
        ppt,
        npt,
        1,
        Scalar(255, 255, 255),
        lineType);
}


void MyLine(cv::Mat img, cv::Point start, cv::Point end)
{
    int thickness = 2;
    int lineType = LINE_8;
    line(img,
        start,
        end,
        Scalar(0, 0, 0),
        thickness,
        lineType);
}