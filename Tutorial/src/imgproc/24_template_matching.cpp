#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>

constexpr auto IMAGE_PATH = "D:/repos/OpenCV/Tutorial/in_files/cadillac.jpg";
constexpr auto TEMPLATE_PATH = "D:/repos/OpenCV/Tutorial/in_files/cadillac_logo.jpg";


namespace templ_match
{
    bool use_mask = false;

    cv::Mat img; cv::Mat templ; cv::Mat mask; cv::Mat result;
    const char* image_window = "Source Image";
    const char* result_window = "Result window";
    int match_method;
    int max_Trackbar = 5;


    void MatchingMethod(int, void*);


    void run()
    {
        img = cv::imread(IMAGE_PATH, cv::IMREAD_COLOR);
        templ = cv::imread(TEMPLATE_PATH, cv::IMREAD_COLOR);

        if (img.empty() || templ.empty() || (use_mask && mask.empty()))
        {
            std::cout << "Can't read one of the images\n";
            return;
        }


        cv::namedWindow(image_window, cv::WINDOW_AUTOSIZE);
        cv::namedWindow(result_window, cv::WINDOW_AUTOSIZE);

        const char* trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
        cv::createTrackbar(trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethod);

        MatchingMethod(0, 0);

        cv::waitKey(0);
    }


    void MatchingMethod(int, void*)
    {
        cv::Mat img_display;
        img.copyTo(img_display);

        int result_cols = img.cols - templ.cols + 1;
        int result_rows = img.rows - templ.rows + 1;

        result.create(result_rows, result_cols, CV_32FC1);

        bool method_accepts_mask = (cv::TM_SQDIFF == match_method || match_method == cv::TM_CCORR_NORMED);
        if (use_mask && method_accepts_mask)
        {
            cv::matchTemplate(img, templ, result, match_method, mask);
        }
        else
        {
            cv::matchTemplate(img, templ, result, match_method);
        }

        cv::normalize(result, result, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

        double minVal; double maxVal; cv::Point minLoc; cv::Point maxLoc;
        cv::Point matchLoc;
        cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());

        matchLoc = (match_method == cv::TM_SQDIFF || match_method == cv::TM_SQDIFF_NORMED) ? minLoc : maxLoc;

        auto green = cv::Scalar(0, 255, 0);

        cv::rectangle(img_display, matchLoc, cv::Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), green, 2, 8, 0);
        cv::rectangle(result, matchLoc, cv::Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), green, 2, 8, 0);

        cv::imshow(image_window, img_display);
        cv::imshow(result_window, result);
    }
}


void template_matching()
{
    templ_match::run();
}